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

#include <api/plugins_api.h>
#include <domain/plugins_service.h>

using namespace cest;


class MockPluginsService : public PluginsService {
public:
    int register_plugin_calls;
    std::string register_plugin_name;

    MockPluginsService() : PluginsService(NULL) {
    }

    Plugin registerPlugin(std::string name) {
        register_plugin_calls++;
        register_plugin_name = name;
        return Plugin();
    }
};



describe("Plugins API", []() {
    beforeEach([&]() {
    });

    afterEach([&]() {
    });

    it("uses the plugin service to register a plugin", [&]() {
        struct http_request request(
            "{\"name\": \"cest\"}"
        );
        struct http_response response;
        MockPluginsService mock_service;
        PluginsApi api(&mock_service);

        response = api.registerPlugin(request);

        expect(response.status_code).toBe(200);
        expect(response.body).toBe("");
        expect(mock_service.register_plugin_calls).toBe(1);
        expect(mock_service.register_plugin_name).toBe("cest");
    });
});
