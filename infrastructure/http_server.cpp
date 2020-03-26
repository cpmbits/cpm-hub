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
#include <iostream>

#include <infrastructure/http_server.h>

using namespace std;

static void eventHandler(struct mg_connection *connection, int event, void *data);


HttpServer::HttpServer()
{
    security_options.security_enabled = false;
}


void HttpServer::startAsync(string address, int port)
{
    createConnection(address, port);

    this->server_thread = new thread(&HttpServer::serve, this);
}


void HttpServer::createConnection(const string &address, int port)
{
    ostringstream string_stream;

    string_stream << address << ":" << port;

    cout << "Started server on " << string_stream.str() << endl;

    HttpServer::port = port;
    running = true;
    mg_mgr_init(&mgr, this);
    connection = mg_bind(&mgr, string_stream.str().c_str(), eventHandler);
    mg_set_protocol_http_websocket(connection);
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


void HttpServer::start(std::string address, int port)
{
    createConnection(address, port);
    serve();
}


void HttpServer::post(string path, ServerCallback callback)
{
    this->posts.insert(make_pair(Endpoint(path), callback));
}


void HttpServer::get(string path, ServerCallback callback)
{
    this->gets.insert(make_pair(Endpoint(path), callback));
}


void HttpServer::put(string path, ServerCallback callback)
{
    this->puts.insert(make_pair(Endpoint(path), callback));
}


static struct http_response notFound(struct http_request request)
{
    return http_response(404, "");
}


ServerCallback HttpServer::parseRequest(struct http_message *message, struct http_request &request)
{
    map<Endpoint, ServerCallback> *callbacks;
    string method(message->method.p, message->method.len);
    string endpoint(message->uri.p, message->uri.len);
    ServerCallback server_callback = notFound;

    request.body = string(message->body.p, message->body.len);

    if (method == "GET") {
        callbacks = &this->gets;
    } else if (method == "POST") {
        callbacks = &this->posts;
    } else if (method == "PUT") {
        callbacks = &this->puts;
    }

    for (pair<Endpoint, ServerCallback> iter: *callbacks) {
        Optional<struct http_request_parameters> match;
        match = iter.first.match(endpoint);
        if (match.isPresent()) {
            server_callback = iter.second;
            request.parameters = match.value();
            break;
        }
    }

    return server_callback;
}


void HttpServer::serveRequest(struct mg_connection *connection, struct http_message *message)
{
    struct http_response response;
    struct http_request request;
    ServerCallback callback = this->parseRequest(message, request);

    response = callback(request);

    mg_send_head(connection, response.status_code, response.body.size(), "");
    mg_printf(connection, "%s", response.body.c_str());

    cout << string(message->method.p, message->method.len) << " " << string(message->uri.p, message->uri.len) << ": " << response.status_code << endl;
}

void HttpServer::configureSecurity(struct HttpSecurityOptions &options)
{
    this->security_options = options;
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
