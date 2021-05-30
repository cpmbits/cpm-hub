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
#include <boost/program_options.hpp>
#include <inih/INIReader.h>
#include <management/program_options_parser.h>

using namespace boost::program_options;
using namespace std;

map<string, ProgramOptions::AuthenticatorType> string_to_authenticator_type = {
        {"unauthenticated", ProgramOptions::UNAUTHENTICATED},
        {"access_file", ProgramOptions::ACCESS_FILE_AUTHENTICATOR},
        {"cpm_hub_auth", ProgramOptions::CPM_HUB_AUTHENTICATOR},
};

map<string, ProgramOptions::KpiSinkType> string_to_kpi_sink = {
        {"none", ProgramOptions::NO_KPIS},
        {"influxdb", ProgramOptions::INFLUXDB},
};

map<string, ProgramOptions::BitsRepositoryType> string_to_bits_repository_type = {
        {"filesystem", ProgramOptions::BITS_REPOSITORY_FILESYSTEM},
        {"sqlite", ProgramOptions::BITS_REPOSITORY_SQLITE},
};


static ProgramOptions parseIniFile(string &ini_file)
{
    ProgramOptions program_options;
    INIReader ini_reader(ini_file);

    program_options.bits_directory = ini_reader.Get("Service", "bits_directory", ".");
    program_options.sqlite_database = ini_reader.Get("Service", "sqlite_database", "bits.db");
    program_options.bits_repository_type = string_to_bits_repository_type[ini_reader.Get("Service", "bits_repository_type", "filesystem")];
    program_options.http_service_ip = ini_reader.Get("Service", "ip_bind", "127.0.0.1");
    program_options.http_service_port = ini_reader.GetInteger("Service", "port", 8000);
    program_options.authenticator_type = string_to_authenticator_type[ini_reader.Get("Service", "authentication", "unauthenticated")];
    program_options.cpm_hub_url = ini_reader.Get("Service", "cpm_hub_url", "http://localhost:1234");
    program_options.http_management_ip = ini_reader.Get("Management", "ip_bind", "127.0.0.1");
    program_options.http_management_port = ini_reader.GetInteger("Management", "port", 8001);
    program_options.security_options.security_enabled = ini_reader.GetBoolean("Management", "security_enabled", false);
    program_options.security_options.certificate_file = ini_reader.Get("Management", "certificate_file", "certificate.pem");
    program_options.security_options.key_file = ini_reader.Get("Management", "key_file", "key.pem");
    program_options.access_file = ini_reader.Get("Management", "access_file", ".access");

    program_options.logger_file = ini_reader.Get("Logging", "logger_file", "cpmhub.log");
    program_options.logger_max_file_size = ini_reader.GetInteger("Logging", "logger_max_file_size", 1024*1024);
    program_options.logger_max_files = ini_reader.GetInteger("Logging", "logger_max_files", 10);

    program_options.kpi_sink = string_to_kpi_sink[ini_reader.Get("KPI", "sink", "none")];
    program_options.influxdb_url = ini_reader.Get("KPI", "influxdb_url", "http://localhost:1234");
    program_options.influxdb_db = ini_reader.Get("KPI", "influxdb_db", "mydb");

    return program_options;
}


static const char *envString(const char *variable, const char *default_value)
{
    char *value = getenv(variable);
    return value != NULL ? value : default_value;
}


static int envInt(const char *variable, int default_value)
{
    char *value = getenv(variable);
    return value != NULL ? atoi(value) : default_value;
}


static bool envBool(const char *variable, bool default_value)
{
    char *value = getenv(variable);
    return value != NULL ? atoi(value) : default_value;
}


static ProgramOptions parseEnvironmentVariables()
{
    ProgramOptions program_options;

    program_options.bits_directory = envString("BITS_DIRECTORY", ".");
    program_options.sqlite_database = envString("SQLITE_DATABASE", "cpmhub.db");
    program_options.bits_repository_type = string_to_bits_repository_type[envString("BITS_REPOSITORY_TYPE", "sqlite")];
    program_options.http_service_ip = envString("SERVICE_IP_BIND", "127.0.0.1");
    program_options.http_service_port = envInt("SERVICE_PORT", 8000);
    program_options.authenticator_type = string_to_authenticator_type[envString("AUTHENTICATION", "unauthenticated")];
    program_options.cpm_hub_url = envString("AUTHENTICATION_URL", "http://localhost:1234");
    program_options.http_management_ip = envString("MANAGEMENT_IP_BIND", "127.0.0.1");
    program_options.http_management_port = envInt("MANAGEMENT_PORT", 8001);
    program_options.security_options.security_enabled = envBool("SECURITY_ENABLED", false);
    program_options.security_options.certificate_file = envString("CERTIFICATE_FILE", "certificate.pem");
    program_options.security_options.key_file = envString("KEY_FILE", "key.pem");
    program_options.access_file = envString("ACCESS_FILE", ".access");

    program_options.logger_file = envString("LOGGER_FILE", "cpmhub.log");
    program_options.logger_max_file_size = envInt("LOGGER_MAX_FILE_SIZE", 1024 * 1024);
    program_options.logger_max_files = envInt("LOGGER_MAX_FILES", 10);

    program_options.kpi_sink = string_to_kpi_sink[envString("SINK", "none")];
    program_options.influxdb_url = envString("INFLUXDB_URL", "http://localhost:1234");
    program_options.influxdb_db = envString("INFLUXDB_DB", "mydb");

    return program_options;
}


ProgramOptions parseProgramOptions(int argc, char *argv[])
{
    options_description cmdline_options("CPM Hub options");
    variables_map args;

    cmdline_options.add_options()
            ("ini,i", value<string>()->default_value("cpmhub.ini"), "ini file with cpm-hub configuration");

    store(parse_command_line(argc, argv, cmdline_options), args);
    notify(args);

    if (args.count("ini")) {
        string ini_file = args["ini"].as<string>();
        return parseIniFile(ini_file);
    } else {
        return parseEnvironmentVariables();
    }
}
