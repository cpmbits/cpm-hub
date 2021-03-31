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

#include <infrastructure/Maybe.h>
#include <bits/BitPublicationData.h>
#include <bits/BitSearchQuery.h>
#include <bits/BitsRepository.h>
#include <bits/Bit.h>

class BitsService {
public:
    BitsService(BitsRepository *bits_repository);

    virtual Bit publishBit(struct BitPublicationData publication_data);

    virtual std::list<Bit> allBits();
    
    virtual Maybe<Bit> bitBy(std::string bit_name);

    virtual Maybe<Bit> bitBy(std::string bit_name, std::string version);

    virtual std::list<BitMetadata> search(BitSearchQuery search_query);

private:
    BitsRepository *bits_repository;
};
