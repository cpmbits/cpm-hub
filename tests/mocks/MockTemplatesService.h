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
#pragma once

#include <templates/TemplatesService.h>

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>


class MockTemplatesService: public TemplatesService {
public:
    Template publishTemplate(TemplatePublicationData &publication_data) {
        last_publication_data = publication_data;
        return *(Template *)mock()
            .actualCall("TemplatesService.publishTemplate")
            .returnPointerValue();

    }

    bool exists(std::string template_name, std::string version) {
        return mock()
            .actualCall("TemplatesService.exists")
            .returnBoolValueOrDefault(false);
    }

    Maybe<Template> templateBy(std::string template_name, std::string version) {
        return *(Maybe<Template> *)mock()
                .actualCall("TemplatesService.templateBy")
                .withStringParameter("template_name", template_name.c_str())
                .withStringParameter("version", version.c_str())
                .returnPointerValue();
    }

    MockExpectedCall &expect(const std::string& call) {
        std::string full_call = std::string("TemplatesService.") + call;
        return mock().expectOneCall(full_call.c_str());
    }

    TemplatePublicationData last_publication_data;
};
