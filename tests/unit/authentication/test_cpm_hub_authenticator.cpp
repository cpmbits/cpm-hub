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
#include <mocks/MockHttpClient.h>
#include <mocks/cpputest.h>

#include <authentication/CpmHubAuthenticator.h>

using namespace std;


TEST_GROUP(CpmHubAuthenticator)
{
};


TEST_WITH_MOCK(CpmHubAuthenticator, fails_to_authenticate_when_cpm_hub_auth_service_is_down)
{
    MockHttpClient http_client;
    CpmHubAuthenticator authenticator("http://localhost:1234", http_client);
    UserCredentials credentials = {"user", "pass"};
    HttpResponse not_found = HttpResponse::notFound();

    http_client.expect("post")
               .withParameter("url", "http://localhost:1234/auth")
               .ignoreOtherParameters()
               .andReturnValue(&not_found);

    ASSERT_FALSE(authenticator.validCredentials(credentials));
}


TEST_WITH_MOCK(CpmHubAuthenticator, fails_to_authenticate_when_not_authorized)
{
    MockHttpClient http_client;
    CpmHubAuthenticator authenticator("http://localhost:1234", http_client);
    UserCredentials credentials = {"user", "pass"};
    HttpResponse unauthorized = HttpResponse::unauthorized();

    http_client.expect("post")
               .withParameter("url", "http://localhost:1234/auth")
               .ignoreOtherParameters()
               .andReturnValue(&unauthorized);

    ASSERT_FALSE(authenticator.validCredentials(credentials));
}


TEST_WITH_MOCK(CpmHubAuthenticator, confirms_valid_credentials_when_authorized)
{
    MockHttpClient http_client;
    CpmHubAuthenticator authenticator("http://localhost:1234", http_client);
    UserCredentials credentials = {"user", "pass"};
    HttpRequest request = HttpRequest("{\"password\":\"7be1b497736a4478f45a07661468dd282edc01d31a403641dd3e2a07cac4fc05\",\"username\":\"user\"}");
    HttpResponse ok = HttpResponse::ok("");

    http_client.expect("post")
               .withParameter("url", "http://localhost:1234/auth")
               .andReturnValue(&ok);

    ASSERT_TRUE(authenticator.validCredentials(credentials));
}


TEST_WITH_MOCK(CpmHubAuthenticator, sends_request_to_authentication_service_when_adding_user)
{
    MockHttpClient http_client;
    CpmHubAuthenticator authenticator("http://localhost:1234", http_client);
    UserCredentials credentials = {"user", "pass"};
    HttpRequest request = HttpRequest("{\"password\":\"7be1b497736a4478f45a07661468dd282edc01d31a403641dd3e2a07cac4fc05\",\"username\":\"user\"}");
    HttpResponse ok = HttpResponse::ok("");

    http_client.expect("post")
               .withParameter("url", "http://localhost:1234/users")
               .andReturnValue(&ok);

    authenticator.addUser(credentials);
}


TEST_WITH_MOCK(CpmHubAuthenticator, sends_request_to_authentication_service_when_adding_user_by_invitation)
{
    MockHttpClient http_client;
    CpmHubAuthenticator authenticator("http://localhost:1234", http_client);
    UserCredentials credentials = {"user", "pass"};
    HttpResponse ok = HttpResponse::ok("");

    http_client.expect("post")
               .withParameter("url", "http://localhost:1234/users")
               .andReturnValue(&ok);

    authenticator.addUserWithInvitation(credentials, "invitation_token");

    ASSERT_STRING(http_client.last_request.body, "{\"password\":\"7be1b497736a4478f45a07661468dd282edc01d31a403641dd3e2a07cac4fc05\",\"username\":\"user\"}");
    ASSERT_STRING(http_client.last_request.headers.get("Content-type"), "application/json");
    ASSERT_STRING(http_client.last_request.headers.get("OTP"), "invitation_token");
}



TEST_WITH_MOCK(CpmHubAuthenticator, throws_an_invalid_invitation_token_exception_when_receiving_unauthorized_response)
{
    MockHttpClient http_client;
    CpmHubAuthenticator authenticator("http://localhost:1234", http_client);
    UserCredentials credentials = {"user", "pass"};
    HttpResponse ok = HttpResponse::unauthorized();

    http_client.expect("post")
            .withParameter("url", "http://localhost:1234/users")
            .andReturnValue(&ok);

    ASSERT_THROWS(InvalidInvitationToken, authenticator.addUserWithInvitation(credentials, "invitation_token"));
}
