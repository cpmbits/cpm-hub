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
#include <unistd.h>

#include <base64/base64.h>
#include <management/DeployService.h>
#include <cstring>
#include <authentication/Authenticator.h>

using namespace std;


static NullAuthenticator unauthenticated;


DeployService::DeployService(Filesystem *filesystem, Authenticator *authenticator, std::vector<std::string> command_line)
{
    this->filesystem = filesystem;
    this->authenticator = authenticator;
    this->command_line = command_line;
}


DeployService::DeployService(Filesystem *filesystem)
{
    this->filesystem = filesystem;
    this->authenticator = &unauthenticated;
}


void DeployService::deploy(const string &payload, const string &version, const string &api_key)
{
    string file_name;

    if (!this->authenticator->authenticate(api_key.c_str()).isPresent()) {
        throw AuthenticationFailure();
    }

    file_name = "cpm-hub-" + version;
    saveBinary(payload, file_name);
    filesystem->changePermissions(file_name, FILESYSTEM_PERMISSION_READ|FILESYSTEM_PERMISSION_WRITE|FILESYSTEM_PERMISSION_EXEC);
    filesystem->deleteFile(command_line.at(0));
    execl(file_name.c_str(),
          file_name.c_str(),
          command_line.at(1).c_str(),
          command_line.at(2).c_str(),
          NULL);
}


void DeployService::saveBinary(const string &payload, const string &file_name) const
{
    string binary_payload = base64_decode(payload);
    filesystem->writeFile(file_name, binary_payload);
}


void DeployService::setCommandLine(vector<string> cmdline)
{
    command_line = cmdline;
}


void DeployService::configureAuthenticator(Authenticator *auth)
{
    authenticator = auth;
}
