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
//                      Copyright 2024-2026 Gavin Blakeman.
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
#include <list>
#include <optional>
#include <string>
#include <variant>
#include <vector>

// Parser header files
#include "include/parsers/html/htmlRawAttribute.hpp"

/* Note:
 * 1. Tokens can store multiple code points. At the time of writing, the thought is to keep the code points and pass the code
 *    points to the tree-builder. The DOM may also benefit from storing the code points. (It is also possible for the DOM to
 *    store UTF8, rather than the code point.
 *    An HTML string is then just a std::string<codePoint_t>.
 * 2. There is a single token class. The tokenType determines the type of the token and the data stored. The functions are
 *    provided to support data changes to the relevant tokenType data. If the current tokenType is not the relevant token,
 *    a CODE_ERROR() will be thrown.
 */

namespace GCL::parsers::html
{
  class CHTMLToken
  {
  public:
    using char_type = codePoint_t;
    using string_type = std::basic_string<char_type>;
    enum token_type { TT_NONE, TT_TAG_START, TT_TAG_END, TT_CHARACTER, TT_DOCTYPE, TT_EOF, TT_COMMENT };
    using attribute_type = CHTMLRawAttribute;
    using attribute_collection = std::list<attribute_type>;

    CHTMLToken() = default;
    CHTMLToken(CHTMLToken const &t) = default;
    CHTMLToken(CHTMLToken &&) = default;
    CHTMLToken(token_type t);
    CHTMLToken(token_type t, char_type const &c);

    ~CHTMLToken() = default;

    CHTMLToken &operator=(CHTMLToken const &other) = default;
    CHTMLToken &operator=(CHTMLToken &&) = default;

    // Shared dependent on tokenType

    void appendName(char_type const &v);
    string_type const &name() const;
    void appendData(char_type const &v);
    void appendData(string_type const &s);

    // DocType Access
    void forceQuirks(bool b);
    bool forceQuirks() const;
    void appendPublicIdentifier(char_type const &v);
    void appendSystemIdentifier(char_type const &v);
    void setPublicIdentifierEmpty();
    void setSystemIdentifierEmpty();
    bool hasPublicIdentifier() const;
    bool hasSystemIdentifier() const;
    string_type const &publicIdentifier() const noexcept;
    string_type const &systemIdentifier() const noexcept;

    // Start and end tags
    void selfClosing(bool b);
    bool isSelfClosing() const;

    token_type const &type() const noexcept { return tokenType; }
    void type(token_type tt);

    bool attrExists(char_type const &v) const noexcept {  }
    void attrStart() { ; }
    void attrConcatName(char_type const &) {};
    void attrConcatValue(char_type const &) {};

  private:
    struct tokenContentDocType_t
    {
      string_type name;
      std::optional<string_type> publicIdentifier;
      std::optional<string_type> systemIdentifier;
      bool forceQuirksFlag = false;

    };

    struct tokenTag_t
    {
      string_type name;
      bool selfClosingFlag = false;
      attribute_collection attrCollection;

      attribute_type *currentAttr = nullptr;
    };
    struct tokenComment_t
    {
      string_type data;
    };
    struct tokenCharacter_t
    {
      char_type data;
    };

    using tokenVariant_t = std::variant<std::monostate, tokenContentDocType_t, tokenTag_t, tokenComment_t, tokenCharacter_t>;

    token_type tokenType = TT_NONE;
    mutable tokenVariant_t tokenValue;

    friend bool operator==(CHTMLToken const &lhs, CHTMLToken const &rhs) { return (lhs.tokenType == rhs.tokenType); }
    friend bool operator==(CHTMLToken const &lhs, token_type rhs) { return (lhs.tokenType == rhs); }

    friend std::ostream &operator<<(std::ostream &os, CHTMLToken const &);
  };

} // namespace

#endif // PARSERS_HTMLTOKEN_H
