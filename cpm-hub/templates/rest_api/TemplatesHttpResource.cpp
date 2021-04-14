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
    Result<TemplatePublicationData> result;
    UserCredentials credentials;

    result = TemplatesContract::parsePost(request.body);

    if (result.failure()) {
        return HttpResponse::badRequest();
    }

    credentials = UserCredentials {result.value().username, result.value().password };
    if (!authenticator->validCredentials(credentials)) {
        return HttpResponse::unauthorized();
    }

    if (templates_service->exists(result.value().template_name, result.value().version)) {
        return HttpResponse::conflict();
    }

    templates_service->publishTemplate(result.value());

    return HttpResponse::ok("");
}


static string templateAsJson(Template &templat)
{
    json json_bit = {
        {"template_name", templat.name},
        {"version",       templat.version},
        {"payload",       templat.payload}
    };
    return json_bit.dump();
}


HttpResponse TemplatesHttpResource::get(HttpRequest &request)
{
    string version;
    Maybe<Template> read_template;

    if (!request.parameters.has("templateName")) {
        return HttpResponse::badRequest();
    }

    version = request.parameters.has("version") ? request.parameters.get("version") : "latest";
    read_template = templates_service->templateBy(request.parameters.get("templateName"), version);

    if (!read_template.isPresent()) {
        return HttpResponse::notFound();
    }

    return HttpResponse::ok(templateAsJson(read_template.value()));
}

