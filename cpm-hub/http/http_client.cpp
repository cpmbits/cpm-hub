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
#include <http/http_client.h>
#include <http/http_headers.h>


static void eventHandler(struct mg_connection *connection, int event, void *data);


HttpResponse HttpClient::post(std::string url, HttpRequest request)
{
    struct mg_connection *connection;

    mg_mgr_init(&mgr, this);
    connection = mg_connect_http(&mgr, "POST", eventHandler, url.c_str(), encodeRequestHeaders(request).c_str(), request.body.c_str());
    mg_set_protocol_http_websocket(connection);

    request_pending = true;
    while (request_pending) {
        mg_mgr_poll(&mgr, 100);
    }

    return this->response;
}


HttpResponse HttpClient::put(std::string url, HttpRequest request)
{
    struct mg_connection *connection;

    mg_mgr_init(&mgr, this);
    connection = mg_connect_http(&mgr, "PUT", eventHandler, url.c_str(), encodeRequestHeaders(request).c_str(), request.body.c_str());
    mg_set_protocol_http_websocket(connection);

    request_pending = true;
    while (request_pending) {
        mg_mgr_poll(&mgr, 100);
    }

    return this->response;
}


HttpResponse HttpClient::method(std::string url, HttpRequest request, std::string method)
{
    struct mg_connection *connection;

    mg_mgr_init(&mgr, this);
    connection = mg_connect_http(&mgr, method.c_str(), eventHandler, url.c_str(), encodeRequestHeaders(request).c_str(), request.body.c_str());
    mg_set_protocol_http_websocket(connection);

    request_pending = true;
    while (request_pending) {
        mg_mgr_poll(&mgr, 100);
    }

    return this->response;
}


HttpResponse HttpClient::get(std::string url, HttpRequest request)
{
    struct mg_connection *connection;

    mg_mgr_init(&mgr, this);
    connection = mg_connect_http(&mgr, "GET", eventHandler, url.c_str(), encodeRequestHeaders(request).c_str(), NULL);
    mg_set_protocol_http_websocket(connection);

    request_pending = true;
    while (request_pending) {
        mg_mgr_poll(&mgr, 100);
    }

    return this->response;
}


void HttpClient::responseArrived(HttpResponse response)
{
    this->response = response;
    this->request_pending = false;
}


static void eventHandler(struct mg_connection *connection, int event, void *data)
{
    HttpClient *client = (HttpClient *)connection->mgr->user_data;
    struct http_message *message = (struct http_message *)data;
    HttpResponse response;

    switch (event) {
    case MG_EV_CONNECT:
        if (*(int *)data != 0) {
            response.status_code = 400;
            response.body = "";
            client->responseArrived(response);
        }
        break;

    case MG_EV_HTTP_REPLY:
        response.status_code = message->resp_code;
        response.body.assign(message->body.p, message->body.len);
        client->responseArrived(response);
        connection->flags |= MG_F_SEND_AND_CLOSE;
        break;

    case MG_EV_CLOSE:
        client->responseArrived(response);
        break;

    default:
        break;
    }
}
