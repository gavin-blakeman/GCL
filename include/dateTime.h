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
//                      Copyright 2017-2024 Gavin Blakeman.
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
  using year_t = std::chrono::year;
  using month_t = std::chrono::month;
  using day_t = std::chrono::day;

  ///@brief Type used for storing dates.

  class date_t
  {
  public:
    using valueType = std::chrono::time_point<std::chrono::system_clock>;

    date_t() { value_ = std::chrono::system_clock::now(); }
    date_t(std::string const &);
    date_t(date_t const &) = default;
    date_t(date_t &&) = default;
    date_t(std::chrono::time_point<std::chrono::system_clock> d) { value_ = d; }

    date_t &operator=(std::chrono::time_point<std::chrono::system_clock> d) { value_ = d; return *this; }
    date_t &operator=(date_t const &d) = default;
    date_t &operator=(date_t &&) = default;

    bool operator ==(date_t const &rhs) const noexcept { return (value_ == rhs.value_); }
    bool operator <(date_t const &rhs) const noexcept { return (value_ < rhs.value_); }
    bool operator <=(date_t const &rhs) const noexcept { return (value_ <= rhs.value_); }
    bool operator >(date_t const &rhs) const noexcept { return (value_ > rhs.value_); }
    bool operator >=(date_t const &rhs) const noexcept { return (value_ >= rhs.value_); }
    bool operator !=(date_t const rhs) const noexcept { return (value_ != rhs.value_); }

    std::chrono::time_point<std::chrono::system_clock> date() const { return value_; }

    std::chrono::days operator-(date_t const &rhs) const
    {
      return std::chrono::duration_cast<std::chrono::days>(value_- rhs.value_);
    }

    month_t month() const;
    year_t year() const;
    day_t day() const;

    date_t monthStart() const;
    date_t monthEnd() const;

    date_t yearStart() const;
    date_t yearEnd() const;


    static date_t max() { return valueType::max(); }
    static date_t min() { return valueType::min(); }

  private:
    valueType value_;
  };

  ///@brief Type used for storing times.
  class time_t
  {
  public:
    time_t() { value_ = std::chrono::system_clock::now(); }
    time_t(time_t const &) = default;
    time_t(time_t &&) = default;
    time_t(std::chrono::time_point<std::chrono::system_clock> t) { value_ = t; }

    time_t &operator=(time_t const &) = default;
    time_t &operator=(time_t &&) = default;

    bool operator <(time_t const &rhs) const
    {
      return (value_ < rhs.value_);
    }

    std::chrono::time_point<std::chrono::system_clock> time() const { return value_; }


  private:
    std::chrono::time_point<std::chrono::system_clock> value_;
  };

  ///@brief Type used for storing date/time combinations.
  class dateTime_t
  {
  public:
    dateTime_t() { value_ = std::chrono::system_clock::now(); }
    dateTime_t(dateTime_t const &) = default;
    dateTime_t(dateTime_t &&) = default;

    dateTime_t(std::chrono::time_point<std::chrono::system_clock> t) { value_ = t; }

    dateTime_t &operator=(dateTime_t const &) = default;
    dateTime_t &operator=(dateTime_t &&) = default;

    bool operator <(dateTime_t const &rhs) const
    {
      return (value_ < rhs.value_);
    }

    std::chrono::time_point<std::chrono::system_clock> dateTime() const { return value_; }

  private:

    std::chrono::time_point<std::chrono::system_clock> value_;
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
  bool isDate(std::string const &) noexcept;

}

#endif // GCL_DATETIME

