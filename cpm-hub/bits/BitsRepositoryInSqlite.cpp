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
#include <bits/BitsRepositoryInSqlite.h>

using namespace std;


BitsRepositoryInSqlite::BitsRepositoryInSqlite(string filename): database(move(filename))
{
    database.createTable(
            "CREATE TABLE bits ("
                                      "name varchar(255), "
                                      "version varchar(255), "
                                      "user_name varchar(255), "
                                      "payload text,"
                                      "PRIMARY KEY(name, version) )");
}


static Bit bitFromSqlRow(SqlRow row)
{
    return Bit(row["name"],row["version"],row["user_name"],row["payload"]);
}


static BitMetadata bitMetadataFromSqlRow(SqlRow row)
{
    return BitMetadata(row["name"],row["version"],row["user_name"]);
}


void BitsRepositoryInSqlite::add(Bit &bit)
{
    ostringstream string_stream;

    string_stream << "INSERT INTO bits VALUES ("
                  << "'" << bit.metadata.name << "',"
                  << "'" << bit.metadata.version << "',"
                  << "'" << bit.metadata.user_name << "',"
                  << "'" << bit.payload << "'"
                  << ")";

    database.insert(string_stream.str());
}


Optional<Bit> BitsRepositoryInSqlite::bitBy(string name)
{
    ostringstream string_stream;
    list<SqlRow> rows;
    Optional<Bit> bit;

    string_stream << "SELECT * FROM bits WHERE name='" << name << "' "
                  << "ORDER BY version";

    rows = database.select(string_stream.str());
    if (rows.size() > 0) {
        bit = bitFromSqlRow(rows.back());
    }

    return bit;
}


Optional<Bit> BitsRepositoryInSqlite::bitBy(string name, string version)
{
    ostringstream string_stream;
    list<SqlRow> rows;
    Optional<Bit> bit;

    string_stream << "SELECT * FROM bits WHERE name='" << name << "' AND version='" << version << "'";

    rows = database.select(string_stream.str());
    if (rows.size() > 0) {
        bit = bitFromSqlRow(rows.front());
    }

    return bit;
}


list<BitMetadata> BitsRepositoryInSqlite::search(BitSearchQuery search_query)
{
    ostringstream string_stream;
    list<SqlRow> rows;
    list<BitMetadata> bits;

    string_stream << "SELECT name, version, user_name FROM bits WHERE name LIKE '" << search_query.name << "'";
    rows = database.select(string_stream.str());
    for (const auto& row: rows) {
        bits.emplace_back(bitMetadataFromSqlRow(row));
    }

    return bits;
}


list<Bit> BitsRepositoryInSqlite::allBits()
{
    list<SqlRow> rows;
    list<Bit> bits;

    rows = database.select("SELECT * FROM bits");
    for (const auto& row: rows) {
        bits.emplace_back(bitFromSqlRow(row));
    }

    return bits;
}
