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
#include <mocks/cpputest.h>

#include <templates/TemplatesService.h>
#include <templates/TemplatesRepositoryInSqlite.h>


TEST_GROUP(TemplatesService) {
};


TEST_WITH_MOCK(TemplatesService, no_template_is_found_when_template_does_not_exist)
{
    SqlDatabaseSqlite3 database(":memory:");
    TemplatesRepositoryInSqlite repository(&database);
    TemplatesService service(&repository);

    ASSERT_FALSE(service.templateBy("name", "version").isPresent());
}


TEST_WITH_MOCK(TemplatesService, template_can_be_published)
{
    SqlDatabaseSqlite3 database(":memory:");
    TemplatesRepositoryInSqlite repository(&database);
    TemplatesService service(&repository);
    TemplatePublicationData publication_data = {
        "template_name",
        "version",
        "username",
        "password",
        "payload"
    };
    Template templat;

    templat = service.publishTemplate(publication_data);

    ASSERT_STRING("template_name", templat.name);
    ASSERT_STRING("version", templat.version);
    ASSERT_STRING("username", templat.username);
    ASSERT_STRING("payload", templat.payload);
}


TEST_WITH_MOCK(TemplatesService, template_can_be_found_after_being_published)
{
    SqlDatabaseSqlite3 database(":memory:");
    TemplatesRepositoryInSqlite repository(&database);
    TemplatesService service(&repository);
    TemplatePublicationData publication_data = {
        "template_name",
        "version",
        "username",
        "password",
        "payload"
    };
    Maybe<Template> templat;

    service.publishTemplate(publication_data);

    templat = service.templateBy("template_name", "version");

    ASSERT_TRUE(templat.isPresent());
    ASSERT_STRING("template_name", templat.value().name);
    ASSERT_STRING("version", templat.value().version);
    ASSERT_STRING("username", templat.value().username);
    ASSERT_STRING("payload", templat.value().payload);
}


TEST_WITH_MOCK(TemplatesService, template_exists_after_being_published)
{
    SqlDatabaseSqlite3 database(":memory:");
    TemplatesRepositoryInSqlite repository(&database);
    TemplatesService service(&repository);
    TemplatePublicationData publication_data = {
        "template_name",
        "version",
        "username",
        "password",
        "payload"
    };

    service.publishTemplate(publication_data);

    ASSERT_TRUE(service.exists("template_name", "version"));
}
