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
#include <users/UsersService.h>


UsersService::UsersService(UsersRepository *users_repository, Authenticator *authenticator)
{
    this->users_repository = users_repository;
    this->authenticator = authenticator;
}


User UsersService::registerUser(UserRegistrationData &registration_data)
{
    User user(registration_data.username);
    UserCredentials credentials = {
        registration_data.username,
        registration_data.password,
        registration_data.email,
    };

    if (this->users_repository->exists(registration_data.username)) {
        throw UsernameAlreadyTaken(registration_data.username);
    }

    this->authenticator->addUser(credentials);
    this->users_repository->add(user);

    return user;
}
