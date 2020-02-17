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
#include <unistd.h>
#include <iostream>

#include <infrastructure/plugins_repository_in_memory.h>
#include <infrastructure/http_server.h>
#include <domain/plugins_service.h>
#include <api/plugins_api.h>


static PluginsRepositoryInMemory plugins_repository;
static PluginsService plugins_service(&plugins_repository);
static PluginsApi plugins_api(&plugins_service);


void installRoutes(HttpServer& http_server)
{
    http_server.post("/plugins", [&](struct http_request request) -> struct http_response {
        return plugins_api.registerPlugin(request);
    });
}
