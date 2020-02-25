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
#include <cest/cest.h>

#include <infrastructure/http_server.h>
#include <infrastructure/http_client.h>

using namespace cest;


static struct http_response get_plugins_response;

static struct http_response getPlugins(struct http_request request)
{
    return get_plugins_response;
}


static struct http_response post_plugin_response;

static struct http_response postPlugin(struct http_request request)
{
    return post_plugin_response;
}


static struct http_response put_plugin_response;

static struct http_response putPlugin(struct http_request request)
{
    return put_plugin_response;
}


describe("HTTP server based on Cesanta Mongoose", []() {
    beforeEach([&]() {
    });

    afterEach([&]() {
    });

    it("starts and stops an HTTP server", [&]() {
        HttpServer server;
        server.start(8000);
        server.stop();
    });

    it("allows registering a callback for a given verb and path", [&]() {
        HttpServer server;

        server.get("/plugins", getPlugins);
    });

    it("returns status code 404 when endpoint is not known", [&]() {
        HttpServer server;
        HttpClient client;
        struct http_response response;

        server.start(8000);

        response = client.get("http://127.0.0.1:8000/plugins", http_request(""));

        expect(response.status_code).toBe(404);

        server.stop();
    });

    it("calls GET method callback when receiving a request", []() {
        HttpServer server;
        HttpClient client;
        struct http_response response;

        get_plugins_response.body = "hello";
        get_plugins_response.status_code = 200;
        server.get("/plugins", getPlugins);
        server.start(8000);

        response = client.get("http://127.0.0.1:8000/plugins", http_request(""));

        expect(response.status_code).toBe(200);
        expect(response.body).toBe("hello");

        server.stop();
    });

    it("calls POST method callback when receiving a request", []() {
        HttpServer server;
        HttpClient client;
        struct http_response response;

        post_plugin_response.body = "hello";
        post_plugin_response.status_code = 200;
        server.post("/plugins", postPlugin);
        server.start(8000);

        response = client.post("http://127.0.0.1:8000/plugins", http_request("post data"));

        expect(response.status_code).toBe(200);
        expect(response.body).toBe("hello");

        server.stop();
    });

    it("calls PUT method callback when receiving a request", []() {
        HttpServer server;
        HttpClient client;
        struct http_response response;

        put_plugin_response.body = "hello";
        put_plugin_response.status_code = 200;
        server.put("/plugins", putPlugin);
        server.start(8000);

        response = client.put("http://127.0.0.1:8000/plugins", http_request("post data"));

        expect(response.status_code).toBe(200);
        expect(response.body).toBe("hello");

        server.stop();
    });
});