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
#include <list>
#include <string>

#include <infrastructure/Optional.h>
#include <infrastructure/Filesystem.h>
#include <plugins/PluginMetadata.h>


struct PluginIndexEntry {
    std::string username;
    std::string directory;
};


class PluginIndex {
public:
    virtual void indexPlugin(std::string name, std::string username, std::string directory);

    virtual Optional<std::string> find(std::string name);

    virtual Optional<std::string> find(std::string name, std::string version);

    virtual std::string serialize();

    virtual void restore(std::string serialized);

private:
    const std::string index_version = "1";
    Filesystem *filesystem;
    std::string directory;
    std::map<std::string, PluginIndexEntry> plugins;

    void restoreFromVersion0(std::string serialized);
};
