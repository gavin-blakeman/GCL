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
// HISTORY:             2024-02-19 GGB - Added section names to the dataTable_t member.
//                      2020-10-13 GGB - File Created
//
//**********************************************************************************************************************************

#ifndef DATAPARSERCORE_H
#define DATAPARSERCORE_H

  // Standard C++ library header files

#include <any>
#include <cstdint>
#include <istream>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace GCL
{
  /// Used to parse general data streams.

  class CDataTokeniser
  {
  public:
    using data_t = std::string;
    using dataLine_t = std::pair<std::size_t, std::vector<data_t>>;
    using dataFile_t = std::vector<dataLine_t>;
    struct dataTable_t
    {
      std::optional<std::string> tableName;
      dataLine_t headings;
      dataFile_t data;
    };
    using dataTables_t = std::vector<dataTable_t>;

    CDataTokeniser(dataTables_t &dt, std::istream &is) : inputStream(is), dataTables_(dt) {}
    virtual ~CDataTokeniser() = default;

    dataFile_t &data(std::size_t indx = 0)  { return dataFile(indx); }
    dataLine_t &header(std::size_t indx = 0)  { return dataHeader(indx); }
    dataTable_t &dataTable(std::size_t indx = 0) { return dataTables_[indx]; }
    dataTable_t &dataTable(std::string const &);
    std::size_t tableCount() const { return dataTables_.size(); }

    void parseData() { processParseData(); }
    void parseFile() { processParseFile(); }
    void parseFile(std::size_t ll) { lineLimit = ll; processParseFile(); }
    void parseHeader() { processParseHeader(); }

  protected:
    struct pData_t
    {
      std::size_t posnStart;
      std::size_t posnEnd;
      std::size_t posnNext;
    };

    std::istream &inputStream;
    dataTables_t &dataTables_;
    std::optional<std::size_t> lineLimit;     ///< Only parse the file up to the line limit.

    virtual void processParseData() = 0;
    virtual void processParseHeader() = 0;
    virtual void processParseFile() = 0;
    virtual dataFile_t &dataFile(std::size_t indx);
    virtual dataLine_t &dataHeader(std::size_t indx);

  private:
    CDataTokeniser() = delete;
    CDataTokeniser(CDataTokeniser const &) = delete;
    CDataTokeniser(CDataTokeniser &&) = delete;
    CDataTokeniser &operator=(CDataTokeniser const &) = delete;
    CDataTokeniser &operator=(CDataTokeniser &&) = delete;
  };

} // namespace GCL

#endif // DATAPARSERCORE_H
