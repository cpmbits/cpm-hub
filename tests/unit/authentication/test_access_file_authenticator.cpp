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
#include <mocks/cpputest.h>
#include <mocks/MockFilesystem.h>

#include <authentication/AccessFileAuthenticator.h>

using namespace std;


TEST_GROUP(AccessFileAuthenticator)
{
    void teardown()
    {
        mock().clear();
    }
};



TEST_WITH_MOCK(AccessFileAuthenticator, allows_setting_the_access_file_name)
{
    MockFilesystem filesystem;
    AccessFileAuthenticator access_file_authenticator(&filesystem);

    access_file_authenticator.setAccessFile(".access");
}


TEST_WITH_MOCK(AccessFileAuthenticator, returns_empty_when_authenticating_api_key_and_access_file_is_empty)
{
    MockFilesystem filesystem;
    AccessFileAuthenticator access_file_authenticator(&filesystem);
    Maybe<string> username;

    access_file_authenticator.setAccessFile(".access");
    filesystem.expect("readFile")
              .withParameter("file_name", ".access")
              .andReturnValue("");

    username = access_file_authenticator.authenticate("any_api_key");

    ASSERT_FALSE(username.isPresent());
}


TEST_WITH_MOCK(AccessFileAuthenticator, returns_empty_when_authenticating_api_key_and_user_in_file_does_not_match)
{
    MockFilesystem filesystem;
    AccessFileAuthenticator access_file_authenticator(&filesystem);
    Maybe<string> username;

    access_file_authenticator.setAccessFile(".access");
    filesystem.expect("readFile")
              .withParameter("file_name", ".access")
              .andReturnValue("another_api_key: username");

    username = access_file_authenticator.authenticate("one_api_key");

    ASSERT_FALSE(username.isPresent());
}


TEST_WITH_MOCK(AccessFileAuthenticator, returns_username_when_authenticating_api_key_and_user_in_file_matches_api_key)
{
    MockFilesystem filesystem;
    AccessFileAuthenticator access_file_authenticator(&filesystem);
    Maybe<string> username;

    access_file_authenticator.setAccessFile(".access");
    filesystem.expect("readFile")
              .withParameter("file_name", ".access")
              .andReturnValue("one_api_key:username");

    username = access_file_authenticator.authenticate("one_api_key");

    ASSERT_TRUE(username.isPresent());
    ASSERT_STRING("username", username.value());
}


TEST_WITH_MOCK(AccessFileAuthenticator, returns_username_when_authenticating_api_key_and_user_in_file_with_whitespace_matches_api_key)
{
    MockFilesystem filesystem;
    AccessFileAuthenticator access_file_authenticator(&filesystem);
    Maybe<string> username;

    access_file_authenticator.setAccessFile(".access");
    filesystem.expect("readFile")
            .withParameter("file_name", ".access")
            .andReturnValue("\n one_api_key :  username \n\n");

    username = access_file_authenticator.authenticate("one_api_key");

    ASSERT_TRUE(username.isPresent());
    ASSERT_STRING("username", username.value());
}


TEST_WITH_MOCK(AccessFileAuthenticator, returns_username_when_authenticating_api_key_and_one_of_the_users_matches_api_key)
{
    MockFilesystem filesystem;
    AccessFileAuthenticator access_file_authenticator(&filesystem);
    Maybe<string> username;

    access_file_authenticator.setAccessFile(".access");
    filesystem.expect("readFile")
            .withParameter("file_name", ".access")
            .andReturnValue("bob_api_key:  bob\n    "
                            "alice_api_key:  alice\n");

    username = access_file_authenticator.authenticate("alice_api_key");

    ASSERT_TRUE(username.isPresent());
    ASSERT_STRING("alice", username.value());
}


TEST_WITH_MOCK(AccessFileAuthenticator, returns_empty_when_authenticating_api_key_and_none_of_the_users_matches_api_key)
{
    MockFilesystem filesystem;
    AccessFileAuthenticator access_file_authenticator(&filesystem);
    Maybe<string> username;

    access_file_authenticator.setAccessFile(".access");
    filesystem.expect("readFile")
            .withParameter("file_name", ".access")
            .andReturnValue("bob_api_key:  bob\n    "
                            "alice_api_key:  alice\n");

    username = access_file_authenticator.authenticate("paul_api_key");

    ASSERT_FALSE(username.isPresent());
}
