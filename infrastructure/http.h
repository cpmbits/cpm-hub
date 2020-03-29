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
#include <map>


struct HttpParameterMap {
public:
    typedef std::map<std::string, std::string>::iterator iterator;
    typedef std::map<std::string, std::string>::const_iterator const_iterator;

    std::string &get(const std::string& parameter) {
        auto iter = parameters.find(parameter);
        return iter->second;
    }

    void set(const std::string& parameter, const std::string& value) {
        parameters.insert(std::make_pair(parameter, value));
    }

    int count() {
        return parameters.size();
    }

    iterator begin() {
        return parameters.begin();
    }

    iterator end() {
        return parameters.end();
    }

private:
    std::map<std::string, std::string> parameters;
};


struct HttpSecurityOptions {
    bool security_enabled;
    std::string certificate_file;
    std::string key_file;
};


struct HttpRequest {
    std::string method;
    std::string body;
    struct HttpParameterMap parameters;
    struct HttpParameterMap headers;

    HttpRequest(std::string _body="") {
        body = _body;
    }
};


struct HttpResponse {
    int status_code;
    std::string body;

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
};
