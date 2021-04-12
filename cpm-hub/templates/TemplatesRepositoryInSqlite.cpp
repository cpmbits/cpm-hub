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
#include <templates/TemplatesRepositoryInSqlite.h>

using namespace std;
static struct SqlColumn database_columns[] = {
        { "name", "varchar(255)" },
        { "version", "varchar(255)" },
        { "username", "varchar(255)" },
        { "payload", "text" },
};
static const string templates_table = "templates";


TemplatesRepositoryInSqlite::TemplatesRepositoryInSqlite(SqlDatabaseSqlite3 *database)
{
    this->database = database;
    sanitizeDatabase();
}


void TemplatesRepositoryInSqlite::sanitizeDatabase()
{
    createTemplatesTable();
    sanitizeTemplatesTableColumns();

}

void TemplatesRepositoryInSqlite::createTemplatesTable()
{
    stringstream create_table_query;

    create_table_query << "CREATE TABLE IF NOT EXISTS " << templates_table << " (";
    for (auto &column: database_columns) {
        create_table_query << column.name << " " << column.type << ", ";
    }
    create_table_query << "PRIMARY KEY(name, version) )";

    database->createTable(create_table_query.str());
}


void TemplatesRepositoryInSqlite::sanitizeTemplatesTableColumns()
{
    for (auto &column: database_columns) {
        if (!database->hasColumn(templates_table, column.name)) {
            stringstream add_column_query;
            add_column_query << "ALTER TABLE " << templates_table << " ADD COLUMN " << column.name << " " << column.type;
            database->execute(add_column_query.str());
        }
    }
}


void TemplatesRepositoryInSqlite::add(Template &templat)
{
    ostringstream string_stream;

    string_stream << "INSERT INTO " << templates_table << " VALUES ("
                  << "'" << templat.name << "',"
                  << "'" << templat.version << "',"
                  << "'" << templat.username << "',"
                  << "'" << templat.payload << "'"
                  << ")";

    database->insert(string_stream.str());
}

static Template templateFromSqlRow(SqlRow row)
{
    return Template(row["name"],row["version"],row["username"],row["payload"]);
}


Maybe<Template> TemplatesRepositoryInSqlite::templateBy(string name, string version)
{
    ostringstream string_stream;
    list<SqlRow> rows;
    Maybe<Template> templat;

    string_stream << "SELECT * FROM " << templates_table << " WHERE name='" << name << "' AND version='" << version << "'";

    rows = database->select(string_stream.str());
    if (!rows.empty()) {
        templat = templateFromSqlRow(rows.front());
    }

    return templat;
}
