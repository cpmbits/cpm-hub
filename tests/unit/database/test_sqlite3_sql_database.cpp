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
    it("operates a sqlite3 database in memory", []() {
        list<SqlRow> selectResult;
        Sqlite3SqlDatabase database(":memory:");

        database.createTable("table", {{"column1", "varchar(255)"}, {"column2", "varchar(255)"}});
        database.insert("table", {{"column1", "value1"}, {"column2", "value2"}});
        selectResult = database.select("table", WHERE, {{"column1", "value1"}});

        expect(selectResult.size()).toBe(1);
    });
});
