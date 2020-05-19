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

#include <authentication/Authenticator.h>
#include <http/http.h>
#include <bits/BitsService.h>
#include <authentication/UserCredentials.h>

class BitsApi {
public:
    BitsApi(BitsService *bits_service);

    BitsApi(BitsService *bits_service, Authenticator *authenticator);

    HttpResponse publishBit(HttpRequest &request);

    HttpResponse listBits(HttpRequest &request);

    HttpResponse downloadBit(HttpRequest &request);

private:
    BitsService *bits_service;

    Authenticator *authenticator;

    bool validCredentials(HttpRequest &request);

    UserCredentials credentialsFrom(HttpRequest &request);
};