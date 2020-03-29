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
#include <fakeit/fakeit.hpp>

#include <cstdarg>
#include <memory.h>
#include <infrastructure/deploy_service.h>
#include <infrastructure/authenticator.h>

using namespace cest;
using namespace fakeit;
using namespace std;

static const char *execl_args[8];


describe("Deploy Service", []() {
    beforeEach([]() {
        memset(execl_args, 0, sizeof(execl_args));
    });

    it("decodes payload and runs new received binary", []() {
        Mock<Filesystem> mock_filesystem;
        DeployService deploy_service(&mock_filesystem.get());
        vector<string> command_line {"cpm-hub", "-i", "cpmhub.ini"};

        deploy_service.setCommandLine(command_line);
        When(Method(mock_filesystem, writeFile)).AlwaysReturn();

        deploy_service.deploy("payload", "api_key");

        expect(execl_args[0]).toBe(command_line.at(0));
    });

    it("fails to deploy received binary when security verification fails", []() {
        Mock<Filesystem> mock_filesystem;
        Mock<Authenticator> mock_authenticator;
        DeployService deploy_service(&mock_filesystem.get());
        vector<string> command_line {"cpm-hub", "-i", "cpmhub.ini"};
        string access_file("access_file.txt");

        When(Method(mock_filesystem, writeFile)).AlwaysReturn();
        When(Method(mock_authenticator, authenticate)).Return(false);
        deploy_service.setCommandLine(command_line);
        deploy_service.configureAuthenticator(&mock_authenticator.get());

        try {
            deploy_service.deploy("payload", "api_key");
            failTest();
        } catch (AuthenticationFailure &error) {
        }
    });
});


extern "C" {
int execl(const char *__path, const char *__arg, ...)
{
    va_list ap;

    execl_args[0] = __arg;

    va_start(ap, __arg);
    execl_args[1] = va_arg(ap, char *);
    execl_args[2] = va_arg(ap, char *);
    execl_args[3] = va_arg(ap, char *);
    execl_args[4] = va_arg(ap, char *);
    va_end(ap);

    return 0;
}
}