//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                dataParser/elfParser.h
// SUBSYSTEM:           Data File Parser
// LANGUAGE:						C++
// TARGET OS:						None.
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2026 Gavin Blakeman.
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
// OVERVIEW:            Implementation of a ELF file parser
//
// CLASSES INCLUDED:
//
// HISTORY:             2026-03-17 GGB - File Created
//
//**********************************************************************************************************************************

#ifndef GCL_ELFPARSER_H
#define GCL_ELFPARSER_H

// Standard C++ library header files.

#include <cstdint>
#include <fstream>
#include <filesystem>
#include <memory>
#include <tuple>
#include <vector>

// GCL Header files.
#include "include/dataParser/ELF/elfHeader.h"

namespace GCL::ELF
{


  class CELFParser
  {
  public:
    CELFParser(std::filesystem::path const &fp) : filePath(fp) { parseFile(); }

  private:
    std::filesystem::path filePath;
    std::unique_ptr<CELFHeader> elfHeader;

    void parseFile();

    CELFParser() = delete;
    CELFParser(CELFParser const &) = delete;
    CELFParser(CELFParser &&) = delete;
    CELFParser &operator=(CELFParser const &) = delete;
    CELFParser &operator=(CELFParser &&) = delete;

  };

}

#endif // GCL_ELFPARSER_H
