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

#include <templates/rest_api/TemplatesContract.h>

TEST_GROUP(TemplatesContract)
{
};


TEST_WITH_MOCK(TemplatesContract, returns_empty_data_when_request_has_missing_fields)
{
    ASSERT_FALSE(TemplatesContract::parsePost("{}").success());
}


TEST_WITH_MOCK(TemplatesContract, returns_publication_data_when_request_is_valid)
{
    Result<TemplatePublicationData> publication_data;

    publication_data = TemplatesContract::parsePost("{"
                                                    "\"template_name\":\"template_name\","
                                                    "\"version\":\"1.0.0\","
                                                    "\"username\":\"username\","
                                                    "\"payload\":\"payload\""
                                                    "}");
    ASSERT_TRUE(publication_data.success());
}


TEST_WITH_MOCK(TemplatesContract, returns_empty_data_when_request_is_invalid)
{
    Result<TemplatePublicationData> publication_data;

    publication_data = TemplatesContract::parsePost("{"
                                                    "\"template_name\":\"template_name\","
                                                    "\"version\":\"1·f*0.0\","
                                                    "\"username\":\"27John;()Doe27\","
                                                    "\"payload\":\"Yml0IHBheWxvYWQ=;\""
                                                    "}");
    ASSERT_FALSE(publication_data.success());
}


//TEST_WITH_MOCK(TemplatesContract, validate_bit_version)
//{
//    ASSERT_TRUE(validBitVersion("1"));
//    ASSERT_TRUE(validBitVersion("12"));
//    ASSERT_TRUE(validBitVersion("1.35"));
//    ASSERT_TRUE(validBitVersion("1.35.16"));
//    ASSERT_TRUE(validBitVersion("1.35.16-p1"));
//    ASSERT_TRUE(validBitVersion("1.35.16-rc14"));
//    ASSERT_TRUE(validBitVersion("1.35.16-final"));
//    ASSERT_FALSE(validBitVersion("1.35.16-toolongprefixforthebitversion"));
//    ASSERT_FALSE(validBitVersion("1.35.16-prefix;with\"invaliðchar$"));
//    ASSERT_FALSE(validBitVersion("latest"));
//    ASSERT_FALSE(validBitVersion("1."));
//    ASSERT_FALSE(validBitVersion("1.-rc14 \";name27"));
//}
//
//
//TEST_WITH_MOCK(TemplatesContract, validate_username)
//{
//    ASSERT_TRUE(validBitUsername("johndoe"));
//    ASSERT_TRUE(validBitUsername("JohnDoe"));
//    ASSERT_TRUE(validBitUsername("JohnDoe27"));
//    ASSERT_TRUE(validBitUsername("John_Doe27"));
//    ASSERT_TRUE(validBitUsername("John-Doe27"));
//    ASSERT_TRUE(validBitUsername("John.Doe27"));
//    ASSERT_FALSE(validBitUsername("John\"Doe27"));
//    ASSERT_FALSE(validBitUsername(" John Doe27 "));
//    ASSERT_FALSE(validBitUsername("27John\"Doe27"));
//    ASSERT_FALSE(validBitUsername("27John;()Doe27"));
//}
//
//
//TEST_WITH_MOCK(TemplatesContract, validate_payload)
//{
//    ASSERT_TRUE(validPayload("Yml0IHBheWxvYWQ="));
//    ASSERT_TRUE(validPayload("Yml0I//HBheWxvYWQ=+"));
//    ASSERT_FALSE(validPayload("Yml0IHBheWxvYWQ=\n"));
//    ASSERT_FALSE(validPayload("Yml0IHBheWxvYWQ=\""));
//    ASSERT_FALSE(validPayload("Yml0IHBheWxvYWQ=;"));
//}
