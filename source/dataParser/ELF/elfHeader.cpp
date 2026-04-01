//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                dataParser/ELF/elfHeader.cpp
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

#include "include/dataParser/ELF/elfHeader.h"

// Standard C++ library
#include <cstring>

// GCL Header files
#include "include/logger/loggerManager.h"
#include "include/error.h"

namespace GCL::ELF
{
  CELFHeader::CELFHeader(std::ifstream &ifs)
  {
    // Grab the first 16 bytes.

    char initialBytes[EI_NIDENT];

    ifs.read(initialBytes, EI_NIDENT);
    switch (initialBytes[0])
    {

      case ELFCLASS32:
      {
        header32 = std::make_unique<Elf32_Ehdr>();
        memcpy(reinterpret_cast<void *>(header32.get()), initialBytes, EI_NIDENT);
        ifs.read(reinterpret_cast<char *>(header32.get() + EI_NIDENT), sizeof(Elf32_Ehdr) - EI_NIDENT);
        elfClass_ = ELFCLASS32;
        break;
      }
      case ELFCLASS64:
      {
        header64 = std::make_unique<Elf64_Ehdr>();
        memcpy(reinterpret_cast<void *>(header64.get()), initialBytes, EI_NIDENT);
        ifs.read(reinterpret_cast<char *>(header64.get() + EI_NIDENT), sizeof(Elf64_Ehdr) - EI_NIDENT);
        elfClass_ = ELFCLASS64;
        break;
      }
      case ELFCLASSNONE:
      {
        elfClass_ = ELFCLASSNONE;
        break;
      }
      default:
      {
        break;
      }
    }
  }

  bool CELFHeader::hasProgramHeader() const
  {
    bool rv = false;

    switch (elfClass_)
    {
      case ELFCLASS32:
      {
        if (header32 && header32->e_phoff && header32->e_phnum)
        {
          rv = true;
        }
        break;
      }
      case ELFCLASS64:
      {
        if (header64 && header64->e_phoff && header64->e_phnum)
        {
          rv = true;
        }
        break;
      }
      case ELFCLASSNONE:
      default:
      {
        RUNTIME_ERROR("No Header Loaded");
        break;
      }
    }

    return rv;
  }

  bool CELFHeader::hasSectionHeader() const
  {
    bool rv = false;

    switch (elfClass_)
    {
      case ELFCLASS32:
      {
        if (header32 && header32->e_shoff && header32->e_shnum)
        {
          rv = true;
        }
        break;
      }
      case ELFCLASS64:
      {
        if (header64 && header64->e_shoff && header64->e_shnum)
        {
          rv = true;
        }
        break;
      }
      case ELFCLASSNONE:
      default:
      {
        RUNTIME_ERROR("No Header Loaded");
        break;
      }
    }

    return rv;
  }

  headerLoc_t CELFHeader::programHeader() const
  {
    headerLoc_t rv;

    switch (elfClass_)
    {
      case ELFCLASS32:
      {
        if (header32)
        {
          rv = std::make_tuple(header32->e_phoff, header32->e_phnum, header32->e_phentsize);
        }
        break;
      }
      case ELFCLASS64:
      {
        if (header64)
        {
          rv = std::make_tuple(header64->e_phoff, header64->e_phnum, header64->e_phentsize);
        }
        break;
      }
      case ELFCLASSNONE:
      default:
      {
        RUNTIME_ERROR("No Header Loaded");
        break;
      }
    }

    return rv;
  }

  headerLoc_t CELFHeader::seciotnHeader() const
  {
    headerLoc_t rv;

    switch (elfClass_)
    {
      case ELFCLASS32:
      {
        if (header32)
        {
          rv = std::make_tuple(header32->e_shnum, header32->e_shnum, header32->e_shentsize);
        }
        break;
      }
      case ELFCLASS64:
      {
        if (header64)
        {
          rv = std::make_tuple(header64->e_shnum, header64->e_shnum, header64->e_shentsize);
        }
        break;
      }
      case ELFCLASSNONE:
      default:
      {
        RUNTIME_ERROR("No Header Loaded");
        break;
      }
    }

    return rv;
  }

}
