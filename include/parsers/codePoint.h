/*
 * codePoint.hpp
 *
 *  Created on: 8 Aug 2024
 *      Author: gavin
 */

#ifndef GCL_INCLUDE_PARSERS_CODEPOINT_HPP_
#define GCL_INCLUDE_PARSERS_CODEPOINT_HPP_

// Standard C++ herader files
#include <cstdint>
#include <variant>

// Miscellaneous libraries
#include <GCL>
#include <fmt/format.h>

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
 *       3. There are aspects of HTML that are limited to ascii. This simplifies the tolower() function as
 *          it only needs to operate on ascii characters.
 *       4. If full localisation is required use std::u32string and char32_t.
 */

namespace GCL::parsers
{
  struct utf_t
  {
    union
    {
      std::uint8_t u8[4];
      std::uint16_t u16[2];
      std::uint32_t u32;
    };
    utf_e type;
  };

  class codePoint_t
  {
  public:
    using value_type = std::uint32_t;
    using utf32_t = std::uint32_t;

    constexpr codePoint_t() = default;
    constexpr codePoint_t(codePoint_t const &) = default;
    constexpr codePoint_t(codePoint_t &&) = default;
    constexpr codePoint_t(std::uint8_t u8)
    {
      if (u8 <= 127)
      {
        value = u8;
      }
      else
      {
        throw std::runtime_error("invalid utf8 single byte");
      }
    }
    constexpr codePoint_t(char c) : value(c) {}
    constexpr codePoint_t(std::uint16_t u16) : value(u16) {}
    constexpr codePoint_t(std::int16_t i16) : value(i16) {}
    constexpr codePoint_t(std::int32_t i32) : value(i32) {}
    constexpr codePoint_t(utf32_t u32) : value(u32) {}
    constexpr codePoint_t(utf_t utf)
    {
      switch (utf.type)
      {
        case UTF_8:
        {
          value = utf.u32;
          break;
        };
        default:
        {
          value = 0;
          break;
        }
      };
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
