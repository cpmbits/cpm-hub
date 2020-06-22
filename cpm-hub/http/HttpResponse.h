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

#include <string>

#include <http/http_status_codes.h>
#include <http/HttpParameterMap.h>

struct HttpResponse {
    int status_code;
    std::string body;
    struct HttpParameterMap headers;

    HttpResponse()
    {
        status_code = 404;
        body = "";
    }

    HttpResponse(int _status_code, std::string _body)
    {
        status_code = _status_code;
        body = _body;
    }

    static HttpResponse withEmptyBody(int status_code) {
        HttpResponse response;
        response.status_code = status_code;
        response.body = "";
        return response;
    }

    static HttpResponse badRequest() {
        return withEmptyBody(HttpStatus::BAD_REQUEST);
    }

    static HttpResponse unauthorized() {
        return withEmptyBody(HttpStatus::UNAUTHORIZED);
    }

    static HttpResponse conflict() {
        return withEmptyBody(HttpStatus::CONFLICT);
    }

    static HttpResponse notFound() {
        return withEmptyBody(HttpStatus::NOT_FOUND);
    }

    static HttpResponse ok(std::string body) {
        HttpResponse response;
        response.status_code = HttpStatus::OK;
        response.body = body;
        return response;
    }

    static HttpResponse noContent() {
        HttpResponse response;
        response.status_code = HttpStatus::NO_CONTENT;
        response.body = "";
        return response;
    }

    static HttpResponse cors(std::string origin, std::string methods) {
        HttpResponse response;
        response.status_code = HttpStatus::NO_CONTENT;
        response.body = "";
        response.headers.set("Access-Control-Allow-Origin", origin);
        response.headers.set("Access-Control-Allow-Methods", methods);
        response.headers.set("Access-Control-Max-Age", "86400");
        return response;
    }
};
