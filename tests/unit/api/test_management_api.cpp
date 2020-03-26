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


describe("Management API", []() {
    it("uses the deploy service to deploy cpm-hub", []() {
        struct http_request request("123456789");
        struct http_response response(200, "");
        Mock<DeployService> mock_service;
        ManagementApi api(&mock_service.get());

        When(Method(mock_service, deploy)).AlwaysReturn();

        response = api.deploy(request);

        expect(response.status_code).toBe(200);
        Verify(Method(mock_service, deploy).Using("123456789"));
    });
});
