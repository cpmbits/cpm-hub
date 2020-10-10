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

#include <http/HttpClient.h>

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>


class MockHttpClient: public HttpClient {
public:
    HttpResponse get(std::string url, HttpRequest request) {
        last_request = request;
        return *(HttpResponse *)mock().actualCall("HttpClient.get")
                                      .withParameter("url", url.c_str())
                                      .returnPointerValue();
    }
    
    HttpResponse post(std::string url, HttpRequest request) {
        last_request = request;
        return *(HttpResponse *)mock().actualCall("HttpClient.post")
                .withParameter("url", url.c_str())
                .returnPointerValue();
    }

    HttpResponse put(std::string url, HttpRequest request) {
        last_request = request;
        return *(HttpResponse *)mock().actualCall("HttpClient.put")
                .withParameter("url", url.c_str())
                .returnPointerValue();
    }

    HttpResponse method(std::string url, HttpRequest request, std::string method) {
        last_request = request;
        return *(HttpResponse *)mock().actualCall("HttpClient.method")
                .withParameter("url", url.c_str())
                .withParameter("method", method.c_str())
                .returnPointerValue();
    }

    void responseArrived(HttpResponse response) {
    }

    void connectionClosed() {
    }

    MockExpectedCall &expect(const std::string& call) {
        std::string full_call = std::string("HttpClient.") + call;
        return mock().expectOneCall(full_call.c_str());
    }

    HttpRequest last_request;
};
