/*
 * Copyright (c) 2020. Jordi Sánchez
 */
#include <mocks/cpputest.h>

#include <bits/rest_api/field_validation.h>

TEST_GROUP(field_validation)
{

};


TEST_WITH_MOCK(field_validation, validate_bit_name_field)
{
    ASSERT_TRUE(validBitName("bitname"));
    ASSERT_TRUE(validBitName("bit_name"));
    ASSERT_TRUE(validBitName("bit_name27"));
    ASSERT_TRUE(validBitName("BitName2"));
    ASSERT_FALSE(validBitName("_bit_name27"));
    ASSERT_FALSE(validBitName("09bit_name27"));
    ASSERT_FALSE(validBitName("09bit name27"));
    ASSERT_FALSE(validBitName("09bit \";name27"));
}
