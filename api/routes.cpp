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
#include <infrastructure/basic_authenticator.h>
#include <infrastructure/plugins_repository_in_filesystem.h>
#include <infrastructure/http_server.h>
#include <infrastructure/deploy_service.h>
#include <domain/plugins_service.h>
#include <api/routes.h>
#include <api/management_api.h>
#include <api/plugins_api.h>

using namespace std;


static Filesystem filesystem;
static PluginIndex plugin_index;
static PluginsRepositoryInFilesystem plugins_repository(&filesystem, &plugin_index);
static PluginsService plugins_service(&plugins_repository);
static PluginsApi plugins_api(&plugins_service);

static BasicAuthenticator authenticator(&filesystem);
static DeployService deploy_service(&filesystem);
static ManagementApi management_api(&deploy_service);


void installServiceRoutes(HttpServer& http_server, std::string plugins_directory, ProgramOptions &options)
{
    http_server.post("/plugins", [&](HttpRequest &request) -> HttpResponse {
        return plugins_api.publishPlugin(request);
    });
    http_server.get("/plugins/:pluginName", [&](HttpRequest &request) -> HttpResponse {
        return plugins_api.downloadPlugin(request);
    });
    plugins_repository.restore(options.plugins_directory);
}


void installManagementRoutes(HttpServer &http_server, std::vector<std::string> command_line, ProgramOptions &options)
{
    deploy_service.setCommandLine(command_line);
    authenticator.setAccessFile(options.access_file);
    deploy_service.configureAuthenticator(&authenticator);

    http_server.post("/deploy", [&](HttpRequest &request) -> HttpResponse {
        return management_api.deploy(request);
    });
}
