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

#include <bits/rest_api/BitsHttpResource.h>
#include <bits/BitsService.h>

using namespace cest;
using namespace fakeit;
using namespace std;


describe("Bits API", []() {
    it("uses the bit service to publish a bit", [&]() {
        HttpRequest request("{"
            "\"bit_name\": \"cest\","
            "\"version\": \"1.0\","
            "\"payload\": \"ABCDEabcde\","
            "\"username\": \"john_doe\","
            "\"password\": \"pass\""
        "}");
        HttpResponse response;
        Bit bit("");
        Mock<BitsService> mock_service;
        BitsHttpResource api(&mock_service.get());

        When(Method(mock_service, publishBit)).Return(bit);

        response = api.post(request);

        expect(response.status_code).toBe(200);
        expect(response.body).toBe("");
        Verify(Method(mock_service, publishBit).Matching([](struct BitPublicationData data) {
            return data.bit_name == "cest" &&
                   data.version == "1.0" &&
                   data.user_name == "john_doe" &&
                   data.payload == "ABCDEabcde";
        }));
    });

    it("returns error 401 when publishing a bit and authentication fails", [&]() {
        HttpRequest request("{"
            "\"bit_name\": \"cest\","
            "\"version\": \"1.0\","
            "\"payload\": \"ABCDEabcde\","
            "\"username\": \"john_doe\","
            "\"password\": \"pass\""
        "}");
        HttpResponse response;
        Mock<BitsService> mock_service;
        Mock<Authenticator> mock_authenticator;
        BitsHttpResource api(&mock_service.get(), &mock_authenticator.get());

        When(Method(mock_authenticator, validCredentials)).Return(false);

        response = api.post(request);

        expect(response.status_code).toBe(HttpStatus::UNAUTHORIZED);
    });

    it("uses the bit service to list the available bits", [&]() {
        HttpRequest request("");
        HttpResponse response;
        Bit bit("cest");
        list<Bit> bits {bit};
        Mock<BitsService> mock_service;
        BitsHttpResource api(&mock_service.get());

        When(Method(mock_service, allBits)).Return(bits);

        response = api.listBits(request);

        expect(response.status_code).toBe(200);
        expect(response.body).toBe("[{\"bit_name\":\"cest\"}]");
    });
        
    it("returns error 404 when downloading a bit that is not found", [&]() {
        HttpRequest request;
        HttpResponse response;
        Mock<BitsService> mock_service;
        BitsHttpResource api(&mock_service.get());
        Optional<Bit> no_bit;

        request.parameters.set("bitName", "cest");
        When(OverloadedMethod(mock_service, find, Optional<Bit>(string))).Return(no_bit);

        response = api.get(request);

        Verify(OverloadedMethod(mock_service, find, Optional<Bit>(string)).Using("cest"));
        expect(response.status_code).toBe(404);
    });
            
    it("returns latest bit version when downloading an existing bit", [&]() {
        HttpRequest request;
        HttpResponse response;
        Mock<BitsService> mock_service;
        BitsHttpResource api(&mock_service.get());
        Optional<Bit> cest_bit;

        request.parameters.set("bitName", "cest");
        cest_bit = Bit("cest", "1.0", "user", "ABCDEabcde");
        When(OverloadedMethod(mock_service, find, Optional<Bit>(string))).Return(cest_bit);

        response = api.get(request);

        Verify(OverloadedMethod(mock_service, find, Optional<Bit>(string)).Using("cest"));
        expect(response.status_code).toBe(200);
        expect(response.body).toBe("{"
            "\"bit_name\":\"cest\","
            "\"payload\":\"ABCDEabcde\","
            "\"version\":\"1.0\""
        "}");
    });

    it("returns specific bit version when downloading an existing bit", [&]() {
        HttpRequest request;
        HttpResponse response;
        Mock<BitsService> mock_service;
        BitsHttpResource api(&mock_service.get());
        Optional<Bit> cest_bit;

        request.parameters.set("bitName", "cest");
        request.parameters.set("bitVersion", "1.1");
        cest_bit = Bit("cest", "1.1", "user", "ABCDEabcde");
        When(OverloadedMethod(mock_service, find, Optional<Bit>(string, string))).Return(cest_bit);

        response = api.get(request);

        Verify(OverloadedMethod(mock_service, find, Optional<Bit>(string, string)).Using("cest", "1.1"));
        expect(response.status_code).toBe(200);
        expect(response.body).toBe("{"
           "\"bit_name\":\"cest\","
           "\"payload\":\"ABCDEabcde\","
           "\"version\":\"1.1\""
        "}");
    });
});
