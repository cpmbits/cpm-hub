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

#include <sqlite3/sqlite3.h>
#include <database/SqlDatabase.h>


class SqlDatabaseSqlite3: public SqlDatabase {
public:
    SqlDatabaseSqlite3(std::string file);

    ~SqlDatabaseSqlite3();

    virtual void execute(std::string query);

    virtual void createTable(std::string query);

    virtual bool hasColumn(std::string table, std::string column);

    virtual void insert(std::string query);

    virtual std::list<SqlRow> select(std::string query);

private:
    std::string file_path;

    sqlite3 *handle;
};

