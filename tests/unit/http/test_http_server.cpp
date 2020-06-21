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

#include <logging/LoggerInConsole.h>
#include <http/HttpServer.h>
#include <http/HttpClient.h>
#include <http/HttpResource.h>

using namespace cest;


class TestHttpResource: public HttpResource {
public:
    HttpResponse get(HttpRequest &request) {
        return get_response;
    }

    HttpResponse post(HttpRequest &request) {
        post_request = request;
        return post_response;
    }

    HttpResponse put(HttpRequest &request) {
        return put_response;
    }

    HttpResponse options(HttpRequest &request) {
        return options_response;
    }

    HttpRequest post_request;
    HttpResponse get_response;
    HttpResponse post_response;
    HttpResponse put_response;
    HttpResponse options_response;
};

class ResourceGetThrowsException: public HttpResource {
public:
    HttpResponse get(HttpRequest &request) {
        throw std::exception();
    }
};


static LoggerInConsole test_logger;
static TestHttpResource *test_resource;


describe("HTTP server using resources", []() {
    beforeEach([]() {
        logger = &test_logger;
        test_resource = new TestHttpResource();
    });

    afterEach([]() {
        delete test_resource;
    });

    it("allows registering a resource", [&]() {
        HttpServer server;

        server.addResource(Endpoint("/bits"), test_resource);
    });

    it("allows registering a resource to many resources", [&]() {
        HttpServer server;

        server.addResource(Endpoint("/bits"), test_resource);
        server.addResource(Endpoint("/bits/:bitVersion"), test_resource);
    });

    it("returns status code 404 when endpoint is not known", [&]() {
        HttpServer server;
        HttpClient client;
        HttpResponse response;

        server.startAsync("127.0.0.1", 8000);

        response = client.get("http://127.0.0.1:8000/bits", HttpRequest(""));

        expect(response.status_code).toBe(HttpStatus::NOT_FOUND);

        server.stop();
    });

    it("calls GET method callback when receiving a request", []() {
        HttpServer server;
        HttpClient client;
        HttpResponse response;

        test_resource->get_response = HttpResponse::ok("hello");
        server.addResource(Endpoint("/bits"), test_resource);
        server.startAsync("127.0.0.1", 8000);

        response = client.get("http://127.0.0.1:8000/bits", HttpRequest(""));

        expect(response.status_code).toBe(HttpStatus::OK);
        expect(response.body).toBe("hello");

        server.stop();
    });

    it("calls POST method callback when receiving a request", []() {
        HttpServer server;
        HttpClient client;
        HttpResponse response;

        test_resource->post_response = HttpResponse::ok("hello");
        server.addResource(Endpoint("/bits"), test_resource);
        server.startAsync("127.0.0.1", 8000);

        response = client.post("http://127.0.0.1:8000/bits", HttpRequest(""));

        expect(response.status_code).toBe(HttpStatus::OK);
        expect(response.body).toBe("hello");

        server.stop();
    });

    it("calls PUT method callback when receiving a request", []() {
        HttpServer server;
        HttpClient client;
        HttpResponse response;

        test_resource->put_response = HttpResponse::ok("hello");
        server.addResource(Endpoint("/bits"), test_resource);
        server.startAsync("127.0.0.1", 8000);

        response = client.put("http://127.0.0.1:8000/bits", HttpRequest(""));

        expect(response.status_code).toBe(HttpStatus::OK);
        expect(response.body).toBe("hello");

        server.stop();
    });

    it("calls OPTIONS method callback when receiving a request", []() {
        HttpServer server;
        HttpClient client;
        HttpResponse response;

        test_resource->options_response = HttpResponse::ok("GET, POST, PUT");
        server.addResource(Endpoint("/bits"), test_resource);
        server.startAsync("127.0.0.1", 8000);

        response = client.method("http://127.0.0.1:8000/bits", HttpRequest(""), "OPTIONS");

        expect(response.status_code).toBe(HttpStatus::OK);
        expect(response.body).toBe("GET, POST, PUT");

        server.stop();
    });

    it("gets response with headers", []() {
        HttpServer server;
        HttpClient client;
        HttpResponse response;

        test_resource->options_response = HttpResponse::ok("");
        test_resource->options_response.headers.set("header", "value");
        server.addResource(Endpoint("/bits"), test_resource);
        server.startAsync("127.0.0.1", 8000);

        response = client.method("http://127.0.0.1:8000/bits", HttpRequest(""), "OPTIONS");

        expect(response.status_code).toBe(200);
        expect(response.body).toBe("");
        expect(response.headers.get("header")).toBe("value");

        server.stop();
    });

    it("calls POST method callback passing provided headers", []() {
        HttpServer server;
        HttpClient client;
        HttpRequest request("post data");
        HttpResponse response;

        request.headers.set("header1", "value1");
        test_resource->post_response = HttpResponse::ok("hello");
        server.addResource(Endpoint("/bits"), test_resource);
        server.startAsync("127.0.0.1", 8000);

        response = client.post("http://127.0.0.1:8000/bits", request);

        expect(test_resource->post_request.headers.get("header1")).toBe("value1");

        server.stop();
    });

    it("returns 400 when method is not valid for registered endpoint", []() {
        HttpServer server;
        HttpClient client;
        HttpResponse response;

        test_resource->put_response = HttpResponse::ok("hello");
        server.addResource(Endpoint("/bits"), test_resource);
        server.startAsync("127.0.0.1", 8000);

        response = client.method("http://127.0.0.1:8000/bits", HttpRequest("post data"), "CONNECT");

        expect(response.status_code).toBe(HttpStatus::BAD_REQUEST);

        server.stop();
    });

    it("returns 404 when method is not registered for endpoint", []() {
        HttpServer server;
        HttpClient client;
        HttpResponse response;
        HttpResource resource;

        server.addResource(Endpoint("/bits"), &resource);
        server.startAsync("127.0.0.1", 8000);

        response = client.post("http://127.0.0.1:8000/bits", HttpRequest("post data"));

        expect(response.status_code).toBe(HttpStatus::NOT_FOUND);

        server.stop();
    });

    it("returns error 500 when callback throws an exception", [&]() {
        HttpServer server;
        HttpClient client;
        HttpResponse response;
        ResourceGetThrowsException resource;

        server.addResource(Endpoint("/bits"), &resource);
        server.startAsync("127.0.0.1", 8000);

        response = client.get("http://127.0.0.1:8000/bits", HttpRequest(""));

        expect(response.status_code).toBe(HttpStatus::INTERNAL_SERVER_ERROR);

        server.stop();
    });

    it("handles CORS pre-flight request", []() {
        HttpServer server;
        HttpClient client;
        HttpRequest request;
        HttpResponse response;
        HttpResource resource;

        request.headers.set("Access-Control-Request-Method", "POST");
        request.headers.set("Access-Control-Request-Headers", "origin");
        request.headers.set("Origin", "https://cpmbits.com");
        server.addResource(Endpoint("/bits"), &resource);
        server.startAsync("127.0.0.1", 8000);

        response = client.method("http://127.0.0.1:8000/bits", HttpRequest(""), "OPTIONS");

        expect(response.status_code).toBe(HttpStatus::NO_CONTENT);
        expect(response.body).toBe("");
        expect(response.headers.get("Access-Control-Allow-Origin")).toBe("*");
        expect(response.headers.get("Access-Control-Allow-Methods")).toBe("GET, POST");
        expect(response.headers.get("Access-Control-Max-Age")).toBe("86400");

        server.stop();
    });
});