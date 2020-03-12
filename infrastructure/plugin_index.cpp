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
#include <iostream>
#include <json/json.hpp>
#include <infrastructure/plugin_index.h>

using namespace std;
using namespace nlohmann;


void PluginIndex::indexPlugin(std::string name, std::string directory)
{
    this->plugins.insert(make_pair(name, directory));
}


Optional<string> PluginIndex::find(string name)
{
    auto iter = this->plugins.find(name);
    Optional<string> directory;

    if (iter == this->plugins.end()) {
        return directory;
    }

    directory = iter->second;

    return directory;
}


string PluginIndex::serialize()
{
    json json_index;
    string plugin_name;
    string directory;

    for (auto iter : this->plugins) {
        plugin_name = iter.first;
        directory = iter.second;
        json_index[plugin_name] = directory;
    }

    return json_index.dump();
}