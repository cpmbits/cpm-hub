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
#include <infrastructure/deploy_service.h>


static NullAuthenticator unauthenticated;


DeployService::DeployService(Filesystem *filesystem)
{
    this->filesystem = filesystem;
    this->authenticator = &unauthenticated;
}


void DeployService::deploy(const std::string &payload, const std::string &api_key)
{
    if (!authenticator->authenticate(api_key.c_str())) {
        throw AuthenticationFailure();
    }

    saveBinary(payload);
    execl(command_line.at(0).c_str(),
          command_line.at(0).c_str(),
            command_line.at(1).c_str(),
            command_line.at(2).c_str(),
            NULL);
}


void DeployService::saveBinary(const std::string &payload) const
{
    std::string binary_payload = base64_decode(payload);
    filesystem->writeFile("cpm-hub", binary_payload);
}


void DeployService::setCommandLine(std::vector<std::string> cmdline)
{
    command_line = cmdline;
}


void DeployService::configureAuthenticator(Authenticator *auth)
{
    authenticator = auth;
}
