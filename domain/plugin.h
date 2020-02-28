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

#include <string>

class PluginMetadata {
public:
    PluginMetadata() {
        this->name = "";
        this->file_name = "";
        this->version = "";
    }

    PluginMetadata(std::string name, std::string file_name, std::string version) {
        this->name = name;
        this->file_name = file_name;
        this->version = version;
    }

    std::string name;
    std::string file_name;
    std::string version;
};

class Plugin {
public:
    Plugin() : metadata("", "", "") {
        this->payload = "";
    } 

    Plugin(std::string name) : metadata(name, "", "")  {
        this->payload = "";
    }

    Plugin(std::string name, std::string file_name, std::string payload) : metadata(name, file_name, "")  {
        this->payload = payload;
    }

    bool operator==(const Plugin &other) {
        return this->metadata.name == other.metadata.name;
    }

    PluginMetadata metadata;
    std::string payload;
};
