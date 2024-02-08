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


#ifndef DATASAPBKG_H
#define DATASAPBKG_H

  // Standard C++ library

#include <iostream>
#include <tuple>

  // GCL header files

#include "include/dataParser/dataParserCore.h"

namespace GCL
{
  /* As the headings and data are 'buried' in the file and are a variable distance into the file, the file needs to be parsed
   * as a whole. IE the parseData() and parseHeader() functions need to perform the same as parseFile().
   * The file may also consist of one or a number of 'tables' of data. Each 'table' has a heading and a data section.
   * The approach is to store each 'table' as a seperate entity. The calling application can then make sense of the data based
   * on the assumptions.
   * If two adjacent 'tables' have the same number of columns and the same column headings, they are assumed to be the same
   * table and the data will be concatenated.
   */

  class CSAPBKGParser : public CDataParser
  {
    public:
      CSAPBKGParser(std::istream &ifs) : CDataParser(ifs) {}

    private:
      CSAPBKGParser() = delete;
      CSAPBKGParser(CSAPBKGParser const &) = delete;
      CSAPBKGParser(CSAPBKGParser &&) = delete;
      CSAPBKGParser &operator=(CSAPBKGParser const &) = delete;
      CSAPBKGParser &operator=(CSAPBKGParser &&) = delete;

      using columnWidth_t = std::tuple<std::size_t, std::size_t, std::size_t>; // Start, Width, InterColumn.
      using columnData_t = std::vector<columnWidth_t>;

      bool parsingComplete = false;

      virtual void processParseData() override;
      virtual void processParseHeader() override;
      virtual void processParseFile() override;

      bool testLineDash(std::string_view const &) noexcept;
      dataLine_t tokeniseLine(std::string_view &);
      dataLine_t tokeniseLine(std::string_view &, columnData_t const &);
      void determineColumnWidths(std::string_view &, columnData_t &);
  };
}

#endif // DATASAPBKG_H
