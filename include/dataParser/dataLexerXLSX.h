//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                dataParser/dataLexerXLSX.h
// SUBSYSTEM:           Excel File Parser
// LANGUAGE:            C++
// TARGET OS:           None.
// NAMESPACE:           GCL
// AUTHOR:              Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2024 Gavin Blakeman.
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
// HISTORY:             2024-03-14 GGB - File Created
//
//**********************************************************************************************************************************

#ifndef GCL_DATAPARSER_DATALEXERXLSX_H
#define GCL_DATAPARSER_DATALEXERXLSX_H

// Standard C++ library

#include <filesystem>

// Miscellaneous libraries

#include <xlnt/xlnt.hpp>

// GCL library

#include "include/dataParser/dataParserCore.h"

namespace GCL
{
  class CDataLexerXLSX
  {
  public:
    CDataLexerXLSX(CDataTokeniser::dataTables_t &, std::filesystem::path const &);
    virtual ~CDataLexerXLSX() = default;

    void parseFile() { processParseFile(); }

  private:
    CDataLexerXLSX() = delete;
    CDataLexerXLSX(CDataLexerXLSX const &) = delete;
    CDataLexerXLSX(CDataLexerXLSX &&) = delete;
    CDataLexerXLSX &operator=(CDataLexerXLSX const &) = delete;
    CDataLexerXLSX &operator=(CDataLexerXLSX &&) = delete;

    CDataTokeniser::dataTables_t &dataTables;
    std::filesystem::path fileName;

    virtual void processParseFile();
    virtual void processSheet(CDataTokeniser::dataTable_t &, xlnt::worksheet);
  };
}

#endif // GCL_DATAPARSER_DATALEXERXLSX_H
