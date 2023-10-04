//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                dataParser/dataParserCore.cpp
// SUBSYSTEM:           Data File Parser
// LANGUAGE:						C++
// TARGET OS:						None.
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2020-2023 Gavin Blakeman.
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

#include "include/dataParser/dataParserCore.h"

  // GCL library header files

#include "include/error.h"

namespace GCL
{

  CDataParser::dataFile_t &CDataParser::dataFile(std::size_t indx)
  {
    RUNTIME_ASSERT(indx < dataTables_.size(), "Requested index is greater than the number of tables.");

    return dataTables_[indx].data;
  }

  CDataParser::dataLine_t &CDataParser::dataHeader(std::size_t indx)
  {
    RUNTIME_ASSERT(indx < dataTables_.size(), "Requested index is greater than the number of tables.");

    return dataTables_[indx].headings;
  }
}
