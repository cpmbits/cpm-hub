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
#include <bits/BitsRepositoryInMemory.h>

using namespace std;


static bool compareVersion(const Bit& first, const Bit& second)
{
    return first.metadata.version < second.metadata.version;
}


void BitsRepositoryInMemory::add(Bit &bit)
{
    if (!bitExists(bit)) {
        this->bits[bit.metadata.name] = list<Bit>();
    }

    this->bits[bit.metadata.name].push_back(bit);
    this->bits[bit.metadata.name].sort(compareVersion);
}


bool BitsRepositoryInMemory::bitExists(const Bit &bit) const
{
    return bits.find(bit.metadata.name) != bits.end();
}


Optional<Bit> BitsRepositoryInMemory::find(string name)
{
    Optional<Bit> bit;

    if (!bitExists(name)) {
        return bit;
    }

    bit = this->bits[name].back();
    
    return bit;
}


Optional<Bit> BitsRepositoryInMemory::find(string name, string version)
{
    Optional<Bit> bit;

    if (!bitExists(name)) {
        return bit;
    }

    for (auto &bit_version: this->bits[name]) {
        if (bit_version.metadata.version == version) {
            bit = bit_version;
        }
    }

    return bit;
}


list<Bit> BitsRepositoryInMemory::allBits()
{
    list<Bit> stored_bits;

    for (auto &bit_map: this->bits) {
        for (auto &bit: bit_map.second) {
            stored_bits.push_back(bit);
        }
    }

    return stored_bits;
}
