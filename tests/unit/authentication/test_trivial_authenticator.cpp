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
        string username("user");
        string api_key("password");

        authenticator.addUser(username, api_key);
    });

    it("fails to authenticate user when api_key is not in the database", []() {
        TrivialAuthenticator authenticator;
        Optional<string> user;

        user = authenticator.authenticate("api_key");
        expect(user.isPresent()).toBe(false);
    });

    it("authenticates user when api_key is in the database", []() {
        TrivialAuthenticator authenticator;
        string username("user");
        string api_key("api_key");
        Optional<string> user;

        authenticator.addUser(username, api_key);

        user = authenticator.authenticate("api_key");
        expect(user.value()).toBe("user");
    });
});