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
#include <templates/TemplatesService.h>

using namespace std;


TemplatesService::TemplatesService(TemplatesRepository *repository)
{
    this->repository = repository;
}


Template TemplatesService::publishTemplate(TemplatePublicationData &publication_data)
{
    Template templat(
        publication_data.template_name,
        publication_data.version,
        publication_data.username,
        publication_data.payload
    );

    repository->add(templat);

    return templat;
}


Maybe<Template> TemplatesService::templateBy(const string &template_name, const string &version)
{
    return repository->templateBy(template_name, version);
}


bool TemplatesService::exists(const string &template_name, const string &version)
{
    return repository->templateBy(template_name, version).isPresent();
}
