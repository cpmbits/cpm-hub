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

#include <http/HttpServer.h>
#include <http/http_headers.h>


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
    mg_bind_opts bind_opts = configureBindOpts();

    string_stream << address << ":" << port;

    HttpServer::port = port;
    running = true;
    mg_mgr_init(&mgr, this);

    connection = mg_bind_opt(&mgr, string_stream.str().c_str(), eventHandler, bind_opts);

    if (security_options.security_enabled) {
        cout << "Started server on https://" << string_stream.str() << endl;
    } else {
        cout << "Started server on http://" << string_stream.str() << endl;
    }

    mg_set_protocol_http_websocket(connection);
}


mg_bind_opts HttpServer::configureBindOpts() const
{
    struct mg_bind_opts bind_opts;

    memset(&bind_opts, 0, sizeof(bind_opts));
    if (security_options.security_enabled) {
        bind_opts.ssl_cert = security_options.certificate_file.c_str();
        bind_opts.ssl_key = security_options.key_file.c_str();
    }

    return bind_opts;
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


static HttpResponse notFound(HttpRequest request)
{
    return HttpResponse(404, "");
}


static HttpResponse badRequest(HttpRequest request)
{
    return HttpResponse(400, "");
}


ServerCallback HttpServer::parseRequest(struct http_message *message, HttpRequest &request)
{
    map<Endpoint, ServerCallback> *callbacks;
    ServerCallback server_callback = notFound;
    string method(message->method.p, message->method.len);
    string endpoint(message->uri.p, message->uri.len);

    request.body = string(message->body.p, message->body.len);

    if (method == "GET") {
        callbacks = &this->gets;
    } else if (method == "POST") {
        callbacks = &this->posts;
    } else if (method == "PUT") {
        callbacks = &this->puts;
    } else {
        return badRequest;
    }

    for (pair<Endpoint, ServerCallback> iter: *callbacks) {
        Optional<struct HttpParameterMap> match;
        match = iter.first.match(endpoint);
        if (match.isPresent()) {
            server_callback = iter.second;
            request.parameters = match.value();
            break;
        }
    }

    digestHeaders(message, request);

    return server_callback;
}


void HttpServer::digestHeaders(struct http_message *message, HttpRequest &request)
{
    for (int i=0; message->header_names[i].len > 0; ++i) {
        request.headers.set(
                string(message->header_names[i].p, message->header_names[i].len),
                string(message->header_values[i].p, message->header_values[i].len)
        );
    }
}


void HttpServer::serveRequest(struct mg_connection *connection, struct http_message *message)
{
    HttpResponse response;
    HttpRequest request;
    ServerCallback callback = this->parseRequest(message, request);

    try {
        response = callback(request);
    } catch (exception &e) {
        response = HttpResponse(500, "");
    }

    mg_send_head(connection, response.status_code, response.body.size(), "");
    mg_printf(connection, "%s", response.body.c_str());

    logRequest(connection, message, response);
}


void HttpServer::logRequest(struct mg_connection *connection, const http_message *message, const HttpResponse &response) const
{
    char client_address[INET6_ADDRSTRLEN];
    time_t rawtime;
    struct tm *local_time;
    char current_time_string[128];

    mg_conn_addr_to_str(connection, client_address, sizeof(client_address), MG_SOCK_STRINGIFY_REMOTE|MG_SOCK_STRINGIFY_IP);
    time(&rawtime);
    local_time = localtime(&rawtime);
    strftime(current_time_string, sizeof(current_time_string), "%d/%b/%Y:%H:%M:%S %z", local_time);

    cout
        << client_address
        << " -"
        << " -"
        << " [" << current_time_string << "]"
        << " \"" << string(message->method.p, message->method.len)
        << " " << string(message->uri.p, message->uri.len)
        << " " << string(message->proto.p, message->proto.len)
        << "\""
        << " " << response.status_code
        << " " << response.body.size()
        << endl;
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
