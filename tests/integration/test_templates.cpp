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

#include <authentication/TrivialAuthenticator.h>
#include <templates/rest_api/TemplatesHttpResource.h>
#include <templates/TemplatesRepositoryInSqlite.h>

TEST_GROUP(Templates) {
};


TEST(Templates, a_template_can_be_published_when_no_authentication_is_configured)
{
    HttpRequest request("{"
                        "\"template_name\": \"arduino-uno\","
                        "\"version\": \"1.0.0\","
                        "\"payload\": \"ABCDEabcde\","
                        "\"username\": \"john_doe\","
                        "\"password\": \"12345\""
                        "}");
    HttpResponse response;
    SqlDatabaseSqlite3 database(":memory:");
    TemplatesRepositoryInSqlite repository(&database);
    TemplatesService service(&repository);
    TemplatesHttpResource api(&service);

    response = api.post(request);

    ASSERT_EQUAL(HttpStatus::OK, response.status_code);
    ASSERT_STRING("", response.body);
}


TEST(Templates, a_template_fails_to_be_published_when_authentication_fails)
{
    HttpRequest request("{"
                        "\"template_name\": \"arduino-uno\","
                        "\"version\": \"1.0.0\","
                        "\"payload\": \"ABCDEabcde\","
                        "\"username\": \"john_doe\","
                        "\"password\": \"12345\""
                        "}");
    HttpResponse response;
    SqlDatabaseSqlite3 database(":memory:");
    TemplatesRepositoryInSqlite repository(&database);
    TemplatesService service(&repository);
    TrivialAuthenticator authenticator;
    TemplatesHttpResource api(&service, &authenticator);

    response = api.post(request);

    ASSERT_EQUAL(HttpStatus::UNAUTHORIZED, response.status_code);
}


TEST(Templates, a_template_can_be_published_when_authentication_is_configured)
{
    HttpRequest request("{"
                        "\"template_name\": \"arduino-uno\","
                        "\"version\": \"1.0.0\","
                        "\"payload\": \"ABCDEabcde\","
                        "\"username\": \"john_doe\","
                        "\"password\": \"12345\""
                        "}");
    HttpResponse response;
    SqlDatabaseSqlite3 database(":memory:");
    TemplatesRepositoryInSqlite repository(&database);
    TemplatesService service(&repository);
    TrivialAuthenticator authenticator;
    TemplatesHttpResource api(&service, &authenticator);
    UserCredentials user_credentials{"john_doe", "12345", "john_doe@example.com"};

    authenticator.addUser(user_credentials);

    response = api.post(request);

    ASSERT_EQUAL(HttpStatus::OK, response.status_code);
    ASSERT_STRING("", response.body);
}


TEST(Templates, a_template_can_be_downloaded_by_name_and_version_after_publication)
{
    HttpRequest publish_request("{"
                        "\"template_name\": \"arduino-uno\","
                        "\"version\": \"1.0.0\","
                        "\"payload\": \"ABCDEabcde\","
                        "\"username\": \"john_doe\","
                        "\"password\": \"12345\""
                        "}");
    HttpRequest download_request;
    HttpResponse response;
    SqlDatabaseSqlite3 database(":memory:");
    TemplatesRepositoryInSqlite repository(&database);
    TemplatesService service(&repository);
    TemplatesHttpResource api(&service);

    api.post(publish_request);

    download_request.parameters.set("templateName", "arduino-uno");
    download_request.parameters.set("version", "1.0.0");
    response = api.get(download_request);

    ASSERT_EQUAL(HttpStatus::OK, response.status_code);
    ASSERT_STRING("{\"payload\":\"ABCDEabcde\",\"template_name\":\"arduino-uno\",\"version\":\"1.0.0\"}", response.body);
}


TEST(Templates, a_template_is_not_found_when_not_published)
{
    HttpRequest publish_request("{"
                        "\"template_name\": \"arduino-uno\","
                        "\"version\": \"1.0.0\","
                        "\"payload\": \"ABCDEabcde\","
                        "\"username\": \"john_doe\","
                        "\"password\": \"12345\""
                        "}");
    HttpRequest download_request;
    HttpResponse response;
    SqlDatabaseSqlite3 database(":memory:");
    TemplatesRepositoryInSqlite repository(&database);
    TemplatesService service(&repository);
    TemplatesHttpResource api(&service);

    api.post(publish_request);

    download_request.parameters.set("templateName", "arduino-uno");
    download_request.parameters.set("version", "1.1.0");
    response = api.get(download_request);

    ASSERT_EQUAL(HttpStatus::NOT_FOUND, response.status_code);
}
