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

struct http_request {
    std::string body;

    http_request(std::string _body)
    {
        body = _body;
    }
};


struct http_response {
    int status_code;
    std::string body;

    http_response()
    {
        status_code = 404;
        body = "";
    }

    http_response(int _status_code, std::string _body)
    {
        status_code = _status_code;
        body = _body;
    }
};
