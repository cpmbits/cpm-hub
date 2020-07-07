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

#include <http/http_url_parameters_encoder.h>


describe("HTTP url parameters encoder", []() {
    it("encodes no parameters", []() {
        HttpParameterMap parameters;
        expect(encodeUrlParameters(parameters)).toBe("");
    });

    it("encodes a single header", []() {
        HttpParameterMap parameters;

        parameters.set("parameter1", "value1");

        expect(encodeUrlParameters(parameters)).toBe("?parameter1=value1");
    });

    it("encodes many parameters", []() {
        HttpParameterMap parameters;

        parameters.set("parameter1", "value1");
        parameters.set("parameter2", "value2");

        expect(encodeUrlParameters(parameters)).toBe("?parameter1=value1&parameter2=value2");
    });
});