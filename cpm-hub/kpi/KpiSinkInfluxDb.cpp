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
// curl -i -XPOST 'http://localhost:8086/write?db=mydb' --data-binary "cpu_load_short,host=server01,region=us-west value=0.1 $(date '+%s%N')"
#include <sstream>
#include <kpi/KpiSinkInfluxDb.h>
#include <http/HttpClient.h>

using namespace std;


KpiSinkInfluxDb::KpiSinkInfluxDb(string influxdb_url, string database)
{
    this->influxdb_url = influxdb_url;
    this->influxdb_write_url = influxdb_url + "/write";
    this->database = database;
}


void KpiSinkInfluxDb::newMeasure(string kpi, double value, map <string, string> tags, chrono::nanoseconds time)
{
    HttpClient client;
    HttpRequest request(this->formatMessage(move(kpi), value, move(tags), time));

    request.query_parameters.set("db", this->database);

    client.post(this->influxdb_write_url, request);
}


string KpiSinkInfluxDb::formatMessage(string kpi, double value, map<string, string> tags, chrono::nanoseconds time)
{
    ostringstream stream;
    auto now = chrono::high_resolution_clock::now();

    stream << kpi;
    for (auto& tag: tags) {
        stream << "," << tag.first << "=" << tag.second;
    }

    stream << " value=" << value;
    stream << " " << time.count();

    return stream.str();
}
