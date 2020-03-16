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

#include <infrastructure/endpoint.h>


describe("Endpoint", []() {
    it("can be created from plain path", []() {
        Endpoint endpoint("/plain_path");
    });

    it("returns empty match when plain path doesn't match", []() {
        Endpoint endpoint("/plain_path");
        Optional<struct http_request_parameters> match;

        match = endpoint.match("/non_matching");

        expect(match.isPresent()).toBe(false);
    });

    it("returns match when path matches plain path", []() {
        Endpoint endpoint("/plain_path");
        Optional<struct http_request_parameters> match;

        match = endpoint.match("/plain_path");

        expect(match.isPresent()).toBe(true);
    });

    it("can be created from path with parameters", []() {
        Endpoint endpoint("/plain_path/:parameter");
    });

    it("returns empty match when path with parameters doesn't match", []() {
        Endpoint endpoint("/plain_path/:parameter");
        Optional<struct http_request_parameters> match;

        match = endpoint.match("/plain_pa/parameter");

        expect(match.isPresent()).toBe(false);
    });

    xit("creates regex from path", []() {
        expect(Endpoint::parsePath("/plain_path")).toBe("/plain_path");
        expect(Endpoint::parsePath("/plain_path/:parameter")).toBe("/plain_path/[\\w-]+");
    });


    xit("creates regex from long path with multiple parameters", []() {
        expect(Endpoint::parsePath("/plain_path")).toBe("/plain_path");
        expect(Endpoint::parsePath("/leve1/:parameter1/level2/:parameter2")).toBe("/leve1/[\\w-]+/level2/[\\w-]+");
    });

    it("returns match when path with one parameter match", []() {
        Endpoint endpoint("/plain_path/:parameter");
        Optional<struct http_request_parameters> match;

        match = endpoint.match("/plain_path/value");

        expect(match.isPresent()).toBe(true);
        expect(match.value().get("parameter")).toBe("value");
    });

    it("returns match when path with many parameter matches", []() {
        Endpoint endpoint("/leve1/:parameter1/level2/:parameter2");
        Optional<struct http_request_parameters> match;

        match = endpoint.match("/leve1/value1/level2/value2");

        expect(match.isPresent()).toBe(true);
        expect(match.value().get("parameter1")).toBe("value1");
        expect(match.value().get("parameter2")).toBe("value2");
    });
});
