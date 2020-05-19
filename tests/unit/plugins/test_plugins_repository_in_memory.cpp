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

#include <bits/BitsRepositoryInMemory.h>

using namespace cest;


describe("Bits Repository in Memory", []() {
    it("stores a bit", [&]() {
        BitsRepositoryInMemory repository;
        Bit bit("cest");

        repository.add(bit);
    });

    it("lists stored bits", [&]() {
        BitsRepositoryInMemory repository;
        Bit bit("cest");
        std::list<Bit> stored_bits;

        repository.add(bit);

        stored_bits = repository.allBits();

        expect(stored_bits.size()).toBe(1);
    });

    it("doesn't find a bit when it's not stored", [&]() {
        BitsRepositoryInMemory repository;
        Optional<Bit> bit;

        bit = repository.find("cest");

        expect(bit.isPresent()).toBe(false);
    });

    it("finds the bit with the same name when one bit is stored", [&]() {
        BitsRepositoryInMemory repository;
        Bit bit("cest");
        Optional<Bit> stored_bit;

        repository.add(bit);

        stored_bit = repository.find("cest");

        expect(stored_bit.value().metadata.name).toBe(bit.metadata.name);
    });

    it("finds the bit with the same name when many bits are stored", [&]() {
        BitsRepositoryInMemory repository;
        Bit cest_bit("cest"), fakeit_bit("fakeit");
        Optional<Bit> stored_bit;

        repository.add(cest_bit);
        repository.add(fakeit_bit);

        stored_bit = repository.find("fakeit");

        expect(stored_bit.value().metadata.name).toBe(fakeit_bit.metadata.name);
    });

    it("doesn't find a bit given version when it's not stored", [&]() {
        BitsRepositoryInMemory repository;
        Bit cest_bit("cest");
        Optional<Bit> stored_bit;

        cest_bit.metadata.version = "1.0";
        repository.add(cest_bit);

        stored_bit = repository.find("cest", "1.1");

        expect(stored_bit.isPresent()).toBe(false);
    });

    it("finds a bit given version when it's stored", [&]() {
        BitsRepositoryInMemory repository;
        Bit cest_bit_1_0("cest");
        Bit cest_bit_1_1("cest");
        Optional<Bit> stored_bit;

        cest_bit_1_1.metadata.version = "1.1";
        repository.add(cest_bit_1_1);
        cest_bit_1_0.metadata.version = "1.0";
        repository.add(cest_bit_1_0);

        stored_bit = repository.find("cest", "1.1");

        expect(stored_bit.isPresent()).toBe(true);
        expect(stored_bit.value().metadata.version).toBe("1.1");
    });

    it("finds the latest version of a bit when many are stored but version is not specified", [&]() {
        BitsRepositoryInMemory repository;
        Bit cest_bit_1_0("cest");
        Bit cest_bit_1_1("cest");
        Optional<Bit> stored_bit;

        cest_bit_1_1.metadata.version = "1.1";
        repository.add(cest_bit_1_1);
        cest_bit_1_0.metadata.version = "1.0";
        repository.add(cest_bit_1_0);

        stored_bit = repository.find("cest");

        expect(stored_bit.isPresent()).toBe(true);
        expect(stored_bit.value().metadata.version).toBe("1.1");
    });
});
