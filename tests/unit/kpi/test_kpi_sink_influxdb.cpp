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

#include <http/HttpServer.h>
#include <http/HttpResource.h>
#include <kpi/KpiSinkInfluxDb.h>
#include <logging/LoggerInConsole.h>

using namespace cest;
using namespace std;


class FakeInfluxDb: public HttpResource {
public:
    HttpResponse post(HttpRequest &request) {
        post_request = request;
        return post_response;
    }

    HttpRequest post_request;
    HttpResponse post_response;
};

static LoggerInConsole test_logger;


describe("KpiSinkInfluxDb", []() {
    beforeEach([]() {
        logger = &test_logger;
    });

    it("formats the request for influxdb when no tags are used", []() {
        KpiSinkInfluxDb sink("", "mydb");
        std::chrono::nanoseconds now {1250};

        expect(
            sink.formatMessage("kpi", 1.25, map<string, string>(), now))
        .toBe(
            "kpi value=1.25 1250");
    });

    it("formats the request for influxdb when tags are used", []() {
        KpiSinkInfluxDb sink("", "mydb");
        std::chrono::nanoseconds now {1250};
        map<string, string> tags = {
            {"ip", "127.0.0.1"},
            {"status_code", "204"}
        };

        expect(
             sink.formatMessage("kpi", 1.25, tags, now))
         .toBe(
             "kpi,ip=127.0.0.1,status_code=204 value=1.25 1250");
    });

    it("sends a formatted post request when recording a new measure", []() {
        HttpServer server;
        FakeInfluxDb fake_influxdb;
        KpiSinkInfluxDb sink("http://127.0.0.1:8000", "mydb");
        std::chrono::nanoseconds now {1250};
        map<string, string> tags = {
                {"ip", "127.0.0.1"},
                {"status_code", "204"}
        };

        fake_influxdb.post_response = HttpResponse::ok("hello");
        server.addResource(Endpoint("/write"), &fake_influxdb);
        server.startAsync("127.0.0.1", 8000);

        sink.newMeasure("kpi", 1.25, tags, now);

        expect(fake_influxdb.post_request.body).toBe("kpi,ip=127.0.0.1,status_code=204 value=1.25 1250");
//        expect(fake_influxdb.post_request.parameters.get("db")).toBe("mydb");

        server.stop();
    });
});
