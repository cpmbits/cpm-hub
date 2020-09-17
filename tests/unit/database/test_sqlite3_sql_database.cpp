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
#include <cest/cest.h>

#include <database/Sqlite3SqlDatabase.h>

using namespace cest;
using namespace std;


describe("Sqlite3 Database", []() {
    it("finds a sqlite3 database row after insertion", []() {
        list<SqlRow> selectResult;
        Sqlite3SqlDatabase database(":memory:");

        database.createTable("CREATE TABLE users (name varchar(255), age int)");
        database.insert("INSERT INTO users VALUES ('fulano', 18)");
        selectResult = database.select("SELECT * FROM users");

        expect(selectResult.size()).toBe(1);
        expect(selectResult.front()["name"]).toBe("fulano");
        expect(selectResult.front()["age"]).toBe("18");
    });

    it("gets an empty list when select finds zero elements", []() {
        list<SqlRow> selectResult;
        Sqlite3SqlDatabase database(":memory:");

        database.createTable("CREATE TABLE users (name varchar(255), age int)");
        selectResult = database.select("SELECT * FROM users");

        expect(selectResult.size()).toBe(0);
    });
});
