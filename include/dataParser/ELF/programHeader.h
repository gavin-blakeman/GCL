//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                dataParser/ELF/programHeader.h
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

#ifndef GCL_ELF_PROGRAMHEADER_H
#define GCL_ELF_PROGRAMHEADER_H

// Standard C++ library
#include <istream>
#include <memory>

// GCL Files
#include "include/dataParser/ELF/elfCommon.h"

namespace GCL::ELF
{
  class CProgramHeader
  {
  public:
    CProgramHeader(std::istream &ifs, ei_class elfClass);

  private:
    std::unique_ptr<Elf32_Phdr> header32;
    std::unique_ptr<Elf64_Phdr> header64;

    CProgramHeader() = delete;
    CProgramHeader(CProgramHeader const &) = delete;
    CProgramHeader(CProgramHeader &&) = delete;
    CProgramHeader &operator=(CProgramHeader const &) = delete;
    CProgramHeader &operator=(CProgramHeader &&) = delete;
  };

}

#endif // GCL_ELF_PROGRAMHEADER_H
