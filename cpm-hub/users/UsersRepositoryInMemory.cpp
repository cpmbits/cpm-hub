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
#include <users/UsersRepositoryInMemory.h>


void UsersRepositoryInMemory::add(User &user)
{
    this->users.insert(std::make_pair(user.name, user));
}


bool UsersRepositoryInMemory::exists(std::string user_name)
{
    return this->users.find(user_name) != this->users.end();
}


Maybe<User> UsersRepositoryInMemory::find(std::string user_name)
{
    auto iter = this->users.find(user_name);
    Maybe<User> user;

    if (iter == this->users.end()) {
        return user;
    }

    user = iter->second;
    
    return user;
}
