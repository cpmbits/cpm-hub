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

#include <bits/BitsRepository.h>
#include <bits/BitsService.h>

using namespace cest;
using namespace fakeit;
using namespace std;


describe("Bits Service", []() {
    it("stores a bit with the given name when registering a bit", [&]() {
        Mock<BitsRepository> mock_repository;
        BitsService bits_service(&mock_repository.get());
        Bit expected_bit("cest", "1.0", "john_doe", "ABCDEedcba");
        struct BitPublicationData publication_data = {
            "cest", "1.0", "john_doe", "ABCDEedcba"
        };

        When(Method(mock_repository, add)).AlwaysReturn();

        bits_service.publishBit(publication_data);

        Verify(Method(mock_repository, add));
    });

    it("returns all available bits", [&]() {
        Mock<BitsRepository> mock_repository;
        BitsService bits_service(&mock_repository.get());
        Bit bit("cest");
        std::list<Bit> bits {bit};
        std::list<Bit> returned_bits;

        When(Method(mock_repository, allBits)).Return(bits);

        returned_bits = bits_service.allBits();

        expect(returned_bits.size()).toBe(1);
        expect(returned_bits.front().metadata.name).toBe(bit.metadata.name);
    });
    
    it("uses the repository to find a bit by name", [&]() {
        Mock<BitsRepository> mock_repository;
        BitsService bits_service(&mock_repository.get());
        Optional<Bit> bit;

        bit = Bit("cest");
        When(OverloadedMethod(mock_repository, bitBy, Optional<Bit>(string))).Return(bit);

        auto found_bit = bits_service.bitBy("cest");

        expect(found_bit.value().metadata.name).toBe("cest");
    });

    it("uses the repository to find a bit by name and version", [&]() {
        Mock<BitsRepository> mock_repository;
        BitsService bits_service(&mock_repository.get());
        Optional<Bit> bit;

        bit = Bit("cest");
        When(OverloadedMethod(mock_repository, bitBy, Optional<Bit>(string, string))).Return(bit);

        auto found_bit = bits_service.bitBy("cest", "1.1");

        expect(found_bit.value().metadata.name).toBe("cest");
    });

    it("uses the repository to search for a bit by name", [&]() {
        Mock<BitsRepository> mock_repository;
        BitsService bits_service(&mock_repository.get());
        Bit bit("cest");
        std::list<BitMetadata> search_results {bit.metadata};
        BitSearchQuery search_query{"cest"};

        When(Method(mock_repository, search)).Return(search_results);

        search_results = bits_service.search(search_query);

        expect(search_results.size()).toBe(1);
        expect(search_results.front().name).toBe("cest");
    });
});
