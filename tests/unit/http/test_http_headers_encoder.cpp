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

#include <http/http_headers_encoder.h>


describe("HTTP headers encoder", []() {
    it("encodes no headers", []() {
        HttpParameterMap headers;
        expect(encodeHeaders(headers)).toBe("");
    });

    it("encodes a single header", []() {
        HttpParameterMap headers;

        headers.set("header1", "value1");

        expect(encodeHeaders(headers)).toBe("header1: value1");
    });

    it("encodes many headers", []() {
        HttpParameterMap headers;

        headers.set("header1", "value1");
        headers.set("header2", "value2");

        expect(encodeHeaders(headers)).toBe(
            "header1: value1\r\n"
            "header2: value2"
        );
    });
});