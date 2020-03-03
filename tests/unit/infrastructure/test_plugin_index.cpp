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
    beforeEach([&]() {
    });

    afterEach([&]() {
    });

    it("indexes one plugin", [&]() {
        Mock<Filesystem> mock_filesystem;
        PluginIndex plugin_index(&mock_filesystem.get(), ".");
        Plugin plugin("cest", "cGx1Z2luIHBheWxvYWQ=");

        When(Method(mock_filesystem, writeFile)).AlwaysReturn();

        plugin_index.indexPlugin(&plugin);

        Verify(Method(mock_filesystem, writeFile).Using(
            "./plugin_index.json", 
            "{\"cest\":\"cest.zip\"}"
        ));
    });
});
