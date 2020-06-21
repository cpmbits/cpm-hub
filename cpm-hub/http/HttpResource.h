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

#include <http/HttpRequest.h>
#include <http/HttpResponse.h>
#include <http/Endpoint.h>


class HttpResource {
public:
    virtual HttpResponse get(HttpRequest &request) {
        return HttpResponse::notFound();
    }

    virtual HttpResponse post(HttpRequest &request) {
        return HttpResponse::notFound();
    }

    virtual HttpResponse put(HttpRequest &request) {
        return HttpResponse::notFound();
    }

    virtual HttpResponse patch(HttpRequest &request) {
        return HttpResponse::notFound();
    }

    virtual HttpResponse options(HttpRequest &request) {
        return HttpResponse::notFound();
    }
};