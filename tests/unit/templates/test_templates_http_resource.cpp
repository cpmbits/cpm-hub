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
#include <fakeit/fakeit.hpp>

#include <list>
#include <templates/rest_api/TemplatesHttpResource.h>
#include <templates/TemplatesService.h>

using namespace fakeit;
using namespace std;

HttpRequest post_request_with_invalid_name(
"{"
    "\"template_name\": \"cest'; DELETE * from bits;\","
    "\"version\": \"1.0\","
    "\"payload\": \"ABCD\","
    "\"username\": \"johndoe\","
    "\"password\": \"pass\""
"}");

HttpRequest post_request_with_invalid_version(
"{"
    "\"template_name\": \"cest\","
    "\"version\": \"dd 1.0\","
    "\"payload\": \"ABCD\","
    "\"username\": \"johndoe\","
    "\"password\": \"pass\""
"}");

HttpRequest post_request_with_invalid_username(
"{"
    "\"template_name\": \"cest'; DELETE * from bits;\","
    "\"version\": \"dd 1.0\","
    "\"payload\": \"ABCD\","
    "\"username\": \"john} doe\","
    "\"password\": \"pass\""
"}");

HttpRequest post_request_with_invalid_payload(
"{"
    "\"template_name\": \"cest'; DELETE * from bits;\","
    "\"version\": \"dd 1.0\","
    "\"payload\": \"ABCD !;Eabcde\","
    "\"username\": \"john} doe\","
    "\"password\": \"pass\""
"}");


TEST_GROUP(TemplatesHttpResource)
{
};


TEST(TemplatesHttpResource, uses_the_templates_service_to_publish_a_template)
{
    HttpRequest request("{"
        "\"template_name\": \"cest\","
        "\"version\": \"1.0\","
        "\"payload\": \"ABCDEabcde\","
        "\"username\": \"john_doe\","
        "\"password\": \"pass\""
    "}");
    HttpResponse response;
    Template temp;
    Mock<TemplatesService> mock_service;
    TemplatesHttpResource api(&mock_service.get());

    When(Method(mock_service, publishTemplate)).Return(temp);
    When(OverloadedMethod(mock_service, templateBy, Maybe<Template>(string, string))).Return(Maybe<Template>());

    response = api.post(request);

    ASSERT_EQUAL(200, response.status_code);
    ASSERT_STRING("", response.body);
    Verify(Method(mock_service, publishTemplate).Matching([](struct TemplatePublicationData data) {
        return data.template_name == "cest" &&
               data.version == "1.0" &&
               data.user_name == "john_doe" &&
               data.payload == "ABCDEabcde";
    }));
}

TEST(TemplatesHttpResource, returns_status_code_400_when_request_contains_invalid_fields)
{
    HttpResponse response;
    Template bit("");
    Mock<TemplatesService> mock_service;
    TemplatesHttpResource api(&mock_service.get());

    ASSERT_EQUAL(400, api.post(post_request_with_invalid_name).status_code);
    ASSERT_EQUAL(400, api.post(post_request_with_invalid_version).status_code);
    ASSERT_EQUAL(400, api.post(post_request_with_invalid_username).status_code);
    ASSERT_EQUAL(400, api.post(post_request_with_invalid_payload).status_code);
}


