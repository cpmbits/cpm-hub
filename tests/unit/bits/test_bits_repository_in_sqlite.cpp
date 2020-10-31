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

#include <bits/BitsRepositoryInSqlite.h>


TEST_GROUP(BitsRepositoryInSqlite)
{
};


TEST_WITH_MOCK(BitsRepositoryInSqlite, stores_a_bit)
{
    SqlDatabaseSqlite3 database(":memory:");
    BitsRepositoryInSqlite repository(&database);
    Bit bit("cest");

    repository.add(bit);
}


TEST_WITH_MOCK(BitsRepositoryInSqlite, lists_stored_bits)
{
    SqlDatabaseSqlite3 database(":memory:");
    BitsRepositoryInSqlite repository(&database);
    Bit bit("cest");
    std::list<Bit> stored_bits;

    repository.add(bit);

    stored_bits = repository.allBits();

    ASSERT_EQUAL(1, stored_bits.size());
}


TEST_WITH_MOCK(BitsRepositoryInSqlite, doesnt_find_a_bit_when_its_not_stored)
{
    SqlDatabaseSqlite3 database(":memory:");
    BitsRepositoryInSqlite repository(&database);
    Optional<Bit> bit;

    bit = repository.bitBy("cest");

    ASSERT_FALSE(bit.isPresent());
}


TEST_WITH_MOCK(BitsRepositoryInSqlite, finds_the_bit_with_the_same_name_when_one_bit_is_stored)
{
    SqlDatabaseSqlite3 database(":memory:");
    BitsRepositoryInSqlite repository(&database);
    Bit bit("cest");
    Optional<Bit> stored_bit;

    repository.add(bit);

    stored_bit = repository.bitBy("cest");

    ASSERT_TRUE(stored_bit.isPresent());
    ASSERT_STRING(bit.metadata.name, stored_bit.value().metadata.name);
}


TEST_WITH_MOCK(BitsRepositoryInSqlite, finds_the_bit_with_the_same_name_when_many_bits_are_stored)
{
    SqlDatabaseSqlite3 database(":memory:");
    BitsRepositoryInSqlite repository(&database);
    Bit cest_bit("cest"), fakeit_bit("fakeit");
    Optional<Bit> stored_bit;

    repository.add(cest_bit);
    repository.add(fakeit_bit);

    stored_bit = repository.bitBy("fakeit");

    ASSERT_STRING(fakeit_bit.metadata.name, stored_bit.value().metadata.name);
}


TEST_WITH_MOCK(BitsRepositoryInSqlite, doesnt_find_a_bit_given_version_when_its_not_stored)
{
    SqlDatabaseSqlite3 database(":memory:");
    BitsRepositoryInSqlite repository(&database);
    Bit cest_bit("cest");
    Optional<Bit> stored_bit;

    cest_bit.metadata.version = "1.0";
    repository.add(cest_bit);

    stored_bit = repository.bitBy("cest", "1.1");

    ASSERT_FALSE(stored_bit.isPresent());
}


TEST_WITH_MOCK(BitsRepositoryInSqlite, gets_a_bit_given_version_when_its_stored)
{
    SqlDatabaseSqlite3 database(":memory:");
    BitsRepositoryInSqlite repository(&database);
    Bit cest_bit_1_0("cest");
    Bit cest_bit_1_1("cest");
    Optional<Bit> stored_bit;

    cest_bit_1_1.metadata.version = "1.1";
    repository.add(cest_bit_1_1);
    cest_bit_1_0.metadata.version = "1.0";
    repository.add(cest_bit_1_0);

    stored_bit = repository.bitBy("cest", "1.1");

    ASSERT_TRUE(stored_bit.isPresent());
    ASSERT_STRING("1.1", stored_bit.value().metadata.version);
}


TEST_WITH_MOCK(BitsRepositoryInSqlite, gets_the_latest_version_of_a_bit_when_many_are_stored_but_version_is_not_specified)
{
    SqlDatabaseSqlite3 database(":memory:");
    BitsRepositoryInSqlite repository(&database);
    Bit cest_bit_1_0("cest");
    Bit cest_bit_1_1("cest");
    Optional<Bit> stored_bit;

    cest_bit_1_1.metadata.version = "1.1";
    repository.add(cest_bit_1_1);
    cest_bit_1_0.metadata.version = "1.0";
    repository.add(cest_bit_1_0);

    stored_bit = repository.bitBy("cest");

    ASSERT_TRUE(stored_bit.isPresent());
    ASSERT_STRING("1.1", stored_bit.value().metadata.version);
}


TEST_WITH_MOCK(BitsRepositoryInSqlite, returns_empty_list_when_searching_for_bits_and_repository_is_empty)
{
    SqlDatabaseSqlite3 database(":memory:");
    BitsRepositoryInSqlite repository(&database);
    BitSearchQuery search_query;

    ASSERT_EQUAL(0, repository.search(search_query).size())
}


TEST_WITH_MOCK(BitsRepositoryInSqlite, returns_list_with_bit_found_when_searching_for_bits_and_repository_contains_one_matching_bit)
{
    SqlDatabaseSqlite3 database(":memory:");
    BitsRepositoryInSqlite repository(&database);
    Bit cest_bit("cest");
    BitSearchQuery search_query;
    std::list<BitMetadata> found_bits;

    search_query.name = "ces";
    repository.add(cest_bit);

    found_bits = repository.search(search_query);

    ASSERT_EQUAL(1, found_bits.size());
    ASSERT_STRING("cest", found_bits.front().name);
}


TEST_WITH_MOCK(BitsRepositoryInSqlite, sanitizes_bits_table_when_there_are_missing_columns)
{
    SqlDatabaseSqlite3 database(":memory:");
    BitsRepositoryInSqlite *repository;

    database.execute("CREATE TABLE bits (name varchar(255), version varchar(255), PRIMARY KEY(name, version) )");
    repository = new BitsRepositoryInSqlite(&database);

    ASSERT_TRUE(database.hasColumn("bits", "version"));
    ASSERT_TRUE(database.hasColumn("bits", "user_name"));
    ASSERT_TRUE(database.hasColumn("bits", "payload"));

    delete repository;
}
