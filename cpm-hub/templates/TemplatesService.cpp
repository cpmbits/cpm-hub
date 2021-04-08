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


Template TemplatesService::publishTemplate(struct TemplatePublicationData &publication_data)
{
    return Template();
}


Maybe<Template> TemplatesService::templateBy(std::string template_name, std::string version)
{
    return Maybe<Template>();
}


bool TemplatesService::exists(std::string template_name, std::string version)
{
    return false;
}
