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
#include <iostream>

#include <boost/algorithm/string.hpp>
#include <infrastructure/endpoint.h>

using namespace std;


Endpoint::Endpoint(string path)
{
    parsePath(path);
}


Optional<EndpointMatch> Endpoint::match(string path) 
{
    Optional<EndpointMatch> match;
    smatch base_match;

    if (regex_match(path, base_match, this->matching_regex)) {
        match = EndpointMatch();
        for (size_t i=1; i<base_match.size(); ++i) {
            std::ssub_match sub_match = base_match[i];
            match.value().set(this->parameter_names.at(i-1), sub_match.str());
        }
    }
    
    return match;
}


void Endpoint::parsePath(string path) 
{
    vector<string> tokens;
    string regex_string("");

    boost::trim_if(path, boost::is_any_of("/"));
    boost::split(tokens, path, boost::is_any_of("/"));
    for(auto&& token: tokens) {
        if (token.front() == ':') {
            regex_string += "/([\\w-]+)";
            this->parameter_names.push_back(token.substr(1));
        } else {
            regex_string += "/" + token;
        }
    }

    this->matching_regex = regex(regex_string);
}
