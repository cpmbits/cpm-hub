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


using namespace std;


void installServiceRoutes(HttpServer& http_server, BitsApi *bits_api)
{
    http_server.post("/bits", [bits_api](HttpRequest &request) -> HttpResponse {
        return bits_api->publishBit(request);
    });
    http_server.get("/bits/:bitName", [bits_api](HttpRequest &request) -> HttpResponse {
        return bits_api->downloadBit(request);
    });
    http_server.get("/bits/:bitName/:bitVersion", [bits_api](HttpRequest &request) -> HttpResponse {
        return bits_api->downloadBit(request);
    });
}


void installManagementRoutes(HttpServer &http_server, ManagementApi *management_api)
{
    http_server.post("/deploy", [management_api](HttpRequest &request) -> HttpResponse {
        return management_api->deploy(request);
    });
}
