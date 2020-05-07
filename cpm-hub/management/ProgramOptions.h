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
#include <http/http.h>

struct ProgramOptions {
    enum AuthenticatorType {
        UNAUTHENTICATED,
        ACCESS_FILE_AUTHENTICATOR,
        CPM_HUB_AUTHENTICATOR,
    };
    std::string plugins_directory = ".";
    AuthenticatorType authenticator_type = UNAUTHENTICATED;
    std::string access_file = ".access";
    std::string cpm_hub_url = "http://localhost:1234";
    int http_service_port = 8000;
    int http_management_port = 8001;
    HttpSecurityOptions security_options;
};
