//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// SUBSYSTEM:           Parsers::HTML Parser
// FILE:                htmlElements.cpp
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
// OVERVIEW:            Class that represents the HTML elements.
//
// CLASSES INCLUDED:
//
// HISTORY:             2024-06-18 GGB - File Created
//
//**********************************************************************************************************************************

#include "include/parsers/html/htmlElement.h"

// Miscellaneous library header files.
#include <SCL>

namespace GCL::parsers::html
{

  struct elementAttributes_t
  {
    bool isVoid = false;
    bool isTemplate = false;
    bool isRawText = false;
    bool isEscapeableRawText = false;
    bool isForeign = false;
    bool isNormal = false;
    bool canOmitEnd = false;

  };

  static std::map<htmlElements_e, elementAttributes_t> const elementDetails =
  {
    { HTML_AREA, {.isVoid = true}},
    { HTML_BASE, {.isVoid = true}},
    { HTML_BR, {.isVoid = true}},
    { HTML_COL, {.isVoid = true}},
    { HTML_EMBED, {.isVoid = true}},
    { HTML_HR, {.isVoid = true}},
    { HTML_IMG, {.isVoid = true}},
    { HTML_INPUT, {.isVoid = true}},
    { HTML_LINK, {.isVoid = true}},
    { HTML_META, {.isVoid = true}},
    { HTML_SOURCE, {.isVoid = true}},
    { HTML_TRACK, {.isVoid = true}},
    { HTML_WBR, {.isVoid = true}},

    { HTML_TEMPLATE, {false, true}},
    { HTML_SCRIPT, {false, true}},
    { HTML_STYLE, {false, true}},

    { HTML_LI, {.canOmitEnd = true }},
    { HTML_DT, {.canOmitEnd = true }},
    { HTML_DD, {.canOmitEnd = true }},
    { HTML_RT, {.canOmitEnd = true }},
    { HTML_RP, {.canOmitEnd = true }},
    { HTML_TD, {.canOmitEnd = true }},
  };

  static SCL::bimap<htmlElements_e, std::string> const elementText =
  {
    { HTML_AREA, "area"},
    { HTML_BASE, "base"},
    { HTML_BR, "br" },
    { HTML_COL, "col" },
    { HTML_EMBED, "embed"},
    { HTML_HR, "hr"},
    { HTML_IMG, "img"},
    { HTML_INPUT, "input"},
    { HTML_LINK, "link"},
    { HTML_META, "meta"},
    { HTML_SOURCE, "source"},
    { HTML_TRACK, "track"},
    { HTML_WBR, "wbr"},

    { HTML_TEMPLATE, "template"},
    { HTML_SCRIPT, "script"},
    { HTML_STYLE, "style"},

    { HTML_LI, "li"},
    { HTML_DT, "dt"},
    { HTML_DD, "dd"},
    { HTML_RT, "rt"},
    { HTML_RP, "rp"},
    { HTML_TD, "td"},

  };

  void CHTMLElement::insert(CHTMLAttribute &&)
  {

  }

  CHTMLElement::child_reference CHTMLElement::insert(CHTMLElement &&)
  {

  }



  void CHTMLElement::insert(std::string const &attr, std::string const &val)
  {

  }

  CHTMLElement::child_reference CHTMLElement::insert(std::string const &)
  {

  }



  bool CHTMLElement::isVoid(std::string const &element) noexcept
  {
    if (elementText.contains_RHS(element))
    {
      return isVoid(elementText.LHS(element));
    }
    else
    {
      return false;
    }
  }

  bool CHTMLElement::isVoid(htmlElements_e element) noexcept
  {
    return elementDetails.at(element).isVoid;
  }

  htmlElements_e CHTMLElement::string2elementType(std::string const &str) noexcept
  {
    if (elementText.contains_RHS(str))
    {
      return elementText.LHS(str);
    }
    else
    {
      return HTML_UNKNOWN;
    }
  }

  void CHTMLElement::value(std::string const &v) noexcept
  {
    elementValue = v;
  }

}
