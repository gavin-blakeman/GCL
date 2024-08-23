//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                codePoint.hpp
// SUBSYSTEM:           UTF code point
// LANGUAGE:            C++
// TARGET OS:           None.
// NAMESPACE:           GCL
// AUTHOR:              Gavin Blakeman.
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
// OVERVIEW:            Provides a codepoint class for storing and converting codepoints.
//
// CLASSES INCLUDED:
//
// HISTORY:             2024-08-08 GGB - File Created
//
//**********************************************************************************************************************************/

#ifndef GCL_INCLUDE_PARSERS_CODEPOINT_HPP_
#define GCL_INCLUDE_PARSERS_CODEPOINT_HPP_

// Standard C++ header files
#include <cstdint>
#include <string>
#include <variant>

// Miscellaneous libraries
#include <fmt/format.h>

// GCL library
#include "include/concepts.hpp"
#include "include/utf/utfExceptions.hpp"
#include "include/utf/utf.h"

/* codePoint_t stores codePoints as UTF32. This class should have the same storage as a UTF32 char.
 * A basic_string of type codePoint_t should have the same memory layout and values as a UTF32 string.
 * It should be possible to reinterpret_cast between UTF32 and codePoint_t. (1)
 * codePoint_t provides the conversions from UTF8 and UTF16 to/from UTF32.
 * Note: 1. U_EOF is defined as a invalid character. If this is processed using a UTF32 checker, it should/will
 *          fail. The HTML parser/tokeniser is such that the U_EOF will never appear in the output, only in the
 *          intermediate queue that is used for buffering the stream and/or included files.
 *       2. UTF32 was chosen as it is a fixed size representation. It is only used as the internal (intermediate)
 *          representation in the tokeniser/tree builder. As the codePoints are fixed length there is no
 *          requirement to pass/manage variable length encodings. The 32 bit nature also fits easily
 *          in common register sizes.
 *       3. There are aspects of HTML that are limited to ASCII. This simplifies the tolower() function as
 *          it only needs to operate on ASCII characters.
 *       4. If full localisation is required use std::u32string and char32_t.
 */

namespace GCL::parsers
{
  class codePoint_t
  {
  public:
    using utf32_t = std::uint32_t;
    using value_type = utf32_t;

    constexpr codePoint_t() = default;
    constexpr codePoint_t(codePoint_t const &) = default;
    constexpr codePoint_t(codePoint_t &&) = default;

    /*! @brief      Converts a string object (begin and end iterator) to a unicode code point.
     *  @param[in/out] begin: The starting iterator. The value is changed to reflect the next point to process.
     */
    template <typename Iter>
    requires UTFChar<typename Iter::value_type>
    constexpr codePoint_t(Iter &begin, Iter end)
    {
      begin = decodeUTF(begin, end, value);
    }

    constexpr codePoint_t(char c) : value(c) 
    {
      if (c >= 127)
      {
        throw bad_codepoint();
      }
    }
    constexpr codePoint_t(std::uint16_t u16) : value(u16)
    {
      if ((u16 <= 0xD7FF) && (u16 >= 0xE000))
      {
        throw bad_codepoint();
      }
    }

    constexpr codePoint_t(std::int16_t i16) : value(static_cast<std::uint16_t>(i16))
    {
      if ((i16 <= 0xD7FF) && (i16 >= 0xE000))
      {
        throw bad_codepoint();
      }
    }
    constexpr codePoint_t(std::int32_t i32) : value(i32) {}
    constexpr codePoint_t(utf32_t u32) : value(u32) {}

    constexpr codePoint_t(std::istream &inStrm, utf_e encoding)
    {
//      decodeUTF(inStrm, value, encoding);
    }

    constexpr ~codePoint_t() = default;

    codePoint_t &operator=(codePoint_t const &) = default;
    codePoint_t &operator=(codePoint_t &&) = default;

    constexpr operator utf32_t() const { return value; }

    constexpr bool isalpha() const noexcept { return islower() || isupper(); }
    constexpr bool isnumeric() const noexcept { return (value >= 0x30 && value <= 0x39); }
    constexpr bool isalphanumeric() const noexcept { return isalpha() || isnumeric(); }
    constexpr bool isHexDigit() const noexcept{ return isnumeric() || ( value >= 0x41 && value <= 0x46) || (value >= 0x61 && value <= 0x66); }
    constexpr bool islower() const noexcept { return (value >= 0x61 && value <= 0x7A); }
    constexpr bool isupper() const noexcept { return (value >= 0x41 && value <= 0x5A); }
    constexpr codePoint_t tolower() const noexcept { return (isupper() ? value + 0x20 : value); }


  private:
    value_type value;

    friend constexpr bool operator==(codePoint_t const &lhs, codePoint_t const &rhs) { return lhs.value == rhs.value; }
    friend constexpr bool operator==(codePoint_t const &lhs, char rhs) { return lhs.value == codePoint_t(rhs).value; }
    friend std::ostream &operator<<(std::ostream &os, codePoint_t const &cp)
    {
      os << std::to_string(cp);
      return os;
    }
  };

  static_assert(sizeof(codePoint_t) == 4, "Expected 4");

  constexpr codePoint_t U_0000(0x0000);   // null
  constexpr codePoint_t U_0009(0x0009);   // Tab
  constexpr codePoint_t U_000A(0x000A);   // LF
  constexpr codePoint_t U_000C(0x000C);   // FF
  constexpr codePoint_t U_0020(0x0020);   // Space
  constexpr codePoint_t U_0021(0x0021);   // '!'
  constexpr codePoint_t U_0022(0x0022);   // '"'
  constexpr codePoint_t U_0023(0x0023);   // '#'
  constexpr codePoint_t U_0026(0x0026);   // '&'
  constexpr codePoint_t U_0027(0x0027);   // '''
  constexpr codePoint_t U_002D(0x002D);   // '-'
  constexpr codePoint_t U_002F(0x002F);   // '/'
  constexpr codePoint_t U_003A(0x003A);   // ':'
  constexpr codePoint_t U_003B(0x003B);   // ';'
  constexpr codePoint_t U_003C(0x003C);   // '<'
  constexpr codePoint_t U_003D(0x003D);   // '='
  constexpr codePoint_t U_003E(0x003E);   // '>'
  constexpr codePoint_t U_003F(0x003F);   // '?'
  constexpr codePoint_t U_0058(0x0058);   // 'X'
  constexpr codePoint_t U_005D(0x005D);   // '['
  constexpr codePoint_t U_0060(0x0060);   // '`'
  constexpr codePoint_t U_0078(0x0078);   // 'x'
  constexpr codePoint_t U_FFFD(0xFFFD);   // Replacement Character
  constexpr codePoint_t U_EOF(0xFEFF);    // eof


}

#endif /* GCL_INCLUDE_PARSERS_CODEPOINT_HPP_ */
