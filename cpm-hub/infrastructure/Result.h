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


template<class T> class Result {
public:
    Result<T>() {
        this->successful = false;
    }

    Result<T>(const std::string &error) {
        this->successful = false;
        this->error = error_message;
    }

    Result<T>(T value) {
        this->successful = true;
        this->stored_value = value;
    }

    bool success() {
        return this->successful;
    }

    bool failure() {
        return !this->successful;
    }

    std::string error() {
        return this->error_message;
    }

    T& value() {
        if (!this->successful) {
            throw "Optional has no value";
        }
        return this->stored_value;
    }

    Result<T>& operator =(const T &value) {
        this->stored_value = value;
        this->successful = true;
        return *this;
    }

private:
    bool successful;
    std::string error_message;
    T stored_value;
};