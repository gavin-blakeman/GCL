//*********************************************************************************************************************************
//
// PROJECT:							General Class Library
// FILE:								dateTime
// SUBSYSTEM:						Date & Time Functions
// LANGUAGE:						C++
// TARGET OS:						None.
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

#ifndef GCL_DATETIME
#define GCL_DATETIME

  // Standard C++ library header files

#include <chrono>
#include <ctime>
#include <string>

namespace GCL
{
  std::string sprintDate(std::tm *);
  std::string sprintDateTime(std::tm *);
  std::string sprintTime(std::tm *);

  std::chrono::time_point<std::chrono::system_clock> parseDate(std::string const &);
  std::chrono::time_point<std::chrono::system_clock> parseDate(std::string const &, std::string const &);

}

#endif // GCL_DATETIME

