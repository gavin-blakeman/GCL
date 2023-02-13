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
//                      Copyright 2022-2023 Gavin Blakeman.
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

  void CDelimitedParser::processParseData()
  {
    while (!inputStream.eof())
    {
      dataLine_t dataLine;
      std::string szLine;

      std::getline(inputStream, szLine);

      std::string_view sv(szLine);

      if (sv.ends_with('\r')) // For windows
      {
        sv.remove_suffix(1);
      }

      if (delimiterSE_ && sv.starts_with(delimiter_))
      {
        sv.remove_prefix(delimiter_.size());
      }
      if (delimiterSE_ && sv.ends_with(delimiter_))
      {
        sv.remove_suffix(delimiter_.size());
      }

      if (!inputStream.eof())     // eof is not asseted until attempt to read the empty stream.
      {
        parseString(sv, dataLine);

        dataFile.push_back(std::move(dataLine));
      }
    }
  }

  /// @brief Parses a string with delimited values. This is used to read the headers as well as the data.
  /// @param[in] sv: The string to process
  /// @param[in] dataLine: The dataline to write the data to.
  /// @throws
  /// @version 2023-02-02/GGB - Bug#275 fixed
  /// @version 2022-12-14/GGB - Function created.

  void CDelimitedParser::parseString(std::string_view &sv, dataLine_t &dataLine)
  {
    std::size_t tokenEnd;
    std::string_view token;

    dataLine.clear();

    while (sv.size() != 0)
    {
      if (sv.starts_with('"'))
      {
        tokenEnd = 1;
        bool endFound = false;
        while (!endFound)
        {
          tokenEnd = sv.find('"', tokenEnd);
          if (sv[tokenEnd+1] != '"')
          {
            endFound = true;
          }
          else
          {
            tokenEnd += 2;
          }
        };
        tokenEnd = sv.find(delimiter_, tokenEnd);
        token = sv.substr(0, tokenEnd);

        if (token.starts_with('"'))
        {
          token.remove_prefix(1);
        }
        if (token.ends_with('"'))
        {
          token.remove_suffix(1);
        }
      }
      else
      {
        tokenEnd = sv.find(delimiter_);
        token = sv.substr(0, tokenEnd);
      };

      dataLine.push_back(std::string{token});

      if (tokenEnd != std::string::npos)
      {
        sv.remove_prefix(tokenEnd + 1);

        // At least one more token.

        if (sv.size() == 0)
        {
            // The last token is an empty token.

          dataLine.push_back(std::string{""});
        }
      }
      else
      {
        sv.remove_prefix(sv.size());
      }
    }
  }


  /// @brief      Parses a string and sets the data into the data structures.
  /// @throws
  /// @version 2022-12-01/GGB - Function created.

  void CDelimitedParser::processParseFile()
  {
    if (includesHeader_)
    {
      processParseHeader();
    }
    processParseData();
  }

  /// @brief      Processes the data for a header.
  /// @version    2020-10-13/GGB - Function created.

  void CDelimitedParser::processParseHeader()
  {
    std::string szLine;

    std::getline(inputStream, szLine);

    if (!ignoreHeader_)
    {
      std::string_view sv(szLine);

      if (delimiterSE_ && sv.starts_with(delimiter_))
      {
        sv.remove_prefix(delimiter_.size());
      }
      if (delimiterSE_ && sv.ends_with(delimiter_))
      {
        sv.remove_suffix(delimiter_.size());
      }

      parseString(sv, headerData);
    }

  }
}
