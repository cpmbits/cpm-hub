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

#include <infrastructure/plugin_index.h>

using namespace cest;
using namespace fakeit;


describe("Plugins Repository in file system", []() {
    it("indexes one plugin", [&]() {
        PluginIndex plugin_index;

        plugin_index.indexPlugin("cest", "user/cest/1.0");

        expect(plugin_index.serialize()).toBe(
            "{"
                "\"cest\":\"user/cest/1.0\""
            "}"
        );
    });

    it("indexes many plugins", [&]() {
        PluginIndex plugin_index;

        plugin_index.indexPlugin("cest", "user/cest/1.0");
        plugin_index.indexPlugin("fakeit", "user/fakeit/3.1");

        expect(plugin_index.serialize()).toBe(
            "{"
                "\"cest\":\"user/cest/1.0\","
                "\"fakeit\":\"user/fakeit/3.1\""
            "}"
        );
    });

    it("fails to find a plugin when it's not indexed", [&]() {
        PluginIndex plugin_index;

        std::string entry = plugin_index.find("cest");

        expect(entry).toBe("");
    });

    it("finds an indexed plugin", [&]() {
        PluginIndex plugin_index;

        plugin_index.indexPlugin("cest", "user/cest/1.0");
        plugin_index.indexPlugin("fakeit", "user/fakeit/3.1");

        expect(plugin_index.find("cest")).toBe("user/cest/1.0");
        expect(plugin_index.find("fakeit")).toBe("user/fakeit/3.1");
    });
});
