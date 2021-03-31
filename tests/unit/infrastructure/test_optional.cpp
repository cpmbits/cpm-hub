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

#include <infrastructure/Maybe.h>


describe("Optional", []() {
    it("doesn't have value after creation", []() {
        Maybe<int> optional;

        expect(optional.isPresent()).toBe(false);
    });

    it("has value after assigning one", []() {
        Maybe<int> optional;

        optional = 32;

        expect(optional.isPresent()).toBe(true);
        expect(optional.value()).toBe(32);
    });

    it("throws an exception when requesting non stored value", []() {
        Maybe<int> optional;

        try {
            optional.value();
            expect(true).toBe(false);
        } catch(const char *msg) {
        }
    });
});
