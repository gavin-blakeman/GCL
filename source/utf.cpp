//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                utf.h
// LANGUAGE:            C++
// TARGET OS:           None.
// NAMESPACE:           GCL
// AUTHOR:              Gavin Blakeman (GGB)
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
// OVERVIEW:            UTF constants and strings.
//
// HISTORY:             2024-04-23 GGB - File Created
//
//*********************************************************************************************************************************
#include "include/utf.h"

// Standard C++ library header files
#include <map>
#include <vector>

namespace GCL
{
  std::map<utf_e, std::vector<std::uint8_t>> bomMap =
  {
    { UTF_8, { 0xEF, 0xBB, 0xBF }},
    { UTF_16BE, { 0xFE, 0xFF }},
    { UTF_16LE,  { 0xFF, 0xFE }},
  };

  /// @brief      Reads the initial bytes in a stream to determine if the stream has a BOM. The stream is also left in a position
  ///             that the next byte to read is a data byte.
  /// @param[in]  ifs: The input stream.
  /// @returns    A value indicating the BOM type and if the stream has a BOM.
  /// @note       If the stream does not have a BOM, it will be left pointing to the first character. If it does have a BOM, the
  ///             stream will point to the first character after the BOM.
  /// @version    2024-04-23/GGB - Function created.

  utf_e getBOM(std::istream &ifs)
  {
    utf_e rv = UTF_NONE;
    // Read the first four bytes from the stream
    std::uint8_t BOM[4];
    std::uint_fast8_t indx = 0;

    while ((rv == UTF_NONE) && (indx != 4) && ifs.good())
    {
      ifs.get(reinterpret_cast<char &>(BOM[indx]));
      if (ifs.good())
      {
        auto iter = bomMap.begin();

        while ((iter != bomMap.end()) && rv == UTF_NONE)
        {
          /// Only test options that are already at length.
          if ((iter->second.size() - 1)  == indx)
          {
            bool found = true;
            std::uint_fast8_t indx2;

            for (indx2 = 0; (indx2 != indx) && found; indx2++)
            {
              found = found && (iter->second)[indx2] == BOM[indx2];
            }
            if (found)
            {
              rv = iter->first;
            }
          }
          iter++;
        }
      }
      indx++;
    }
    if (rv == UTF_NONE)
    {
      // Return the stream to the first character.
      ifs.seekg(0);
    }
    return rv;
  }

}
