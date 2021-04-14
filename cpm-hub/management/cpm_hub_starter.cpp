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
#include <sys/prctl.h>
#include <authentication/AccessFileAuthenticator.h>
#include <authentication/CpmHubAuthenticator.h>
#include <http/HttpServer.h>
#include <management/cpm_hub_starter.h>
#include <management/http_routes.h>
#include <bits/BitsRepositoryInSqlite.h>
#include <bits/BitsService.h>
#include <bits/rest_api/BitsHttpResource.h>
#include <templates/rest_api/TemplatesHttpResource.h>
#include <templates/TemplatesRepositoryInSqlite.h>
#include <logging/LoggerInRotatingFile.h>
#include <logging/LoggerInConsole.h>
#include <users/UsersRepositoryInMemory.h>
#include <kpi/KpiSinkInfluxDb.h>
#include <kpi/kpi.h>


static Filesystem filesystem;
static HttpServer service_http_server;
static BitsHttpResource *bits_resource;
static TemplatesHttpResource *templates_resource;
static HttpServer management_http_server;
static UsersHttpResource *users_resource;
static HttpClient cpm_hub_auth_client;
Logger *logger;


void startServiceServer(ProgramOptions &options)
{
    BitsRepository *bits_repository;
    Authenticator *resource_authenticator;
    BitsService *bits_service;
    TemplatesRepositoryInSqlite *templates_repository;
    TemplatesService *templates_service;
    UsersService *users_service;
    UsersRepository *users_repository;
    SqlDatabaseSqlite3 *database;

    database = new SqlDatabaseSqlite3(options.sqlite_database);
    bits_repository = new BitsRepositoryInSqlite(database);

    switch (options.authenticator_type) {
    case ProgramOptions::CPM_HUB_AUTHENTICATOR:
        resource_authenticator = new CpmHubAuthenticator(options.cpm_hub_url, cpm_hub_auth_client);
        break;

    case ProgramOptions::ACCESS_FILE_AUTHENTICATOR:
        resource_authenticator = new AccessFileAuthenticator(&filesystem, options.access_file);
        break;

    case ProgramOptions::UNAUTHENTICATED:
    default:
        resource_authenticator = new NullAuthenticator();
        break;
    }

    bits_service = new BitsService(bits_repository);
    bits_resource = new BitsHttpResource(bits_service, resource_authenticator);

    templates_repository = new TemplatesRepositoryInSqlite(database);
    templates_service = new TemplatesService(templates_repository);
    templates_resource = new TemplatesHttpResource(templates_service, resource_authenticator);

    users_repository = new UsersRepositoryInMemory();
    users_service = new UsersService(users_repository, resource_authenticator);
    users_resource = new UsersHttpResource(users_service);

    installServiceRoutes(service_http_server, bits_resource, users_resource, templates_resource);
    service_http_server.configureSecurity(options.security_options);
    service_http_server.start(options.http_service_ip, options.http_service_port);
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
            prctl(PR_SET_PDEATHSIG, SIGTERM);
            start(program_options, command_line);
        } else {
            waitpid(pid, &wstatus, 0);
            logger->error("cpm-hub restarted");
        }
    } while(true);
}
