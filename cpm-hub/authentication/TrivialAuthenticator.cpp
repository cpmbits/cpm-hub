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
#include <authentication/TrivialAuthenticator.h>
#include <infrastructure/Maybe.h>

using namespace std;


Maybe<std::string> TrivialAuthenticator::authenticate(const char *key)
{
    auto iter = this->users_by_key.find(key);
    Maybe<string> username;

    if (iter == this->users_by_key.end()) {
        return username;
    }

    username = iter->second;

    return username;
}


void TrivialAuthenticator::addUser(UserCredentials &credentials)
{
    users_by_key.insert(make_pair(credentials.password, credentials.username));
    users[credentials.username] = credentials.password;
}


bool TrivialAuthenticator::validCredentials(UserCredentials &credentials)
{
    auto iter = users.find(credentials.username);

    if (iter == users.end()) {
        return false;
    }


    return iter->second == credentials.password;
}


void TrivialAuthenticator::addUserWithInvitation(UserCredentials &credentials, std::string invitation_token)
{
}
