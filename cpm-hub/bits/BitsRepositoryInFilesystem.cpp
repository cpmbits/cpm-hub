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
#include <iostream>
#include <json/json.hpp>
#include <base64/base64.h>
#include <bits/BitsRepositoryInFilesystem.h>

using namespace std;
using namespace nlohmann;


static string metadataFileName(const string& name, const string& directory)
{
    return directory + "/" + name + ".json";
}


static string payloadFileName(const string& name, const string& directory)
{
    return directory + "/" + name + ".zip";
}


BitsRepositoryInFilesystem::BitsRepositoryInFilesystem(Filesystem *filesystem, BitIndex *index)
{
    this->filesystem = filesystem;
    this->directory = ".";
    this->index = index;
    this->index_file = this->directory + "/index.json";
}


BitsRepositoryInFilesystem::BitsRepositoryInFilesystem(Filesystem *filesystem, BitIndex *index, string &directory)
{
    this->filesystem = filesystem;
    this->directory = directory;
    this->index = index;
    this->index_file = this->directory + "/index.json";

    this->restore(directory);
}


void BitsRepositoryInFilesystem::add(Bit &bit)
{
    string base_directory = bit.metadata.user_name + "/" + bit.metadata.name;
    string bit_directory = this->directory + "/" + base_directory + "/" + bit.metadata.version;

    this->filesystem->createDirectory(bit_directory);
    this->savePayload(bit.metadata.name, bit_directory, bit.payload);
    this->saveMetadata(bit.metadata.name, bit_directory, bit.metadata);
    this->index->indexBit(bit.metadata.name, bit.metadata.user_name, base_directory);
    this->filesystem->writeFile(this->index_file, this->index->serialize());
}


void BitsRepositoryInFilesystem::savePayload(const string& name, const string& bit_directory, const string& base64_payload)
{
    string payload_file_path = payloadFileName(name, bit_directory);
    string binary_payload = base64_decode(base64_payload);
    this->filesystem->writeFile(payload_file_path, binary_payload);
}


void BitsRepositoryInFilesystem::saveMetadata(const string& name, const string& bit_directory, BitMetadata metadata)
{
    string metadata_file_path = metadataFileName(name, bit_directory);
    json metadata_json = {
        {"name", metadata.name},
        {"user_name", metadata.user_name},
        {"version", metadata.version},
    };
    this->filesystem->writeFile(metadata_file_path, metadata_json.dump());
}


Optional<Bit> BitsRepositoryInFilesystem::bitBy(std::string name)
{
    Optional<Bit> bit;
    Optional<string> index_directory;
    string bit_directory;

    index_directory = this->index->find(name);
    if (!index_directory.isPresent()) {
        return bit;
    }

    bit_directory = latestVersionDirectory(this->directory + "/" + index_directory.value());
    BitMetadata metadata = this->loadMetadata(name, bit_directory);
    string payload = this->loadPayload(name, bit_directory);
    bit = Bit(name, metadata.version, metadata.user_name, payload);

    return bit;
}


string BitsRepositoryInFilesystem::latestVersionDirectory(string base_directory)
{
    list<string> versions = filesystem->listDirectories(base_directory);
    versions.sort();
    return versions.back();
}


Optional<Bit> BitsRepositoryInFilesystem::bitBy(std::string name, std::string version)
{
    Optional<Bit> bit;
    Optional<string> base_directory;
    string bit_directory;

    base_directory = this->index->find(name);
    if (!base_directory.isPresent()) {
        return bit;
    }

    bit_directory = this->directory + "/" + base_directory.value() + "/" + version;
    if (!this->filesystem->directoryExists(bit_directory)) {
        return bit;
    }

    BitMetadata metadata = this->loadMetadata(name, bit_directory);
    string payload = this->loadPayload(name, bit_directory);
    bit = Bit(name, metadata.version, metadata.user_name, payload);

    return bit;
}


string BitsRepositoryInFilesystem::loadPayload(string name, string bit_directory)
{
    string payload_file_path = payloadFileName(name, bit_directory);
    string payload = this->filesystem->readFile(payload_file_path);
    return base64_encode((const unsigned char *)payload.c_str(), payload.size());
}


BitMetadata BitsRepositoryInFilesystem::loadMetadata(const string& name, string bit_directory)
{
    string metadata_file_path = metadataFileName(name, bit_directory);
    string metadata = this->filesystem->readFile(metadata_file_path);
    auto metadata_json = json::parse(metadata);
    return BitMetadata(name, metadata_json.at("user_name"), metadata_json.at("version"));
}


list<Bit> BitsRepositoryInFilesystem::allBits()
{
    list<Bit> bits;
    return bits;
}


void BitsRepositoryInFilesystem::restore(string directory)
{
    this->directory = directory;
    this->index_file = this->directory + "/index.json";

    if (this->filesystem->fileExists(this->index_file)) {
        this->index->restore(this->filesystem->readFile(this->index_file));
    }
}


std::list<BitMetadata> BitsRepositoryInFilesystem::search(BitSearchQuery search_query)
{
    return std::list<BitMetadata>();
}
