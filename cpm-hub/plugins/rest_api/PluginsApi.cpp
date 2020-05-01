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
#include <json/json.hpp>
#include <plugins/rest_api/PluginsApi.h>
#include <plugins/PluginsService.h>
#include <authentication/UserCredentials.h>

using namespace nlohmann;
using namespace std;

static NullAuthenticator unauthenticated;


PluginsApi::PluginsApi(PluginsService *plugins_service) {
    this->plugins_service = plugins_service;
    this->authenticator = &unauthenticated;
}


PluginsApi::PluginsApi(PluginsService *plugins_service, Authenticator *authenticator)
{
    this->plugins_service = plugins_service;
    this->authenticator = authenticator;
}


HttpResponse PluginsApi::publishPlugin(HttpRequest &request)
{
    auto json = json::parse(request.body);
    Optional<string> user;
    struct PluginPublicationData publication_data;
    UserCredentials credentials(json.at("username"),json.at("password"));

    if (!this->authenticator->validCredentials(credentials)) {
        return HttpResponse::unauthorized();
    }

    publication_data.plugin_name = json.at("plugin_name");
    publication_data.version = json.at("version");
    publication_data.user_name = json.at("username");
    publication_data.payload = json.at("payload");

    plugins_service->publishPlugin(publication_data);

    return HttpResponse::ok("");
}


HttpResponse PluginsApi::listPlugins(HttpRequest &request)
{
    HttpResponse response(HttpStatus::OK, "");
    json json_plugin_list = json::array();

    for (Plugin plugin : plugins_service->allPlugins()) {
        json json_plugin = {{"plugin_name", plugin.metadata.name}};
        json_plugin_list.push_back(json_plugin);
    }

    response.body = json_plugin_list.dump();

    return response;
}


static string asJson(Plugin plugin)
{
    json json_plugin = {
        {"plugin_name", plugin.metadata.name},
        {"version", plugin.metadata.version},
        {"payload", plugin.payload}
    };
    return json_plugin.dump();
}


HttpResponse PluginsApi::downloadPlugin(HttpRequest &request)
{
    Optional<Plugin> plugin;

    plugin = plugins_service->find(request.parameters.get("pluginName"));
    if (!plugin.isPresent()) {
        return HttpResponse(HttpStatus::NOT_FOUND, "");
    }

    return HttpResponse(HttpStatus::OK, asJson(plugin.value()));
}
