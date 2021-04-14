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
#include <logging/Logger.h>
#include <http/HttpServer.h>
#include <http/http_headers_encoder.h>
#include <boost/algorithm/string.hpp>
#include <kpi/kpi.h>


using namespace std;

static void eventHandler(struct mg_connection *connection, int event, void *data);
static void digestHeaders(struct http_message *message, HttpRequest &request);
static void digestQueryString(struct http_message *message, HttpRequest &request);


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

    INFO("Started server on http%s://%s", security_options.security_enabled ? "s" : "", string_stream.str().c_str());

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


void HttpServer::addResource(Endpoint endpoint, HttpResource *resource)
{
    this->resources.insert(make_pair(endpoint, resource));
}


HttpResponse HttpServer::dispatchRequest(HttpRequest &request)
{
    HttpResource *resource = nullptr;
    HttpResponse response;

    for (pair<Endpoint, HttpResource *> iter: this->resources) {
        Maybe<struct HttpParameterMap> match;
        match = iter.first.match(request.path);
        if (match.isPresent()) {
            resource = iter.second;
            request.parameters = match.value();
            break;
        }
    }

    if (!resource) {
        return HttpResponse::notFound();
    }

    if (request.method == "GET") {
        response = resource->get(request);
    } else if (request.method == "POST") {
        response = resource->post(request);
    } else if (request.method == "PUT") {
        response = resource->put(request);
    } else if (request.method == "OPTIONS") {
        response = resource->options(request);
    } else {
        return HttpResponse::badRequest();
    }

    resource->handleCors(request, response);

    return response;
}


HttpRequest HttpServer::parseRequest(struct mg_connection *connection, struct http_message *message)
{
    HttpRequest request;

    digestHeaders(message, request);
    digestQueryString(message, request);
    request.body = string(message->body.p, message->body.len);
    request.method = string(message->method.p, message->method.len);
    request.path = string(message->uri.p, message->uri.len);
    request.protocol = string(message->proto.p, message->proto.len);

    return request;
}


static void digestQueryString(struct http_message *message, HttpRequest &request)
{
    string query_string = string(message->query_string.p, message->query_string.len);
    vector<string> fragments;

    boost::split(fragments, query_string, boost::is_any_of("&"));
    for(auto&& fragment: fragments) {
        vector<string> parameter_value;
        boost::split(parameter_value, fragment, boost::is_any_of("="));
        if (parameter_value.size() == 2) {
            request.query_parameters.set(parameter_value.at(0), parameter_value.at(1));
        }
    }
}


static void digestHeaders(struct http_message *message, HttpRequest &request)
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
    HttpRequest request = this->parseRequest(connection, message);

    try {
        response = this->dispatchRequest(request);
    } catch (exception &e) {
        response = HttpResponse(500, "");
    }

    mg_send_head(connection, response.status_code, response.body.size(), encodeHeaders(response.headers).c_str());
    mg_printf(connection, "%s", response.body.c_str());

    logRequest(connection, message, request, response);
}


void HttpServer::logRequest(struct mg_connection *connection, const http_message *message, const HttpRequest &request, const HttpResponse &response) const
{
    char client_address[INET6_ADDRSTRLEN];
    time_t rawtime;
    struct tm *local_time;
    char current_time_string[128];

    mg_conn_addr_to_str(connection, client_address, sizeof(client_address), MG_SOCK_STRINGIFY_REMOTE|MG_SOCK_STRINGIFY_IP);
    time(&rawtime);
    local_time = localtime(&rawtime);
    strftime(current_time_string, sizeof(current_time_string), "%d/%b/%Y:%H:%M:%S %z", local_time);

    recordKpi("http_request",
        1.0,
        map<string, string>{
            {"path", request.path},
            {"method", request.method},
            {"status", to_string(response.status_code)},
            {"ip", to_string(response.status_code)}
    });
    INFO("%s - - [%s] \"%s %s %s\" %d %d",
         client_address,
         current_time_string,
         string(message->method.p, message->method.len).c_str(),
         string(message->uri.p, message->uri.len).c_str(),
         string(message->proto.p, message->proto.len).c_str(),
         response.status_code,
         response.body.size());
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
