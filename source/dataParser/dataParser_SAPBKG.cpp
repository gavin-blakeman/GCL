//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                dataParser/dataParser_SAPBKG.h
// SUBSYSTEM:           Data File Parser
// LANGUAGE:						C++
// TARGET OS:						None.
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2020-2024 Gavin Blakeman.
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
// HISTORY:             2023-10-02 GGB - File Created
//
//**********************************************************************************************************************************

#include "include/dataParser/dataParser_SAPBKG.h"

  // Standard C++ libraries

#include <limits>
#include <string>
#include <string_view>
#include <vector>

  // Miscellaneous library header files

namespace GCL
{

  /// @brief      Parse the data. Just forwards the call to processParseFile();
  /// @throws
  /// @version    2023-10-02/GGB - Function created.

  void CSAPBKGTokeniser::processParseData()
  {
    if (!parsingComplete)
    {
      processParseFile();
    };
  }

  /// @brief      Parse the header. Just forwards the call to processParseFile();
  /// @throws
  /// @version    2023-10-02/GGB - Function created.

  void CSAPBKGTokeniser::processParseHeader()
  {
    if (!parsingComplete)
    {
      processParseFile();
    }
  }

  /// @brief      Parses the entire file.
  /// @throws
  /// @version    2023-10-02/GGB - Function created.

  void CSAPBKGTokeniser::processParseFile()
  {
    enum stateMachine_t
    {
      SM_START,
      SM_HEADER,
      SM_DATA,
    };

    columnData_t columnWidths;
    stateMachine_t state = SM_START;
    dataLine_t headings;
    std::size_t lineNumber = 0;
    std::size_t lineMax = lineLimit ? *lineLimit : std::numeric_limits<std::size_t>::max();

      /* The file consists of a heading section, (with varying layouts), and a data section that consits of an indeterminite
       * number of pages.
       * Column layout in the data section is not consistent and each line needs to be individually parsed.
       * Column values may be present of absent. Data type is report dependent, with some columns that may be 99% numeric, but
       * have a small number of string records. Therefore data conversion is left to the application.
       */

    dataTables_.clear();

    while (!inputStream.eof() && (lineNumber < lineMax) )
    {
      std::string szLine;

      std::getline(inputStream, szLine);

      std::string_view svLine(szLine);

      if (svLine.ends_with('\r')) // For windows
      {
        svLine.remove_suffix(1);
      }

      if (svLine.size() != 0)
      {
        switch (state)
        {
          case SM_START:
          {
              /* Started parsing the file. Looking for the start of the first row of headings.
               * A row of headings is always preceded by a line of '----'
               * All other data is discarded.
               */

            if (testLineDash(svLine))
            {
              state = SM_HEADER;
            }
            break;
          }
          case SM_HEADER:
          {
              /* The first line of the header is read and discarded in the case above.
               * If the line starts with "|-" it is an indication that the line is a seperator between headings and data.
               * Else the line is the headings.
               */

            if (svLine[0] == '|' && svLine[1] == '-')
            {
                // Headings have ended.

              state = SM_DATA;
            }
            else
            {
                /* Read and tokenize the line as a heading. */

              headings = tokeniseLine(svLine);
              headings.first = lineNumber;
              if (dataTables_.empty())
              {
                dataTables_.push_back({});
                dataTables_.back().headings = std::move(headings);
                svLine = szLine;
                determineColumnWidths(svLine, columnWidths);
              }
              else if (dataTables_.back().headings.second != headings.second)
              {
                dataTables_.push_back({});
                dataTables_.back().headings = std::move(headings);
                svLine = szLine;
                determineColumnWidths(svLine, columnWidths);
              }
            }
            break;
          }
          case SM_DATA:
          {
              /* A table is terminated with a row of '---' */

            if (testLineDash(svLine))
            {
              state = SM_START;
            }
            else
            {
              dataTables_.back().data.emplace_back(std::move(tokeniseLine(svLine, columnWidths)));
              dataTables_.back().data.back().first = lineNumber;
            }
            break;
          }
        }
        /* string view and string are destructed automatically. */
      }
      lineNumber++;
    }
    parsingComplete = true;
  }

  /// @brief      Tests if a line only consists of '-' characters.
  /// @param[in]  svLine: The line to test.
  /// @throws     None.
  /// @returns    true if the line is only '-' characters.

  bool CSAPBKGTokeniser::testLineDash(std::string_view const &svLine) noexcept
  {
    bool rv = true;
    std::size_t indx = 0;

    while (indx < svLine.length() && rv)
    {
      if (svLine[indx++] != '-')
      {
        rv = false;
      }
    }

    return rv;
  }

  /// @brief      Tokenise a line when the widths are not known. (Use only for headers)
  /// @param[in]  svLine: The string_view to tokenise.
  /// @returns    A dataLine containing all the data.
  /// @version    2023-10-02/GGB - Function created.

  GCL::CDataTokeniser::dataLine_t CSAPBKGTokeniser::tokeniseLine(std::string_view &svLine)
  {
    dataLine_t rv;
    std::size_t tokenEnd;

    svLine.remove_prefix(1);  // Remove the initial "|"

    while (!svLine.empty())
    {
      tokenEnd = svLine.find("|", 0);
      if (tokenEnd != std::string_view::npos)
      {
        rv.second.push_back(std::string{svLine.substr(0, tokenEnd)});
        svLine.remove_prefix(tokenEnd + 1);
      };
    }

    return rv;
  }

  /// @brief      Tokenise a line when the widths are known. (Data) It is possible that the SAP files do not accurately follow
  ///             the column widths. The column width may be less than, or more that the widths determined by the headers.
  ///             This is the reason for the tests to check if the inter-colum character is in fact a '|'. If it is not, the
  ///             line will be toeknised using the alternate tokeniser.
  /// @param[in]  svLine: The line to tokenise.
  /// @param[in]  columnWidths: The widths of each column.
  /// @returns    A dataLine containing all the data from the line.
  /// @version    2023-10-02/GGB - Function created.

  GCL::CDataTokeniser::dataLine_t CSAPBKGTokeniser::tokeniseLine(std::string_view &svLine, columnData_t const & columnWidths)
  {
    dataLine_t rv;
    bool parseFail = false;

    for (auto const &columnWidth : columnWidths)
    {
      if (svLine[std::get<2>(columnWidth)] != '|')
      {
        parseFail = true;
        break;
      }
      else
      {
        rv.second.push_back(std::string{svLine.substr(std::get<0>(columnWidth), std::get<1>(columnWidth))});
      };
    }

    if (parseFail)
    {
      return tokeniseLine(svLine);
    }
    else
    {
      return rv;
    };
  }

  /// @brief      Determines the start and width of each column.
  /// @param[in]  svLine: The line containing the headings.
  /// @param[out] columnData: The start and width of each column.
  /// @version    2023-10-02/GGB - Function created.

  void CSAPBKGTokeniser::determineColumnWidths(std::string_view &svLine, columnData_t &columnData)
  {
    std::size_t columnStart = 0, columnEnd;
    std::vector<std::string> columnHeadings;

    columnData.clear();
    columnStart++;

    while (columnStart < svLine.length())
    {
      columnEnd = svLine.find("|", columnStart);
      if (columnEnd != std::string_view::npos)
      {
        columnData.emplace_back(columnStart, columnEnd - columnStart, columnEnd);
        columnHeadings.emplace_back(svLine.substr(columnStart, columnEnd - columnStart));
        columnStart = columnEnd + 1;
      }
      else
      {
        columnStart = svLine.length();
      }
    }
  }

} // namespace GCL;
