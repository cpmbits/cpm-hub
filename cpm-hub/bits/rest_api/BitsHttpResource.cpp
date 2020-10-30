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
#include <bits/rest_api/BitsHttpResource.h>
#include <bits/BitsService.h>
#include <authentication/UserCredentials.h>

using namespace nlohmann;
using namespace std;

static NullAuthenticator unauthenticated;


BitsHttpResource::BitsHttpResource(BitsService *bits_service)
{
    this->bits_service = bits_service;
    this->authenticator = &unauthenticated;
    this->allow_origin = "*";
    this->allow_methods = "GET";
}


BitsHttpResource::BitsHttpResource(BitsService *bits_service, Authenticator *authenticator)
{
    this->bits_service = bits_service;
    this->authenticator = authenticator;
    this->allow_origin = "*";
    this->allow_methods = "GET";
}


HttpResponse BitsHttpResource::post(HttpRequest &request)
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

    if (this->bits_service->bitBy(publication_data.bit_name, publication_data.version).isPresent()) {
        return HttpResponse::conflict();
    }

    bits_service->publishBit(publication_data);

    return HttpResponse::ok("");
}


HttpResponse BitsHttpResource::get(HttpRequest &request)
{
    if (!request.parameters.has("bitName")) {
        return searchForBit(request);
    } else {
        return getBit(request);
    }
}


HttpResponse BitsHttpResource::listBits(HttpRequest &request)
{
    HttpResponse response;
    json json_bits = json::array();

    for (Bit bit : bits_service->allBits()) {
        json json_bit = {{"bit_name", bit.metadata.name}};
        json_bits.push_back(json_bit);
    }

    return HttpResponse::ok(json_bits.dump());
}


static string bitAsJson(Bit &bit)
{
    json json_bit = {
            {"bit_name", bit.metadata.name},
            {"version", bit.metadata.version},
            {"payload", bit.payload}
    };
    return json_bit.dump();
}


HttpResponse BitsHttpResource::getBit(HttpRequest &request)
{
    Optional<Bit> bit;

    if (!request.parameters.has("bitVersion")) {
        bit = this->bits_service->bitBy(request.parameters.get("bitName"));
    } else {
        bit = this->bits_service->bitBy(
                request.parameters.get("bitName"),
                request.parameters.get("bitVersion"));
    }

    if (!bit.isPresent()) {
        return HttpResponse(HttpStatus::NOT_FOUND, "");
    }

    return HttpResponse::ok(bitAsJson(bit.value()));
}


static string bitSearchResultsAsJson(list<BitMetadata> &bits_found)
{
    HttpResponse response;
    json json_bits = json::array();

    for (auto &bit : bits_found) {
        json json_bit = {
            {"name", bit.name},
            {"author", bit.user_name}
        };
        json_bits.push_back(json_bit);
    }

    return json_bits.dump();
}


HttpResponse BitsHttpResource::searchForBit(HttpRequest &request)
{
    list<BitMetadata> bits_found;
    BitSearchQuery search_query;

    if (!request.query_parameters.has("name")) {
        return HttpResponse::badRequest();
    }

    search_query.name = request.query_parameters.get("name");
    if (search_query.name.empty()) {
        return HttpResponse::badRequest();
    }

    bits_found = this->bits_service->search(search_query);

    return HttpResponse::ok(bitSearchResultsAsJson(bits_found));
}
