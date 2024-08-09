//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// SUBSYSTEM:           Parsers
// FILE:                htmlToken.h
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
// OVERVIEW:            Class provides a generic token used for lexing files.
//
// CLASSES INCLUDED:
//
// HISTORY:             2024-06-18 GGB - File Created
//
//**********************************************************************************************************************************/

#ifndef PARSERS_HTMLTOKEN_H
#define PARSERS_HTMLTOKEN_H

// Standard C++ library header files
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

// Miscellaneous library header files.
#include <SCL>

// Parser header files
#include "include/parsers/codePoint.h"

/* Note:
 * Tokens can store multiple code points. At the time of writing, the thought is to keep the code points and pass the code
 * points to the tree-builder. The DOM may alse benifit from storing the code points. (It is also possible for the DOM to
 * store UTF8, rather than the code point.
 * An HTML string is then just a std::vector<codePoint_t>.
 * Musings: It may be possible to store 32bit values only and have codePoint_t be a static-class that operates on the 32bit
 * values. This will make string conversion functions easier to use.
 */

namespace GCL::parsers::html
{
  class CHTMLToken
  {
    public:
      using value_type = codePoint_t;
      using string_type = std::basic_string<value_type>;
      enum token_type { TT_START_TAG, TT_END_TAG, TT_CHARACTER, TT_DOCTYPE, TT_EOF, TT_COMMENT };
      using tokenStringMap_t = SCL::bimap<token_type, std::string>;

      CHTMLToken();
      CHTMLToken(CHTMLToken const &t) : tokenStringMap(t.tokenStringMap), tokenType(t.tokenType), tokenValue(t.tokenValue) {}

      CHTMLToken &operator=(CHTMLToken const &other);
      CHTMLToken &operator+=(string_type const &rhs) { tokenValue.append(rhs); return *this; }
      CHTMLToken &operator+=(value_type const &rhs) { tokenValue.push_back(rhs); return *this; }

      operator string_type() const;

      token_type const &type() const noexcept { return tokenType; }
      void type(token_type tt) noexcept { tokenType = tt; }

      string_type const &value() const noexcept { return tokenValue; }
      string_type &value() noexcept { return tokenValue; }
      void value(string_type const &v) { tokenValue = v; }
      void value(value_type const &v) { tokenValue.clear(); tokenValue.push_back(v); }

    private:
      tokenStringMap_t const &tokenStringMap;
      token_type tokenType;
      string_type tokenValue;

  friend bool operator==(CHTMLToken const &lhs, CHTMLToken const &rhs) { return ((lhs.tokenType == rhs.tokenType) && (lhs.tokenValue == rhs.tokenValue)); }
  friend bool operator==(CHTMLToken const &lhs, token_type rhs) { return (lhs.tokenType == rhs); }
  friend std::ostream &operator<<(std::ostream &os, CHTMLToken const &);
  };

} // namespace

#endif // PARSERS_HTMLTOKEN_H
