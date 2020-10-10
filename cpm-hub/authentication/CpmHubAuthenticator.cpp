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
#include <json/json.hpp>
#include <blake3/blake3.h>
#include <authentication/CpmHubAuthenticator.h>

using namespace std;
using namespace nlohmann;

constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};


CpmHubAuthenticator::CpmHubAuthenticator(const string &auth_service_url, HttpClient &client)
{
    this->authentication_endpoint = auth_service_url + "/auth";
    this->users_endpoint = auth_service_url + "/users";
    this->client = &client;
}


static string hashed(string &password)
{
    uint8_t bin_output[BLAKE3_OUT_LEN];
    string output(BLAKE3_OUT_LEN * 2, ' ');
    blake3_hasher hasher;

    blake3_hasher_init(&hasher);
    blake3_hasher_update(&hasher, password.c_str(), password.size());
    blake3_hasher_finalize(&hasher, bin_output, BLAKE3_OUT_LEN);

    for (int i=0; i<BLAKE3_OUT_LEN; ++i) {
        output[2*i] = hexmap[(bin_output[i] & 0xF0) >> 4];
        output[2*i+1] = hexmap[bin_output[i] & 0x0F];
    }

    return output;
}


static string asJson(UserCredentials &credentials)
{
    json json_credentials = {
        {"username", credentials.username},
        {"password", hashed(credentials.password)},
    };

    return json_credentials.dump();
}


bool CpmHubAuthenticator::validCredentials(UserCredentials &credentials)
{
    HttpRequest request(asJson(credentials));
    HttpResponse response;

    request.headers.set("Content-type", "application/json");
    response = this->client->post(this->authentication_endpoint, request);

    return response.status_code == HttpStatus::OK;
}


Optional<string> CpmHubAuthenticator::authenticate(const char *key)
{
    return Optional<string>();
}


void CpmHubAuthenticator::addUser(UserCredentials &credentials)
{
    HttpRequest request(asJson(credentials));
    request.headers.set("Content-type", "application/json");
    this->client->post(this->users_endpoint, request);
}


void CpmHubAuthenticator::addUserWithInvitation(UserCredentials &credentials, std::string invitation_token)
{
    HttpRequest request(asJson(credentials));
    HttpResponse response;

    request.headers.set("Content-type", "application/json");
    request.headers.set("OTP", invitation_token);
    response = this->client->post(this->users_endpoint, request);

    if (response.status_code == HttpStatus::UNAUTHORIZED) {
        throw InvalidInvitationToken();
    }
}

