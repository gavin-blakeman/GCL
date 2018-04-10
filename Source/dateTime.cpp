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
//                      Copyright 2017 Gavin Blakeman.
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

#include "../Include/dateTime.h"

#include "boost/format.hpp"

namespace GCL
{
  /// @brief Outputs the passed date as a string value.
  /// @param[in] dateTime - Structure with the date value.
  /// @returns A string containing the date/time (YYYY-MM-DD)
  /// @throws None.
  /// @version 2017-08-12/GGB - Function created.

  std::string sprintDate(struct tm *dateTime)
  {
    return boost::str(boost::format("%1$04d-%2$02d-%3$02d") % (dateTime->tm_year + 1900) %
                      (dateTime->tm_mon + 1) % dateTime->tm_mday);
  }

  /// @brief Outputs the passed date time as a string value.
  /// @param[in] dateTime - Structure with the date and time values.
  /// @returns A string containing the date/time (YYYY-MM-DD HH:mm:ss)
  /// @throws None.
  /// @version 2017-08-12/GGB - Function created.

  std::string sprintDateTime(struct tm *dateTime)
  {
    return boost::str(boost::format("%1$04d-%2$02d-%3$02d %4$02d:%5$02d:%6$02d") % (dateTime->tm_year + 1900) %
                      (dateTime->tm_mon +1) % dateTime->tm_mday % dateTime->tm_hour % dateTime->tm_min % dateTime->tm_sec);
  }

  /// @brief Outputs the passed time as a string value.
  /// @param[in] dateTime - Structure with the time value.
  /// @returns A string containing the date/time (HH:mm:ss)
  /// @throws None.
  /// @version 2017-08-12/GGB - Function created.

  std::string sprintTime(struct tm *dateTime)
  {
    return boost::str(boost::format("%1$02d:%2$02d:%3$02d") % dateTime->tm_hour % dateTime->tm_min % dateTime->tm_sec);
  }

}   // GCL
