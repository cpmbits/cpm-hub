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

#include <bits/BitMetadata.h>

class Bit {
public:
    Bit() : Bit("") {
    } 

    Bit(std::string name) : Bit("", "") {
        this->metadata.name = name;
    }

    Bit(std::string name, std::string payload) {
        this->metadata.name = name;
        this->metadata.user_name = "";
        this->metadata.version = "";
        this->payload = payload;
    }

    Bit(std::string name, std::string version, std::string user_name, std::string payload) {
        this->metadata.name = name;
        this->metadata.user_name = user_name;
        this->metadata.version = version;
        this->payload = payload;
    }

    bool operator==(const Bit &other) {
        return this->metadata.name == other.metadata.name;
    }

    BitMetadata metadata;
    std::string payload;
};