//describe("Templates API", []() {
//
//    it("returns status code 400 when request contains invalid fields", [&]() {
//        HttpResponse response;
//        Template bit("");
//        Mock<TemplatesService> mock_service;
//        TemplatesHttpResource api(&mock_service.get());
//
//        expect(api.post(post_request_with_invalid_template_name).status_code).toBe(400);
//        expect(api.post(post_request_with_invalid_version).status_code).toBe(400);
//        expect(api.post(post_request_with_invalid_username).status_code).toBe(400);
//        expect(api.post(post_request_with_invalid_payload).status_code).toBe(400);
//    });
//
//    it("returns error 401 when publishing a bit and authentication fails", [&]() {
//        HttpRequest request("{"
//            "\"template_name\": \"cest\","
//            "\"version\": \"1.0\","
//            "\"payload\": \"ABCDEabcde\","
//            "\"username\": \"john_doe\","
//            "\"password\": \"pass\""
//        "}");
//        HttpResponse response;
//        Mock<TemplatesService> mock_service;
//        Mock<Authenticator> mock_authenticator;
//        TemplatesHttpResource api(&mock_service.get(), &mock_authenticator.get());
//
//        When(Method(mock_authenticator, validCredentials)).Return(false);
//
//        response = api.post(request);
//
//        expect(response.status_code).toBe(HttpStatus::UNAUTHORIZED);
//    });
//
//    it("returns error 409 when publishing a bit and version already exists", [&]() {
//        HttpRequest request("{"
//                            "\"template_name\": \"cest\","
//                            "\"version\": \"1.0\","
//                            "\"payload\": \"ABCDEabcde\","
//                            "\"username\": \"john_doe\","
//                            "\"password\": \"pass\""
//                            "}");
//        HttpResponse response;
//        Mock<TemplatesService> mock_service;
//        Mock<Authenticator> mock_authenticator;
//        TemplatesHttpResource api(&mock_service.get(), &mock_authenticator.get());
//        Optional<Template> cest_bit;
//
//        request.parameters.set("bitName", "cest");
//        cest_bit = Template("cest", "1.0", "user", "ABCDEabcde");
//
//        When(Method(mock_authenticator, validCredentials)).Return(true);
//        When(OverloadedMethod(mock_service, bitBy, Optional<Template>(string, string))).Return(cest_bit);
//
//        response = api.post(request);
//
//        expect(response.status_code).toBe(HttpStatus::CONFLICT);
//    });
//
//    it("returns error 404 when downloading a bit that is not found", [&]() {
//        HttpRequest request;
//        HttpResponse response;
//        Mock<TemplatesService> mock_service;
//        TemplatesHttpResource api(&mock_service.get());
//        Optional<Template> no_bit;
//
//        request.parameters.set("bitName", "cest");
//        When(OverloadedMethod(mock_service, bitBy, Optional<Template>(string))).Return(no_bit);
//
//        response = api.get(request);
//
//        Verify(OverloadedMethod(mock_service, bitBy, Optional<Template>(string)).Using("cest"));
//        expect(response.status_code).toBe(404);
//    });
//
//    it("returns latest bit version when downloading an existing bit", [&]() {
//        HttpRequest request;
//        HttpResponse response;
//        Mock<TemplatesService> mock_service;
//        TemplatesHttpResource api(&mock_service.get());
//        Optional<Template> cest_bit;
//
//        request.parameters.set("bitName", "cest");
//        cest_bit = Template("cest", "1.0", "user", "ABCDEabcde");
//        When(OverloadedMethod(mock_service, bitBy, Optional<Template>(string))).Return(cest_bit);
//
//        response = api.get(request);
//
//        Verify(OverloadedMethod(mock_service, bitBy, Optional<Template>(string)).Using("cest"));
//        expect(response.status_code).toBe(200);
//        expect(response.body).toBe("{"
//            "\"template_name\":\"cest\","
//            "\"payload\":\"ABCDEabcde\","
//            "\"version\":\"1.0\""
//        "}");
//    });
//
//    it("returns specific bit version when downloading an existing bit", [&]() {
//        HttpRequest request;
//        HttpResponse response;
//        Mock<TemplatesService> mock_service;
//        TemplatesHttpResource api(&mock_service.get());
//        Optional<Template> cest_bit;
//
//        request.parameters.set("bitName", "cest");
//        request.parameters.set("bitVersion", "1.1");
//        cest_bit = Template("cest", "1.1", "user", "ABCDEabcde");
//        When(OverloadedMethod(mock_service, bitBy, Optional<Template>(string, string))).Return(cest_bit);
//
//        response = api.get(request);
//
//        Verify(OverloadedMethod(mock_service, bitBy, Optional<Template>(string, string)).Using("cest", "1.1"));
//        expect(response.status_code).toBe(200);
//        expect(response.body).toBe("{"
//           "\"template_name\":\"cest\","
//           "\"payload\":\"ABCDEabcde\","
//           "\"version\":\"1.1\""
//        "}");
//    });
//
//    it("uses the bit service to search for bits based on given criteria", [&]() {
//        HttpRequest request;
//        HttpResponse response;
//        Mock<TemplatesService> mock_service;
//        TemplatesHttpResource api(&mock_service.get());
//        std::list<TemplateMetadata> one_bit_found = {
//            TemplateMetadata("cest", "pepe", "1.0"),
//        };
//
//        request.query_parameters.set("name", "cest");
//        When(Method(mock_service, search)).Return(one_bit_found);
//
//        response = api.get(request);
//
//        expect(response.status_code).toBe(HttpStatus::OK);
//        Verify(Method(mock_service, search).Matching([](TemplateSearchQuery search_query) {
//            return search_query.name == "cest";
//        }));
//        expect(response.body).toBe("[{\"author\":\"pepe\",\"name\":\"cest\"}]");
//    });
//
//    it("returns bad request when search query does not contain 'name' parameter", [&]() {
//        HttpRequest request;
//        HttpResponse response;
//        Mock<TemplatesService> mock_service;
//        TemplatesHttpResource api(&mock_service.get());
//
//        response = api.get(request);
//
//        expect(response.status_code).toBe(HttpStatus::BAD_REQUEST);
//    });
//
//    it("returns bad request when 'name' parameter in search query is empty", [&]() {
//        HttpRequest request;
//        HttpResponse response;
//        Mock<TemplatesService> mock_service;
//        TemplatesHttpResource api(&mock_service.get());
//
//        request.query_parameters.set("name", "");
//        response = api.get(request);
//
//        expect(response.status_code).toBe(HttpStatus::BAD_REQUEST);
//    });
//});
