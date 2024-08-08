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

namespace GCL::parsers
{
  struct eof {};

  class CCodePoint
  {
  public:
    using utf8_t = std::uint8_t;
    using utf16_t = std::uint16_t;
    using utf32_t = std::uint32_t;
    using codePoint_t = std::variant<std::monostate, utf8_t, utf16_t, utf32_t, eof>;

    CCodePoint() = default;
    CCodePoint(std::uint32_t);

  private:
    codePoint_t value;
  };
}

#endif /* GCL_INCLUDE_PARSERS_CODEPOINT_HPP_ */
