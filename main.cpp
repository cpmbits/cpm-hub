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
#include <string>
#include <boost/program_options.hpp>
#include <inih/INIReader.h>

#include <api/program_options.h>
#include <api/routes.h>

using namespace boost::program_options;
using namespace std;


static ProgramOptions parseIniFile(string &ini_file)
{
    ProgramOptions program_options;
    INIReader ini_reader(ini_file);

    program_options.plugins_directory = ini_reader.Get("Service", "plugins_directory", ".");
    program_options.http_service_port = ini_reader.GetInteger("Service", "port", 8000);
    program_options.http_management_port = ini_reader.GetInteger("Management", "port", 8001);
    program_options.management_server_security_options.security_enabled = ini_reader.GetBoolean("Management", "security_enabled", true);
    program_options.management_server_security_options.certificate_file = ini_reader.Get("Management", "certificate_file", "certificate.pem");
    program_options.management_server_security_options.key_file = ini_reader.Get("Management", "key_file", "key.pem");

    return program_options;
}


static ProgramOptions parseProgramOptions(int argc, char *argv[])
{
    options_description cmdline_options("CPM Hub options");
    variables_map args;
    string ini_file;

    cmdline_options.add_options()
            ("ini,i", value<string>()->default_value("cpmhub.ini"), "ini file with cpm-hub configuration");

    store(parse_command_line(argc, argv, cmdline_options), args);
    notify(args);

    ini_file = args["ini"].as<string>();

    return parseIniFile(ini_file);
}


static vector<string> saveCommandLine(int argc, char *argv[])
{
    vector<string> command_line;

    for (int i=0; i<argc; i++) {
        command_line.emplace_back(string(argv[i]));
    }

    return command_line;
}


int main(int argc, char *argv[])
{
    ProgramOptions program_options;
    HttpServer service_http_server;
    HttpServer management_http_server;
    vector<string> command_line = saveCommandLine(argc, argv);

    program_options = parseProgramOptions(argc, argv);

    installServiceRoutes(service_http_server, program_options);
    service_http_server.startAsync("0.0.0.0", program_options.http_service_port);

    installManagementRoutes(management_http_server, command_line, program_options);
    management_http_server.configureSecurity(program_options.management_server_security_options);
    management_http_server.start("0.0.0.0", program_options.http_management_port);

    return 0;
}
