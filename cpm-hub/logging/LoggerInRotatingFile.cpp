/*buted in the hope that it will be useful,
 * but WITHOUT ANY WARRAN
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
#include <stdarg.h>
#include <logging/LoggerInRotatingFile.h>

using namespace std;


LoggerInRotatingFile::LoggerInRotatingFile(std::string filename, int max_size, int max_files)
{
    logger = spdlog::rotating_logger_mt("cpm_hub_logger", filename, max_size, max_files);
    logger->set_pattern("%Y-%m-%d %H:%M:%S.%e %l - %v");
    spdlog::flush_on(spdlog::level::info);
}


void LoggerInRotatingFile::info(const char *message, ...)
{
    char formatted[512];
    va_list ap;

    va_start(ap, message);
    vsprintf(formatted, message, ap);
    va_end(ap);

    logger->info(formatted);
}


void LoggerInRotatingFile::warn(const char *message, ...)
{
    char formatted[512];
    va_list ap;

    va_start(ap, message);
    vsprintf(formatted, message, ap);
    va_end(ap);

    logger->warn(formatted);
}


void LoggerInRotatingFile::error(const char *message, ...)
{
    char formatted[512];
    va_list ap;

    va_start(ap, message);
    vsprintf(formatted, message, ap);
    va_end(ap);

    logger->error(formatted);
}
