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
#include <fakeit/fakeit.hpp>

#include <base64/base64.h>
#include <bits/BitIndex.h>
#include <bits/BitsRepositoryInFilesystem.h>

using namespace std;
using namespace fakeit;

TEST_GROUP(BitsRepositoryInFilesystem)
{
};


TEST_WITH_MOCK(BitsRepositoryInFilesystem, stores_and_indexes_one_bit)
{
    Mock<Filesystem> filesystem;
    BitIndex bit_index;
    BitsRepositoryInFilesystem repository(&filesystem.get(), &bit_index);
    Bit bit("cest", "1.0", "user", "Yml0IHBheWxvYWQ=");

    When(Method(filesystem, createDirectory)).AlwaysReturn();
    When(Method(filesystem, writeFile)).AlwaysReturn();

    repository.add(bit);

    Verify(Method(filesystem, createDirectory).Using("./user/cest/1.0"));
    Verify(Method(filesystem, writeFile).Using(
        "./user/cest/1.0/cest.zip",
        "bit payload"
    ));
    Verify(Method(filesystem, writeFile).Using(
        "./user/cest/1.0/cest.json",
        "{\"name\":\"cest\",\"user_name\":\"user\",\"version\":\"1.0\"}"
    ));
    Verify(Method(filesystem, writeFile).Using(
        "./index.json",
        "{\"__version__\":\"1\",\"cest\":{\"directory\":\"user/cest\",\"username\":\"user\"}}"
    ));
}


TEST_WITH_MOCK(BitsRepositoryInFilesystem, stores_a_new_version_of_a_given_bit)
{
    Mock<Filesystem> filesystem;
    BitIndex bit_index;
    BitsRepositoryInFilesystem repository(&filesystem.get(), &bit_index);
    Bit bit("cest", "0.1", "user", "Yml0IHBheWxvYWQ=");

    When(Method(filesystem, createDirectory)).AlwaysReturn();
    When(Method(filesystem, writeFile)).AlwaysReturn();
    repository.add(bit);
    bit = Bit("cest", "1.0", "user", "Yml0IHBheWxvYWQ=");

    repository.add(bit);

    Verify(Method(filesystem, writeFile).Using(
        "./index.json",
        "{\"__version__\":\"1\",\"cest\":{\"directory\":\"user/cest\",\"username\":\"user\"}}"
    )).AtLeastOnce();
    Verify(Method(filesystem, writeFile).Using(
        "./index.json",
        "{\"__version__\":\"1\",\"cest\":{\"directory\":\"user/cest\",\"username\":\"user\"}}"
    )).AtLeastOnce();
}


TEST_WITH_MOCK(BitsRepositoryInFilesystem, does_not_find_a_non_existent_bit)
{
    Mock<Filesystem> filesystem;
    BitIndex bit_index;
    BitsRepositoryInFilesystem repository(&filesystem.get(), &bit_index);
    Optional<Bit> bit;

    bit = repository.bitBy("cest");

    ASSERT_FALSE(bit.isPresent());
}


TEST_WITH_MOCK(BitsRepositoryInFilesystem, finds_an_indexed_bit)
{
    Mock<Filesystem> filesystem;
    BitIndex bit_index;
    BitsRepositoryInFilesystem repository(&filesystem.get(), &bit_index);
    Bit cest_bit("cest", "1.0", "user", "Yml0IHBheWxvYWQ=");
    Optional<Bit> bit;

    When(Method(filesystem, createDirectory)).AlwaysReturn();
    When(Method(filesystem, writeFile)).AlwaysReturn();
    When(Method(filesystem, listDirectories)).Return(list<string>{"user/cest/1.0"});
    When(Method(filesystem, readFile))
            .Return("{\"name\":\"cest\",\"user_name\":\"user\",\"version\":\"1.0\"}")
            .Return("bit payload");

    repository.add(cest_bit);

    bit = repository.bitBy("cest");

    ASSERT_TRUE(bit.isPresent());
    ASSERT_STRING("cest", bit.value().metadata.name);
    ASSERT_STRING("1.0", bit.value().metadata.version);
    ASSERT_STRING("user", bit.value().metadata.user_name);
    ASSERT_STRING("Yml0IHBheWxvYWQ=", bit.value().payload);
}


TEST_WITH_MOCK(BitsRepositoryInFilesystem, does_not_restore_index_when_repository_does_not_have_an_index_file)
{
    Mock<Filesystem> filesystem;
    BitIndex bit_index;
    BitsRepositoryInFilesystem repository(&filesystem.get(), &bit_index);
    Optional<Bit> bit;
    Optional<string> directory;

    When(Method(filesystem, fileExists)).Return(false);

    repository.restore(".");
}


