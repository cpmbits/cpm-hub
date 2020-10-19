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
#include <bits/BitsRepository.h>
#include <infrastructure/Filesystem.h>
#include <bits/BitIndex.h>


class BitsRepositoryInFilesystem: public BitsRepository {
public:
    BitsRepositoryInFilesystem(Filesystem *filesystem, BitIndex *index);

    BitsRepositoryInFilesystem(Filesystem *filesystem, BitIndex *index, std::string &directory);

    virtual void add(Bit &bit);

    virtual Optional<Bit> bitBy(std::string name);

    virtual Optional<Bit> bitBy(std::string name, std::string version);

    virtual std::list<BitMetadata> search(BitSearchQuery search_query);

    virtual std::list<Bit> allBits();

    void restore(std::string directory);

private:
    std::string directory;
    std::string index_file;
    Filesystem *filesystem;
    BitIndex *index;

    void savePayload(const std::string& name, const std::string& bit_directory, const std::string& base64_payload);

    void saveMetadata(const std::string& name, const std::string& bit_directory, BitMetadata metadata);

    std::string loadPayload(std::string name, std::string bit_directory);

    BitMetadata loadMetadata(const std::string& name, std::string bit_directory);

    std::string latestVersionDirectory(const std::string& base_directory);

    std::list<std::string> allVersionsForBit(const std::string &base_directory);

    std::string bitBaseDirectory(const std::string& bit_name);
};
