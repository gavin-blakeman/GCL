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

/* The challenge with representing code points, is that the utf8 and utf16 code points are variable
 * byte code points. In this case what is the best value to return for the coversion functions?
 * Should UTF8 and UTF16 then be represented as classes in their own right, or as arrays, or vectors?
 */

namespace GCL::parsers
{
  union utf_t
  {
    std::uint8_t u8[4];
    std::uint16_t u16[2];
    std::uint32_t u32;
  };

  class codePoint_t
  {
  public:
    using value_type = std::uint32_t;
    using utf32_t = std::uint32_t;

    constexpr codePoint_t() = default;
    constexpr codePoint_t(codePoint_t const &) = default;
    constexpr codePoint_t(codePoint_t &&) = default;
    constexpr codePoint_t(utf32_t u32) : value(u32) {}

    constexpr ~codePoint_t() = default;

    codePoint_t &operator=(codePoint_t const &) = default;
    codePoint_t &operator=(codePoint_t &&) = default;

    constexpr operator utf32_t() const { return value; }

  private:
    value_type value;
  };

  constexpr codePoint_t U_0000(0x0000);  // null
  constexpr codePoint_t U_0009(0x0009);  // Tab
  constexpr codePoint_t U_000A(0x000A);  // LF
  constexpr codePoint_t U_000C(0x000C);  // FF
  constexpr codePoint_t U_0020(0x0020);  // Space
  constexpr codePoint_t U_0021(0x0021);  // '!'
  constexpr codePoint_t U_002D(0x002D);  // '-'
  constexpr codePoint_t U_002F(0x002F);  // '/'
  constexpr codePoint_t U_003C(0x003C);  // '<'
  constexpr codePoint_t U_003E(0x003E);  // '>'
  constexpr codePoint_t U_003F(0x003F);  // '?'
  constexpr codePoint_t U_EOF(0xFEFF);   // eof
}

#endif /* GCL_INCLUDE_PARSERS_CODEPOINT_HPP_ */
