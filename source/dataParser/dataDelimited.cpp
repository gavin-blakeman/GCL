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

#include <iostream>

  // Miscellaneous libraries

#include <boost/algorithm/string.hpp>

namespace GCL
{

  CDelimitedParser::CDelimitedParser(std::istream &is, std::string const &dc, bool ih)
    : CDataParser(is), includesHeader_(ih), delimiter_(dc)
  {

  }

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

  CDelimitedParser &CDelimitedParser::ignoreHeader(bool b) noexcept
  {
    ignoreHeader_ = b;

    return *this;
  }

  CDelimitedParser &CDelimitedParser::includesHeader(bool b) noexcept
  {
    includesHeader_ = b;

    return *this;
  }

  /// @brief Processes the entire file. Each token is pushed into the dataLine and dataFile as a string.
  ///        The calling function is required to convert strings to alternate values.
  /// @throws
  /// @version 2022-11-29/GGB - Function created.

  void CDelimitedParser::processData()
  {
    while (!inputStream.eof())
    {
      dataLine_t dataLine;
      std::string szLine;
      std::size_t tokenEnd;
      std::string token;

      std::getline(inputStream, szLine);
      if (szLine.ends_with('\r'))
      {
        szLine.pop_back();
      }

      if (!inputStream.eof())     // eof is not asseted until attempt to read the empty stream.
      {
        while (szLine.size() != 0)
        {
          tokenEnd = szLine.find(",");
          token = szLine.substr(0, tokenEnd);
          dataLine.push_back(token);

          if (tokenEnd != std::string::npos)
          {
            szLine = szLine.substr(tokenEnd + 1);

            // At least one more token.

            if (szLine.size() == 0)
            {
                // The last token is an empty token.

              token.clear();
              dataLine.push_back(token);
            }
          }
          else
          {
            szLine.clear();
          }
        }

        dataFile.push_back(std::move(dataLine));
      }
    }
  }

  /// @brief      Parses a string and sets the data into the data structures.
  /// @throws
  /// @version 2022-12-01/GGB - Function created.

  void CDelimitedParser::processParseData()
  {
    if (includesHeader_)
    {
      processHeader();
    }
    processData();
  }

  /// @brief      Processes the data for a header.
  /// @version    2020-10-13/GGB - Function created.

  void CDelimitedParser::processHeader()
  {

    if (ignoreHeader_)
    {
      std::string szLine;

      std::getline(inputStream, szLine);
    }
    else
    {
       // Assume a number of fields, seperated by the seperator character(s).

      std::string::size_type startIndex, endIndex;

      startIndex = currentPosn;
    }

  }
}
