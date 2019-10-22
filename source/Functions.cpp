//**********************************************************************************************************************************
//
// PROJECT:							General Class Library (GCL)
// FILE:								General functions.
// SUBSYSTEM:						General Class Library Exception subsystem
// LANGUAGE:						C++
// TARGET OS:						All (Standard C++)
// LIBRARY DEPENDANCE:	None.
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2015, 2017-2018 Gavin Blakeman.
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
// OVERVIEW:            This file implements general functions.
//
// CLASSES INCLUDED:    None.
//
// FUNCTIONS INCLUDED:  prettyPrintBytes(...)
//
// HISTORY:             2017-09-24/GGB - Moved sprintf functions from AstroFunctions to this file.
//                      2015-09-22 GGB - AIRDAS 2015.09 release
//                      2015-01-02/GGB - File created.
//
//**********************************************************************************************************************************

#include "include/Functions.h"

  // Standard libraries

#include <cmath>
#include <sstream>
#include <vector>

  // Boost libraries

#include <boost/format.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

namespace GCL
{
  /// @Pretty print a 2^ value. (IE bytes.)
  /// @details Convert a value in bytes into a string including the size. (KB, MB etc)
  /// @param[in] bytes - The number of bytes to print.
  /// @returns The string indicating the bytes in a relevant human readable format.
  /// @throws None.
  //
  // 2015-01-02/GGB - Function created.

