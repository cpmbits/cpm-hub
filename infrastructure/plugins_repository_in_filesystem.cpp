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
#include <base64/base64.h>
#include <infrastructure/plugins_repository_in_filesystem.h>

using namespace std;

PluginsRepositoryInFilesystem::PluginsRepositoryInFilesystem(Filesystem *filesystem, PluginIndex *index, std::string directory)
{
    this->filesystem = filesystem;
    this->directory = directory;
    this->index = index;
}


void PluginsRepositoryInFilesystem::store(Plugin *plugin)
{
    string binary_payload = base64_decode(plugin->payload);
    string file_path = this->directory + "/public/" + plugin->file_name;

    this->filesystem->writeFile(file_path, binary_payload);
    this->index->indexPlugin(plugin);
}


Plugin *PluginsRepositoryInFilesystem::find(std::string name)
{
    return NULL;
}


std::list<Plugin *> PluginsRepositoryInFilesystem::allPlugins()
{
    return this->index->allPlugins();
}
