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

#include <api/management_api.h>

using namespace cest;
using namespace fakeit;


class MockDeployService: public DeployService {
public:
    MockDeployService(): DeployService(NULL) {}

    void deploy(const std::string &payload, const std::string &version, const std::string &api_key) override {
        arg_payload = payload;
        arg_version = version;
        arg_api_key = api_key;
    }

    std::string arg_payload;
    std::string arg_version;
    std::string arg_api_key;
};


describe("Management API", []() {
    it("uses the deploy service to deploy cpm-hub", []() {
        HttpRequest request("{"
                                "\"payload\":\"123456789\","
                                "\"version\":\"987654321\""
                            "}");
        HttpResponse response(200, "");
        MockDeployService mock_service;
        ManagementApi api(&mock_service);

        request.headers.set("API_KEY", "cafecafe");

        response = api.deploy(request);

        expect(mock_service.arg_payload).toBe("123456789");
        expect(mock_service.arg_version).toBe("987654321");
        expect(mock_service.arg_api_key).toBe("cafecafe");
    });

    it("deploy returns error 401 on authentication failure", []() {
        HttpRequest request("{\"payload\":\"123456789\",\"version\":\"987654321\"}");
        HttpResponse response(200, "");
        Mock<DeployService> mock_service;
        ManagementApi api(&mock_service.get());

        request.headers.set("API_KEY", "cafecafe");
        When(Method(mock_service, deploy)).Throw(AuthenticationFailure());

        response = api.deploy(request);

        expect(response.status_code).toBe(401);
    });
});
