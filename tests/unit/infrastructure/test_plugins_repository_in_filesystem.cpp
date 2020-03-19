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

#include <base64/base64.h>
#include <infrastructure/plugin_index.h>
#include <infrastructure/plugins_repository_in_filesystem.h>

using namespace std;
using namespace cest;
using namespace fakeit;


describe("Plugins Repository in file system", []() {
    it("stores and indexes one plugin", [&]() {
        Mock<Filesystem> mock_filesystem;
        PluginIndex plugin_index;
        PluginsRepositoryInFilesystem repository(&mock_filesystem.get(), &plugin_index, ".");
        Plugin plugin("cest", "1.0", "user", "cGx1Z2luIHBheWxvYWQ=");

        When(Method(mock_filesystem, createDirectory)).AlwaysReturn();
        When(Method(mock_filesystem, writeFile)).AlwaysReturn();

        repository.add(plugin);

        Verify(Method(mock_filesystem, createDirectory).Using("./user/cest/1.0"));
        Verify(Method(mock_filesystem, writeFile).Using("./user/cest/1.0/cest.zip", "plugin payload"));
        Verify(Method(mock_filesystem, writeFile).Using(
            "./user/cest/1.0/cest.json", 
            "{\"name\":\"cest\",\"user_name\":\"user\",\"version\":\"1.0\"}"
        ));
    });

    it("doesn't find a non existent plugin", []() {
        Mock<Filesystem> mock_filesystem;
        PluginIndex plugin_index;
        PluginsRepositoryInFilesystem repository(&mock_filesystem.get(), &plugin_index, ".");
        Optional<Plugin> plugin;

        plugin = repository.find("cest");

        expect(plugin.isPresent()).toBe(false);
    });

    it("finds an indexed plugin", [&]() {
        Mock<Filesystem> mock_filesystem;
        Mock<PluginIndex> mock_plugin_index;
        PluginsRepositoryInFilesystem repository(&mock_filesystem.get(), &mock_plugin_index.get(), ".");
        Optional<Plugin> plugin;
        Optional<string> directory;

        directory = string("./user/cest/1.0");
        When(Method(mock_plugin_index, find)).Return(directory);
        When(Method(mock_filesystem, readFile))
                .Return("{\"name\":\"cest\",\"user_name\":\"user\",\"version\":\"1.0\"}")
                .Return("plugin payload");

        try {
            plugin = repository.find("cest");
        } catch(std::exception e) {
            cout << e.what() << endl;
        }

        expect(plugin.value().metadata.name).toBe("cest");
        expect(plugin.value().metadata.version).toBe("1.0");
        expect(plugin.value().metadata.user_name).toBe("user");
        expect(plugin.value().payload).toBe("cGx1Z2luIHBheWxvYWQ=");
    });
});
