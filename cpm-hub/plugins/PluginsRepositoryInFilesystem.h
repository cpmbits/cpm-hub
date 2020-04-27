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
#pragma once

#include <map>
#include <string>
#include <plugins/PluginsRepository.h>
#include <infrastructure/Filesystem.h>
#include <plugins/PluginIndex.h>


class PluginsRepositoryInFilesystem: public PluginsRepository {
public:
    PluginsRepositoryInFilesystem(Filesystem *filesystem, PluginIndex *index);

    PluginsRepositoryInFilesystem(Filesystem *filesystem, PluginIndex *index, std::string &directory);

    virtual void add(Plugin &plugin);

    virtual Optional<Plugin> find(std::string name);

    virtual std::list<Plugin> allPlugins();

    void restore(std::string directory);

private:
    std::string directory;
    std::string index_file;
    Filesystem *filesystem;
    PluginIndex *index;

    void savePayload(const std::string& name, const std::string& plugin_directory, const std::string& base64_payload);

    void saveMetadata(const std::string& name, const std::string& plugin_directory, PluginMetadata metadata);

    std::string loadPayload(std::string name, std::string plugin_directory);

    PluginMetadata loadMetadata(const std::string& name, std::string plugin_directory);
};
