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
#include <authentication/UserCredentials.h>
#include <templates/Template.h>
#include <templates/rest_api/TemplatesHttpResource.h>
#include <templates/rest_api/TemplatesContract.h>
#include <templates/TemplatesService.h>

using namespace nlohmann;
using namespace std;

static NullAuthenticator unauthenticated;


TemplatesHttpResource::TemplatesHttpResource(TemplatesService *templates_service)
{
    this->templates_service = templates_service;
    this->authenticator = &unauthenticated;
    this->allow_origin = "*";
    this->allow_methods = "GET";
}


TemplatesHttpResource::TemplatesHttpResource(TemplatesService *templates_service, Authenticator *authenticator)
{
    this->templates_service = templates_service;
    this->authenticator = authenticator;
    this->allow_origin = "*";
    this->allow_methods = "GET";
}


HttpResponse TemplatesHttpResource::post(HttpRequest &request)
{
    auto json = json::parse(request.body);
    Result<TemplatePublicationData> publication_data;

    publication_data = TemplatesContract::parsePost(request.body);

//  if (!this->authenticator->validCredentials(credentials)) {
//      return HttpResponse::unauthorized();
//  }

    if (publication_data.failure()) {
        return HttpResponse::badRequest();
    }

    if (this->templates_service->templateBy(publication_data.value().template_name, publication_data.value().version).isPresent()) {
        return HttpResponse::conflict();
    }

    templates_service->publishTemplate(publication_data.value());

    return HttpResponse::ok("");
}


HttpResponse TemplatesHttpResource::get(HttpRequest &request)
{
    return HttpResponse::ok("");
}

//
//static string bitAsJson(Template &bit)
//{
//    json json_bit = {
//            {"bit_name", bit.metadata.name},
//            {"version", bit.metadata.version},
//            {"payload", bit.payload}
//    };
//    return json_bit.dump();
//}

