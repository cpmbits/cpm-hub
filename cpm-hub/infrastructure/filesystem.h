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

#define FILESYSTEM_PERMISSION_READ  (1U)
#define FILESYSTEM_PERMISSION_WRITE (2U)
#define FILESYSTEM_PERMISSION_EXEC  (4U)

class Filesystem {
public:
    virtual void writeFile(std::string file_name, std::string contents);

    virtual std::string readFile(std::string file_name);

    virtual void createDirectory(std::string path);

    virtual bool fileExists(std::string file_name);

    virtual void deleteFile(std::string file_name);

    virtual void changePermissions(std::string file_name, unsigned int mask);
};
