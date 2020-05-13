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
#include <boost/algorithm/string.hpp>
#include <json/json.hpp>
#include <plugins/PluginIndex.h>

using namespace std;
using namespace nlohmann;


void PluginIndex::indexPlugin(string name, string username, std::string directory)
{
    PluginIndexEntry index_entry = {username, directory};

    this->plugins[name] = index_entry;
}


Optional<string> PluginIndex::find(string name)
{
    auto iter = this->plugins.find(name);
    Optional<string> directory;

    if (iter == this->plugins.end()) {
        return directory;
    }

    directory = iter->second.directory;

    return directory;
}


Optional<std::string> PluginIndex::find(std::string name, std::string version)
{
    return Optional<std::string>();
}


string PluginIndex::serialize()
{
    json json_index;
    string plugin_name;
    PluginIndexEntry index_entry;

    json_index["__version__"] = this->index_version;
    for (auto iter : this->plugins) {
        plugin_name = iter.first;
        index_entry = iter.second;
        json_index[plugin_name]["username"] = index_entry.username;
        json_index[plugin_name]["directory"] = index_entry.directory;
    }

    return json_index.dump();
}


void PluginIndex::restore(std::string serialized)
{
    auto json = json::parse(serialized);

    if (!json.contains("__version__")) {
        this->restoreFromVersion0(serialized);
    }

    for (auto& element: json.items()) {
        this->indexPlugin(element.key(), element.value().at("username"), element.value().at("directory"));
    }
}


void PluginIndex::restoreFromVersion0(std::string serialized)
{
    auto json = json::parse(serialized);

    for (auto& element: json.items()) {
        string directory = element.value();
        vector<string> tokens;

        boost::split(tokens, directory, boost::is_any_of("/"));
        this->indexPlugin(element.key(), tokens.at(0), tokens.at(1));
    }
}
