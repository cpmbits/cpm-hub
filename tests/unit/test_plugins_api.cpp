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

#include <api/plugins_api.h>
#include <domain/plugins_service.h>

using namespace cest;
using namespace fakeit;


describe("Plugins API", []() {
    beforeEach([&]() {
    });

    afterEach([&]() {
    });

    it("uses the plugin service to register a plugin", [&]() {
        struct http_request request("{\"name\": \"cest\"}");
        struct http_response response;
        Mock<PluginsService> mock;
        PluginsApi api(&mock.get());

        When(Method(mock, registerPlugin)).Return(Plugin(""));

        response = api.registerPlugin(request);

        expect(response.status_code).toBe(200);
        expect(response.body).toBe("");
        Verify(Method(mock, registerPlugin).Using("cest"));
    });
});
