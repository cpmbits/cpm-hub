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

using namespace cest;
using namespace std;


describe("TrivialAuthenticator", []() {
    it("allows adding users", []() {
        TrivialAuthenticator authenticator;
        UserCredentials credentials = {"user", "pass"};

        authenticator.addUser(credentials);
    });

    it("fails to validCredentials user when user is not in the database", []() {
        TrivialAuthenticator authenticator;
        UserCredentials credentials = {"user", "pass"};

        expect(authenticator.validCredentials(credentials)).toBe(false);
    });

    it("authenticates user when api_key is in the database", []() {
        TrivialAuthenticator authenticator;
        UserCredentials credentials = {"user", "pass"};

        authenticator.addUser(credentials);

        expect(authenticator.validCredentials(credentials)).toBe(false);
    });
});