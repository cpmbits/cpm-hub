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


DeployService::DeployService(Filesystem *filesystem)
{
    this->filesystem = filesystem;
}


void DeployService::deploy(std::string &payload)
{
    std::string binary_payload = base64_decode(payload);
    this->filesystem->writeFile("cpm-hub", binary_payload);
    execl(command_line.at(0).c_str(),
          command_line.at(0).c_str(),
            command_line.at(1).c_str(),
            command_line.at(2).c_str(),
            NULL);
}

void DeployService::setCommandLine(std::vector<std::string> cmdline)
{
    this->command_line = cmdline;
}
