//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// SUBSYSTEM:           Parsers::HTML Parser
// FILE:                htmlAttributes.cpp
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

#include "include/parsers/html/htmlAttributes.h"

// Miscellaneous library header files
#include <fmt/format.h>
#include <SCL>

namespace GCL::parsers::html
{
  static SCL::bimap<htmlAttributes_e, std::string> const attributeStrings =
  {
//      ATTR_ACCESSKEY,
//      ATTR_AUTOCAPITALISE,
//      ATTR_AUTOFOCUS,
//      ATTR_CONTENTEDITABLE,
      { ATTR_DIR, "dir" },
//      ATTR_DRAGGABLE,
//      ATTR_ENTERKEYHINT,
//      ATTR_HIDDEN,
//      ATTR_INERT,
//      ATTR_INPUTMODE,
//      ATTR_IS,
//      ATTR_ITEMID,
//      ATTR_ITEMPROP,
//      ATTR_ITEMREF,
//      ATTR_ITEMSCOPE,
//      ATTR_ITEMTYPE,
//      ATTR_LANG,
//      ATTR_NONCE,
//      ATTR_POPOVER,
//      ATTR_SPELLCHECK,
//      ATTR_STYLE,
//      ATTR_TABINDEX,
//      ATTR_TITLE,
//      ATTR_TRANSLATE,
//      ATTR_WRITINGSUGGESTIONS,
//
      { ATTR_CLASS, "class" },
      { ATTR_ID, "id" },
//      ATTR_SLOT,
  };


  CHTMLAttribute::CHTMLAttribute(std::string const &attr, std::string const &val) : attributeValue(val)
  {
    if (attributeStrings.contains_RHS(attr))
    {
      attributeType = attributeStrings.LHS(attr);
    }
    else
    {
      attributeType = attr;
    }
  }

  CHTMLAttribute::CHTMLAttribute(htmlAttributes_e attr, std::string const &val) : attributeType(attr), attributeValue(val) {}
  CHTMLAttribute::CHTMLAttribute(htmlAttributes_e attr, std::string &&val) : attributeType(attr), attributeValue(std::move(val)) {}

  std::string CHTMLAttribute::attribute() const noexcept
  {
    if (std::holds_alternative<htmlAttributes_e>(attributeType))
    {
      return attribute2string(std::get<htmlAttributes_e>(attributeType));
    }
    else if (std::holds_alternative<std::string>(attributeType))
    {
      return std::get<std::string>(attributeType);
    }
    else
    {
      return std::string();
    }
  }

  std::string CHTMLAttribute::attribute2string(htmlAttributes_e attr) noexcept
  {
    if (attributeStrings.contains_LHS(attr))
    {
      return attributeStrings.RHS(attr);
    }
    else
    {
      return std::string();
    }
  }

  htmlAttributes_e CHTMLAttribute::string2attribute(std::string const &str) noexcept
  {
    if (attributeStrings.contains_RHS(str))
    {
      return attributeStrings.LHS(str);
    }
    else
    {
      return ATTR_UNKNOWN;
    }
  }

  bool operator==(CHTMLAttribute const &lhs, CHTMLAttribute const &rhs) noexcept
  {
    if (std::holds_alternative<htmlAttributes_e>(lhs.attributeType) && std::holds_alternative<htmlAttributes_e>(rhs.attributeType))
    {
      return std::get<htmlAttributes_e>(lhs.attributeType) == std::get<htmlAttributes_e>(rhs.attributeType);
    }
    else
    {
      std::string szLHS, szRHS;

      if (std::holds_alternative<htmlAttributes_e>(lhs.attributeType))
      {
        szLHS = CHTMLAttribute::attribute2string(std::get<htmlAttributes_e>(lhs.attributeType));
      }
      else
      {
        szLHS = std::get<std::string>(lhs.attributeType);
      }

      if (std::holds_alternative<htmlAttributes_e>(rhs.attributeType))
      {
        szRHS = CHTMLAttribute::attribute2string(std::get<htmlAttributes_e>(rhs.attributeType));
      }
      else
      {
        szRHS = std::get<std::string>(rhs.attributeType);
      }

      return szLHS == szRHS;
    }
  }
  bool operator==(CHTMLAttribute const &lhs, std::string const &rhs) noexcept
  {
    std::string szLHS;

    if (std::holds_alternative<htmlAttributes_e>(lhs.attributeType))
    {
      szLHS = CHTMLAttribute::attribute2string(std::get<htmlAttributes_e>(lhs.attributeType));
    }
    else
    {
      szLHS = std::get<std::string>(lhs.attributeType);
    }

    return szLHS == rhs;
  }

  bool operator==(CHTMLAttribute const &lhs, htmlAttributes_e rhs) noexcept
  {
    if (std::holds_alternative<htmlAttributes_e>(lhs.attributeType))
    {
      return std::get<htmlAttributes_e>(lhs.attributeType) == rhs;
    }
    else
    {
      std::string szLHS, szRHS;

      szLHS = CHTMLAttribute::attribute2string(std::get<htmlAttributes_e>(lhs.attributeType));
      szRHS = CHTMLAttribute::attribute2string(rhs);

      return szLHS == szRHS;
    }
  }

  std::ostream &operator<<(std::ostream &os, CHTMLAttribute const &attr)
  {
    if (std::holds_alternative<std::string>(attr.attributeType))
    {
      os << fmt::format("Attr: {}: Val: {}", std::get<std::string>(attr.attributeType), attr.attributeValue);
    }
    else if (std::holds_alternative<htmlAttributes_e>(attr.attributeType))
    {
      os << fmt::format("Attr: {}: Val: {}", attributeStrings.RHS(std::get<htmlAttributes_e>(attr.attributeType)), attr.attributeValue);
    }
    else
    {
      os << fmt::format("Attr: Unknown: Val: {}", attr.attributeValue);
    }
    return os;
  }

}
