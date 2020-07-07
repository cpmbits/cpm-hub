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
#include <http/HttpClient.h>
#include <http/http_headers_encoder.h>
#include <http/http_url_parameters_encoder.h>

using namespace std;


static void eventHandler(struct mg_connection *connection, int event, void *data);
static void digestHeaders(struct http_message *message, HttpResponse &response);


HttpResponse HttpClient::method(std::string url, HttpRequest request, std::string method)
{
    struct mg_connection *connection;
    string url_with_parameters = url + encodeUrlParameters(request.parameters);

    mg_mgr_init(&mgr, this);
    connection = mg_connect_http(&mgr,
            method.c_str(),
            eventHandler,
            url_with_parameters.c_str(),
            encodeHeaders(request.headers).c_str(),
            request.body.c_str());
    mg_set_protocol_http_websocket(connection);

    request_pending = true;
    while (request_pending) {
        mg_mgr_poll(&mgr, 100);
    }

    return this->response;
}


HttpResponse HttpClient::post(std::string url, HttpRequest request)
{
    return this->method(url, request, "POST");
}


HttpResponse HttpClient::put(std::string url, HttpRequest request)
{
    return this->method(url, request, "PUT");
}


HttpResponse HttpClient::get(std::string url, HttpRequest request)
{
    return this->method(url, request, "GET");
}


void HttpClient::responseArrived(HttpResponse response)
{
    this->response = response;
    this->request_pending = false;
}


static void digestHeaders(struct http_message *message, HttpResponse &response)
{
    for (int i=0; message->header_names[i].len > 0; ++i) {
        response.headers.set(
                string(message->header_names[i].p, message->header_names[i].len),
                string(message->header_values[i].p, message->header_values[i].len)
        );
    }
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
        digestHeaders(message, response);
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
