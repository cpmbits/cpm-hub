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

#include <string>
#include <list>

#include <infrastructure/Optional.h>
#include <bits/BitSearchQuery.h>
#include <bits/Bit.h>


class BitsRepository {
public:
    virtual void add(Bit &bit) = 0;

    virtual Optional<Bit> bitBy(std::string name) = 0;

    virtual Optional<Bit> bitBy(std::string name, std::string version) = 0;

    virtual std::list<BitMetadata> search(BitSearchQuery search_query) = 0;

    virtual std::list<Bit> allBits() = 0;
};
