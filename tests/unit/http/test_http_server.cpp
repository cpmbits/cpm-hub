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

#include <http/HttpServer.h>
#include <http/HttpClient.h>

using namespace cest;


static HttpResponse get_plugins_response;

static HttpResponse getPlugins(HttpRequest request)
{
    return get_plugins_response;
}


static HttpRequest post_plugin_request;
static HttpResponse post_plugin_response;

static HttpResponse postPlugin(HttpRequest request)
{
    post_plugin_request = request;
    return post_plugin_response;
}


static HttpResponse put_plugin_response;

static HttpResponse putPlugin(HttpRequest request)
{
    return put_plugin_response;
}

static HttpResponse throwsException(HttpRequest request)
{
    throw std::exception();
}


describe("HTTP server based on Cesanta Mongoose", []() {
    it("starts and stops an HTTP server", [&]() {
        HttpServer server;
        server.startAsync("127.0.0.1", 8000);
        server.stop();
    });

    it("allows registering a callback for a given verb and path", [&]() {
        HttpServer server;

        server.get("/plugins", getPlugins);
    });

    it("returns status code 404 when endpoint is not known", [&]() {
        HttpServer server;
        HttpClient client;
        HttpResponse response;

        server.startAsync("127.0.0.1", 8000);

        response = client.get("http://127.0.0.1:8000/plugins", HttpRequest(""));

        expect(response.status_code).toBe(404);

        server.stop();
    });

    it("calls GET method callback when receiving a request", []() {
        HttpServer server;
        HttpClient client;
        HttpResponse response;

        get_plugins_response.body = "hello";
        get_plugins_response.status_code = 200;
        server.get("/plugins", getPlugins);
        server.startAsync("127.0.0.1", 8000);

        response = client.get("http://127.0.0.1:8000/plugins", HttpRequest(""));

        expect(response.status_code).toBe(200);
        expect(response.body).toBe("hello");

        server.stop();
    });

    it("calls POST method callback when receiving a request", []() {
        HttpServer server;
        HttpClient client;
        HttpResponse response;

        post_plugin_response.body = "hello";
        post_plugin_response.status_code = 200;
        server.post("/plugins", postPlugin);
        server.startAsync("127.0.0.1", 8000);

        response = client.post("http://127.0.0.1:8000/plugins", HttpRequest("post data"));

        expect(response.status_code).toBe(200);
        expect(response.body).toBe("hello");

        server.stop();
    });

    it("calls POST method callback passing provided headers", []() {
        HttpServer server;
        HttpClient client;
        HttpRequest request("post data");
        HttpResponse response;

        request.headers.set("header1", "value1");
        server.post("/plugins", postPlugin);
        server.startAsync("127.0.0.1", 8000);

        response = client.post("http://127.0.0.1:8000/plugins", request);

        expect(post_plugin_request.headers.get("header1")).toBe("value1");

        server.stop();
    });

    it("calls PUT method callback when receiving a request", []() {
        HttpServer server;
        HttpClient client;
        HttpResponse response;

        put_plugin_response.body = "hello";
        put_plugin_response.status_code = 200;
        server.put("/plugins", putPlugin);
        server.startAsync("127.0.0.1", 8000);

        response = client.put("http://127.0.0.1:8000/plugins", HttpRequest("post data"));

        expect(response.status_code).toBe(200);
        expect(response.body).toBe("hello");

        server.stop();
    });

    it("returns 400 when method is not valid", []() {
        HttpServer server;
        HttpClient client;
        HttpResponse response;

        put_plugin_response.body = "hello";
        put_plugin_response.status_code = 200;
        server.startAsync("127.0.0.1", 8000);

        response = client.method("http://127.0.0.1:8000/plugins", HttpRequest("post data"), "CONNECT");

        expect(response.status_code).toBe(400);

        server.stop();
    });

    it("returns 404 when method is not registered for endpoint", []() {
        HttpServer server;
        HttpClient client;
        HttpResponse response;

        put_plugin_response.body = "hello";
        put_plugin_response.status_code = 200;
        server.put("/plugins", putPlugin);
        server.startAsync("127.0.0.1", 8000);

        response = client.post("http://127.0.0.1:8000/plugins", HttpRequest("post data"));

        expect(response.status_code).toBe(404);

        server.stop();
    });

    it("returns error 500 when callback throws an exception", [&]() {
        HttpServer server;
        HttpClient client;
        HttpResponse response;

        get_plugins_response.body = "hello";
        get_plugins_response.status_code = 200;
        server.get("/plugins", throwsException);
        server.startAsync("127.0.0.1", 8000);

        response = client.get("http://127.0.0.1:8000/plugins", HttpRequest(""));

        expect(response.status_code).toBe(500);

        server.stop();
    });
});