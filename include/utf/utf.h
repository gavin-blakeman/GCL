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

#ifndef GCL_INCLUDE_UTF_H_
#define GCL_INCLUDE_UTF_H_

// Standard C++ library
#include <cstdint>
#include <istream>

// Utf header files
#include "include//utf/utfExceptions.hpp"

namespace GCL
{
  enum utf_e
  {
    UTF_NONE,
    UTF_8,
    UTF_16BE,
    UTF_16LE,
  };

  utf_e getBOM(std::istream &);

  /*! @brief      Converts a sequence of UTF8 code units (uint8 or char8) and converts it to a code point value. The sequence
   *              defined by the begin and end iterator may be longer than required. The function will only consume the required
   *              number of code units and return an iterator to the first unconsumed value.
   *  @param[in]  begin: Iterator to the start of the UTF8 code units.
   *  @param[in]  end: Iterator to the end of the UTF8 code units.
   *  @param[out] codePoint: The codepoint value.
   *  @returns    Iterator to the first unconsumed code unit.
   *  @throws     bad_codePoint
   *  @throws     bad_utf8_value
   */
  template<typename Iter>
  Iter encodeUTF8(Iter begin, Iter end, std::uint32_t &codePoint)
  {
    //7, 11, 16, 21

    if (*begin >= 0b10000000)
    {
      std::uint8_t byteCounter;
      std::uint32_t finalMask = 0b00011111;
      int byteCount = 1;

      codePoint = byteCounter = *begin++;
      byteCounter = byteCounter << 1;

      do
      {
        finalMask <<= 6;
        finalMask |= 0b111111;
        if (begin != end)
        {
          codePoint <<= 6;            // Shift up by 6
          codePoint |= static_cast<uint32_t>(*begin++) & 0b00111111;
        }
        else
        {
          throw unexpected_eof();
        }
        byteCounter <<= 1;
        byteCount++;
      }
      while (((byteCounter & 0b10000000) != 0) && (byteCount <= 4));

      if (byteCount <= 4)
      {
        byteCount--;
        while(--byteCount)
        {
          finalMask >>= 1;
        }
        codePoint &= finalMask;
      }
      else
      {
        throw   bad_codepoint();
      }
    }
    else
    {
      codePoint = *begin++;
    }

    return begin;
  }

  template<typename Iter>
  std::uint32_t encodeUTF16(Iter begin, Iter end)
  {
    std::uint32_t codePoint;

    return codePoint;
  }

}

#endif /* GCL_INCLUDE_UTF_H_ */
