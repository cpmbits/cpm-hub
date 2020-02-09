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

#include <domain/plugins_repository.h>
#include <domain/plugins_service.h>


class MockPluginsRepository : public PluginsRepository {
public:
    virtual void store(Plugin *plugin) {
        this->stored_plugin = plugin;
    }
    
    Plugin *stored_plugin;
};


describe("Plugins Service", []() {
    beforeEach([&]() {
    });

    afterEach([&]() {
    });

    it("stores a plugin with the given name when registering a plugin", [&]() {
        MockPluginsRepository mock_repository;
        PluginsService plugins_service(&mock_repository);
        Plugin expected_plugin("cest");

        plugins_service.registerPlugin("cest");

        expect(*mock_repository.stored_plugin == expected_plugin).toBe(true);

        delete mock_repository.stored_plugin;
    });
});