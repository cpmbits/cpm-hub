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
#include <iostream>
#include <json/json.hpp>
#include <infrastructure/JsonContract.h>


using namespace nlohmann;
using namespace std;


JsonContract::JsonContract(const vector<struct Field> &fields)
{
    this->fields = fields;
}


ValidatedContract JsonContract::validate(const string &body)
{
    ValidatedContract validated_contract;
    auto json = json::parse(body);

    for (auto &field: fields) {
        if (json.contains(field.name)) {
            validated_contract.setString(field.name, json.at(field.name));
        } else {
            if (field.presence == FieldPresence::Required) {
                validated_contract.setValidity(false);
                validated_contract.setString(field.name, field.name + " is missing");
            }
        }
    }

    return validated_contract;
}


void ValidatedContract::setValidity(bool valid)
{
    this->validity = valid;
}


bool ValidatedContract::isValid()
{
    return validity;
}


string ValidatedContract::stringAt(const string &key)
{
    return boost::any_cast<string>(this->fields[key]);
}


string ValidatedContract::errorAt(const string &key)
{
    return boost::any_cast<string>(this->fields[key]);
}


void ValidatedContract::setString(const string &key, const string& value)
{
    this->fields[key] = value;
}


bool ValidatedContract::isPresent(const string &key)
{
    return this->fields.find(key) != this->fields.end();
}
