//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                dateTime
// SUBSYSTEM:           Date & Time Functions
// LANGUAGE:            C++
// TARGET OS:           None.
// LIBRARY DEPENDANCE:  boost.
// NAMESPACE:           GCL
// AUTHOR:              Gavin Blakeman.
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
//*********************************************************************************************************************************/

#include "include/dateTime.h"

  // Standard C++ library

#include <forward_list>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>

  // Miscellaneous library header files

#include <fmt/format.h>
#include <fmt/chrono.h>

namespace GCL
{
  static std::vector<std::pair<std::string, std::string>> dateFormats =
  {
    { "%Y-%m-%d", "{:%Y-%m-%d}" },
    { "%Y/%m/%d", "{:%Y/%m/%d}" },
    { "%d/%m/%Y", "{:%d/%m/%Y}" },
    { "%d/%m/%y", "{:%d/%m/%y}" },
    { "%d.%m.%Y", "{:%d.%m.%Y}" },
  };

  /// @brief      Constructor accepting a date string.
  /// @param[in]  s: The input string.
  /// @throws     On bad conversion.
  /// @version    2024-02-21/GGB - Function created.

  date_t::date_t(std::string const &s)
  {
    value_ = parseDate(s);
  }

  day_t date_t::day() const
  {
   std::chrono::year_month_day ymd(std::chrono::floor<std::chrono::days>(value_));
    return ymd.day();
  }

  /// @brief      Returns the month of the date.
  /// @returns    The month value.
  /// @throws
  /// @version    2024-02-13/GGB - Function created.

  month_t date_t::month() const
  {
    std::chrono::year_month_day ymd(std::chrono::floor<std::chrono::days>(value_));
    return ymd.month();
  }

  /// @brief      Returns the year of the date.
  /// @returns    The month value.
  /// @throws
  /// @version    2024-02-13/GGB - Function created.

  year_t date_t::year() const
  {
    std::chrono::year_month_day ymd(std::chrono::floor<std::chrono::days>(value_));
    return ymd.year();
  }

  /// @brief      Returns the first day of the month that the date falls within.
  /// @returns    The month value.
  /// @throws
  /// @version    2024-02-13/GGB - Function created.

  date_t date_t::monthStart() const
  {
    using namespace std::chrono;

    year_month_day ymd(year(), month(), 1d);
    return date_t(sys_days(ymd));
  }

  date_t date_t::monthEnd() const
  {
    using namespace std::chrono;

    month_day_last mdl(month());
    year_month_day_last ymd(year(), mdl);
    return date_t(sys_days(ymd));
  }

  /// @brief      Returns the last day of the year that the date falls within.
  /// @returns    The month value.
  /// @throws
  /// @version    2024-02-13/GGB - Function created.

  date_t date_t::yearEnd() const
  {
    using namespace std::chrono;

    year_month_day ymd(year(), std::chrono::December, 31d);
    return date_t(sys_days(ymd));
  }

  /// @brief      Returns the first day of the year that the date falls within.
  /// @returns    The month value.
  /// @throws
  /// @version    2024-02-13/GGB - Function created.

  date_t date_t::yearStart() const
  {
    using namespace std::chrono;

    year_month_day ymd(year(), std::chrono::January, 1d);
    return date_t(sys_days(ymd));
  }

  std::ostream &operator<<(std::ostream &strm, date_t date)
  {
    strm << fmt::format("{:%Y-%m-%d}", date.date());
    return strm;
  }


  /// @brief      Tests if a string contains a date value.
  /// @param[in]  str: The string to test.
  /// @returns    true if the string can be converted to a date. It does not need to be a valid date.
  /// @throws     None.
  /// @version    2024-03-19/GGB - Function created.

  bool isDate(std::string const &str) noexcept
  {
    bool rv = false;
    try
    {
      parseDate(str);
      rv = true;
    }
    catch(...){}

    return rv;
  }

  /// @brief      Outputs the passed date as a string value.
  /// @param[in]  dateTime: Structure with the date value.
  /// @returns    A string containing the date/time (YYYY-MM-DD)
  /// @throws     None.
  /// @version    2023-11-16/GGB - Converted to use fmt::format instead of boost::format.
  /// @version    2017-08-12/GGB - Function created.

  std::string sprintDate(std::tm *dateTime)
  {
    return fmt::format("{:%Y-%m-%d}", *dateTime);
  }

