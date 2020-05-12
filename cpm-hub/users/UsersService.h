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
#pragma once

#include <string.h>
#include <users/UsersRepository.h>
#include <users/UserRegistrationData.h>
#include <authentication/Authenticator.h>


class UsersService {
public:
    UsersService(UsersRepository *users_repository, Authenticator *authenticator);

    virtual User registerUser(UserRegistrationData &registration_data);

private:
    UsersRepository *users_repository;
    Authenticator *authenticator;
};


class UsernameAlreadyTaken: public std::exception {
public:
    UsernameAlreadyTaken(std::string user_name) throw() {
        sprintf(message, "username %s already in use", user_name.c_str());
    }

	const char *what() const throw () {
    	return message;
    }

private:
    char message[256];
};
