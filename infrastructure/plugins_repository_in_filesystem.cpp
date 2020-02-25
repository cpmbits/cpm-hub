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

#include <base64/base64.h>
#include <infrastructure/plugins_repository_in_filesystem.h>

using namespace std;

PluginsRepositoryInFilesystem::PluginsRepositoryInFilesystem(Filesystem *filesystem, std::string directory)
{
    this->filesystem = filesystem;
    this->directory = directory;
}


void PluginsRepositoryInFilesystem::store(Plugin *plugin)
{
    std::vector<BYTE> binary_payload = base64_decode(plugin->payload);
    this->filesystem->writeFile(plugin->file_name, &binary_payload.front());
}


Plugin *PluginsRepositoryInFilesystem::find(std::string name)
{
    return NULL;
}


std::list<Plugin *> PluginsRepositoryInFilesystem::allPlugins()
{
    std::list<Plugin *> stored_plugins;
    return stored_plugins;
}
