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
#include <wait.h>
#include <authentication/AccessFileAuthenticator.h>
#include <authentication/CpmHubAuthenticator.h>
#include <http/HttpServer.h>
#include <management/cpm_hub_starter.h>
#include <management/DeployService.h>
#include <management/rest_api/ManagementHttpResource.h>
#include <management/http_routes.h>
#include <bits/BitsRepositoryInFilesystem.h>
#include <bits/BitsRepositoryInSqlite.h>
#include <bits/BitsService.h>
#include <bits/rest_api/BitsHttpResource.h>
#include <logging/LoggerInRotatingFile.h>
#include <logging/LoggerInConsole.h>
#include <users/UsersRepositoryInMemory.h>
#include <kpi/KpiSinkInfluxDb.h>
#include <kpi/kpi.h>


static Filesystem filesystem;
static HttpServer service_http_server;
static BitsHttpResource *bits_resource;
static HttpServer management_http_server;
static ManagementHttpResource *management_resource;
static UsersHttpResource *users_resource;
static HttpClient cpm_hub_auth_client;
Logger *logger;


void startServiceServer(ProgramOptions &options)
{
    BitsRepository *bits_repository;
    Authenticator *bits_resource_authenticator;
    BitsService *bits_service;
    UsersService *users_service;
    UsersRepository *users_repository;
    SqlDatabaseSqlite3 *database;

    if (options.bits_repository_type == ProgramOptions::BITS_REPOSITORY_SQLITE) {
        database = new SqlDatabaseSqlite3(options.sqlite_database);
        bits_repository = new BitsRepositoryInSqlite(database);
    } else {
        BitIndex *index = new BitIndex();
        bits_repository = new BitsRepositoryInFilesystem(&filesystem, index, options.bits_directory);
    }

    switch (options.authenticator_type) {
    case ProgramOptions::CPM_HUB_AUTHENTICATOR:
        bits_resource_authenticator = new CpmHubAuthenticator(options.cpm_hub_url, cpm_hub_auth_client);
        break;

    case ProgramOptions::ACCESS_FILE_AUTHENTICATOR:
        bits_resource_authenticator = new AccessFileAuthenticator(&filesystem, options.access_file);
        break;

    case ProgramOptions::UNAUTHENTICATED:
    default:
        bits_resource_authenticator = new NullAuthenticator();
        break;
    }

    bits_service = new BitsService(bits_repository);
    bits_resource = new BitsHttpResource(bits_service, bits_resource_authenticator);

    users_repository = new UsersRepositoryInMemory();
    users_service = new UsersService(users_repository, bits_resource_authenticator);
    users_resource = new UsersHttpResource(users_service);

    installServiceRoutes(service_http_server, bits_resource, users_resource);
    service_http_server.configureSecurity(options.security_options);
    service_http_server.start(options.http_service_ip, options.http_service_port);
}


void startManagementServer(ProgramOptions &options, std::vector<std::string> command_line)
{
    Authenticator *management_authenticator;
    DeployService *deploy_service;

    management_authenticator = new AccessFileAuthenticator(&filesystem, options.access_file);
    deploy_service = new DeployService(&filesystem, management_authenticator, command_line);
    management_resource = new ManagementHttpResource(deploy_service, nullptr);

    installManagementRoutes(management_http_server, management_resource);
    management_http_server.configureSecurity(options.security_options);
    management_http_server.start(options.http_management_ip, options.http_management_port);
}


static void start(ProgramOptions &program_options, std::vector<std::string> command_line)
{
    if (program_options.kpi_sink == ProgramOptions::INFLUXDB) {
        configureKpiSink(new KpiSinkInfluxDb(program_options.influxdb_url, program_options.influxdb_db));
    }

    startServiceServer(program_options);
}


void startCpmHub(ProgramOptions &program_options, std::vector<std::string> command_line)
{
    pid_t pid;
    int wstatus;

    if (!program_options.logger_file.empty()) {
        logger = new LoggerInRotatingFile(program_options.logger_file, program_options.logger_max_file_size, program_options.logger_max_files);
    } else {
        logger = new LoggerInConsole();
    }

    do {
        pid = fork();
        if (pid == 0) {
            start(program_options, command_line);
        } else {
            waitpid(pid, &wstatus, 0);
            logger->error("cpm-hub restarted");
        }
    } while(true);
}
