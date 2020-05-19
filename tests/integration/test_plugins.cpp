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
#include <cest/cest.h>

#include <authentication/TrivialAuthenticator.h>
#include <bits/rest_api/BitsApi.h>
#include <bits/BitsRepositoryInMemory.h>

using namespace cest;


describe("CPM Hub bits management", []() {
    it("publishes a bit when no authentication is configured", [&]() {
        HttpRequest request("{"
                            "\"bit_name\": \"cest\","
                            "\"version\": \"1.0\","
                            "\"payload\": \"ABCDEabcde\","
                            "\"username\": \"john_doe\","
                            "\"password\": \"12345\""
                            "}");
        HttpResponse response;
        BitsRepositoryInMemory repository;
        BitsService service(&repository);
        BitsApi api(&service);

        response = api.publishBit(request);

        expect(response.status_code).toBe(HttpStatus::OK);
        expect(response.body).toBe("");
    });

    it("fails to publish a bit when authentication fails", [&]() {
        HttpRequest request("{"
                            "\"bit_name\": \"cest\","
                            "\"version\": \"1.0\","
                            "\"payload\": \"ABCDEabcde\","
                            "\"username\": \"john_doe\","
                            "\"password\": \"12345\""
                            "}");
        HttpResponse response;
        TrivialAuthenticator authenticator;
        BitsRepositoryInMemory repository;
        BitsService service(&repository);
        BitsApi api(&service, &authenticator);

        response = api.publishBit(request);

        expect(response.status_code).toBe(HttpStatus::UNAUTHORIZED);
    });

    it("lists a bit after it has been registered", [&]() {
        HttpRequest request("{"
            "\"bit_name\": \"cest\","
            "\"version\": \"1.0\","
            "\"payload\": \"ABCDEabcde\","
            "\"username\": \"john_doe\","
            "\"password\": \"12345\""
        "}");
        HttpResponse response;
        BitsRepositoryInMemory repository;
        BitsService service(&repository);
        BitsApi api(&service);

        api.publishBit(request);

        response = api.listBits(request);

        expect(response.status_code).toBe(HttpStatus::OK);
        expect(response.body).toBe("[{\"bit_name\":\"cest\"}]");
    });

    it("fails to download bit when it's not found", [&]() {
        HttpRequest request;
        HttpResponse response;
        BitsRepositoryInMemory repository;
        BitsService service(&repository);
        BitsApi api(&service);

        request.parameters.set("bitName", "cest");

        response = api.downloadBit(request);

        expect(response.status_code).toBe(HttpStatus::NOT_FOUND);
    });

    it("finds a bit after it has been registered", [&]() {
        HttpRequest publish_request("{"
            "\"bit_name\": \"cest\","
            "\"version\": \"1.0\","
            "\"payload\": \"ABCDEabcde\","
            "\"username\": \"john_doe\","
            "\"password\": \"12345\""
        "}");
        HttpRequest download_request;
        HttpResponse response;
        BitsRepositoryInMemory repository;
        BitsService service(&repository);
        BitsApi api(&service);

        api.publishBit(publish_request);

        download_request.parameters.set("bitName", "cest");
        response = api.downloadBit(download_request);

        expect(response.status_code).toBe(HttpStatus::OK);
        expect(response.body).toBe("{\"payload\":\"ABCDEabcde\",\"bit_name\":\"cest\",\"version\":\"1.0\"}");
    });

    it("finds a bit given version after it has been registered", [&]() {
        HttpRequest publish_request("{"
            "\"bit_name\": \"cest\","
            "\"version\": \"1.0\","
            "\"payload\": \"ABCDEabcde\","
            "\"username\": \"john_doe\","
            "\"password\": \"12345\""
        "}");
        HttpRequest download_request;
        HttpResponse response;
        BitsRepositoryInMemory repository;
        BitsService service(&repository);
        BitsApi api(&service);

        api.publishBit(publish_request);

        download_request.parameters.set("bitName", "cest");
        download_request.parameters.set("bitVersion", "1.0");
        response = api.downloadBit(download_request);

        expect(response.status_code).toBe(HttpStatus::OK);
        expect(response.body).toBe("{\"payload\":\"ABCDEabcde\",\"bit_name\":\"cest\",\"version\":\"1.0\"}");
    });
});