  std::string prettyPrintBytes(size_t bytes)
  {
    static std::vector<std::string> suffixes = { "B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };
    int left, right;

    size_t suffix = 0;

    double count = bytes;

    while (count >= 1024 && suffix < suffixes.size() )
    {
      count /= 1024;
      suffix++;
    };

    left = std::floor(count);
    count -= left;
    count *= 10;
    right = std::floor(count);

    return (std::to_string(left) + "." + std::to_string(right) + suffixes[suffix]);
  }

  /// @brief Converts an IPv4 address to a string
  /// @param[in] ip - The IPv4 address to convert to a string.
  /// @returns The ipV4 address as a string.
  /// @throws None.
  //
  // 2015-06-14/GGB - Function created.

  std::string IPtoString(const TIPV4 &ip)
  {
    std::string returnValue;
    std::stringstream s;

    s << std::get<0>(ip) << "." << std::get<1>(ip) << "." << std::get<2>(ip) << "." << std::get<3>(ip);

    returnValue = s.str();
    return returnValue;
  }

  /// @brief Converts a string to an IP address.
  /// @param[in] ip - The string to search for the IP address.
  /// @returns The IPV4 address.
  /// @throws None.
  //
  // 2015-06-13/GGB - Function created.

  TIPV4 stringToIP(std::string const &ip)
  {
    std::stringstream s(ip);
    int bytes[4];
    char ch;

    s >> bytes[0] >> ch >> bytes[1] >> ch >> bytes[2] >> ch >> bytes[3];

    return std::make_tuple(bytes[0], bytes[1], bytes[2], bytes[3]);
  }

  /// @brief Firmats a time value in (seconds past midnight) to a time.
  /// @param[in] seconds - number of seconds past midnight.
  /// @returns The time as a string HH:mm:ss.ss
  /// @throws
  /// @version 2017-08-11/GGB - Function created.

  std::string sprintfHMS(std::uint32_t const &seconds)
  {
    std::uint16_t hrs = std::floor(seconds / 3600);
    std::uint16_t min = std::floor((seconds % 3600) / 60);
    std::uint16_t sec = seconds % 60;

    std::string test = boost::str(boost::format("%|02d|:%|02d|:%|02d|") % hrs % min % sec);

    return test;
  }

  /// @brief Formats a double in HMS format with the required number of decimal places.
  /// @param[in] dNumber - number to format in hh.hhhhhh format.
  /// @param[in] dDecimal - Number of digits after the decimal place
  /// @param[out] szBuffer = "23h59'59.99s
  /// @throws None.
  /// @details Uses the secure form of sprintf.
  /// @version 2009-09-06/GGB - Function created.

  int sprintfHMS(char *szBuffer, FP_t dNumber, int nDecimal)
  {
    FP_t hr, min, sec;
    char szFormat1[50] = "%02.0fh%02.0f'%0";
    char szFormat2[10];

    if (nDecimal == 0)
    {
      strcpy(szFormat2, "2.0");
    }
    else
    {
      sprintf(szFormat2, "%i.%i", nDecimal+3, nDecimal);
    };

    strcat(szFormat1, szFormat2);
    strcat(szFormat1, "f\"");

    hr = std::floor(dNumber);
    sec = dNumber - hr;
    sec *= 60;
    min = std::floor(sec);
    sec -= min;
    sec *= 60;

    return ( sprintf(szBuffer, szFormat1, hr, min, sec) );
  }

  /// @brief Converts a hh.hhh value into a HH:mm:ss string.
  /// @param[in] hours - The value to print in hours.
  /// @param[in] decimals - The number of decimal places to display.
  /// @returns A string representation of the value.
  /// @throws None.
  /// @version 2012-01-10/GGB - Function created.

  std::string sprintfHMS(const FP_t &hours, int decimals)
  {
    char szNumber[30];

    sprintfHMS(szNumber, hours, decimals);

    return std::string(szNumber);
  }

  /// @brief Formats a double in DMS format with the required number of decimal places.
  /// @param[out] szBuffer - The array to write to
  /// @param[in] dNumber - The value to convert.
  ///
  /// converts the dNumber to a string representation of the degrees to nDecimal decimal places.
  /// sign == true forces the number to display as + or - while sign == false shows no + or -.
  /// @version 2013-08-16/GGB - Fixed some errors (using abs instead of fabs) (Bug #1213039)
  /// @version 2010-11-08/GGB
  ///   @li Changed to const function
  ///   @li Changed parameters to const
  ///   @li Added parameter sign
  ///   @li Changed parameter 3 & 4 to default params
  /// @version 2009-09-06/GGB - Function created.

  int sprintfDMS(char *szBuffer, FP_t const dNumber, const bool sign, const int nDecimal)
  {
    FP_t deg, min, sec;
    char szFormat1[50];
    char szFormat2[10];

    if (dNumber < 0)
    {
      if (sign)
      {
        std::strcpy(szFormat1, "-%02.0f°%02.0f'%0");
      }
      else
      {
        std::strcpy(szFormat1, "%02.0f°%02.0f'%0");
      };
      sec = std::fabs(dNumber);  // Make positive.
    }
    else
    {
      if (sign)
      {
        std::strcpy(szFormat1, "+%02.0f°%02.0f'%0");
      }
      else
      {
        std::strcpy(szFormat1, "%02.0f°%02.0f'%0");
      };
      sec = dNumber;
    };

    if (nDecimal == 0)
    {
      std::strcpy(szFormat2, "2.0");
    }
    else
    {
      sprintf(szFormat2, "%i.%i", nDecimal+3, nDecimal);
    };
    std::strcat(szFormat1, szFormat2);
    std::strcat(szFormat1, "f\"");

    deg = std::floor(sec);
    sec -= deg;
    sec *= 60;
    min = std::floor(sec);
    sec -= min;
    sec *= 60;

    return ( sprintf(szBuffer, szFormat1, deg, min, sec) );
  }

  /// @brief Converts a number in degrees to a std::string in dd°mm'ss".ss.
  /// @param[in] degrees - The value in degrees
  /// @param[in] sign - Determine if the sign must be displayed or if it is optional
  /// @param[in] decimal - The number of decimal places to be displayed.
  /// @returns A string representation of the value.
  /// @throws None.
  /// @version 2012-01-10/GGB - Function created.

  std::string sprintfDMS(FP_t const &degrees, bool sign, int decimal)
  {
    char szString[30];

    sprintfDMS(szString, degrees, sign, decimal);
    return std::string(szString);
  }

} // namespace
