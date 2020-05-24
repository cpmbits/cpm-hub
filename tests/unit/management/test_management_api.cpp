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

#include <management/rest_api/ManagementApi.h>

using namespace cest;
using namespace fakeit;
using namespace std;


class MockDeployService: public DeployService {
public:
    MockDeployService(): DeployService(NULL) {}

    void deploy(const string &payload, const string &version, const string &api_key) override {
        arg_payload = payload;
        arg_version = version;
        arg_api_key = api_key;
    }

    string arg_payload;
    string arg_version;
    string arg_api_key;
};


describe("Management API", []() {
    it("uses the deploy service to deploy cpm-hub", []() {
        HttpRequest request("{"
                                "\"payload\":\"123456789\","
                                "\"version\":\"987654321\""
                            "}");
        HttpResponse response(200, "");
        MockDeployService mock_service;
        Mock<Logger> mock_logger;
        ManagementApi api(&mock_service, &mock_logger.get());

        request.headers.set("API_KEY", "cafecafe");

        response = api.deploy(request);

        expect(mock_service.arg_payload).toBe("123456789");
        expect(mock_service.arg_version).toBe("987654321");
        expect(mock_service.arg_api_key).toBe("cafecafe");
    });

    it("returns error 401 on authentication failure when deploying", []() {
        HttpRequest request("{\"payload\":\"123456789\",\"version\":\"987654321\"}");
        HttpResponse response(200, "");
        Mock<DeployService> mock_service;
        Mock<Logger> mock_logger;
        ManagementApi api(&mock_service.get(), &mock_logger.get());

        request.headers.set("API_KEY", "cafecafe");
        When(Method(mock_service, deploy)).Throw(AuthenticationFailure());

        response = api.deploy(request);

        expect(response.status_code).toBe(401);
    });

    it("returns error 401 on authentication failure for missing API KEY when getting logs", []() {
        HttpRequest request;
        HttpResponse response;
        Mock<DeployService> mock_service;
        Mock<Logger> mock_logger;
        Mock<Authenticator> mock_authenticator;
        ManagementApi api(&mock_service.get(), &mock_logger.get(), &mock_authenticator.get());

        When(Method(mock_authenticator, authenticate)).Return(Optional<string>());

        response = api.getLogs(request);

        expect(response.status_code).toBe(401);
    });

    it("returns error 401 on authentication failure for invalid API KEY when getting logs", []() {
        HttpRequest request;
        HttpResponse response;
        Mock<DeployService> mock_service;
        Mock<Logger> mock_logger;
        Mock<Authenticator> mock_authenticator;
        ManagementApi api(&mock_service.get(), &mock_logger.get(), &mock_authenticator.get());

        request.headers.set("API_KEY", "cafecafe");
        When(Method(mock_authenticator, authenticate)).Return(Optional<string>());

        response = api.getLogs(request);

        expect(response.status_code).toBe(401);
    });

    it("returns logs in logger as json when no logs are stored", []() {
        HttpRequest request;
        HttpResponse response;
        Mock<DeployService> mock_service;
        Mock<Logger> mock_logger;
        Mock<Authenticator> mock_authenticator;
        ManagementApi api(&mock_service.get(), &mock_logger.get(), &mock_authenticator.get());

        request.headers.set("API_KEY", "cafecafe");
        When(Method(mock_authenticator, authenticate)).Return(Optional<string>("admin"));
        When(Method(mock_logger, snapshot)).Return(vector<string>());

        response = api.getLogs(request);

        expect(response.status_code).toBe(200);
        expect(response.body).toBe("[]");
    });

    it("returns logs in logger as json when many logs are stored", []() {
        HttpRequest request;
        HttpResponse response;
        Mock<DeployService> mock_service;
        Mock<Logger> mock_logger;
        Mock<Authenticator> mock_authenticator;
        ManagementApi api(&mock_service.get(), &mock_logger.get(), &mock_authenticator.get());

        request.headers.set("API_KEY", "cafecafe");
        When(Method(mock_authenticator, authenticate)).Return(Optional<string>("admin"));
        When(Method(mock_logger, snapshot)).Return(vector<string>{"log1", "log2"});

        response = api.getLogs(request);

        expect(response.status_code).toBe(200);
        expect(response.body).toBe("[\"log1\",\"log2\"]");
    });
});
