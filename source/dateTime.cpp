//*********************************************************************************************************************************
//
// PROJECT:							General Class Library
// FILE:								dateTime
// SUBSYSTEM:						Date & Time Functions
// LANGUAGE:						C++
// TARGET OS:						None.
// LIBRARY DEPENDANCE:	boost.
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2017-2022 Gavin Blakeman.
//                      This file is part of the General Class Library (GCL)
//
//                      GCL is free software: you can redistribute it and/or modify it under the terms of the GNU General
//                      Public License as published by the Free Software Foundation, either version 2 of the License, or
//                      (at your option) any later version.
//
//                      GCL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
//                      implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
//                      for more details.
//
//                      You should have received a copy of the GNU General Public License along with GCL.  If not,
//                      see <http://www.gnu.org/licenses/>.
//
// OVERVIEW:            This file provides functions for working with Date & Time values.
//
// CLASSES INCLUDED:
//
// HISTORY:             2017-08-12 GGB - File Created.
//
//*********************************************************************************************************************************

#include "include/dateTime.h"

  // Standard C++ library

#include <iomanip>
#include <sstream>

  // Miscellaneous library header files

#include "boost/format.hpp"

namespace GCL
{
  /// @brief      Outputs the passed date as a string value.
  /// @param[in]  dateTime: Structure with the date value.
  /// @returns    A string containing the date/time (YYYY-MM-DD)
  /// @throws     None.
  /// @version    2017-08-12/GGB - Function created.

  std::string sprintDate(std::tm *dateTime)
  {
    return boost::str(boost::format("%1$04d-%2$02d-%3$02d") % (dateTime->tm_year + 1900) %
                      (dateTime->tm_mon + 1) % dateTime->tm_mday);
  }

  /// @brief      Outputs the passed date time as a string value.
  /// @param[in]  dateTime: Structure with the date and time values.
  /// @returns    A string containing the date/time (YYYY-MM-DD HH:mm:ss)
  /// @throws     None.
  /// @version    2017-08-12/GGB - Function created.

  std::string sprintDateTime(std::tm *dateTime)
  {
    return boost::str(boost::format("%1$04d-%2$02d-%3$02d %4$02d:%5$02d:%6$02d") % (dateTime->tm_year + 1900) %
                      (dateTime->tm_mon +1) % dateTime->tm_mday % dateTime->tm_hour % dateTime->tm_min % dateTime->tm_sec);
  }

  /// @brief      Outputs the passed time as a string value.
  /// @param[in]  dateTime: Structure with the time value.
  /// @returns    A string containing the date/time (HH:mm:ss)
  /// @throws     None.
  /// @version    2017-08-12/GGB - Function created.

  std::string sprintTime(struct tm *dateTime)
  {
    return boost::str(boost::format("%1$02d:%2$02d:%3$02d") % dateTime->tm_hour % dateTime->tm_min % dateTime->tm_sec);
  }

  /// @brief Parses a YYYY-MM-DD into a std::chrono::system_clock instance
  /// @param[in[ str: The string to parse
  /// @returns The system_clock
  /// @throws
  /// @version 2022-11-29/GGB - Function created.

  std::chrono::time_point<std::chrono::system_clock> parseDate(std::string const &str)
  {
    std::tm tm = {};
    std::istringstream ss(str);

    ss >> std::get_time(&tm, "%Y-%m-%d");

    std::time_t t = std::mktime(&tm);

    return std::chrono::system_clock::from_time_t(t);
  }

  std::chrono::time_point<std::chrono::system_clock> parseDate(std::string const &str, std::string const &fmt)
  {
    std::tm tm = {};
    std::istringstream ss(str);

    ss >> std::get_time(&tm, fmt.c_str());

    std::time_t t = std::mktime(&tm);

    return std::chrono::system_clock::from_time_t(t);
  }

}   // GCL
