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
#include <regex>
#include <bits/rest_api/field_validation.h>

using namespace std;

static const regex valid_bit_name_regex(string("[a-zA-Z][a-zA-Z0-9_\\-]+"));
static const regex valid_bit_version_regex(string(R"(\d+(\.\d+){0,2}(-?[a-zA-Z0-9]{0,15})?)"));
static const regex valid_username_regex(string("[a-zA-Z][a-zA-Z0-9-_\\.]{3,20}"));


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
    for (char const &c: payload) {
        if (c >= 'a' && c <= 'z') {
            continue;
        }
        if (c >= 'A' && c <= 'Z') {
            continue;
        }
        if (c >= '0' && c <= '9') {
            continue;
        }
        if (string("=/+").find(c) != std::string::npos) {
            continue;
        }
        return false;
    }

    return true;
}
