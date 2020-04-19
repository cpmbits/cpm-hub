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
#include <base64/base64.h>
#include <plugins/PluginsRepositoryInFilesystem.h>

using namespace std;
using namespace nlohmann;


static string metadataFileName(const string& name, const string& directory)
{
    return directory + "/" + name + ".json";
}


static string payloadFileName(const string& name, const string& directory)
{
    return directory + "/" + name + ".zip";
}


PluginsRepositoryInFilesystem::PluginsRepositoryInFilesystem(Filesystem *filesystem, PluginIndex *index)
{
    this->filesystem = filesystem;
    this->directory = ".";
    this->index = index;
    this->index_file = this->directory + "/index.json";
}


void PluginsRepositoryInFilesystem::add(Plugin &plugin)
{
    string relative_directory = plugin.metadata.user_name + "/" + plugin.metadata.name + "/" + plugin.metadata.version;
    string plugin_directory = this->directory + "/" + relative_directory;

    this->filesystem->createDirectory(plugin_directory);
    this->savePayload(plugin.metadata.name, plugin_directory, plugin.payload);
    this->saveMetadata(plugin.metadata.name, plugin_directory, plugin.metadata);
    this->index->indexPlugin(plugin.metadata.name, relative_directory);
    this->filesystem->writeFile(this->index_file, this->index->serialize());
}


void PluginsRepositoryInFilesystem::savePayload(const string& name, const string& plugin_directory, const string& base64_payload)
{
    string payload_file_path = payloadFileName(name, plugin_directory);
    string binary_payload = base64_decode(base64_payload);
    this->filesystem->writeFile(payload_file_path, binary_payload);
}


void PluginsRepositoryInFilesystem::saveMetadata(const string& name, const string& plugin_directory, PluginMetadata metadata)
{
    string metadata_file_path = metadataFileName(name, plugin_directory);
    json metadata_json = {
        {"name", metadata.name},
        {"user_name", metadata.user_name},
        {"version", metadata.version},
    };
    this->filesystem->writeFile(metadata_file_path, metadata_json.dump());
}


Optional<Plugin> PluginsRepositoryInFilesystem::find(string name)
{
    Optional<Plugin> plugin;
    Optional<string> relative_directory;

    relative_directory = this->index->find(name);
    if (relative_directory.isPresent()) {
        string directory = this->directory + "/" + relative_directory.value();
        PluginMetadata metadata = this->loadMetadata(name, directory);
        string payload = this->loadPayload(name, directory);
        plugin = Plugin(name, metadata.version, metadata.user_name, payload);
    }

    return plugin;
}


string PluginsRepositoryInFilesystem::loadPayload(string name, string plugin_directory)
{
    string payload_file_path = payloadFileName(name, plugin_directory);
    string payload = this->filesystem->readFile(payload_file_path);
    return base64_encode((const unsigned char *)payload.c_str(), payload.size());
}


PluginMetadata PluginsRepositoryInFilesystem::loadMetadata(const string& name, string plugin_directory)
{
    string metadata_file_path = metadataFileName(name, plugin_directory);
    string metadata = this->filesystem->readFile(metadata_file_path);
    auto metadata_json = json::parse(metadata);
    return PluginMetadata(name, metadata_json.at("user_name"), metadata_json.at("version"));
}


list<Plugin> PluginsRepositoryInFilesystem::allPlugins()
{
    list<Plugin> plugins;
    return plugins;
}


void PluginsRepositoryInFilesystem::restore(string directory)
{
    this->directory = directory;
    this->index_file = this->directory + "/index.json";

    if (this->filesystem->fileExists(this->index_file)) {
        this->index->restore(this->filesystem->readFile(this->index_file));
    }
}
