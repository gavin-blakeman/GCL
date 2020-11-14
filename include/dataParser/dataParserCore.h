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
#include <map>
#include <string>
#include <vector>

namespace GCL
{
  class CDataParser
  {
  public:
    using headerData_t = std::map<std::string, std::uint16_t>;
    using data_t = std::any;
    using dataLine_t = std::vector<std::any>;
    using dataFile_t = std::vector<dataLine_t>;

  private:
  protected:
    headerData_t headerData;
    dataFile_t dataFile;

  public:
  };

} // namespace GCL

#endif // DATAPARSERCORE_H
