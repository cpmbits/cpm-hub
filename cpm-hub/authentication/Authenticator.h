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
#pragma once

#include <string>
#include <infrastructure/Maybe.h>
#include <authentication/UserCredentials.h>


class Authenticator {
public:
    virtual Maybe<std::string> authenticate(const char *key) = 0;

    virtual bool validCredentials(UserCredentials &credentials) = 0;

    virtual void addUser(UserCredentials &credentials) = 0;

    virtual void addUserWithInvitation(UserCredentials &credentials, std::string invitation_token) = 0;
};


class NullAuthenticator: public Authenticator {
public:
    Maybe <std::string> authenticate(const char *key) {
        return Maybe<std::string>("john_doe");
    }

    bool validCredentials(UserCredentials &credentials) {
        return true;
    }

    void addUser(UserCredentials &credentials) {}

    void addUserWithInvitation(UserCredentials &credentials, std::string invitation_token) {}
};


class AuthenticationFailure: public std::exception {
public:
    AuthenticationFailure() throw() {
        sprintf(message, "unauthorized");
    }

    const char *what() const throw () {
        return message;
    }

private:
    char message[256];
};


class InvalidInvitationToken: public std::exception {
public:
    InvalidInvitationToken() throw() {
        sprintf(message, "invalid invitation token");
    }

    const char *what() const throw () {
        return message;
    }

private:
    char message[256];
};
