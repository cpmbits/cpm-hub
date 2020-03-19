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
#include <domain/plugins_repository.h>
#include <infrastructure/filesystem.h>
#include <infrastructure/plugin_index.h>


class PluginsRepositoryInFilesystem: public PluginsRepository {
public:
    PluginsRepositoryInFilesystem(Filesystem *filesystem, PluginIndex *index);

    virtual void add(Plugin &plugin);

    virtual Optional<Plugin> find(std::string name);

    virtual std::list<Plugin> allPlugins();

    void restore(std::string directory);

private:
    std::string directory;
    std::string index_file;
    Filesystem *filesystem;
    PluginIndex *index;

    void savePayload(std::string name, std::string plugin_directory, std::string base64_payload);

    void saveMetadata(std::string name, std::string plugin_directory, PluginMetadata metadata);

    std::string loadPayload(std::string name, std::string plugin_directory);

    PluginMetadata loadMetadata(std::string name, std::string plugin_directory);
};
