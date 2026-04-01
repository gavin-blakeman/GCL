//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                dataParser/ELF/elfHeader.h
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

#ifndef GCL_ELF_ELFHEADER_HPP
#define GCL_ELF_ELFHEADER_HPP

// Standard C++ library header files.
#include <fstream>
#include <memory>
#include <tuple>

#include "include/dataParser/ELF/elfCommon.h"

namespace GCL::ELF
{
  struct Elf32_Ehdr;
  struct Elf64_Ehdr;

  class CELFHeader
  {
  public:
    CELFHeader(std::ifstream &ifs);
    ei_class elfClass() const noexcept { return elfClass_; }

    bool hasProgramHeader() const;
    bool hasSectionHeader() const;
    headerLoc_t programHeader() const;
    headerLoc_t seciotnHeader() const;


  protected:
    std::unique_ptr<Elf32_Ehdr> header32;
    std::unique_ptr<Elf64_Ehdr> header64;

  private:
    ei_class elfClass_;

    CELFHeader() = delete;
    CELFHeader(CELFHeader const &) = delete;
    CELFHeader(CELFHeader &&) = delete;
    CELFHeader &operator=(CELFHeader const &) = delete;
    CELFHeader &operator=(CELFHeader &&) = delete;
  };
}

#endif // GCL_ELF_ELFHEADER_HPP
