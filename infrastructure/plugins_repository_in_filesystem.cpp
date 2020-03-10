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
#include <json/json.hpp>
#include <base64/base64.h>
#include <infrastructure/plugins_repository_in_filesystem.h>

using namespace std;
using namespace nlohmann;

PluginsRepositoryInFilesystem::PluginsRepositoryInFilesystem(Filesystem *filesystem, PluginIndex *index, string directory)
{
    this->filesystem = filesystem;
    this->directory = directory;
    this->index = index;
}


void PluginsRepositoryInFilesystem::store(Plugin &plugin)
{
    string plugin_directory = this->directory + "/" + plugin.metadata.user_name + "/" + plugin.metadata.name + "/" + plugin.metadata.version;

    this->filesystem->createDirectory(plugin_directory);
    this->savePayload(plugin.metadata.name, plugin_directory, plugin.payload);
    this->saveMetadata(plugin.metadata.name, plugin_directory, plugin.metadata);
    this->index->indexPlugin(plugin.metadata.name, plugin_directory);
}


void PluginsRepositoryInFilesystem::savePayload(string name, string plugin_directory, string base64_payload)
{
    string payload_file_path = plugin_directory + "/" + name + ".zip";
    string binary_payload = base64_decode(base64_payload);
    this->filesystem->writeFile(payload_file_path, binary_payload);
}


void PluginsRepositoryInFilesystem::saveMetadata(string name, string plugin_directory, PluginMetadata metadata)
{
    string metadata_file_path = plugin_directory + "/" + name + ".json";
    json metadata_json = {
        {"name", metadata.name},
        {"user_name", metadata.user_name},
        {"version", metadata.version},
    };
    this->filesystem->writeFile(metadata_file_path, metadata_json.dump());
}


Plugin PluginsRepositoryInFilesystem::find(string name)
{
    return Plugin();
}


list<Plugin> PluginsRepositoryInFilesystem::allPlugins()
{
    list<Plugin> plugins;
    return plugins;
}
