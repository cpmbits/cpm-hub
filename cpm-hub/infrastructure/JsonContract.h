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
#pragma once

#include <vector>
#include <map>
#include <string>
#include <boost/any.hpp>


class ValidatedContract {
public:
    ValidatedContract() {
        validity = true;
    }

    std::string stringAt(const std::string &key);

    std::string errorAt(const std::string &key);

    void setString(const std::string &key, const std::string& value);

    bool isValid();

    bool isPresent(const std::string &key);

    void setValidity(bool valid);

private:
    std::map<std::string, boost::any> fields;

    bool validity;
};


class JsonContract {
public:
    enum FieldPresence {
        Required,
        Optional
    };

    struct Field {
        FieldPresence presence;
        std::string name;
        std::string type;
    };

    explicit JsonContract(const std::vector<struct Field> &fields);

    ValidatedContract validate(const std::string &body);

private:
    std::vector<struct Field> fields;
};
