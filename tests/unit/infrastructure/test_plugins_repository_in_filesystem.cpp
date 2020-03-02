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
#include <infrastructure/plugins_repository_in_filesystem.h>

using namespace std;
using namespace cest;
using namespace fakeit;


describe("Plugins Repository in file system", []() {
    beforeEach([&]() {
    });

    afterEach([&]() {
    });

    it("stores a plugin", [&]() {
        Mock<Filesystem> mock_filesystem;
        PluginsRepositoryInFilesystem repository(&mock_filesystem.get(), ".");
        Plugin plugin("cest", "cest.zip", "cGx1Z2luIHBheWxvYWQ=");
        string base64_decode(string const&);

        When(Method(mock_filesystem, writeFile)).AlwaysReturn();

        repository.store(&plugin);

        Verify(Method(mock_filesystem, writeFile).Using("./public/cest.zip", "plugin payload"));
        Verify(Method(mock_filesystem, writeFile).Using("./public/plugin_index.json", "{}"));
    });

    xit("lists stored plugins", [&]() {
        Mock<Filesystem> mock_filesystem;
        PluginsRepositoryInFilesystem repository(&mock_filesystem.get(), ".");
        Plugin plugin("cest", "cest.zip", "cGx1Z2luIHBheWxvYWQ=");
        string base64_decode(string const&);

        When(Method(mock_filesystem, writeFile)).AlwaysReturn();
        repository.store(&plugin);

        Verify(Method(mock_filesystem, writeFile).Matching([](string file_name, string contents) {
            return file_name == "./public/cest.zip" &&
                   contents == "plugin payload";
        })).Exactly(Once);
    });

    xit("lists stored plugins", [&]() {
    });

    xit("doesn't find a plugin when it's not stored", [&]() {
    });

    xit("finds the plugin with the same name when one plugin is stored", [&]() {
    });

    xit("finds the plugin with the same name when many plugins are stored", [&]() {
    });
});
