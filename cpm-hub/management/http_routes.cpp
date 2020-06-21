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
#include <management/http_routes.h>
#include <users/rest_api/UsersHttpResource.h>


using namespace std;


void installServiceRoutes(HttpServer &http_server, BitsHttpResource *bits_resource, UsersHttpResource *users_resource)
{
    http_server.addResource(Endpoint("/bits"), bits_resource);
    http_server.addResource(Endpoint("/bits/:bitName"), bits_resource);
    http_server.addResource(Endpoint("/bits/:bitName/:bitVersion"), bits_resource);
    http_server.addResource(Endpoint("/users"), users_resource);
}


void installManagementRoutes(HttpServer &http_server, ManagementHttpResource *management_resource)
{
}
