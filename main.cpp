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


static HttpServer service_http_server;
static int http_service_port = 8000;
static HttpServer management_http_server;
static int http_management_port = 8001;
static std::string plugins_directory = ".";


static void parseCommandLineArgs(int argc, char *argv[])
{
    options_description cmdline_options("CPM Hub options");
    variables_map args;

    cmdline_options.add_options()
            ("plugins-directory,d", value<std::string>()->default_value("."), "plugins directory repository location")
            ("port,p", value<int>()->default_value(8000), "HTTP service port");

    store(parse_command_line(argc, argv, cmdline_options), args);
    notify(args);

    plugins_directory = args["plugins-directory"].as<std::string>();
    http_service_port = args["port"].as<int>();
}


int main(int argc, char *argv[])
{
    parseCommandLineArgs(argc, argv);

    installRoutes(service_http_server, plugins_directory);

    service_http_server.startAsync("0.0.0.0", http_service_port);

    management_http_server.start("0.0.0.0", http_management_port);

    return 0;
}
