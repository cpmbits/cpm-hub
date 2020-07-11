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
#include <bits/BitsService.h>

using namespace std;


BitsService::BitsService(BitsRepository *bits_repository)
{
    this->bits_repository = bits_repository;
}


Bit BitsService::publishBit(struct BitPublicationData publication_data)
{
    Bit bit(publication_data.bit_name,
                  publication_data.version,
                  publication_data.user_name,
                  publication_data.payload);

    bits_repository->add(bit);

    return bit;
}


list<Bit> BitsService::allBits()
{
    return bits_repository->allBits();
}


Optional<Bit> BitsService::bitBy(std::string bit_name)
{
    return bits_repository->bitBy(bit_name);
}


Optional<Bit> BitsService::bitBy(std::string bit_name, std::string version)
{
    return bits_repository->bitBy(bit_name, version);
}


std::list<BitMetadata> BitsService::search(BitSearchQuery search_query)
{
    return bits_repository->search(search_query);
}
