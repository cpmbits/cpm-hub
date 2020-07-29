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
 * but WITHOUT ANY WARRANTY; without even the implcolumnied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once

#include <string>
#include <list>

struct SqlColumn {
    std::string type;
    std::string name;
};

struct SqlField {
    std::string column;
    std::string value;
};

enum SqlSelectFlavour {
    WHERE,
    LIKE
};


using SqlRow = std::list<SqlField>;

class SqlDatabase {
public:
    virtual void execute(std::string query);

    virtual void createTable(std::string table, std::list<SqlColumn> columns) = 0;

    virtual void insert(std::string table, std::list<SqlField> fields) = 0;

    virtual std::list<SqlRow> select(std::string table, SqlSelectFlavour flavour, std::list<SqlField> fields) = 0;
};

