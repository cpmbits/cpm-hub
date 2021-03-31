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
#include <infrastructure/JsonContract.h>
#include <templates/rest_api/TemplatesContract.h>


using namespace std;


static const regex valid_template_name_regex(string("[a-zA-Z][a-zA-Z0-9_\\-]+"));
static const regex valid_template_version_regex(string(R"(\d+(\.\d+){0,2}(-?[a-zA-Z0-9]{0,15})?)"));
static const regex valid_username_regex(string("[a-zA-Z][a-zA-Z0-9-_\\.]{3,20}"));

static JsonContract post_contract({
    {JsonContract::FieldPresence::Required, "template_name", "string"},
    {JsonContract::FieldPresence::Required, "version", "string"},
    {JsonContract::FieldPresence::Required, "username", "string"},
    {JsonContract::FieldPresence::Required, "payload", "string"},
});


static bool validTemplateName(const string& bit_name)
{
    smatch base_match;
    return regex_match(bit_name, base_match, valid_template_name_regex);
}


static bool validTemplateVersion(const std::string& version)
{
    smatch base_match;
    return regex_match(version, base_match, valid_template_version_regex);
}


static bool validTemplateUsername(const std::string& username)
{
    smatch base_match;
    return regex_match(username, base_match, valid_username_regex);
}


static bool validPayload(const std::string& payload)
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


static bool isValidPublicationData(TemplatePublicationData &publication_data)
{
    return validTemplateName(publication_data.template_name) &&
           validTemplateUsername(publication_data.user_name) &&
           validTemplateVersion(publication_data.version) &&
           validPayload(publication_data.payload);
}


Result<TemplatePublicationData> TemplatesContract::parsePost(const string &request_body)
{
    TemplatePublicationData publication_data;
    ValidatedContract validated_contract;

    validated_contract = post_contract.validate(request_body);
    if (!validated_contract.isValid()) {
        return Result<TemplatePublicationData>();
    }

    publication_data.template_name = validated_contract.stringAt("template_name");
    publication_data.version = validated_contract.stringAt("version");
    publication_data.user_name = validated_contract.stringAt("username");
    publication_data.payload = validated_contract.stringAt("payload");

    if (!isValidPublicationData(publication_data)) {
        return Result<TemplatePublicationData>();
    }

    return Result<TemplatePublicationData>(publication_data);
}
