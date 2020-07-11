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

#include <base64/base64.h>
#include <bits/BitIndex.h>
#include <bits/BitsRepositoryInFilesystem.h>

using namespace cest;
using namespace fakeit;
using namespace std;


describe("Bits Repository in file system", []() {
    it("stores and indexes one bit", [&]() {
        Mock<Filesystem> mock_filesystem;
        BitIndex bit_index;
        BitsRepositoryInFilesystem repository(&mock_filesystem.get(), &bit_index);
        Bit bit("cest", "1.0", "user", "Yml0IHBheWxvYWQ=");

        When(Method(mock_filesystem, createDirectory)).AlwaysReturn();
        When(Method(mock_filesystem, writeFile)).AlwaysReturn();

        repository.add(bit);

        Verify(Method(mock_filesystem, createDirectory).Using("./user/cest/1.0"));
        Verify(Method(mock_filesystem, writeFile).Using("./user/cest/1.0/cest.zip", "bit payload"));
        Verify(Method(mock_filesystem, writeFile).Using(
            "./user/cest/1.0/cest.json", 
            "{\"name\":\"cest\",\"user_name\":\"user\",\"version\":\"1.0\"}"
        ));
        Verify(Method(mock_filesystem, writeFile).Using(
            "./index.json",
            "{\"__version__\":\"1\",\"cest\":{\"directory\":\"user/cest\",\"username\":\"user\"}}"
        ));
    });

    it("stores a new version of a given bit", [&]() {
        Mock<Filesystem> mock_filesystem;
        BitIndex bit_index;
        BitsRepositoryInFilesystem repository(&mock_filesystem.get(), &bit_index);
        Bit bit("cest", "0.1", "user", "Yml0IHBheWxvYWQ=");

        When(Method(mock_filesystem, createDirectory)).AlwaysReturn();
        When(Method(mock_filesystem, writeFile)).AlwaysReturn();
        repository.add(bit);
        bit = Bit("cest", "1.0", "user", "Yml0IHBheWxvYWQ=");

        repository.add(bit);

        Verify(Method(mock_filesystem, writeFile).Using(
            "./index.json",
            "{\"__version__\":\"1\",\"cest\":{\"directory\":\"user/cest\",\"username\":\"user\"}}"
        )).AtLeastOnce();
        Verify(Method(mock_filesystem, writeFile).Using(
            "./index.json",
            "{\"__version__\":\"1\",\"cest\":{\"directory\":\"user/cest\",\"username\":\"user\"}}"
        )).AtLeastOnce();
    });

    it("doesn't find a non existent bit", []() {
        Mock<Filesystem> mock_filesystem;
        BitIndex bit_index;
        BitsRepositoryInFilesystem repository(&mock_filesystem.get(), &bit_index);
        Optional<Bit> bit;

        bit = repository.bitBy("cest");

        expect(bit.isPresent()).toBe(false);
    });

    it("finds an indexed bit", [&]() {
        Mock<Filesystem> mock_filesystem;
        BitIndex bit_index;
        BitsRepositoryInFilesystem repository(&mock_filesystem.get(), &bit_index);
        Bit cest_bit("cest", "1.0", "user", "Yml0IHBheWxvYWQ=");
        Optional<Bit> bit;

        When(Method(mock_filesystem, createDirectory)).AlwaysReturn();
        When(Method(mock_filesystem, writeFile)).AlwaysReturn();
        When(Method(mock_filesystem, listDirectories)).Return(list<string>{"user/cest/1.0"});
        When(Method(mock_filesystem, readFile))
                .Return("{\"name\":\"cest\",\"user_name\":\"user\",\"version\":\"1.0\"}")
                .Return("bit payload");

        repository.add(cest_bit);

        bit = repository.bitBy("cest");

        expect(bit.isPresent()).toBe(true);
        expect(bit.value().metadata.name).toBe("cest");
        expect(bit.value().metadata.version).toBe("1.0");
        expect(bit.value().metadata.user_name).toBe("user");
        expect(bit.value().payload).toBe("Yml0IHBheWxvYWQ=");
    });

    it("doesn't restore index when repository doesn't have an index file", [&]() {
        Mock<Filesystem> mock_filesystem;
        BitIndex bit_index;
        BitsRepositoryInFilesystem repository(&mock_filesystem.get(), &bit_index);
        Optional<Bit> bit;
        Optional<string> directory;

        When(Method(mock_filesystem, fileExists)).Return(false);

        repository.restore(".");
    });

    it("finds an indexed bit after index was restored from filesystem from version 0", [&]() {
        Mock<Filesystem> mock_filesystem;
        BitIndex bit_index;
        BitsRepositoryInFilesystem repository(&mock_filesystem.get(), &bit_index);
        Optional<Bit> bit;
        Optional<string> directory;

        When(Method(mock_filesystem, fileExists)).Return(true);
        When(Method(mock_filesystem, readFile))
                .Return("{\"cest\":\"user/cest/1.0\"}")
                .Return("{\"name\":\"cest\",\"user_name\":\"user\",\"version\":\"1.0\"}")
                .Return("bit payload");
        When(Method(mock_filesystem, listDirectories)).Return(list<string>{"1.0"});

        repository.restore(".");
        bit = repository.bitBy("cest");

        expect(bit.value().metadata.name).toBe("cest");
        expect(bit.value().metadata.version).toBe("1.0");
        expect(bit.value().metadata.user_name).toBe("user");
        expect(bit.value().payload).toBe("Yml0IHBheWxvYWQ=");
    });

    it("finds an indexed bit after index was restored from filesystem from version 1", [&]() {
        Mock<Filesystem> mock_filesystem;
        BitIndex bit_index;
        BitsRepositoryInFilesystem repository(&mock_filesystem.get(), &bit_index);
        Optional<Bit> bit;

        When(Method(mock_filesystem, fileExists)).Return(true);
        When(Method(mock_filesystem, readFile))
                .Return("{\"__version__\":\"1\",\"cest\":{\"directory\":\"user/cest\",\"username\":\"user\"}}")
                .Return("{\"name\":\"cest\",\"user_name\":\"user\",\"version\":\"1.0\"}")
                .Return("bit payload");
        When(Method(mock_filesystem, listDirectories)).Return(list<string>{"1.0"});

        repository.restore(".");
        bit = repository.bitBy("cest");

        expect(bit.value().metadata.name).toBe("cest");
        expect(bit.value().metadata.version).toBe("1.0");
        expect(bit.value().metadata.user_name).toBe("user");
        expect(bit.value().payload).toBe("Yml0IHBheWxvYWQ=");
    });

    it("finds given version of an indexed bit", [&]() {
        Mock<Filesystem> mock_filesystem;
        BitIndex bit_index;
        BitsRepositoryInFilesystem repository(&mock_filesystem.get(), &bit_index);
        Bit cest_bit("cest", "1.1", "user", "Yml0IHBheWxvYWQ=");
        Optional<Bit> bit;

        When(Method(mock_filesystem, createDirectory)).AlwaysReturn();
        When(Method(mock_filesystem, directoryExists)).Return(true);
        When(Method(mock_filesystem, writeFile)).AlwaysReturn();
        When(Method(mock_filesystem, readFile))
                .Return("{\"name\":\"cest\",\"user_name\":\"user\",\"version\":\"1.1\"}")
                .Return("bit payload");

        repository.add(cest_bit);

        bit = repository.bitBy("cest", "1.1");

        Verify(Method(mock_filesystem, directoryExists).Using("./user/cest/1.1"));
        expect(bit.isPresent()).toBe(true);
        expect(bit.value().metadata.name).toBe("cest");
        expect(bit.value().metadata.version).toBe("1.1");
        expect(bit.value().metadata.user_name).toBe("user");
        expect(bit.value().payload).toBe("Yml0IHBheWxvYWQ=");
    });

    it("fails to find non existent version of a bit", [&]() {
        Mock<Filesystem> mock_filesystem;
        BitIndex bit_index;
        BitsRepositoryInFilesystem repository(&mock_filesystem.get(), &bit_index);
        Bit cest_bit("cest", "1.1", "user", "Yml0IHBheWxvYWQ=");
        Optional<Bit> bit;

        When(Method(mock_filesystem, createDirectory)).AlwaysReturn();
        When(Method(mock_filesystem, directoryExists)).Return(false);
        When(Method(mock_filesystem, writeFile)).AlwaysReturn();

        repository.add(cest_bit);

        bit = repository.bitBy("cest", "1.0");

        Verify(Method(mock_filesystem, directoryExists).Using("./user/cest/1.0"));
        expect(bit.isPresent()).toBe(false);
    });

    it("returns search results with one bit when search matches", [&]() {
        Mock<Filesystem> mock_filesystem;
        BitIndex bit_index;
        BitsRepositoryInFilesystem repository(&mock_filesystem.get(), &bit_index);
        Bit cest_bit("cest", "1.1", "user", "Yml0IHBheWxvYWQ=");
        list<BitMetadata> search_results;

        When(Method(mock_filesystem, createDirectory)).AlwaysReturn();
        When(Method(mock_filesystem, directoryExists)).Return(true);
        When(Method(mock_filesystem, writeFile)).AlwaysReturn();
        When(Method(mock_filesystem, readFile))
                .Return("{\"name\":\"cest\",\"user_name\":\"user\",\"version\":\"1.1\"}");

        repository.add(cest_bit);

        search_results = repository.search(BitSearchQuery{"cest"});

        expect(search_results.size()).toBe(1);
        expect(search_results.front().name).toBe("cest");
    });
});
