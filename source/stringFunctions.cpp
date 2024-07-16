//*********************************************************************************************************************************
//
// PROJECT:							General Class Library
// FILE:								string.cpp
// SUBSYSTEM:						Database library
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	None.
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2013-2024 Gavin Blakeman.
//                      This file is part of the General Class Library (GCL)
//
//                      GCL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
//                      License as published by the Free Software Foundation, either version 2 of the License, or (at your option)
//                      any later version.
//
//                      GCL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
//                      warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
//                      more details.
//
//                      You should have received a copy of the GNU General Public License along with GCL.  If not,
//                      see <http://www.gnu.org/licenses/>.
//
// OVERVIEW:            This file provides string support that is missing in the windows API.
//
// CLASSES INCLUDED:    None
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-01-26 GGB - Development of class for application AIRDAS
//
//*********************************************************************************************************************************

#include <string.h> // C++ std library string.h

#include "include/stringFunctions.h"

// GCL header files
#include "../include/error.h"

namespace GCL
{
  void cleanDecimal(std::string &str)
  {
    /* When opening text files, decimal and integer values may have been written in a different locale to what we are reading them
     * in. The challenge is then to correclty convert the decmimals into
     * What we need to do is to determine if the string has a fractional decimal separator (decimal point). This is complicated as
     * there are two commonly used decimal points, ".", ",". The number grouping separator can also appear in the number, and this
     * could be either of the two characters.
     * Also correct values that have the negative at the end of the string.
     * The number of decimal fractions is unknown.
     * Assume the number is well formed.
     *  Case 1: 123.456
     *  Case 2: 123,456
     *  Case 3: 123,456.78
     *  Case 4: 12,456.78
     *  Case 5: 12.456,78
     *
     *  Assumptions:
     *  1. If there is only one type of decimal delimiter ',' or '.' then it is taken as the decimal point.
     *  2. If there is more than one of the decimal delimeters, then the left mot one is the grouper.
     */

    bool isNegative = false;
    bool hasPoint = false;
    bool hasComma = false;
    bool pointFirst = false;
    bool commaFirst = false;
    std::string cvtStr0;
    cvtStr0.reserve(str.size());
    std::string cvtStr1;          // Work on all versions of the string simultaneously. 0=> '.' = dec. 1=>',' = dec
    cvtStr0.reserve(str.size());
    char localeDecPnt = std::use_facet<std::numpunct<char>>(std::locale()).decimal_point();

    for (auto const &c: str)
    {
      if (std::isdigit(c))
      {
        cvtStr0.push_back(c);
        cvtStr1.push_back(c);
      }
      else if (c == '-')
      {
        isNegative = true;
      }
      else if (c == '.')
      {
        hasPoint = true;
        pointFirst = pointFirst || !commaFirst;
        cvtStr0.push_back(localeDecPnt);
      }
      else if (c == ',')
      {
        hasComma = true;
        commaFirst = commaFirst || !pointFirst;
        cvtStr1.push_back(localeDecPnt);
      }
      else if (std::isspace(c))
      {
        // Do nothing. Want to get rid of the whitespace.
      }
      else
      {
        SIGNAL(1);  // Malformed number.
      }
    }

    if (isNegative)
    {
      cvtStr0.insert(0, "-");
      cvtStr1.insert(0, "-");
    }

    // At this time, one of cvtStr contains the correct string and the other contains the incorrect string.

    if ( !hasPoint && !hasComma )
    {
      std::swap(str, cvtStr0);  // Both strings the same. No decimal or seperators.
    }
    else if (hasPoint && !hasComma)
    {
      std::swap(str, cvtStr0);
    }
    else if (!hasPoint && hasComma)
    {
      std::swap(str, cvtStr1);
    }
    else if (commaFirst)
    {
      std::swap(str, cvtStr0);
    }
    else if (pointFirst)
    {
      std::swap(str, cvtStr1);
    }
    else
    {
      CODE_ERROR();
    }


  }

}


#ifdef _MSC_VER

int __cdecl strncasecmp(const char * _Str1, const char * _Str2, size_t _MaxCount)
{
  return strncmp(_Str1, _Str2, _MaxCount);
};

int __cdecl strcasecmp(const char *str1, const char *str2)
{
  size_t maxLen = strlen(str1);
  if (strlen(str2) > maxLen)
    maxLen = strlen(str2);

  return strncmp(str1, str2, maxLen);
};

#endif
