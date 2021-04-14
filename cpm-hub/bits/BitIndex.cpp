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
#include <boost/algorithm/string.hpp>
#include <json/json.hpp>
#include <bits/BitIndex.h>

using namespace std;
using namespace nlohmann;


void BitIndex::indexBit(string name, string username, string directory)
{
    BitIndexEntry index_entry = {name, username, directory};

    this->bits[name] = index_entry;
}


Maybe<string> BitIndex::find(string name)
{
    auto iter = this->bits.find(name);
    Maybe<string> directory;

    if (iter == this->bits.end()) {
        return directory;
    }

    directory = iter->second.directory;

    return directory;
}


string BitIndex::serialize()
{
    json json_index;
    string bit_name;
    BitIndexEntry index_entry;

    json_index["__version__"] = this->index_version;
    for (auto iter : this->bits) {
        bit_name = iter.first;
        index_entry = iter.second;
        json_index[bit_name]["username"] = index_entry.username;
        json_index[bit_name]["directory"] = index_entry.directory;
    }

    return json_index.dump();
}


void BitIndex::restore(string serialized)
{
    auto json = json::parse(serialized);

    if (!json.contains("__version__")) {
        this->restoreFromVersion0(serialized);
        return;
    }

    for (auto& element: json.items()) {
        if (element.key() != "__version__") {
            this->indexBit(element.key(), element.value().at("username"), element.value().at("directory"));
        }
    }
}


void BitIndex::restoreFromVersion0(string serialized)
{
    auto json = json::parse(serialized);

    for (auto& element: json.items()) {
        string directory = element.value();
        vector<string> tokens;

        boost::split(tokens, directory, boost::is_any_of("/"));
        this->indexBit(element.key(), tokens.at(0), tokens.at(0)+"/"+tokens.at(1));
    }
}

list<BitIndexEntry> BitIndex::search(BitSearchQuery search_query)
{
    list<BitIndexEntry> found_bits;

    for (auto &bit_map: this->bits) {
        if (bit_map.first.find(search_query.name) != string::npos) {
            found_bits.push_back(bit_map.second);
        }
    }

    return found_bits;
}


list<BitIndexEntry> BitIndex::allIndexedBits()
{
    list<BitIndexEntry> all_indexed_bits;

    for (auto &bit_map: this->bits) {
        all_indexed_bits.push_back(bit_map.second);
    }

    return all_indexed_bits;
}
