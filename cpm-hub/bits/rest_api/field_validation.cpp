/*
 * Copyright (c) 2020. Jordi Sánchez
 */
#include <regex>
#include <bits/rest_api/field_validation.h>

using namespace std;

static const string valid_bit_name_regex_string = "[a-zA-Z][a-zA-Z0-9_]+";
static const regex valid_bit_name_regex(valid_bit_name_regex_string);

bool validBitName(string bit_name)
{
    smatch base_match;

    return regex_match(bit_name, base_match, valid_bit_name_regex);
}

