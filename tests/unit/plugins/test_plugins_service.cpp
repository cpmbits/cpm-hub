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

#include <plugins/PluginsRepository.h>
#include <plugins/PluginsService.h>

using namespace cest;
using namespace fakeit;


describe("Plugins Service", []() {
    it("stores a plugin with the given name when registering a plugin", [&]() {
        Mock<PluginsRepository> mock_repository;
        PluginsService plugins_service(&mock_repository.get());
        Plugin expected_plugin("cest", "1.0", "john_doe", "ABCDEedcba");
        struct PluginPublicationData publication_data = {
            "cest", "1.0", "john_doe", "ABCDEedcba"
        };

        When(Method(mock_repository, add)).AlwaysReturn();

        plugins_service.publishPlugin(publication_data);

        Verify(Method(mock_repository, add));
    });

    it("returns all available plugins", [&]() {
        Mock<PluginsRepository> mock_repository;
        PluginsService plugins_service(&mock_repository.get());
        Plugin plugin("cest");
        std::list<Plugin> plugins {plugin};
        std::list<Plugin> returned_plugins;

        When(Method(mock_repository, allPlugins)).Return(plugins);

        returned_plugins = plugins_service.allPlugins();

        expect(returned_plugins.size()).toBe(1);
        expect(returned_plugins.front().metadata.name).toBe(plugin.metadata.name);
    });
    
    it("uses the repository to find a plugin by name", [&]() {
        Mock<PluginsRepository> mock_repository;
        PluginsService plugins_service(&mock_repository.get());
        Optional<Plugin> plugin;

        plugin = Plugin("cest");
        When(Method(mock_repository, find)).Return(plugin);

        auto found_plugin = plugins_service.find("cest");

        expect(found_plugin.value().metadata.name).toBe("cest");
    });
});
