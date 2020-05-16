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

#include <plugins/PluginIndex.h>

using namespace cest;
using namespace fakeit;
using namespace std;


describe("Plugins Repository in file system", []() {
    it("indexes one plugin", [&]() {
        PluginIndex plugin_index;

        plugin_index.indexPlugin("cest", std::string(), "user/cest");

        expect(plugin_index.serialize()).toBe(
            "{\"__version__\":\"1\",\"cest\":{\"directory\":\"user/cest\",\"username\":\"\"}}"
        );
    });

    it("indexes many plugins", [&]() {
        PluginIndex plugin_index;

        plugin_index.indexPlugin("cest", std::string(), "user/cest");
        plugin_index.indexPlugin("fakeit", std::string(), "user/fakeit");

        expect(plugin_index.serialize()).toBe(
            "{"
              "\"__version__\":\"1\","
              "\"cest\":{"
                "\"directory\":\"user/cest\","
                "\"username\":\"\"},"
              "\"fakeit\":{"
                "\"directory\":\"user/fakeit\","
                "\"username\":\"\""
              "}"
            "}"
        );
    });

    it("fails to find a plugin when it's not indexed", [&]() {
        PluginIndex plugin_index;
        Optional<string> directory;

        directory = plugin_index.find("cest");

        expect(directory.isPresent()).toBe(false);
    });

    it("finds an indexed plugin", [&]() {
        PluginIndex plugin_index;
        Optional<string> directory;

        plugin_index.indexPlugin("cest", std::string(), "user/cest/1.0");
        plugin_index.indexPlugin("fakeit", std::string(), "user/fakeit/3.1");

        expect(plugin_index.find("cest").value()).toBe("user/cest/1.0");
        expect(plugin_index.find("fakeit").value()).toBe("user/fakeit/3.1");
    });

    if("loads the index from serialized dump", []() {
        PluginIndex plugin_index;
        string serialized(
        "{"
            "\"cest\":\"user/cest/1.0\","
            "\"fakeit\":\"user/fakeit/3.1\""
        "}");

        plugin_index.restore(serialized);

        expect(plugin_index.find("cest").value()).toBe("user/cest/1.0");
        expect(plugin_index.find("fakeit").value()).toBe("user/fakeit/3.1");
    });
});
