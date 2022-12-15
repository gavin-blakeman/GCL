//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                dataParser/dataParserCore.h
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

#ifndef DATAPARSERCORE_H
#define DATAPARSERCORE_H

  // Standard C++ library header files

#include <any>
#include <cstdint>
#include <istream>
#include <map>
#include <string>
#include <vector>

namespace GCL
{
  /// Used to parse general data streams.

  class CDataParser
  {
  public:
    using data_t = std::string;
    using dataLine_t = std::vector<data_t>;
    using dataFile_t = std::vector<dataLine_t>;

  private:
  protected:
    std::istream &inputStream;
    dataLine_t headerData;
    dataFile_t dataFile;

    virtual void processParseData() = 0;

  public:
    CDataParser(std::istream &is) : inputStream(is) {}
    virtual ~CDataParser() {}

    dataFile_t &data() noexcept { return dataFile; }
    dataLine_t &header() noexcept { return headerData; }

    void parseData() { processParseData(); }


  };

} // namespace GCL

#endif // DATAPARSERCORE_H
