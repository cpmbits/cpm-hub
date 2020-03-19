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
#include <string>
#include <boost/program_options.hpp>

#include <api/routes.h>

using namespace boost::program_options;


static HttpServer http_server;
static std::string plugins_directory = ".";


int main(int argc, char *argv[])
{
    options_description cmdline_options("CPM Hub options");
    variables_map variables_map;

    cmdline_options.add_options()
        ("plugins-directory,p", value<std::string>(), "plugins directory repository location");

    store(parse_command_line(argc, argv, cmdline_options), variables_map);
    notify(variables_map);

    if (variables_map.count("plugins-directory")) {
        plugins_directory = variables_map["plugins-directory"].as<std::string>();
    }

    installRoutes(http_server, plugins_directory);

    http_server.start(8000);

    sleep(50);

    return 0;
}
