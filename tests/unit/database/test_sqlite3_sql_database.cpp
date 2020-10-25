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
#include <mocks/cpputest.h>

#include <database/SqlDatabaseSqlite3.h>

using namespace std;


TEST_GROUP(SqlDatabaseSqlite3) {
};


TEST_WITH_MOCK(SqlDatabaseSqlite3, finds_a_sqlite3_database_row_after_insertion)
{
    list<SqlRow> selectResult;
    SqlDatabaseSqlite3 database(":memory:");

    database.createTable("CREATE TABLE users (name varchar(255), age int)");
    database.insert("INSERT INTO users VALUES ('fulano', 18)");
    selectResult = database.select("SELECT * FROM users");

    ASSERT_EQUAL(1, selectResult.size());
    ASSERT_STRING("fulano", selectResult.front()["name"]);
    ASSERT_STRING("18", selectResult.front()["age"]);
}


TEST_WITH_MOCK(SqlDatabaseSqlite3, gets_an_empty_list_when_select_finds_zero_elements)
{
    list<SqlRow> selectResult;
    SqlDatabaseSqlite3 database(":memory:");

    database.createTable("CREATE TABLE users (name varchar(255), age int)");
    selectResult = database.select("SELECT * FROM users");

    ASSERT_EQUAL(0, selectResult.size());
}


TEST_WITH_MOCK(SqlDatabaseSqlite3, reports_when_a_table_column_exists)
{
    list<SqlRow> selectResult;
    SqlDatabaseSqlite3 database(":memory:");

    database.createTable("CREATE TABLE users (name varchar(255), age int)");

    ASSERT_FALSE(database.hasColumn("usuarios", "columna"));
    ASSERT_FALSE(database.hasColumn("users", "columna"));
    ASSERT_TRUE(database.hasColumn("users", "name"));
}
