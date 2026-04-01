//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                dataParser/ELF/elfCommon.h
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


#ifndef GCL_ELF_ELFCOMMON_H
#define GCL_ELF_ELFCOMMON_H

// Standard C++ library header file
#include <cstdint>
#include <tuple>

namespace GCL::ELF
{
  using Elf32_Addr = std::uint32_t;
  using Elf32_Off = std::uint32_t;
  using Elf32_Half = std::uint16_t;
  using Elf32_Word = std::uint32_t;
  using Elf32_Sword = std::int32_t;

  using Elf64_Addr = std::uint64_t;
  using Elf64_Off = std::uint64_t;
  using Elf64_Half = std::uint16_t;
  using Elf64_Word = std::uint32_t;
  using Elf64_Sword = std::int32_t;
  using Elf64_Xword = std::uint64_t;
  using Elf64_Sxword = std::int64_t;

  using headerLoc_t = std::tuple<Elf64_Off, Elf64_Half, Elf64_Half>;

  std::size_t const EI_NIDENT = 16;

  struct Elf32_Ehdr
  {
    union
    {
      struct
      {
        union
        {
          struct
          {
            std::uint8_t ei_mag0;
            std::uint8_t ei_mag1;
            std::uint8_t ei_mag2;
            std::uint8_t ei_mag3;
          };
          std::uint32_t ei_mag;
        };
        std::uint8_t ei_class;
        std::uint8_t ei_data;
        std::uint8_t ei_version;
        std::uint8_t ei_osabi;
        std::uint8_t ei_abiversion;
        std::uint8_t ei_pad[EI_NIDENT - 8];
      };
      unsigned char e_ident[EI_NIDENT];
    };
    Elf32_Half e_type;
    Elf32_Half e_machine;
    Elf32_Word e_version;
    Elf32_Addr e_entry;
    Elf32_Off e_phoff;
    Elf32_Off e_shoff;
    Elf32_Word e_flags;
    Elf32_Half e_ehsize;
    Elf32_Half e_phentsize;
    Elf32_Half e_phnum;
    Elf32_Half e_shentsize;
    Elf32_Half e_shnum;
    Elf32_Half e_shstrndx;
  };

  struct Elf64_Ehdr
  {
    union
    {
      struct
      {
        union
        {
          struct
          {
            std::uint8_t ei_mag0;
            std::uint8_t ei_mag1;
            std::uint8_t ei_mag2;
            std::uint8_t ei_mag3;
          };
          std::uint32_t ei_mag;
        };
        std::uint8_t ei_class;
        std::uint8_t ei_data;
        std::uint8_t ei_version;
        std::uint8_t ei_osabi;
        std::uint8_t ei_abiversion;
        std::uint8_t ei_pad[EI_NIDENT - 8];
      };
      unsigned char e_ident[EI_NIDENT];
    };
    Elf64_Half e_type;
    Elf64_Half e_machine;
    Elf64_Word e_version;
    Elf64_Addr e_entry;
    Elf64_Off e_phoff;
    Elf64_Off e_shoff;
    Elf64_Word e_flags;
    Elf64_Half e_ehsize;
    Elf64_Half e_phentsize;
    Elf64_Half e_phnum;
    Elf64_Half e_shentsize;
    Elf64_Half e_shnum;
    Elf64_Half e_shstrndx;
  };

  enum ei_class : std::uint8_t
  {
    ELFCLASSNONE  = 0,
    ELFCLASS32    = 1,
    ELFCLASS64    = 2,
  };

  enum ei_data : std::uint8_t
  {
    ELFDATANONE   = 0,
    ELFDATA2LSB   = 1,
    ELFDATA2MSB   = 2,
  };

  enum e_type : Elf64_Half
  {
    ET_NONE     = 0x00,
    ET_REL      = 0x01,
    ET_EXEC     = 0x02,
    ET_DYN      = 0x03,
    ET_CORE     = 0x04,
    ET_LOOS     = 0xFE00,
    ET_HIOS     = 0xFEFF,
    ET_LOPROVC  = 0xFF00,
    ET_HIPROC   = 0xFFFF,
  };

  enum e_machine : Elf64_Half
  {
    EM_DSPIC30F   = 118,      // Microchip dsPIC30F DSP
    EM_MCHP_PIC   = 204,      // Microchip 8-bit PIC
    EM_RISCV      = 243,
  };

  struct Elf32_Shdr
  {
    Elf32_Word sh_name;
    Elf32_Word sh_type;
    Elf32_Word sh_flags;
    Elf32_Addr sh_addr;
    Elf32_Off  sh_offset;
    Elf32_Word sh_size;
    Elf32_Word sh_link;
    Elf32_Word sh_info;
    Elf32_Word sh_addralign;
    Elf32_Word sh_entsize;
  };

  struct Elf64_Shdr
  {
    Elf64_Word sh_name;
    Elf64_Word sh_type;
    Elf64_Xword sh_flags;
    Elf64_Addr sh_addr;
    Elf64_Off
    sh_offset;
    Elf64_Xword sh_size;
    Elf64_Word sh_link;
    Elf64_Word sh_info;
    Elf64_Xword sh_addralign;
    Elf64_Xword sh_entsize;
  };

  enum sh_type : Elf64_Word
  {
    SHT_NULL      = 0,
    SHT_PROGBITS  = 1,
    SHT_NOTE      = 7,
    SHT_NOBITS    = 8,
  };

  enum sh_flags : Elf64_Xword
  {

  };

  struct Elf32_Phdr
  {
    Elf32_Word p_type;
    Elf32_Off p_offset;
    Elf32_Addr p_vaddr;
    Elf32_Addr p_paddr;
    Elf32_Word p_filesz;
    Elf32_Word p_memsz;
    Elf32_Word p_flags;
    Elf32_Word p_align;
  };

  struct Elf64_Phdr
  {
    Elf64_Word p_type;
    Elf64_Word p_flags;
    Elf64_Off p_offset;
    Elf64_Addr p_vaddr;
    Elf64_Addr p_paddr;
    Elf64_Xword p_filesz;
    Elf64_Xword p_memsz;
    Elf64_Xword p_align;
  };
}

#endif // GCL_ELF_ELFCOMMON_H
