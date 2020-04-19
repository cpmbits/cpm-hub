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


template<class T> class Optional {
public:
    Optional<T>() {
        this->contains_value = false;
    }

    Optional<T>(T value) {
        this->contains_value = true;
        this->stored_value = value;
    }

    bool isPresent() {
        return this->contains_value;
    }

    T& value() {
        if (!this->contains_value) {
            throw "Optional has no value";
        }
        return this->stored_value;
    }

    Optional<T>& operator =(const T &value) {
        this->stored_value = value;
        this->contains_value = true;
        return *this;
    }

    operator bool() const {
        return isPresent();
    }

private:
    bool contains_value;
    T stored_value;
};