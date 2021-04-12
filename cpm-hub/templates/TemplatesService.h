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

#include <infrastructure/Maybe.h>
#include <templates/TemplatesRepository.h>
#include <templates/TemplatePublicationData.h>
#include <templates/Template.h>


class TemplatesService {
public:
    TemplatesService(TemplatesRepository *repository);

    virtual Template publishTemplate(struct TemplatePublicationData &publication_data);

    virtual bool exists(const std::string &template_name, const std::string &version);

    virtual Maybe<Template> templateBy(const std::string &template_name, const std::string &version);

private:
    TemplatesRepository *repository;
};

