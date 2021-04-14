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
#include <fakeit/fakeit.hpp>
#include <mocks/cpputest.h>
#include <mocks/MockTemplatesService.h>

#include <list>
#include <templates/rest_api/TemplatesHttpResource.h>
#include <templates/TemplatesService.h>

using namespace fakeit;
using namespace std;

static HttpRequest post_request_with_invalid_name(
"{"
    "\"template_name\": \"arduino'; DELETE * from templates;\","
    "\"version\": \"1.0.0\","
    "\"payload\": \"ABCD\","
    "\"username\": \"johndoe\","
    "\"password\": \"pass\""
"}");

static HttpRequest post_request_with_invalid_version(
"{"
    "\"template_name\": \"arduino\","
    "\"version\": \"1.0\","
    "\"payload\": \"ABCD\","
    "\"username\": \"johndoe\","
    "\"password\": \"pass\""
"}");

static HttpRequest post_request_with_invalid_username(
"{"
    "\"template_name\": \"arduino'; DELETE * from templates;\","
    "\"version\": \"1.0.0\","
    "\"payload\": \"ABCD\","
    "\"username\": \"john} doe\","
    "\"password\": \"pass\""
"}");

static HttpRequest post_request_with_invalid_payload(
"{"
    "\"template_name\": \"arduino'; DELETE * from templates;\","
    "\"version\": \"1.0.0\","
    "\"payload\": \"ABCD !;Eabcde\","
    "\"username\": \"john} doe\","
    "\"password\": \"pass\""
"}");


TEST_GROUP(TemplatesHttpResource)
{
};


TEST_WITH_MOCK(TemplatesHttpResource, uses_the_templates_service_to_publish_a_template)
{
    HttpRequest request("{"
        "\"template_name\": \"arduino\","
        "\"version\": \"1.0.0\","
        "\"payload\": \"ABCDEabcde\","
        "\"username\": \"john_doe\","
        "\"password\": \"pass\""
    "}");
    HttpResponse response;
    Template temp;
    MockTemplatesService service_mock;
    TemplatesHttpResource api(&service_mock);
    Template templt;

    service_mock.expect("exists").andReturnValue(false);
    service_mock.expect("publishTemplate").ignoreOtherParameters().andReturnValue(&templt);

    response = api.post(request);

    ASSERT_EQUAL(HttpStatus::OK, response.status_code);
    ASSERT_STRING("", response.body);
    ASSERT_STRING("arduino", service_mock.last_publication_data.template_name);
    ASSERT_STRING("1.0.0", service_mock.last_publication_data.version);
    ASSERT_STRING("john_doe", service_mock.last_publication_data.username);
    ASSERT_STRING("ABCDEabcde", service_mock.last_publication_data.payload);
    ASSERT_STRING("pass", service_mock.last_publication_data.password);
}


TEST_WITH_MOCK(TemplatesHttpResource, returns_status_code_400_when_request_contains_invalid_fields)
{
    HttpResponse response;
    MockTemplatesService service_mock;
    TemplatesHttpResource api(&service_mock);

    ASSERT_EQUAL(HttpStatus::BAD_REQUEST, api.post(post_request_with_invalid_name).status_code);
    ASSERT_EQUAL(HttpStatus::BAD_REQUEST, api.post(post_request_with_invalid_version).status_code);
    ASSERT_EQUAL(HttpStatus::BAD_REQUEST, api.post(post_request_with_invalid_username).status_code);
    ASSERT_EQUAL(HttpStatus::BAD_REQUEST, api.post(post_request_with_invalid_payload).status_code);
}


