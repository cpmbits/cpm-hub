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

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include <infrastructure/Filesystem.h>


class MockFilesystem: public Filesystem {
public:
    void writeFile(std::string file_name, std::string contents) {
        mock().actualCall("Filesystem.writeFile")
              .withParameter("file_name", file_name.c_str())
              .withParameter("contents", contents.c_str())
              .returnStringValue();
    }

    std::string readFile(std::string file_name) {
        return mock().actualCall("Filesystem.readFile")
                     .withParameter("file_name", file_name.c_str())
                     .returnStringValue();
    }

    void createDirectory(std::string path) {
        mock().actualCall("Filesystem.createDirectory")
              .withParameter("path", path.c_str());
    }

    std::list<std::string> listDirectories(std::string path) {
        mock().actualCall("Filesystem.listDirectories");
        return std::list<std::string>();
    }

    bool fileExists(std::string file_name) {
        mock().actualCall("Filesystem.fileExists");
        return false;
    }

    bool directoryExists(std::string path) {
        mock().actualCall("Filesystem.directoryExists");
        return false;
    }

    void deleteFile(std::string file_name) {
        mock().actualCall("Filesystem.deleteFile");
    }

    void changePermissions(std::string file_name, unsigned int mask) {
        mock().actualCall("Filesystem.changePermissions");
    }

    MockExpectedCall &expect(const std::string& call) {
        std::string full_call = std::string("Filesystem.") + call;
        return mock().expectOneCall(full_call.c_str());
    }
};
