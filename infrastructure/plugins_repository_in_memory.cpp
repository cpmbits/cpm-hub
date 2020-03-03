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
#include <infrastructure/plugins_repository_in_memory.h>


void PluginsRepositoryInMemory::store(Plugin *plugin)
{
    this->plugins.insert(std::make_pair(plugin->metadata.name, plugin));
}


Plugin *PluginsRepositoryInMemory::find(std::string name)
{
    auto iter = this->plugins.find(name);

    if (iter == this->plugins.end()) {
        return NULL;
    }

    return iter->second;
}


std::list<Plugin *> PluginsRepositoryInMemory::allPlugins()
{
    std::list<Plugin *> stored_plugins;

    for (std::pair<std::string, Plugin *> iter : this->plugins) {
        stored_plugins.push_back(iter.second);
    }

    return stored_plugins;
}
