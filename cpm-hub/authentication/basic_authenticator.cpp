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
#include <authentication/basic_authenticator.h>

using namespace std;


BasicAuthenticator::BasicAuthenticator(Filesystem *filesystem)
{
    this->filesystem = filesystem;
}


bool BasicAuthenticator::authenticate(const char *key)
{
    string contents;
    contents = filesystem->readFile(access_file);
    boost::trim(contents);
    return contents == key;
}


void BasicAuthenticator::setAccessFile(std::string filename)
{
    this->access_file = filename;
}
