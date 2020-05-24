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
#include <management/rest_api/ManagementApi.h>

using namespace nlohmann;

static NullAuthenticator unauthenticated;


ManagementApi::ManagementApi(DeployService *deploy_service, Logger *logger)
{
    this->deploy_service = deploy_service;
    this->logger = logger;
    this->authenticator = &unauthenticated;
}


ManagementApi::ManagementApi(DeployService *deploy_service, Logger *logger, Authenticator *authenticator)
{
    this->deploy_service = deploy_service;
    this->logger = logger;
    this->authenticator = authenticator;
}


HttpResponse ManagementApi::deploy(HttpRequest &request)
{
    auto json = json::parse(request.body);

    if (!isAuthorized(request)) {
        return HttpResponse::unauthorized();
    }

    try {
        this->deploy_service->deploy(
                json.at("payload"),
                json.at("version"),
                request.headers.get("API_KEY"));
        return HttpResponse::ok("");
    } catch (AuthenticationFailure &error) {
        return HttpResponse::unauthorized();
    }
}


HttpResponse ManagementApi::getLogs(HttpRequest &request)
{
    json json_logs = json::array();

    if (!isAuthorized(request)) {
        return HttpResponse::unauthorized();
    }

    for (std::string log : logger->snapshot()) {
        json_logs.push_back(log);
    }

    return HttpResponse::ok(json_logs.dump());
}


bool ManagementApi::isAuthorized(HttpRequest &request)
{
    return request.headers.has("API_KEY") &&
           authenticator->authenticate(request.headers.get("API_KEY").c_str()).isPresent();
}
