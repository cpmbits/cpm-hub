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
#include <plugins/PluginsRepositoryInMemory.h>

using namespace std;


static bool compareVersion(const Plugin& first, const Plugin& second)
{
    return first.metadata.version < second.metadata.version;
}


void PluginsRepositoryInMemory::add(Plugin &plugin)
{
    if (!pluginExists(plugin)) {
        this->plugins[plugin.metadata.name] = list<Plugin>();
    }

    this->plugins[plugin.metadata.name].push_back(plugin);
    this->plugins[plugin.metadata.name].sort(compareVersion);
}


bool PluginsRepositoryInMemory::pluginExists(const Plugin &plugin) const
{
    return plugins.find(plugin.metadata.name) != plugins.end();
}


Optional<Plugin> PluginsRepositoryInMemory::find(string name)
{
    Optional<Plugin> plugin;

    if (!pluginExists(name)) {
        return plugin;
    }

    plugin = this->plugins[name].back();
    
    return plugin;
}


Optional<Plugin> PluginsRepositoryInMemory::find(string name, string version)
{
    Optional<Plugin> plugin;

    if (!pluginExists(name)) {
        return plugin;
    }

    for (auto &plugin_version: this->plugins[name]) {
        if (plugin_version.metadata.version == version) {
            plugin = plugin_version;
        }
    }

    return plugin;
}


list<Plugin> PluginsRepositoryInMemory::allPlugins()
{
    list<Plugin> stored_plugins;

    for (auto &plugin_map: this->plugins) {
        for (auto &plugin: plugin_map.second) {
            stored_plugins.push_back(plugin);
        }
    }

    return stored_plugins;
}
