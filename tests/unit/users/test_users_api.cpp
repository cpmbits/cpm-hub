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

#include <users/rest_api/UsersHttpResource.h>
#include <authentication/TrivialAuthenticator.h>

using namespace cest;
using namespace fakeit;


describe("Users HTTP Resource", []() {
    it("reports allowed origin and methods", []() {
        Mock<UsersService> mock_service;
        UsersHttpResource users_resource(&mock_service.get());

        expect(users_resource.allow_methods).toBe("POST");
        expect(users_resource.allow_origin).toBe("*");
    });

    it("returns bad request when invitation token is not provided", []() {
        HttpRequest request("{"
            "\"username\": \"juancho\","
            "\"password\": \"123456\","
            "\"email\": \"juancho@encho.com\""
        "}");
        HttpResponse response;
        UserRegistrationData registration_data;
        User user("mengano");
        Mock<UsersService> mock_service;
        TrivialAuthenticator authenticator;
        UsersHttpResource users_resource(&mock_service.get());

        response = users_resource.post(request);

        expect(response.status_code).toBe(HttpStatus::BAD_REQUEST);
    });

    it("uses the users service to register a user when provided api key is valid", []() {
        HttpRequest request("{"
            "\"invitation_token\": \"cafecafe\","
            "\"username\": \"juancho\","
            "\"password\": \"123456\","
            "\"email\": \"juancho@encho.com\""
        "}");
        HttpResponse response;
        UserRegistrationData registration_data;
        User user("mengano");
        Mock<UsersService> mock_service;
        TrivialAuthenticator authenticator;
        UsersHttpResource users_resource(&mock_service.get());

        When(Method(mock_service, registerUser)).AlwaysDo([&](struct UserRegistrationData &data) {
            registration_data = data;
            return user;
        });

        response = users_resource.post(request);

        expect(response.status_code).toBe(HttpStatus::OK);
        expect(registration_data.invitation_token).toBe("cafecafe");
        expect(registration_data.username).toBe("juancho");
        expect(registration_data.password).toBe("123456");
        expect(registration_data.email).toBe("juancho@encho.com");
    });

    it("returns unauthenticated when register user raises invalid invitation token", []() {
        HttpRequest request("{"
            "\"invitation_token\": \"cafecafe\","
            "\"username\": \"juancho\","
            "\"password\": \"123456\","
            "\"email\": \"juancho@encho.com\""
        "}");
        HttpResponse response;
        UserRegistrationData registration_data;
        User user("mengano");
        Mock<UsersService> mock_service;
        TrivialAuthenticator authenticator;
        UsersHttpResource users_resource(&mock_service.get());

        When(Method(mock_service, registerUser)).AlwaysDo([&](struct UserRegistrationData &data) {
            throw InvalidInvitationToken();
            return user;
        });

        response = users_resource.post(request);

        expect(response.status_code).toBe(HttpStatus::UNAUTHORIZED);
    });

    it("returns conflict when register user raises username already taken", []() {
        HttpRequest request("{"
            "\"invitation_token\": \"cafecafe\","
            "\"username\": \"juancho\","
            "\"password\": \"123456\","
            "\"email\": \"juancho@encho.com\""
        "}");
        HttpResponse response;
        struct UserRegistrationData registration_data;
        User user("mengano");
        Mock<UsersService> mock_service;
        TrivialAuthenticator authenticator;
        UsersHttpResource users_resource(&mock_service.get());

        When(Method(mock_service, registerUser)).AlwaysDo([&](struct UserRegistrationData &data) {
            throw UsernameAlreadyTaken("");
            return user;
        });

        response = users_resource.post(request);

        expect(response.status_code).toBe(HttpStatus::CONFLICT);
    });
});
