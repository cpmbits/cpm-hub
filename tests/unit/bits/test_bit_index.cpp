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
#include <bits/BitIndex.h>

#include <mocks/cpputest.h>

using namespace std;


TEST_GROUP(BitIndex)
{
};


TEST_WITH_MOCK(BitIndex, indexes_one_bit)
{
    BitIndex bit_index;

    bit_index.indexBit("cest", std::string(), "user/cest");

    ASSERT_STRING(
        "{\"__version__\":\"1\",\"cest\":{\"directory\":\"user/cest\",\"username\":\"\"}}",
        bit_index.serialize()
    );
}


TEST_WITH_MOCK(BitIndex, indexes_many_bits)
{
    BitIndex bit_index;

    bit_index.indexBit("cest", std::string(), "user/cest");
    bit_index.indexBit("fakeit", std::string(), "user/fakeit");

    ASSERT_STRING(
        "{"
             "\"__version__\":\"1\","
             "\"cest\":{"
                 "\"directory\":\"user/cest\","
                 "\"username\":\""
             "\"},"
             "\"fakeit\":{"
                 "\"directory\":\"user/fakeit\","
                 "\"username\":\"\""
             "}"
        "}",
        bit_index.serialize()
    );
}


TEST_WITH_MOCK(BitIndex, fails_to_find_a_bit_when_it_is_not_indexed)
{
    BitIndex bit_index;
    Optional<string> directory;

    directory = bit_index.find("cest");

    ASSERT_FALSE(directory.isPresent());
}


TEST_WITH_MOCK(BitIndex, finds_an_indexed_bit)
{
    BitIndex bit_index;
    Optional<string> directory;

    bit_index.indexBit("cest", std::string(), "user/cest/1.0");
    bit_index.indexBit("fakeit", std::string(), "user/fakeit/3.1");

    ASSERT_STRING("user/cest/1.0", bit_index.find("cest").value());
    ASSERT_STRING("user/fakeit/3.1", bit_index.find("fakeit").value());
}


TEST_WITH_MOCK(BitIndex, loads_the_index_from_serialized_dump)
{
    BitIndex bit_index;
    string serialized(
    "{"
        "\"cest\":\"user/cest/1.0\","
        "\"fakeit\":\"user/fakeit/3.1\""
    "}");

    bit_index.restore(serialized);

    ASSERT_STRING("user/cest", bit_index.find("cest").value());
    ASSERT_STRING("user/fakeit", bit_index.find("fakeit").value());
}


TEST_WITH_MOCK(BitIndex, returns_zero_bits_when_search_has_no_matches)
{
    BitIndex bit_index;
    std::list<BitIndexEntry> search_results;
    BitSearchQuery search_query{"name"};

    bit_index.indexBit("cest", std::string(), "user/cest/1.0");

    search_results = bit_index.search(search_query);

    ASSERT_EQUAL(0, search_results.size());
}


TEST_WITH_MOCK(BitIndex, returns_search_with_one_result_when_searching_for_bits_based_on_name_and_one_matches)
{
    BitIndex bit_index;
    std::list<BitIndexEntry> search_results;
    BitSearchQuery search_query{"cest"};

    bit_index.indexBit("cest", std::string(), "user/cest/1.0");

    search_results = bit_index.search(search_query);

    ASSERT_EQUAL(1, search_results.size());
}


TEST_WITH_MOCK(BitIndex, returns_all_bits_in_index_with_one_bit_indexed)
{
    BitIndex bit_index;
    std::list<BitIndexEntry> all_indexed_bits;

    bit_index.indexBit("cest", std::string(), "user/cest/1.0");

    all_indexed_bits = bit_index.allIndexedBits();

    ASSERT_EQUAL(1, all_indexed_bits.size());
    ASSERT_STRING("cest", all_indexed_bits.front().name);
}


TEST_WITH_MOCK(BitIndex, returns_all_bits_in_index_with_many_bits_indexed)
{
    BitIndex bit_index;
    std::list<BitIndexEntry> all_indexed_bits;

    bit_index.indexBit("cest", std::string(), "user/cest/1.0");
    bit_index.indexBit("boost", std::string(), "user/boost/2.0");

    all_indexed_bits = bit_index.allIndexedBits();

    ASSERT_EQUAL(2, all_indexed_bits.size());
    ASSERT_STRING("boost", all_indexed_bits.front().name);
    ASSERT_STRING("cest", next(all_indexed_bits.begin())->name);
}

