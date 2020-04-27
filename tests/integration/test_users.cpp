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
#include <cest/cest.h>

#include <users/rest_api/UsersApi.h>
#include <users/UsersService.h>
#include <users/UsersRepositoryInMemory.h>
#include <http/http.h>

using namespace cest;


describe("CPM Hub users management", []() {
    it("registers a user", [&]() {
        HttpRequest request("{"
            "\"user_name\": \"juancho\","
            "\"password\": \"123456\","
            "\"email\": \"juancho@encho.com\""
        "}");
        HttpResponse response;
        UsersRepositoryInMemory repository;
        UsersService service(&repository);
        UsersApi api(&service);

        response = api.registerUser(request);

        expect(response.status_code).toBe(200);
    });
});