  /// @brief      Outputs the passed date time as a string value.
  /// @param[in]  dateTime: Structure with the date and time values.
  /// @returns    A string containing the date/time (YYYY-MM-DD HH:mm:ss)
  /// @throws     None.
  /// @version    2017-08-12/GGB - Function created.

  std::string sprintDateTime(std::tm *dateTime)
  {
    return fmt::format("{:%Y-%m-%d %H:%M:%S}", *dateTime);
  }

  /// @brief      Outputs the passed time as a string value.
  /// @param[in]  dateTime: Structure with the time value.
  /// @returns    A string containing the date/time (HH:mm:ss)
  /// @throws     None.
  /// @version    2017-08-12/GGB - Function created.

  std::string sprintTime(struct tm *dateTime)
  {
    return fmt::format("{:%H:%M:%S}", *dateTime);
  }

  /// @brief      Parses a YYYY-MM-DD into a std::chrono::system_clock instance
  /// @param[in]  str: The string to parse
  /// @returns    The system_clock
  /// @throws     std::runtime_error
  /// @version    2023-10-06/GGB - Throws std::runtime error when unable to convert.
  /// @version    2022-11-29/GGB - Function created.

  std::chrono::time_point<std::chrono::system_clock> parseDate(std::string const &str)
  {
    std::chrono::time_point<std::chrono::system_clock> returnValue;
    bool found = false;

    for (auto const &fmt: dateFormats)
    {
      try
      {
        returnValue = parseDate(str, fmt.first);

        date_t date(returnValue);

        /* The challenge at this point is to write the date back to a string and to compare the strings. This is generally easy, except that
         * the date and month may or may not be prefixed with a zero. The standard library does not supply a simple way to do this.
         * There are four combinations that need to be tested. The way this can be done is to read the format string and pick up the month position and
         * day position and to remove any zeros at that position. Just brute-force with if then else.
         */
        std::string temp = fmt::format(fmt::runtime(fmt.second), returnValue);  // Write back to a string.

        if (temp == str)
        {
          found = true;
          break;
        }
        else
        {
           std::string tempM0 = std::to_string(static_cast<unsigned>(date.month()));;
           std::string tempM(tempM0);
           std::string tempD0 = std::to_string(static_cast<unsigned>(date.day()));
           std::string tempD(tempD0);
           if (tempM0.size() == 1)
             tempM0 = "0" + tempM0;
           if (tempD0.size() == 1)
             tempD0 = "0" + tempD0;

           std::string temp1, temp2, temp3, temp4;

           std::size_t indx = 0;
           while (indx != fmt.second.size())
           {
             switch (fmt.second[indx])
             {
               case 'y':
               {
                 std::string ts = std::to_string(static_cast<int>(date.year()));
                 std::size_t offset = ts.size() - 2;
                 ts = ts.substr(offset, 2);
                 temp1 += ts;
                 temp2 += ts;
                 temp3 += ts;
                 temp4 += ts;
                 break;
               }
               case 'Y':
               {
                 std::string ts = std::to_string(static_cast<int>(date.year()));
                 temp1 += ts;
                 temp2 += ts;
                 temp3 += ts;
                 temp4 += ts;
                 break;
               }
               case 'm':
               {
                 temp1 += tempM0;
                 temp2 += tempM;
                 temp3 += tempM0;
                 temp4 += tempM;
                 break;
               }
               case 'd':
               {
                 temp1 += tempD0;
                 temp2 += tempD0;
                 temp3 += tempD;
                 temp4 += tempD;
                 break;
               }
               case '{':
               case '}':
               case '%':
               {
                 break;
               };
               case ':':
               {
                 if ( (indx >= 1) && fmt.second[indx-1] == '{')
                 {
                   break;
                 }
                 [[fallthough]];
               }
               default:
               {
                 temp1 += fmt.second[indx];
                 temp2 += fmt.second[indx];
                 temp3 += fmt.second[indx];
                 temp4 += fmt.second[indx];
                 break;
               }
             }
             indx++;
           }
           if ( (temp1 == str) || (temp2 == str) || (temp3 == str) || (temp4 == str))
           {
              found = true;
           }
        }
        if (found)
        {
          break;
        }
      }
      catch(...) {} // Don't send any further.
    }

    if (!found)
    {
      throw(std::runtime_error("Unable to convert string to date value"));
    }

    return returnValue;
  }

