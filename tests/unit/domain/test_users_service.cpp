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

#include <users/users_repository.h>
#include <users/users_service.h>

using namespace cest;
using namespace fakeit;


describe("Users Service", []() {
    it("registers a user with the given name and information", [&]() {
        Mock<UsersRepository> mock_repository;
        UsersService users_service(&mock_repository.get());
        struct user_registration_data registration_data = {
            "sotano", "654321", "sotano@example.com"
        };
        User stored_user("");

        When(Method(mock_repository, exists)).Return(false);
        When(Method(mock_repository, add)).AlwaysDo([&](User &user) {
            stored_user = user;
        });

        users_service.registerUser(registration_data);

        Verify(Method(mock_repository, add));
        expect(stored_user.name).toBe("sotano");
    });

    it("throws an exception when registering a user whose user name is taken", [&]() {
        Mock<UsersRepository> mock_repository;
        UsersService users_service(&mock_repository.get());
        struct user_registration_data registration_data = {
            "sotano", "654321", "sotano@example.com"
        };
        User stored_user("");

        When(Method(mock_repository, exists)).Return(true);

        try {
            users_service.registerUser(registration_data);
            expect(true).toBe(false);
        } catch(UsernameAlreadyTaken error) {
        }
    });
});
