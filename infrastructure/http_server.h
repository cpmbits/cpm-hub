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
#include <string>
#include <thread>

#include <mongoose/mongoose.h>

#include <infrastructure/http.h>
#include <infrastructure/endpoint.h>

typedef std::function<struct http_response(struct http_request &)> ServerCallback;

class HttpServer {
public:
    HttpServer();
    void configureSecurity(struct HttpSecurityOptions &options);
    void get(std::string path, ServerCallback callback);
    void post(std::string path, ServerCallback callback);
    void put(std::string path, ServerCallback callback);
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
    std::map<Endpoint, ServerCallback> gets;
    std::map<Endpoint, ServerCallback> posts;
    std::map<Endpoint, ServerCallback> puts;
    struct HttpSecurityOptions security_options;

    void serve();

    ServerCallback parseRequest(struct http_message *message, struct http_request &request);

    void createConnection(const std::string &address, int port);
};