  /// @brief      Parses a YYYY-MM-DD into a std::chrono::system_clock instance
  /// @param[in]  str: The string to parse
  /// @param[in]  fmt: The format string.
  /// @returns    The system_clock
  /// @throws
  /// @version    2023-10-06/GGB - Throws std::runtime error when unable to convert.
  /// @version    2022-11-29/GGB - Function created.

  std::chrono::time_point<std::chrono::system_clock> parseDate(std::string const &str, std::string const &fmt)
  {
    std::tm tm = {};
    std::istringstream ss(str);

    ss >> std::get_time(&tm, fmt.c_str());
    if (ss.fail())
    {
      throw(std::runtime_error("Unable to convert string to date value"));
    }

    std::chrono::year_month_day ymd(std::chrono::year(tm.tm_year + 1900),
                                    std::chrono::month(tm.tm_mon + 1),
                                    std::chrono::day(tm.tm_mday));

    return std::chrono::sys_days(ymd);
  }

  /// @brief      Parses a HH:MM:SS into a std::chrono::system_clock instance
  /// @param[in]  str: The string to parse
  /// @param[in]  fmt: The format string.
  /// @returns    The parsed value
  /// @throws
  /// @version    2023-10-06/GGB - Throws std::runtime error when unable to convert.
  /// @version    2023-03-28/GGB - Function created.

  std::chrono::time_point<std::chrono::system_clock> parseTime(std::string const &str)
  {
    std::tm tm = {};
    std::istringstream ss(str);

    ss >> std::get_time(&tm, "%H:%M:%S");
    if (ss.fail())
    {
      throw(std::runtime_error("Unable to convert string to date value"));
    }

    std::time_t t = std::mktime(&tm);

    return std::chrono::system_clock::from_time_t(t);
  }

  /// @brief      Parses a HH:MM:SS into a std::chrono::system_clock instance
  /// @param[in]  str: The string to parse
  /// @param[in]  fmt: The format string.
  /// @returns    The parsed value
  /// @throws
  /// @version    2023-10-06/GGB - Throws std::runtime error when unable to convert.
  /// @version    2023-03-28/GGB - Function created.

  std::chrono::time_point<std::chrono::system_clock> parseTime(std::string const &str, std::string const &fmt)
  {
    std::tm tm = {};
    std::istringstream ss(str);

    ss >> std::get_time(&tm, fmt.c_str());
    if (ss.fail())
    {
      throw(std::runtime_error("Unable to convert string to date value"));
    }

    std::time_t t = std::mktime(&tm);

    return std::chrono::system_clock::from_time_t(t);
  }

  /// @brief      Parses a HH:MM:SS into a std::chrono::system_clock instance
  /// @param[in]  str: The string to parse
  /// @returns    The parsed value
  /// @throws
  /// @version    2023-10-06/GGB - Throws std::runtime error when unable to convert.
  /// @version    2023-03-28/GGB - Function created.

  std::chrono::time_point<std::chrono::system_clock> parseDateTime(std::string const &str)
  {
    std::tm tm = {};
    std::istringstream ss(str);

    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    if (ss.fail())
    {
      throw(std::runtime_error("Unable to convert string to date value"));
    }

    std::time_t t = std::mktime(&tm);

    return std::chrono::system_clock::from_time_t(t);
  }

  /// @brief      Parses a HH:MM:SS into a std::chrono::system_clock instance
  /// @param[in]  str: The string to parse
  /// @param[in]  fmt: The format string.
  /// @returns    The parsed value
  /// @throws
  /// @version    2023-10-06/GGB - Throws std::runtime error when unable to convert.
  /// @version    2023-03-28/GGB - Function created.

  std::chrono::time_point<std::chrono::system_clock> parseDateTime(std::string const &str, std::string const &fmt)
  {
    std::tm tm = {};
    std::istringstream ss(str);

    ss >> std::get_time(&tm, fmt.c_str());
    if (ss.fail())
    {
      throw(std::runtime_error("Unable to convert string to date value"));
    }

    std::time_t t = std::mktime(&tm);

    return std::chrono::system_clock::from_time_t(t);
  }

  std::string to_string(GCL::date_t const &date)
  {
    return fmt::format("{:%Y-%m-%d}", date.date());
  }

  std::string to_string(GCL::time_t const &time)
  {
    return std::string();
  }

}   // GCL
