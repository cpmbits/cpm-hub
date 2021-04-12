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
                                                    "\"password\":\"password\","
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


TEST_WITH_MOCK(TemplatesContract, returns_empty_data_when_credentials_are_missing)
{
    Result<TemplatePublicationData> publication_data;

    publication_data = TemplatesContract::parsePost("{"
                                                    "\"template_name\":\"template_name\","
                                                    "\"version\":\"1·f*0.0\","
                                                    "\"payload\":\"Yml0IHBheWxvYWQ=;\""
                                                    "}");
    ASSERT_FALSE(publication_data.success());
}
