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
#include <authentication/AccessFileAuthenticator.h>
#include <authentication/CpmHubAuthenticator.h>
#include <http/HttpServer.h>
#include <management/cpm_hub_starter.h>
#include <management/DeployService.h>
#include <management/rest_api/ManagementApi.h>
#include <management/http_routes.h>
#include <plugins/PluginsRepositoryInFilesystem.h>
#include <plugins/PluginsService.h>
#include <plugins/rest_api/PluginsApi.h>


static Filesystem filesystem;
static HttpServer service_http_server;
static PluginsApi *plugins_api;
static HttpServer management_http_server;
static ManagementApi *management_api;
static HttpClient cpm_hub_auth_client;


void startServiceServer(ProgramOptions &options)
{
    PluginIndex *plugin_index;
    PluginsRepository *plugins_repository;
    Authenticator *plugins_api_authenticator;
    PluginsService *plugins_service;

    plugin_index = new PluginIndex();
    plugins_repository = new PluginsRepositoryInFilesystem(&filesystem, plugin_index, options.plugins_directory);
    switch (options.authenticator_type) {
    case ProgramOptions::UNAUTHENTICATED:
        plugins_api_authenticator = new NullAuthenticator();
        break;

    case ProgramOptions::CPM_HUB_AUTHENTICATOR:
        plugins_api_authenticator = new CpmHubAuthenticator(options.cpm_hub_url, cpm_hub_auth_client);
        break;

    case ProgramOptions::ACCESS_FILE_AUTHENTICATOR:
        plugins_api_authenticator = new AccessFileAuthenticator(&filesystem, options.access_file);
        break;
    }
    plugins_service = new PluginsService(plugins_repository);
    plugins_api = new PluginsApi(plugins_service, plugins_api_authenticator);

    installServiceRoutes(service_http_server, plugins_api);
    service_http_server.configureSecurity(options.security_options);
    service_http_server.startAsync("0.0.0.0", options.http_service_port);
}


void startManagementServer(ProgramOptions &options, std::vector<std::string> command_line)
{
    Authenticator *management_authenticator;
    DeployService *deploy_service;

    management_authenticator = new AccessFileAuthenticator(&filesystem, options.access_file);
    deploy_service = new DeployService(&filesystem, management_authenticator, command_line);
    management_api = new ManagementApi(deploy_service);

    installManagementRoutes(management_http_server, management_api);
    management_http_server.configureSecurity(options.security_options);
    management_http_server.start("0.0.0.0", options.http_management_port);
}


void startCpmHub(ProgramOptions &program_options, std::vector<std::string> command_line)
{
    startServiceServer(program_options);

    startManagementServer(program_options, command_line);
}
