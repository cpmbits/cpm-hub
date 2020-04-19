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
#include <sstream>
#include <boost/filesystem.hpp>
#include <infrastructure/Filesystem.h>

using namespace std;


void Filesystem::writeFile(std::string file_name, std::string contents)
{
    ofstream file;

    file.open(file_name);
    file.write(contents.c_str(), contents.size());
    file.close();
}


std::string Filesystem::readFile(std::string file_name)
{
    ifstream file(file_name, ios::binary);
    ostringstream ostream;

    ostream << file.rdbuf();

    return ostream.str();
}


void Filesystem::createDirectory(std::string path)
{
    boost::filesystem::create_directories(path);
}


bool Filesystem::fileExists(std::string path)
{
    return boost::filesystem::exists(path);
}


void Filesystem::deleteFile(std::string file_name)
{
    remove(file_name.c_str());
}

void Filesystem::changePermissions(std::string file_name, unsigned int mask)
{
    using namespace boost::filesystem;
    perms boost_permissions(no_perms);
    wpath path = file_name;

    if (mask & FILESYSTEM_PERMISSION_READ) {
        boost_permissions |= owner_read|group_read;
    }
    if (mask & FILESYSTEM_PERMISSION_WRITE) {
        boost_permissions |= owner_write|group_write;
    }
    if (mask & FILESYSTEM_PERMISSION_EXEC) {
        boost_permissions |= owner_exe|group_exe;
    }

    permissions(path, boost_permissions);
}