TEST_WITH_MOCK(TemplatesHttpResource, returns_error_401_when_publishing_a_template_and_authentication_fails)
{
    HttpRequest request("{"
        "\"template_name\": \"arduino\","
        "\"version\": \"1.0.0\","
        "\"payload\": \"ABCDEabcde\","
        "\"username\": \"john_doe\","
        "\"password\": \"pass\""
    "}");
    HttpResponse response;
    MockTemplatesService service_mock;
    Mock<Authenticator> mock_authenticator;
    TemplatesHttpResource api(&service_mock, &mock_authenticator.get());

    When(Method(mock_authenticator, validCredentials)).Return(false);

    response = api.post(request);

    Verify(Method(mock_authenticator, validCredentials));
    ASSERT_EQUAL(HttpStatus::UNAUTHORIZED, response.status_code);
}


TEST_WITH_MOCK(TemplatesHttpResource, returns_error_409_when_publishing_a_template_and_version_already_exists)
{
    HttpRequest request("{"
                        "\"template_name\": \"arduino\","
                        "\"version\": \"1.0.0\","
                        "\"payload\": \"ABCDEabcde\","
                        "\"username\": \"john_doe\","
                        "\"password\": \"pass\""
                        "}");
    HttpResponse response;
    MockTemplatesService service_mock;
    TemplatesHttpResource api(&service_mock);

    service_mock.expect("exists").andReturnValue(true);

    response = api.post(request);

    ASSERT_EQUAL(HttpStatus::CONFLICT, response.status_code);
}


TEST_WITH_MOCK(TemplatesHttpResource, returns_error_404_when_downloading_a_template_that_is_not_found)
{
    HttpRequest request;
    HttpResponse response;
    MockTemplatesService service_mock;
    TemplatesHttpResource api(&service_mock);
    Maybe<Template> arduino_template;

    request.parameters.set("templateName", "arduino");
    service_mock.expect("templateBy").ignoreOtherParameters().andReturnValue(&arduino_template);

    response = api.get(request);

    ASSERT_EQUAL(HttpStatus::NOT_FOUND, response.status_code);
}


TEST_WITH_MOCK(TemplatesHttpResource, returns_latest_template_version_when_downloading_an_existing_template)
{
    HttpRequest request;
    HttpResponse response;
    MockTemplatesService service_mock;
    TemplatesHttpResource api(&service_mock);
    Maybe<Template> arduino_template = Template("arduino", "latest");

    request.parameters.set("templateName", "arduino");
    service_mock.expect("templateBy")
        .withStringParameter("template_name", "arduino")
        .withStringParameter("version", "latest")
        .andReturnValue(&arduino_template);

    response = api.get(request);

    ASSERT_EQUAL(HttpStatus::OK, response.status_code);
    ASSERT_STRING("{"
                 "\"payload\":\"\","
                 "\"template_name\":\"arduino\","
                 "\"version\":\"latest\""
                 "}", response.body);
}


TEST_WITH_MOCK(TemplatesHttpResource, returns_specific_template_version_when_downloading_an_existing_template)
{
    HttpRequest request;
    HttpResponse response;
    MockTemplatesService service_mock;
    TemplatesHttpResource api(&service_mock);
    Maybe<Template> arduino_template = Template("arduino", "1.1.0");

    request.parameters.set("templateName", "arduino");
    request.parameters.set("version", "1.1.0");
    service_mock.expect("templateBy")
        .withStringParameter("template_name", "arduino")
        .withStringParameter("version", "1.1.0")
        .andReturnValue(&arduino_template);

    response = api.get(request);

    ASSERT_EQUAL(HttpStatus::OK, response.status_code);
    ASSERT_STRING("{"
                 "\"payload\":\"\","
                 "\"template_name\":\"arduino\","
                 "\"version\":\"1.1.0\""
                 "}", response.body);
}


TEST_WITH_MOCK(TemplatesHttpResource, returns_invalid_request_when_getting_template_without_specifying_the_name)
{
    HttpRequest request;
    HttpResponse response;
    MockTemplatesService service_mock;
    TemplatesHttpResource api(&service_mock);

    response = api.get(request);

    ASSERT_EQUAL(HttpStatus::BAD_REQUEST, response.status_code);
}
