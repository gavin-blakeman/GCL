//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                dataParser/ELF/sectionHeader.h
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

#ifndef GCL_ELF_SECTIONHEADER_H
#define GCL_ELF_SECTIONHEADER_H

// Standard C++ header files
#include <istream>
#include <memory>

// GCL Library header files
#include "include/dataParser/ELF/elfCommon.h"

namespace GCL::ELF
{
  class CSectionHeader
  {
  public:
    CSectionHeader(std::istream &ifs, ei_class elfClass);

  protected:
    std::unique_ptr<Elf32_Shdr> header32;
    std::unique_ptr<Elf64_Shdr> header64;

  private:
    CSectionHeader() = delete;
    CSectionHeader(CSectionHeader const &) = delete;
    CSectionHeader(CSectionHeader &&) = delete;
    CSectionHeader &operator=(CSectionHeader const &) = delete;
    CSectionHeader &operator=(CSectionHeader &&) = delete;
  };
}

#endif // GCL_ELF_SECTIONHEADER_H
