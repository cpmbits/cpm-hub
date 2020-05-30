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

#include <logging/LoggerInRotatingFile.h>
#include <management/rest_api/ManagementApi.h>

using namespace cest;


describe("CPM Hub monitoring", []() {
    it("allows popping all generated logs", []() {
        Filesystem filesystem;
        LoggerInRotatingFile logger;
        DeployService deploy_service(&filesystem);
        ManagementApi management_api(&deploy_service, &logger);
        HttpRequest request;
        HttpResponse response;

        logger.info("First log message");

        response = management_api.getLogs(request);

        expect(response.status_code).toBe(HttpStatus::OK);
        expect(response.body).toBe("[\"First log message\"]");
    });
});
