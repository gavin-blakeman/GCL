//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                dataParser/dataDelimited.h
// SUBSYSTEM:           Data File Parser
// LANGUAGE:						C++
// TARGET OS:						None.
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2020 Gavin Blakeman.
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
// OVERVIEW:            Implementation of a configuration file reader.
//
// CLASSES INCLUDED:
//
// HISTORY:             2020-10-13 GGB - File Created
//
//**********************************************************************************************************************************

#include "include/dataParser/dataDelimited.h"

  // Standard C++ library header files

#include <sstream>


namespace GCL
{

  /// @brief      Reset all data in the instance
  /// @throws     None.
  /// @version    2020-10-13/GGB - Function created.

  void CDelimitedParser::clear() noexcept
  {
    headerData.clear();
    dataFile.clear();
  }

  CDelimitedParser &CDelimitedParser::delimiterCharacter(std::string const &c) noexcept
  {
    delimiter_ = c;

    return *this;
  }

  CDelimitedParser &CDelimitedParser::includesHeader(bool b) noexcept
  {
    includesHeader_ = b;

    return *this;
  }

  /// @brief      Parses a string and sets the data into the data structures.
  /// @param[in]  dataString: The string containing the data to parse.

  bool CDelimitedParser::parseData(std::string const &dataString)
  {
    bool returnValue = false;

    currentString = dataString;
    currentPosn = 0;

    if (!dataString.empty())
    {
      if (includesHeader_)
      {
        returnValue = processHeader();
      };

      if (returnValue)
      {
        returnValue = processData();
      };
    };

    return returnValue;
  }

  /// @brief      Processes the data for a header.
  /// @version    2020-10-13/GGB - Function created.

  bool CDelimitedParser::processHeader()
  {
      // Assume a number of fields, seperated by the seperator character(s).

    std::string::size_type startIndex, endIndex;

    startIndex = currentPosn;

  }
}
