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
#include <users/rest_api/UsersApi.h>

using namespace nlohmann;
using namespace std;


UsersApi::UsersApi(UsersService *users_service)
{
    this->users_service = users_service;
}


HttpResponse UsersApi::registerUser(HttpRequest &request)
{
    HttpResponse response(200, "");
    auto json = json::parse(request.body);
    struct UserRegistrationData registration_data;
    Optional<string> username;

    try {
        registration_data.invitation_token = json.at("invitation_token");
        registration_data.username = json.at("username");
        registration_data.password = json.at("password");
        registration_data.email = json.at("email");

        this->users_service->registerUser(registration_data);

    } catch (InvalidInvitationToken &) {
        return HttpResponse::unauthorized();
    } catch (UsernameAlreadyTaken &) {
        return HttpResponse::conflict();
    } catch (json::out_of_range &) {
        return HttpResponse::badRequest();
    }

    return response;
}
