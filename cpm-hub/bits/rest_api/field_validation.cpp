/*
 * Copyright (c) 2020. Jordi Sánchez
 */
#include <regex>
#include <bits/rest_api/field_validation.h>

using namespace std;

static const regex valid_bit_name_regex(string("[a-zA-Z][a-zA-Z0-9_]+"));
static const regex valid_bit_version_regex(string("\\d+(\\.\\d+){0,2}"));
static const regex valid_username_regex(string("[a-zA-Z][a-zA-Z0-9-_\\.]{1,20}"));
static const regex valid_payload_regex(string("[a-zA-Z0-9=/+]+"));


bool validBitName(const string& bit_name)
{
    smatch base_match;
    return regex_match(bit_name, base_match, valid_bit_name_regex);
}


bool validBitVersion(const std::string& version)
{
    smatch base_match;
    return regex_match(version, base_match, valid_bit_version_regex);
}


bool validBitUsername(const std::string& username)
{
    smatch base_match;
    return regex_match(username, base_match, valid_username_regex);
}


bool validPayload(const std::string& payload)
{
    smatch base_match;
    return regex_match(payload, base_match, valid_payload_regex);
}