TEST_WITH_MOCK(BitsRepositoryInFilesystem, finds_an_indexed_bit_after_index_was_restored_from_filesystem_from_version_0)
{
    Mock<Filesystem> filesystem;
    BitIndex bit_index;
    BitsRepositoryInFilesystem repository(&filesystem.get(), &bit_index);
    Optional<Bit> bit;
    Optional<string> directory;

    When(Method(filesystem, fileExists)).Return(true);
    When(Method(filesystem, readFile))
            .Return("{\"cest\":\"user/cest/1.0\"}")
            .Return("{\"name\":\"cest\",\"user_name\":\"user\",\"version\":\"1.0\"}")
            .Return("bit payload");
    When(Method(filesystem, listDirectories)).Return(list<string>{"1.0"});

    repository.restore(".");
    bit = repository.bitBy("cest");

    ASSERT_STRING("cest", bit.value().metadata.name);
    ASSERT_STRING("1.0", bit.value().metadata.version);
    ASSERT_STRING("user", bit.value().metadata.user_name);
    ASSERT_STRING("Yml0IHBheWxvYWQ=", bit.value().payload);
}


TEST_WITH_MOCK(BitsRepositoryInFilesystem, finds_an_indexed_bit_after_index_was_restored_from_filesystem_from_version_1)
{
    Mock<Filesystem> filesystem;
    BitIndex bit_index;
    BitsRepositoryInFilesystem repository(&filesystem.get(), &bit_index);
    Optional<Bit> bit;

    When(Method(filesystem, fileExists)).Return(true);
    When(Method(filesystem, readFile))
            .Return("{\"__version__\":\"1\",\"cest\":{\"directory\":\"user/cest\",\"username\":\"user\"}}")
            .Return("{\"name\":\"cest\",\"user_name\":\"user\",\"version\":\"1.0\"}")
            .Return("bit payload");
    When(Method(filesystem, listDirectories)).Return(list<string>{"1.0"});

    repository.restore(".");
    bit = repository.bitBy("cest");

    ASSERT_STRING("cest", bit.value().metadata.name);
    ASSERT_STRING("1.0", bit.value().metadata.version);
    ASSERT_STRING("user", bit.value().metadata.user_name);
    ASSERT_STRING("Yml0IHBheWxvYWQ=", bit.value().payload);
}


TEST_WITH_MOCK(BitsRepositoryInFilesystem, finds_given_version_of_an_indexed_bit)
{
    Mock<Filesystem> filesystem;
    BitIndex bit_index;
    BitsRepositoryInFilesystem repository(&filesystem.get(), &bit_index);
    Bit cest_bit("cest", "1.1", "user", "Yml0IHBheWxvYWQ=");
    Optional<Bit> bit;

    When(Method(filesystem, createDirectory)).AlwaysReturn();
    When(Method(filesystem, directoryExists)).Return(true);
    When(Method(filesystem, writeFile)).AlwaysReturn();
    When(Method(filesystem, readFile))
            .Return("{\"name\":\"cest\",\"user_name\":\"user\",\"version\":\"1.1\"}")
            .Return("bit payload");

    repository.add(cest_bit);

    bit = repository.bitBy("cest", "1.1");

    Verify(Method(filesystem, directoryExists).Using("./user/cest/1.1"));
    ASSERT_TRUE(bit.isPresent());
    ASSERT_STRING("cest", bit.value().metadata.name);
    ASSERT_STRING("1.1", bit.value().metadata.version);
    ASSERT_STRING("user", bit.value().metadata.user_name);
    ASSERT_STRING("Yml0IHBheWxvYWQ=", bit.value().payload);
}


TEST_WITH_MOCK(BitsRepositoryInFilesystem, fails_to_find_non_existent_version_of_a_bit)
{
    Mock<Filesystem> filesystem;
    BitIndex bit_index;
    BitsRepositoryInFilesystem repository(&filesystem.get(), &bit_index);
    Bit cest_bit("cest", "1.1", "user", "Yml0IHBheWxvYWQ=");
    Optional<Bit> bit;

    When(Method(filesystem, createDirectory)).AlwaysReturn();
    When(Method(filesystem, directoryExists)).Return(false);
    When(Method(filesystem, writeFile)).AlwaysReturn();

    repository.add(cest_bit);

    bit = repository.bitBy("cest", "1.0");

    Verify(Method(filesystem, directoryExists).Using("./user/cest/1.0"));
    ASSERT_FALSE(bit.isPresent());
}


TEST_WITH_MOCK(BitsRepositoryInFilesystem, returns_search_results_with_one_bit_when_search_matches)
{
    Mock<Filesystem> filesystem;
    BitIndex bit_index;
    BitsRepositoryInFilesystem repository(&filesystem.get(), &bit_index);
    Bit cest_bit("cest", "1.1", "user", "Yml0IHBheWxvYWQ=");
    list<BitMetadata> search_results;

    When(Method(filesystem, createDirectory)).AlwaysReturn();
    When(Method(filesystem, directoryExists)).Return(true);
    When(Method(filesystem, writeFile)).AlwaysReturn();
    When(Method(filesystem, readFile))
            .Return("{\"name\":\"cest\",\"user_name\":\"user\",\"version\":\"1.1\"}");
    When(Method(filesystem, listDirectories)).Return(list<string>{"1.0"});

    repository.add(cest_bit);

    search_results = repository.search(BitSearchQuery{"cest"});

    ASSERT_EQUAL(1, search_results.size());
    ASSERT_STRING("cest", search_results.front().name);
}
