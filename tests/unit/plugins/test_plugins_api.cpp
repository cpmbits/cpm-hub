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
#include <list>

#include <cest/cest.h>
#include <fakeit/fakeit.hpp>

#include <plugins/rest_api/PluginsApi.h>
#include <plugins/PluginsService.h>

using namespace cest;
using namespace fakeit;
using namespace std;


describe("Plugins API", []() {
    it("uses the plugin service to publish a plugin", [&]() {
        HttpRequest request("{"
            "\"plugin_name\": \"cest\","
            "\"version\": \"1.0\","
            "\"payload\": \"ABCDEabcde\","
            "\"username\": \"john_doe\","
            "\"password\": \"pass\""
        "}");
        HttpResponse response;
        Plugin plugin("");
        Mock<PluginsService> mock_service;
        PluginsApi api(&mock_service.get());

        When(Method(mock_service, publishPlugin)).Return(plugin);

        response = api.publishPlugin(request);

        expect(response.status_code).toBe(200);
        expect(response.body).toBe("");
        Verify(Method(mock_service, publishPlugin).Matching([](struct PluginPublicationData data) {
            return data.plugin_name == "cest" &&
                   data.version == "1.0" &&
                   data.user_name == "john_doe" &&
                   data.payload == "ABCDEabcde";
        }));
    });

    it("returns error 401 when publishing a plugin and authentication fails", [&]() {
        HttpRequest request("{"
            "\"plugin_name\": \"cest\","
            "\"version\": \"1.0\","
            "\"payload\": \"ABCDEabcde\","
            "\"username\": \"john_doe\","
            "\"password\": \"pass\""
        "}");
        HttpResponse response;
        Mock<PluginsService> mock_service;
        Mock<Authenticator> mock_authenticator;
        PluginsApi api(&mock_service.get(), &mock_authenticator.get());

        When(Method(mock_authenticator, validCredentials)).Return(false);

        response = api.publishPlugin(request);

        expect(response.status_code).toBe(HttpStatus::UNAUTHORIZED);
    });

    it("uses the plugin service to list the available plugins", [&]() {
        HttpRequest request("");
        HttpResponse response;
        Plugin plugin("cest");
        list<Plugin> plugins {plugin};
        Mock<PluginsService> mock_service;
        PluginsApi api(&mock_service.get());

        When(Method(mock_service, allPlugins)).Return(plugins);

        response = api.listPlugins(request);

        expect(response.status_code).toBe(200);
        expect(response.body).toBe("[{\"plugin_name\":\"cest\"}]");
    });
        
    it("returns error 404 when downloading a plugin that is not found", [&]() {
        HttpRequest request;
        HttpResponse response;
        Mock<PluginsService> mock_service;
        PluginsApi api(&mock_service.get());
        Optional<Plugin> no_plugin;

        request.parameters.set("pluginName", "cest");
        When(Method(mock_service, find)).Return(no_plugin);

        response = api.downloadPlugin(request);

        Verify(Method(mock_service, find).Using("cest"));
        expect(response.status_code).toBe(404);
    });
            
    it("returns plugin when downloading an existing plugin", [&]() {
        HttpRequest request;
        HttpResponse response;
        Mock<PluginsService> mock_service;
        PluginsApi api(&mock_service.get());
        Optional<Plugin> cest_plugin;

        request.parameters.set("pluginName", "cest");
        cest_plugin = Plugin("cest", "1.0", "user", "ABCDEabcde");
        When(Method(mock_service, find)).Return(cest_plugin);

        response = api.downloadPlugin(request);

        Verify(Method(mock_service, find).Using("cest"));
        expect(response.status_code).toBe(200);
        expect(response.body).toBe("{"
            "\"payload\":\"ABCDEabcde\","
            "\"plugin_name\":\"cest\","
            "\"version\":\"1.0\""
        "}");
    });
});
