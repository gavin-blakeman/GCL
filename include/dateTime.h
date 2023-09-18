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
//                      Copyright 2017-2023 Gavin Blakeman.
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

  ///@brief Type used for storing dates.

  class date_t
  {
  public:
    date_t(date_t const &) = default;
    date_t(std::chrono::time_point<std::chrono::system_clock> d) { value_ = d; }
    date_t() { value_ = std::chrono::system_clock::now(); }
    date_t &operator=(std::chrono::time_point<std::chrono::system_clock> d) { value_ = d; return *this; }
    date_t &operator=(date_t const &d)
    {
      if (this != &d)
      {
        value_ = d.value_;
      }
      return *this;
    }
    bool operator <(date_t const &rhs) const
    {
      return (value_ < rhs.value_);
    }

    std::chrono::time_point<std::chrono::system_clock> date() const { return value_; }

  private:
    std::chrono::time_point<std::chrono::system_clock> value_;
  };

  ///@brief Type used for storing times.
  class time_t
  {
  public:
    time_t() { value_ = std::chrono::system_clock::now(); }
    time_t(std::chrono::time_point<std::chrono::system_clock> t) { value_ = t; }

    std::chrono::time_point<std::chrono::system_clock> time() const { return value_; }

  private:
    std::chrono::time_point<std::chrono::system_clock> value_;
  };

  ///@brief Type used for storing date/time combinations.
  struct dateTime_t
  {
    std::chrono::time_point<std::chrono::system_clock> dateTime;
  };

  std::string sprintDate(std::tm *);
  std::string sprintDateTime(std::tm *);
  std::string sprintTime(std::tm *);

  std::chrono::time_point<std::chrono::system_clock> parseDate(std::string const &);
  std::chrono::time_point<std::chrono::system_clock> parseDate(std::string const &, std::string const &);
  std::chrono::time_point<std::chrono::system_clock> parseTime(std::string const &);
  std::chrono::time_point<std::chrono::system_clock> parseTime(std::string const &, std::string const &);
  std::chrono::time_point<std::chrono::system_clock> parseDateTime(std::string const &);
  std::chrono::time_point<std::chrono::system_clock> parseDateTime(std::string const &, std::string const &);

}

#endif // GCL_DATETIME

