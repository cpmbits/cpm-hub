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

#include <domain/users/users_repository_in_memory.h>

using namespace cest;


describe("Users repository in memory", []() {
    it("adds a user to the repository", []() {
        UsersRepositoryInMemory repository;
        User user("");

        repository.add(user);
    });

    it("user does not exist in repository when it hasn't been stored", []() {
        UsersRepositoryInMemory repository;

        expect(repository.exists("pepito")).toBe(false);
    });

    it("user exists in repository after it has been stored", []() {
        UsersRepositoryInMemory repository;
        User user("pepito");

        repository.add(user);
        
        expect(repository.exists("pepito")).toBe(true);
    });

    it("doesn't find a user when it's not stored", []() {
        UsersRepositoryInMemory repository;
        Optional<User> user;

        user = repository.find("pepito");

        expect(user.isPresent()).toBe(false);
    });

    it("finds a user after it has been stored stored", []() {
        UsersRepositoryInMemory repository;
        User user("pepito");
        Optional<User> user_found;

        repository.add(user);

        user_found = repository.find("pepito");

        expect(user_found.isPresent()).toBe(true);
        expect(user_found.value().name).toBe("pepito");
    });
});
