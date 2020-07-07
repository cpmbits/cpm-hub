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
#pragma once

#include <functional>
#include <map>
#include <list>
#include <string>
#include <thread>

#include <mongoose/mongoose.h>

#include <http/http.h>
#include <http/Endpoint.h>
#include "HttpResource.h"


typedef std::function<HttpResponse(HttpRequest &)> ServerCallback;

class HttpServer {
public:
    HttpServer();

    void addResource(Endpoint endpoint, HttpResource *resource);

    void configureSecurity(struct HttpSecurityOptions &options);

    void startAsync(std::string address, int port);

    void start(std::string address, int port);

    void stop();

    void serveRequest(struct mg_connection *connection, struct http_message *message);

private:
    int port;
    bool running;
    struct mg_mgr mgr;
    struct mg_connection *connection;
    std::thread *server_thread;
    struct HttpSecurityOptions security_options;
    std::map<Endpoint, HttpResource *> resources;

    void serve();

    HttpRequest parseRequest(struct mg_connection *connection, struct http_message *message);

    HttpResponse dispatchRequest(HttpRequest &request);

    void createConnection(const std::string &address, int port);

    mg_bind_opts configureBindOpts() const;

    void logRequest(struct mg_connection *connection, const http_message *message, const HttpRequest &request, const HttpResponse &response) const;
};
