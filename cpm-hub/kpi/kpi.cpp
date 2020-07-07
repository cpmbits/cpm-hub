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
#include <utility>

#include <kpi/kpi.h>

using namespace std;

static KpiSinkNone no_sink;
static KpiSink *kpi_sink = &no_sink;


void configureKpiSink(KpiSink *sink)
{
    kpi_sink = sink;
}


void recordKpi(string kpi, double value, map<string, string> tags)
{
    kpi_sink->newMeasure(
            move(kpi),
            value,
            move(tags),
            chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch())
    );
}
