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
#include <fakeit/fakeit.hpp>

#include <authentication/AccessFileAuthenticator.h>

using namespace cest;
using namespace fakeit;
using namespace std;


describe("AccessFileAuthenticator", []() {
    it("allows setting the access file name", []() {
        Mock<Filesystem> mock_filesystem;
        AccessFileAuthenticator access_file_authenticator(&mock_filesystem.get());

        access_file_authenticator.setAccessFile(".access");
    });

    it("returns empty when authenticating api_key and access file is empty", []() {
        Mock<Filesystem> mock_filesystem;
        AccessFileAuthenticator access_file_authenticator(&mock_filesystem.get());
        Optional<string> username;

        access_file_authenticator.setAccessFile(".access");
        When(Method(mock_filesystem, readFile)).Return("");

        username = access_file_authenticator.authenticate("any_api_key");

        expect(username.isPresent()).toBe(false);
    });

    it("returns empty when authenticating api_key and user in file does not match", []() {
        Mock<Filesystem> mock_filesystem;
        AccessFileAuthenticator access_file_authenticator(&mock_filesystem.get());
        Optional<string> username;

        access_file_authenticator.setAccessFile(".access");
        When(Method(mock_filesystem, readFile)).Return("another_api_key: username");

        username = access_file_authenticator.authenticate("one_api_key");

        expect(username.isPresent()).toBe(false);
    });

    it("returns username when authenticating api_key and user in file matches api key", []() {
        Mock<Filesystem> mock_filesystem;
        AccessFileAuthenticator access_file_authenticator(&mock_filesystem.get());
        Optional<string> username;

        access_file_authenticator.setAccessFile(".access");
        When(Method(mock_filesystem, readFile)).Return("one_api_key:username");

        username = access_file_authenticator.authenticate("one_api_key");

        expect(username.isPresent()).toBe(true);
        expect(username.value()).toBe("username");
    });

    it("returns username when authenticating api_key and user in file with whitespace matches api key", []() {
        Mock<Filesystem> mock_filesystem;
        AccessFileAuthenticator access_file_authenticator(&mock_filesystem.get());
        Optional<string> username;

        access_file_authenticator.setAccessFile(".access");
        When(Method(mock_filesystem, readFile)).Return("\n one_api_key :  username \n\n");

        username = access_file_authenticator.authenticate("one_api_key");

        expect(username.isPresent()).toBe(true);
        expect(username.value()).toBe("username");
    });

    it("returns username when authenticating api_key and one of the users matches api key", []() {
        Mock<Filesystem> mock_filesystem;
        AccessFileAuthenticator access_file_authenticator(&mock_filesystem.get());
        Optional<string> username;

        access_file_authenticator.setAccessFile(".access");
        When(Method(mock_filesystem, readFile)).Return(
                "bob_api_key:  bob\n    "
                "alice_api_key:  alice\n");

        username = access_file_authenticator.authenticate("alice_api_key");

        expect(username.isPresent()).toBe(true);
        expect(username.value()).toBe("alice");
    });

    it("returns empty when authenticating api_key and none of the users matches api key", []() {
        Mock<Filesystem> mock_filesystem;
        AccessFileAuthenticator access_file_authenticator(&mock_filesystem.get());
        Optional<string> username;

        access_file_authenticator.setAccessFile(".access");
        When(Method(mock_filesystem, readFile)).Return(
                "bob_api_key:  bob\n    "
                "alice_api_key:  alice\n");

        username = access_file_authenticator.authenticate("paul_api_key");

        expect(username.isPresent()).toBe(false);
    });
});