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
//*********************************************************************************************************************************/

#ifndef GCL_INCLUDE_UTF_H_
#define GCL_INCLUDE_UTF_H_

// Standard C++ library
#include <cstdint>
#include <istream>

// GCL header files
#include "include/concepts.hpp"
#include "include/error.h"
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
   *  @throws     bad_utf8
   */
  template<typename Iter>
  requires isUTF8Char<typename std::iterator_traits<Iter>::value_type>
  Iter decodeUTF(Iter begin, Iter end, std::uint32_t &codePoint)
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
        throw bad_codepoint();
      }
    }
    else
    {
      codePoint = *begin++;
    }

    return begin;
  }

  /*! @brief      Converts a sequence of UTF16 code units (uint16 or char16) to a code point value. The sequence
   *              defined by the begin and end iterator may be longer than required. The function will only consume the required
   *              number of code units and return an iterator to the first unconsumed value.
   *  @param[in]  begin: Iterator to the start of the UTF8 code units.
   *  @param[in]  end: Iterator to the end of the UTF8 code units.
   *  @param[out] codePoint: The codepoint value.
   *  @returns    Iterator to the first unconsumed code unit.
   *  @throws     bad_codePoint
   *  @throws     bad_utf16_value
   */
  template<typename Iter>
  requires isUTF16Char<typename std::iterator_traits<Iter>::value_type>
  Iter decodeUTF(Iter begin, Iter end, std::uint32_t &codePoint)
  {
    codePoint = *begin++;

    if (codePoint & 0xD800)
    {
      if (begin != end)
      {
        // Surrogate area runs from 0xD800 to 0xDFFF
        codePoint -= 0xD800;
        codePoint *= 0x400;

        std::uint32_t ls = *begin++;
        ls -= 0xDC00;
        codePoint += ls + 0x10000;
      }
      else
      {
        throw unexpected_eof();
      }
    }

    return begin;
  }

  template<typename Iter>
  requires isUTF32Char<typename std::iterator_traits<Iter>::value_type>
  Iter decodeUTF(Iter begin, Iter end, std::uint32_t &codePoint)
  {
    codePoint = *begin++;
    return begin;
  }

  /*! @brief      Convert a code point to UTF-8 code units.
   *  @param[in]  codePoint: The code point to convert.
   *  @param[out] str: Thr string to add the code units to. (Must have push() or push_back()
   *  @throws
   */
  template<class T>
  requires isUTF8Char<typename T::value_type> && HasPushBack<T, typename T::value_type>
  void encodeUTF(std::uint32_t codePoint, T &str)
  {
    RUNTIME_ASSERT(codePoint <= 0x10FFFF, "Codepoint outside valid range");  // Sanity check.

    if (codePoint <= 0x007F)
    {
      str.push_back(static_cast<typename T::value_type>(codePoint));
    }
    else if (codePoint <= 0x07FF)
    {
      typename T::value_type cpByte = static_cast<std::uint8_t>(codePoint >> 6);
      cpByte &= 0b00011111;
      str.push_back(0b11000000 | cpByte);

      cpByte = static_cast<std::uint8_t>(codePoint);
      cpByte &= 0b00111111;
      str.push_back(0b10000000 | cpByte);
    }
    else if (codePoint <= 0xFFFF)
    {
      std::uint8_t cpByte = static_cast<std::uint8_t>(codePoint >> 12);
      cpByte &= 0b00001111;
      str.push_back(0b11100000 | cpByte);

      cpByte = static_cast<std::uint8_t>(codePoint >> 6);
      cpByte &= 0b00111111;
      str.push_back(0b10000000 | cpByte);

      cpByte = static_cast<std::uint8_t>(codePoint);
      cpByte &= 0b00111111;
      str.push_back(0b10000000 | cpByte);
    }
    else
    {
      std::uint8_t cpByte = static_cast<std::uint8_t>(codePoint >> 18);
      cpByte &= 0b00000111;
      str.push_back(0b11110000 | cpByte);

      cpByte = static_cast<std::uint8_t>(codePoint >> 12);
      cpByte &= 0b00111111;
      str.push_back(0b10000000 | cpByte);

      cpByte = static_cast<std::uint8_t>(codePoint >> 6);
      cpByte &= 0b00111111;
      str.push_back(0b10000000 | cpByte);

      cpByte = static_cast<std::uint8_t>(codePoint);
      cpByte &= 0b00111111;
      str.push_back(0b10000000 | cpByte);
    }
  }

  /*! @brief      Convert a code point to UTF-8 code units.
   *  @param[in]  codePoint: The code point to convert.
   *  @param[out] str: Thr string to add the code units to. (Must have push() or push_back()
   *  @throws
   */
  template<class T>
  requires isUTF16Char<typename T::value_type> && HasPushBack<T, typename T::value_type>
  void encodeUTF(std::uint32_t codePoint, T &str)
  {
    RUNTIME_ASSERT(codePoint <= 0x10FFFF, "Codepoint outside valid range"); // Sanity check.

    if (codePoint < 0x10000)
    {
      str.push_back(static_cast<char16_t>(codePoint));
    }
    else
    {
      codePoint -= 0x10000;

      str.push_back(0xD800 + static_cast<char16_t>(codePoint >> 10));
      str.push_back(0xDC00 + static_cast<char16_t>(codePoint & 0b1111111111));
    };
  }

  template<class T>
  requires isUTF32Char<typename T::value_type> && HasPushBack<T, typename T::value_type>
  void encodeUTF(std::uint32_t codePoint, T &str)
  {
    str.push_back(codePoint);
  }

  template<class Iter, class C>
  void transcode(Iter begin, Iter end, C &str)
  {

  }

}

#endif /* GCL_INCLUDE_UTF_H_ */
