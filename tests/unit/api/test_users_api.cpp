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
#include <fakeit/fakeit.hpp>

#include <domain/users/api/users_api.h>

using namespace cest;
using namespace fakeit;


describe("Users API", []() {
    it("uses the users service to register a user", []() {
        HttpRequest request("{"
            "\"user_name\": \"juancho\","
            "\"password\": \"123456\","
            "\"email\": \"juancho@encho.com\""
        "}");
        HttpResponse response;
        struct user_registration_data registration_data;
        User user("mengano");
        Mock<UsersService> mock_service;
        UsersApi api(&mock_service.get());

        When(Method(mock_service, registerUser)).AlwaysDo([&](struct user_registration_data &data) {
            registration_data = data;
            return user;
        });

        response = api.registerUser(request);

        expect(response.status_code).toBe(200);
        expect(registration_data.user_name).toBe("juancho");
        expect(registration_data.password).toBe("123456");
        expect(registration_data.email).toBe("juancho@encho.com");
    });
});
