//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// SUBSYSTEM:           Parsers::HTML Parser
// FILE:                htmlAttributes.h
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
// OVERVIEW:            Class to store attributes.
//
// CLASSES INCLUDED:
//
// HISTORY:             2024-06-18 GGB - File Created
//
//**********************************************************************************************************************************

#ifndef PARSERS_HTML_HTML_RAWATTRIBUTE_H
#define PARSERS_HTML_HTML_RAWATTRIBUTE_H

// Standard C++ library header files.
#include <iostream>
#include <string>
#include <variant>

// Parsers
#include "include/parsers/codePoint.h"

namespace GCL::parsers::html
{
  class CHTMLRawAttribute
  {
  public:
    using string_type = std::u32string;
    using char_type = string_type::value_type;

    CHTMLRawAttribute() = default;

    string_type const &name() const noexcept { return attributeName; }
    string_type &name () noexcept { return attributeName; }
    void concatName(char_type c) { attributeName.push_back(c); }

    string_type const &value() const noexcept { return attributeValue; }
    string_type &value() noexcept { return attributeValue; }
    void concatValue(char_type c) { attributeValue.push_back(c); }

  private:
    string_type attributeName;
    string_type attributeValue;

    friend bool operator==(CHTMLRawAttribute const &lhs, CHTMLRawAttribute const &rhs) noexcept { return (lhs.attributeName == rhs.attributeName); }
    friend bool operator<(CHTMLRawAttribute const &lhs, CHTMLRawAttribute const &rhs) noexcept { return (lhs.attributeName < rhs.attributeName); }
    friend std::ostream &operator<<(std::ostream &os, CHTMLRawAttribute const &)
    {
      return os;
    }
  };

  class CHTMLRawAttributeCollection final
  {
  public:
    CHTMLRawAttributeCollection() = default;
    ~CHTMLRawAttributeCollection() = default;

  private:
    CHTMLRawAttributeCollection(CHTMLRawAttributeCollection const &) = delete;
    CHTMLRawAttributeCollection(CHTMLRawAttributeCollection &&) = delete;
    CHTMLRawAttributeCollection &operator=(CHTMLRawAttributeCollection const &) = delete;
    CHTMLRawAttributeCollection &operator=(CHTMLRawAttributeCollection &&) = delete;
  };

}

#endif // PARSERS_HTML_HTML_RAWATTRIBUTE_H
