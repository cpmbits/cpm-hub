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

#include <authentication/CpmHubAuthenticator.h>

using namespace cest;
using namespace fakeit;
using namespace std;


describe("CpmHubAuthenticator", []() {
    it("Fails to authenticate when cpm hub auth service is down", []() {
        Mock<HttpClient> mock_http_client;
        string auth_service_url("http://localhost:1234");
        CpmHubAuthenticator authenticator(auth_service_url, mock_http_client.get());
        UserCredentials credentials = {"user", "pass"};

        When(Method(mock_http_client, post)).Return(HttpResponse::notFound());

        expect(authenticator.validCredentials(credentials)).toBe(false);
    });

    it("Fails to authenticate when not authorized", []() {
        Mock<HttpClient> mock_http_client;
        string auth_service_url("http://localhost:1234");
        CpmHubAuthenticator authenticator(auth_service_url, mock_http_client.get());
        UserCredentials credentials = {"user", "pass"};

        When(Method(mock_http_client, post)).Return(HttpResponse::unauthorized());

        expect(authenticator.validCredentials(credentials)).toBe(false);
    });

    it("Confirms valid credentials when authorized", []() {
        Mock<HttpClient> mock_http_client;
        string auth_service_url("http://localhost:1234");
        CpmHubAuthenticator authenticator(auth_service_url, mock_http_client.get());
        UserCredentials credentials = {"user", "pass"};

        When(Method(mock_http_client, post)).Return(HttpResponse::ok(""));

        expect(authenticator.validCredentials(credentials)).toBe(true);

        Verify(Method(mock_http_client, post).Matching([](string url, HttpRequest request) {
            return url == "http://localhost:1234" &&
                   request.body == "{\"password\":\"7be1b497736a4478f45a07661468dd282edc01d31a403641dd3e2a07cac4fc05\",\"username\":\"user\"}" &&
                   request.headers.get("Content-type") == "application/json";
        }));
    });
});
