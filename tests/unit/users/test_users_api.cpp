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

#include <users/rest_api/UsersApi.h>
#include <authentication/TrivialAuthenticator.h>

using namespace cest;
using namespace fakeit;


describe("Users API", []() {
    it("returns unauthorized when api key is not provided", []() {
        HttpRequest request("{"
            "\"username\": \"juancho\","
            "\"password\": \"123456\","
            "\"email\": \"juancho@encho.com\""
        "}");
        HttpResponse response;
        struct UserRegistrationData registration_data;
        User user("mengano");
        Mock<UsersService> mock_service;
        TrivialAuthenticator authenticator;
        UsersApi api(&mock_service.get(), &authenticator);

        response = api.registerUser(request);

        expect(response.status_code).toBe(401);
    });

    it("returns unauthorized when provided api key is invalid", []() {
        HttpRequest request("{"
            "\"username\": \"juancho\","
            "\"password\": \"123456\","
            "\"email\": \"juancho@encho.com\""
        "}");
        HttpResponse response;
        struct UserRegistrationData registration_data;
        User user("mengano");
        Mock<UsersService> mock_service;
        TrivialAuthenticator authenticator;
        UsersApi api(&mock_service.get(), &authenticator);

        request.headers.set("API_KEY", "cafecafe");

        response = api.registerUser(request);

        expect(response.status_code).toBe(401);
    });

    it("returns unauthorized when provided api key is not valid admin api key", []() {
        HttpRequest request("{"
                            "\"username\": \"juancho\","
                            "\"password\": \"123456\","
                            "\"email\": \"juancho@encho.com\""
                            "}");
        HttpResponse response;
        struct UserRegistrationData registration_data;
        User user("mengano");
        Mock<UsersService> mock_service;
        TrivialAuthenticator authenticator;
        UsersApi api(&mock_service.get(), &authenticator);
        UserCredentials user_credentials = {"user", "afecafec"};
        UserCredentials admin_credentials = {"admin", "cafecafe"};

        authenticator.addUser(user_credentials);
        authenticator.addUser(admin_credentials);
        request.headers.set("API_KEY", "afecafec");

        response = api.registerUser(request);

        expect(response.status_code).toBe(401);
    });

    it("uses the users service to register a user when provided api key is valid", []() {
        HttpRequest request("{"
            "\"username\": \"juancho\","
            "\"password\": \"123456\","
            "\"email\": \"juancho@encho.com\""
        "}");
        HttpResponse response;
        struct UserRegistrationData registration_data;
        User user("mengano");
        Mock<UsersService> mock_service;
        TrivialAuthenticator authenticator;
        UsersApi api(&mock_service.get(), &authenticator);
        UserCredentials credentials = {"admin", "cafecafe"};

        When(Method(mock_service, registerUser)).AlwaysDo([&](struct UserRegistrationData &data) {
            registration_data = data;
            return user;
        });
        authenticator.addUser(credentials);
        request.headers.set("API_KEY", "cafecafe");

        response = api.registerUser(request);

        expect(response.status_code).toBe(200);
        expect(registration_data.user_name).toBe("juancho");
        expect(registration_data.password).toBe("123456");
        expect(registration_data.email).toBe("juancho@encho.com");
    });
});
