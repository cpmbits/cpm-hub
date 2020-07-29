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
#include <sstream>
#include <boost/algorithm/string/join.hpp>
#include <database/Sqlite3SqlDatabase.h>

using namespace std;


Sqlite3SqlDatabase::Sqlite3SqlDatabase(string file)
{
    this->file_path = file;
}


void Sqlite3SqlDatabase::createTable(string table, list<SqlColumn> columns)
{
    stringstream query;
    list<string> fields;

    for (auto &column: columns) {
        fields.emplace_back(column.name + " " + column.type);
    }
    query << "CREATE TABLE " << table << " (";
    query << boost::algorithm::join(fields, ",") << ");";
}


void Sqlite3SqlDatabase::insert(string table, list<SqlField> fields)
{

}


list<SqlRow> Sqlite3SqlDatabase::select(string table, SqlSelectFlavour flavour, list<SqlField> fields)
{
    return list<SqlRow>();
}