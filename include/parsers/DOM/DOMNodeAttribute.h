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

#ifndef PARSERS_HTML_HTMLATTRIBUTES_H
#define PARSERS_HTML_HTMLATTRIBUTES_H

// Standard C++ library header files.
#include <iostream>
#include <optional>
#include <string>
#include <variant>

// DOM header files
#include "include/parsers/DOM/DOMNodeBase.h"

namespace GCL::parsers::DOM
{

  class CDOMElement;

  class CDOMAttribute : public CDOMNodeBase
  {
  public:
    CDOMAttribute(CDOMNodeBase *oe, string_type const &n, string_type const &v) : CDOMNodeBase(oe), ownerElement(oe), localName_(n), value_(v) {}

    virtual nodeType_e nodeType() const noexcept override { return ATTRIBUTE_NODE; }

    string_type prefix() const noexcept { return (prefix_ ? *prefix_: string_type()); }
    string_type name() const noexcept { return (!prefix_ ? localName_ : *prefix_ + U_003A + localName_); }
    string_type const &localName() const noexcept { return localName_; }
    string_type const &value() const noexcept { return value_; }

  private:
    CDOMAttribute() = delete;

    std::optional<string_type> namespaceURI;
    std::optional<string_type> prefix_;
    string_type localName_;
    string_type value_;
    CDOMNodeBase *ownerElement = nullptr;

    friend std::ostream &operator<<(std::ostream &os, CDOMAttribute const &attr)
    {
      //os << "(" << attr.name() << ", " << attr.value() << ")" << std::endl;
      return os;
    }

  };
}

#endif // PARSERS_HTML_HTMLATTRIBUTES_H
