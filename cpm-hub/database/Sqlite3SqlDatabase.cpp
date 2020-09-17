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
#include <database/Sqlite3SqlDatabase.h>

using namespace std;


Sqlite3SqlDatabase::~Sqlite3SqlDatabase()
{
    sqlite3_close(this->handle);
}


Sqlite3SqlDatabase::Sqlite3SqlDatabase(string file)
{
    this->file_path = file;
    sqlite3_open(file.c_str(), &this->handle);
}


void Sqlite3SqlDatabase::execute(string query)
{
    char *error_message = nullptr;

    sqlite3_exec(this->handle, query.c_str(), nullptr, nullptr, &error_message);
}


void Sqlite3SqlDatabase::createTable(string query)
{
    char *error_message = nullptr;

    sqlite3_exec(this->handle, query.c_str(), nullptr, nullptr, &error_message);
}


void Sqlite3SqlDatabase::insert(string query)
{
    char *error_message = nullptr;

    sqlite3_exec(this->handle, query.c_str(), nullptr, nullptr, &error_message);
}


static int addRow(void *arg, int num_columns, char **values, char **names)
{
    auto *rows = (list<SqlRow> *)arg;
    SqlRow row;

    for (int i=0; i<num_columns; ++i) {
        row[names[i]] = values[i];
    }
    rows->emplace_back(row);

    return 0;
}


list<SqlRow> Sqlite3SqlDatabase::select(string query)
{
    list<SqlRow> rows;
    char *error_message = nullptr;

    sqlite3_exec(this->handle, query.c_str(), addRow, (void *)&rows, &error_message);

    return rows;
}


