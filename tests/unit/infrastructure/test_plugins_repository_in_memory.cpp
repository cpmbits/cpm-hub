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

#include <domain/plugins/plugins_repository_in_memory.h>

using namespace cest;


describe("Plugins Repository in Memory", []() {
    it("stores a plugin", [&]() {
        PluginsRepositoryInMemory repository;
        Plugin plugin("cest");

        repository.add(plugin);
    });

    it("lists stored plugins", [&]() {
        PluginsRepositoryInMemory repository;
        Plugin plugin("cest");
        std::list<Plugin> stored_plugins;

        repository.add(plugin);

        stored_plugins = repository.allPlugins();

        expect(stored_plugins.size()).toBe(1);
    });

    it("doesn't find a plugin when it's not stored", [&]() {
        PluginsRepositoryInMemory repository;
        Optional<Plugin> plugin;

        plugin = repository.find("cest");

        expect(plugin.isPresent()).toBe(false);
    });

    it("finds the plugin with the same name when one plugin is stored", [&]() {
        PluginsRepositoryInMemory repository;
        Plugin plugin("cest");
        Optional<Plugin> stored_plugin;

        repository.add(plugin);

        stored_plugin = repository.find("cest");

        expect(stored_plugin.value().metadata.name).toBe(plugin.metadata.name);
    });

    it("finds the plugin with the same name when many plugins are stored", [&]() {
        PluginsRepositoryInMemory repository;
        Plugin cest_plugin("cest"), fakeit_plugin("fakeit");
        Optional<Plugin> stored_plugin;

        repository.add(cest_plugin);
        repository.add(fakeit_plugin);

        stored_plugin = repository.find("fakeit");

        expect(stored_plugin.value().metadata.name).toBe(fakeit_plugin.metadata.name);
    });
});
