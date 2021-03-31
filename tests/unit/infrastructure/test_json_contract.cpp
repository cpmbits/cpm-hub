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

#include <infrastructure/JsonContract.h>

using namespace std;


struct ContractData {
    string field1;
    string field2;
};


TEST_GROUP(JsonContract)
{
};


TEST_WITH_MOCK(JsonContract, can_be_defined)
{
    JsonContract contract({
      {JsonContract::FieldPresence::Required, "field1", "string" },
      {JsonContract::FieldPresence::Optional, "field2", "string" },
    });
}


TEST_WITH_MOCK(JsonContract, validates_presence_of_required_field)
{
    JsonContract contract({
      {JsonContract::FieldPresence::Required, "field1", "string"},
    });

    ValidatedContract validated_contract = contract.validate(R"({"field1": "value"})");

    ASSERT_TRUE(validated_contract.isValid());
}


TEST_WITH_MOCK(JsonContract, returns_value_of_valid_contract_field)
{
    JsonContract contract({
      {JsonContract::FieldPresence::Required, "field1", "string"},
    });

    ValidatedContract validated_contract = contract.validate(R"({"field1": "value"})");

    ASSERT_STRING("value", validated_contract.stringAt("field1"));
}


TEST_WITH_MOCK(JsonContract, returns_error_message_when_contract_field_is_missing)
{
    JsonContract contract({
        {JsonContract::FieldPresence::Required, "field1", "string"},
    });

    ValidatedContract validated_contract = contract.validate(R"({})");

    ASSERT_FALSE(validated_contract.isValid());
    ASSERT_STRING("field1 is missing", validated_contract.errorAt("field1"));
}


TEST_WITH_MOCK(JsonContract, informs_when_optional_field_is_missing)
{
    JsonContract contract({
        {JsonContract::FieldPresence::Required, "field1", "string"},
        {JsonContract::FieldPresence::Optional, "field2", "string"},
    });

    ValidatedContract validated_contract = contract.validate(R"({"field1": "value"})");

    ASSERT_TRUE(validated_contract.isValid());
    ASSERT_FALSE(validated_contract.isPresent("field2"));
}


TEST_WITH_MOCK(JsonContract, informs_when_optional_field_is_present)
{
    JsonContract contract({
        {JsonContract::FieldPresence::Required, "field1", "string"},
        {JsonContract::FieldPresence::Optional, "field2", "string"},
    });

    ValidatedContract validated_contract = contract.validate(R"({"field1": "value1", "field2": "value2"})");

    ASSERT_TRUE(validated_contract.isValid());
    ASSERT_TRUE(validated_contract.isPresent("field2"));
    ASSERT_STRING("value2", validated_contract.stringAt("field2"));
}
