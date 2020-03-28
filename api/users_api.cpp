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
#include <api/users_api.h>

using namespace nlohmann;


UsersApi::UsersApi(UsersService *users_service)
{
    this->users_service = users_service;
}


HttpResponse UsersApi::registerUser(struct HttpRequest &request)
{
    HttpResponse response(200, "");
    auto json = json::parse(request.body);
    struct user_registration_data registration_data = {
        json.at("user_name"),
        json.at("password"),
        json.at("email"),
    };

    this->users_service->registerUser(registration_data);

    return response;
}
