//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                dataParser/ELF/elfParser.cpp
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

#include "include/dataParser/ELF/elfParser.h"

// Standard C++ library header files.


// GCL Header files.
#include "include/logger/loggerManager.h"
#include "include/dataParser/ELF/elfHeader.h"

namespace GCL::ELF
{
  using GCL::logger::ERRORMESSAGE;

  void CELFParser::parseFile()
  {
    std::ifstream ifs{filePath, std::ios::in | std::ios::ate};

    if (!ifs.is_open())
    {
      ERRORMESSAGE("Unable to open file: " + filePath.string());
    }
    else
    {
      elfHeader = std::make_unique<CELFHeader>(ifs);
      if (elfHeader->hasProgramHeader())
      {
      }
    }
  }


}
