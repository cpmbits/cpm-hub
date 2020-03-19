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
static int http_server_port = 8000;


static void parseCommandLineArgs(int argc, char *argv[])
{
    options_description cmdline_options("CPM Hub options");
    variables_map args;

    cmdline_options.add_options()
            ("plugins-directory,d", value<std::string>(), "plugins directory repository location")
            ("port,p", value<int>(), "HTTP server port");

    store(parse_command_line(argc, argv, cmdline_options), args);
    notify(args);

    if (args.count("plugins-directory")) {
        plugins_directory = args["plugins-directory"].as<std::string>();
    }
    if (args.count("port")) {
        http_server_port = args["port"].as<int>();
    }
}


int main(int argc, char *argv[])
{
    parseCommandLineArgs(argc, argv);

    installRoutes(http_server, plugins_directory);

    http_server.start(http_server_port);

    sleep(50);

    return 0;
}
