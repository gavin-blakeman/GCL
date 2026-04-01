//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                dataParser/ELF/programHeader.cpp
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
#include "include/dataParser/ELF/programHeader.h"

namespace GCL::ELF
{
  CProgramHeader::CProgramHeader(std::istream &ifs, ei_class elfClass)
  {
    switch (elfClass)
    {
      case ELFCLASS32:
      {
        header32 = std::make_unique<Elf32_Phdr>();
        ifs.read(reinterpret_cast<char *>(header32.get()), sizeof(Elf32_Phdr));
        break;
      }
      case ELFCLASS64:
      {
        header64 = std::make_unique<Elf64_Phdr>();
        ifs.read(reinterpret_cast<char *>(header64.get()), sizeof(Elf64_Phdr));
        break;
      }
      case ELFCLASSNONE:
      default:
      {
        break;
      }
    }
  }
} // namespace
