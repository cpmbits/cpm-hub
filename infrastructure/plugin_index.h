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

#include <list>
#include <string>

#include <infrastructure/filesystem.h>
#include <domain/plugin.h>

class PluginIndex {
public:
    PluginIndex() {};

    PluginIndex(Filesystem *filesystem, std::string directory);

    virtual void indexPlugin(Plugin *plugin);

    virtual std::list<Plugin *> allPlugins();

private:
    Filesystem *filesystem;
    std::string directory;
};