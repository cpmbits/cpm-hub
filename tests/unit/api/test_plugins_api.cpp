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
#include <list>

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
        struct http_request request("{\"plugin_name\": \"cest\"}");
        struct http_response response;
        Plugin plugin("");
        Mock<PluginsService> mock_service;
        PluginsApi api(&mock_service.get());

        When(Method(mock_service, registerPlugin)).Return(&plugin);

        response = api.registerPlugin(request);

        expect(response.status_code).toBe(200);
        expect(response.body).toBe("");
        Verify(Method(mock_service, registerPlugin).Using("cest"));
    });
    
    it("uses the plugin service to list the available plugins", [&]() {
        struct http_response response;
        Plugin plugin("cest");
        std::list<Plugin *> plugins {&plugin};
        Mock<PluginsService> mock_service;
        PluginsApi api(&mock_service.get());

        When(Method(mock_service, allPlugins)).Return(plugins);

        response = api.listPlugins(http_request(""));

        expect(response.status_code).toBe(200);
        expect(response.body).toBe("[{\"plugin_name\":\"cest\"}]");
    });
});
