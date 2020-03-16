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

#include <map>
#include <regex>
#include <string>
#include <vector>

#include <domain/optional.h>


class EndpointMatch {
public:
    std::string get(std::string parameter) {
        auto iter = parameters.find(parameter);
        return iter->second;
    }

    void set(std::string parameter, std::string value) {
        parameters.insert(std::make_pair(parameter, value));
    }

private:
    std::map<std::string, std::string> parameters;
};


class Endpoint {
public:
    Endpoint(std::string path);

    Optional<EndpointMatch> match(std::string path);


private:
    std::regex matching_regex;
    std::vector<std::string> parameter_names;
    
    void parsePath(std::string path);
};
