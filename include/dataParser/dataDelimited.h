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

#ifndef DATADELIMITED_H
#define DATADELIMITED_H

  // Standard C++ library

#include <string>
#include <string_view>

  // GCL header files

#include "include/dataParser/dataParserCore.h"

namespace GCL
{
  class CDelimitedParser : public CDataParser
  {
  public:
    CDelimitedParser(std::istream &is, std::string const &, bool = true);

    CDelimitedParser &delimiterCharacter(std::string const &) noexcept;
    CDelimitedParser &includesHeader(bool) noexcept;
    CDelimitedParser &ignoreHeader(bool) noexcept;
    CDelimitedParser &delimiterSE(bool se) noexcept { delimiterSE_ = se; return *this; }

  protected:
    void parseString(std::string_view &, dataLine_t &);
    void processParseHeader() override;
    void processParseData() override;
    void processParseFile() override;

  private:
    bool ignoreHeader_ = false;
    bool includesHeader_;
    std::string delimiter_ = ",";
    bool delimiterSE_ = false;

    std::string currentString;
    std::string::size_type currentPosn;



    void clear() noexcept;
  };
}

#endif // DATADELIMITED_H
