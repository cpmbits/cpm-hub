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
#include <api/plugins_api.h>
#include <domain/plugins_service.h>

using namespace nlohmann;
using namespace std;


PluginsApi::PluginsApi(PluginsService *plugins_service) {
    this->plugins_service = plugins_service;
}


struct http_response PluginsApi::publishPlugin(struct http_request request)
{
    struct http_response response = {
        200, ""
    };
    auto json = json::parse(request.body);
    struct plugin_publication_data registration_data = {
        json.at("plugin_name"), 
        json.at("version"), 
        "john_doe" ,
        json.at("payload"),
    };

    plugins_service->publishPlugin(registration_data);

    return response;
}


struct http_response PluginsApi::listPlugins(struct http_request request)
{
    struct http_response response(200, "");
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


struct http_response PluginsApi::downloadPlugin(struct http_request request)
{
    Optional<Plugin> plugin;

    plugin = plugins_service->find(request.getParameter("plugin_name"));
    if (!plugin.isPresent()) {
        return http_response(404, "");
    }

    return http_response(200, asJson(plugin.value()));
}
