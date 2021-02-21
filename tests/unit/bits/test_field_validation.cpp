/*
 * Copyright (c) 2020. Jordi Sánchez
 */
#include <mocks/cpputest.h>

#include <bits/rest_api/field_validation.h>

TEST_GROUP(field_validation)
{

};


TEST_WITH_MOCK(field_validation, validate_bit_name)
{
    ASSERT_TRUE(validBitName("bitname"));
    ASSERT_TRUE(validBitName("bit_name"));
    ASSERT_TRUE(validBitName("bit-name"));
    ASSERT_TRUE(validBitName("bit_name27"));
    ASSERT_TRUE(validBitName("BitName2"));
    ASSERT_FALSE(validBitName("_bit_name27"));
    ASSERT_FALSE(validBitName("09bit_name27"));
    ASSERT_FALSE(validBitName("09bit name27"));
    ASSERT_FALSE(validBitName("09bit \";name27"));
}


TEST_WITH_MOCK(field_validation, validate_bit_version)
{
    ASSERT_TRUE(validBitVersion("1"));
    ASSERT_TRUE(validBitVersion("12"));
    ASSERT_TRUE(validBitVersion("1.35"));
    ASSERT_TRUE(validBitVersion("1.35.16"));
    ASSERT_TRUE(validBitVersion("1.35.16-p1"));
    ASSERT_TRUE(validBitVersion("1.35.16-rc14"));
    ASSERT_TRUE(validBitVersion("1.35.16-final"));
    ASSERT_FALSE(validBitVersion("1.35.16-toolongprefixforthebitversion"));
    ASSERT_FALSE(validBitVersion("1.35.16-prefix;with\"invaliðchar$"));
    ASSERT_FALSE(validBitVersion("latest"));
    ASSERT_FALSE(validBitVersion("1."));
    ASSERT_FALSE(validBitVersion("1.-rc14 \";name27"));
}


TEST_WITH_MOCK(field_validation, validate_username)
{
    ASSERT_TRUE(validBitUsername("johndoe"));
    ASSERT_TRUE(validBitUsername("JohnDoe"));
    ASSERT_TRUE(validBitUsername("JohnDoe27"));
    ASSERT_TRUE(validBitUsername("John_Doe27"));
    ASSERT_TRUE(validBitUsername("John-Doe27"));
    ASSERT_TRUE(validBitUsername("John.Doe27"));
    ASSERT_FALSE(validBitUsername("John\"Doe27"));
    ASSERT_FALSE(validBitUsername(" John Doe27 "));
    ASSERT_FALSE(validBitUsername("27John\"Doe27"));
    ASSERT_FALSE(validBitUsername("27John;()Doe27"));
}


TEST_WITH_MOCK(field_validation, validate_payload)
{
    ASSERT_TRUE(validPayload("Yml0IHBheWxvYWQ="));
    ASSERT_TRUE(validPayload("Yml0I//HBheWxvYWQ=+"));
    ASSERT_FALSE(validPayload("Yml0IHBheWxvYWQ=\n"));
    ASSERT_FALSE(validPayload("Yml0IHBheWxvYWQ=\""));
    ASSERT_FALSE(validPayload("Yml0IHBheWxvYWQ=;"));
}
