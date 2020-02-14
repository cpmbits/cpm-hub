/*
 * Copyright (C) 2020  Jordi Sánchez
 * This file is part of CPM Hub
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <sstream>
#include <infrastructure/http_server.h>

static void eventHandler(struct mg_connection *connection, int event, void *data);


void HttpServer::start(int port)
{
    std::ostringstream string_stream;

    string_stream << "127.0.0.1:" << port;

    this->port = port;
    this->running = true;
    mg_mgr_init(&mgr, this);
    connection = mg_bind(&mgr, string_stream.str().c_str(), eventHandler);
    mg_set_protocol_http_websocket(connection);

    this->server_thread = new std::thread(&HttpServer::serve, this);
}


void HttpServer::stop()
{
    this->running = false;
    this->server_thread->join();
    mg_mgr_free(&mgr);
}


void HttpServer::serve()
{
    while (this->running) {
        mg_mgr_poll(&mgr, 100);
    }
}


void HttpServer::get(std::string path, ServerCallback callback)
{
    this->gets.insert(std::make_pair(path, callback));
}


void HttpServer::serveRequest(struct mg_connection *connection, struct http_message *message)
{
    struct http_response response;
    struct http_request request("");
    ServerCallback callback;
    std::string endpoint(message->uri.p, message->uri.len);
    auto iter = this->gets.find(endpoint);

    callback = iter->second;
    response = callback(request);

    mg_printf(connection,
          "HTTP/1.1 %d OK\r\n"
          "Transfer-Encoding: chunked\r\n\r\n",
          response.status_code);

    mg_printf_http_chunk(connection,
          "%s",
          response.body.c_str());

    mg_send_http_chunk(connection, "", 0);
}


static void eventHandler(struct mg_connection *connection, int event, void *data)
{
    HttpServer *server = (HttpServer *)connection->mgr->user_data;
    struct http_message *message = (struct http_message *)data;

    switch (event) {
    case MG_EV_HTTP_REQUEST:
        server->serveRequest(connection, message);
        break;

    default:
        break;
    }
}
