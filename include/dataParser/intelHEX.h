//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                dataParser/intelHex.h
// SUBSYSTEM:           Data File Parser
// LANGUAGE:            C++
// TARGET OS:           None.
// NAMESPACE:           GCL
// AUTHOR:              Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2021, 2024, 2026 Gavin Blakeman.
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
// HISTORY:             2021-07-08 GGB - File Created
//
//**********************************************************************************************************************************


#ifndef GCL_INTELHEX_H
#define GCL_INTELHEX_H

// Standard C++ library header files

#include <cstdint>
#include <filesystem>
#include <tuple>
#include <vector>

namespace GCL
{

  class CIntelHEX
  {
  public:
    using record_t = std::tuple<std::uint64_t, std::vector<std::uint8_t>>;
    using recordSet_t = std::vector<record_t>;
    using iterator = recordSet_t::iterator;
    using const_iterator = recordSet_t::const_iterator;

    /// @brief Constructor for the class.
    /// @param[in] fn: The file name of the HEX file.
    CIntelHEX(std::filesystem::path const &fn) : ifn(fn) {}

    /// @brief Reads a file in Intel HEX format.
    void readFile();



  private:
    std::filesystem::path ifn;
    recordSet_t memoryRecords;

    CIntelHEX() = delete;
    CIntelHEX(CIntelHEX const &) = delete;
    CIntelHEX(CIntelHEX &&) = delete;
    CIntelHEX &operator=(CIntelHEX const &) = delete;
    CIntelHEX &operator=(CIntelHEX &&) = delete;
  };
}   // namespace

#endif //GCL_INTELHEX_H
