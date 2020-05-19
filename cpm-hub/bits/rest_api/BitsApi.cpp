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
#include <json/json.hpp>
#include <bits/rest_api/BitsApi.h>
#include <bits/BitsService.h>
#include <authentication/UserCredentials.h>

using namespace nlohmann;
using namespace std;

static NullAuthenticator unauthenticated;


BitsApi::BitsApi(BitsService *bits_service) {
    this->bits_service = bits_service;
    this->authenticator = &unauthenticated;
}


BitsApi::BitsApi(BitsService *bits_service, Authenticator *authenticator)
{
    this->bits_service = bits_service;
    this->authenticator = authenticator;
}


HttpResponse BitsApi::publishBit(HttpRequest &request)
{
    auto json = json::parse(request.body);
    Optional<string> user;
    struct BitPublicationData publication_data;
    UserCredentials credentials = {json.at("username"),json.at("password")};

    if (!this->authenticator->validCredentials(credentials)) {
        return HttpResponse::unauthorized();
    }

    publication_data.bit_name = json.at("bit_name");
    publication_data.version = json.at("version");
    publication_data.user_name = json.at("username");
    publication_data.payload = json.at("payload");

    bits_service->publishBit(publication_data);

    return HttpResponse::ok("");
}


HttpResponse BitsApi::listBits(HttpRequest &request)
{
    HttpResponse response(HttpStatus::OK, "");
    json json_bit_list = json::array();

    for (Bit bit : bits_service->allBits()) {
        json json_bit = {{"bit_name", bit.metadata.name}};
        json_bit_list.push_back(json_bit);
    }

    response.body = json_bit_list.dump();

    return response;
}


static string asJson(Bit bit)
{
    json json_bit = {
        {"bit_name", bit.metadata.name},
        {"version", bit.metadata.version},
        {"payload", bit.payload}
    };
    return json_bit.dump();
}


HttpResponse BitsApi::downloadBit(HttpRequest &request)
{
    Optional<Bit> bit;

    if (!request.parameters.has("bitVersion")) {
        bit = bits_service->find(request.parameters.get("bitName"));
    } else {
        bit = bits_service->find(
                request.parameters.get("bitName"),
                request.parameters.get("bitVersion"));
    }
    if (!bit.isPresent()) {
        return HttpResponse(HttpStatus::NOT_FOUND, "");
    }

    return HttpResponse(HttpStatus::OK, asJson(bit.value()));
}
