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

#include <infrastructure/Maybe.h>
#include <infrastructure/Filesystem.h>
#include <bits/BitMetadata.h>
#include <bits/BitSearchQuery.h>


struct BitIndexEntry {
    std::string name;
    std::string username;
    std::string directory;
};


class BitIndex {
public:
    virtual void indexBit(std::string name, std::string username, std::string directory);

    virtual Maybe<std::string> find(std::string name);

    virtual std::string serialize();

    virtual void restore(std::string serialized);

    virtual std::list<BitIndexEntry> search(BitSearchQuery search_query);

    virtual std::list<BitIndexEntry> allIndexedBits();

private:
    const std::string index_version = "1";
    Filesystem *filesystem;
    std::string directory;
    std::map<std::string, BitIndexEntry> bits;

    void restoreFromVersion0(std::string serialized);
};
