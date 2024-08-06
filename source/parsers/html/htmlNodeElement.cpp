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
//**********************************************************************************************************************************/

#include "include/parsers/html/htmlNodeElement.h"

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
    std::set<CHTMLElement::categories_e> elementCategories;
  };

  static std::map<htmlElements_e, elementAttributes_t> const elementDetails;
//  {
//    { HTML_A, { .elementCategories{CHTMLElement::CAT_FLOWCONTROL}}},
//    { HTML_ABBR, {.elementCategories{CHTMLElement::CAT_FLOWCONTROL}}},
//    { HTML_AREA, {.isVoid = true}},
//    { HTML_BASE, {.isVoid = true, .elementCategories{CHTMLElement::CAT_METADATA}}},
//    { HTML_BR, {.isVoid = true}},
//    { HTML_COL, {.isVoid = true}},
//    { HTML_EMBED, {.isVoid = true}},
//    { HTML_HR, {.isVoid = true}},
//    { HTML_IMG, {.isVoid = true}},
//    { HTML_INPUT, {.isVoid = true}},
//    { HTML_LINK, {.isVoid = true, .elementCategories{CHTMLElement::CAT_METADATA}}},
//    { HTML_META, {.isVoid = true, .elementCategories{CHTMLElement::CAT_METADATA}}},
//    { HTML_SOURCE, {.isVoid = true}},
//    { HTML_TRACK, {.isVoid = true}},
//    { HTML_WBR, {.isVoid = true}},
//
//    { HTML_TEMPLATE, {.isTempate = true, .elementCategories{CHTMLElement::CAT_METADATA}}},
//    { HTML_SCRIPT, {.isTemplate =true, .elementCategories{CHTMLElement::CAT_METADATA}}},
//    { HTML_STYLE, {.isTemplate = true, .elementCategories{CHTMLElement::CAT_METADATA}}},
//    { HTML_TITLE, {.elementCategories{CHTMLElement::CAT_METADATA}}},
//
//    { HTML_LI, {.canOmitEnd = true }},
//    { HTML_DT, {.canOmitEnd = true }},
//    { HTML_DD, {.canOmitEnd = true }},
//    { HTML_RT, {.canOmitEnd = true }},
//    { HTML_RP, {.canOmitEnd = true }},
//    { HTML_TD, {.canOmitEnd = true }},
//  };

  static SCL::bimap<htmlElements_e, std::string> const elementText =
  {
    { HTML_AREA, "area"},

    { HTML_BASE, "base"},
    { HTML_BODY, "body"},
    { HTML_BR, "br" },

    { HTML_COL, "col" },
    { HTML_COLGROUP, "colgroup"},

    { HTML_DD, "dd"},
    { HTML_DT, "dt"},


    { HTML_EMBED, "embed"},

    { HTML_HR, "hr"},

    { HTML_IMG, "img"},
    { HTML_INPUT, "input"},

    { HTML_LI, "li"},
    { HTML_LINK, "link"},

    { HTML_META, "meta"},

    { HTML_RP, "rp"},
    { HTML_RT, "rt"},

    { HTML_SCRIPT, "script"},
    { HTML_SOURCE, "source"},
    { HTML_STYLE, "style"},

    { HTML_TABLE, "table"},
    { HTML_TBODY, "tbody"},
    { HTML_TD, "td"},
    { HTML_TEMPLATE, "template"},
    { HTML_TR, "tr"},
    { HTML_TRACK, "track"},

    { HTML_WBR, "wbr"},

  };

  bool operator==(std::string const &lhs, htmlElements_e rhs)
  {
    return (lhs == CHTMLElement::elementType2string(rhs));
  }

  bool operator!=(std::string const &lhs, htmlElements_e rhs)
  {
    return !(lhs == rhs);
  }

  CHTMLElement::CHTMLElement(CHTMLNodeBase *parent, std::string const &typeText) : CHTMLNodeBase(parent)
  {
std::cout << "Create Node: " << typeText << std::endl;
    if (elementText.contains_RHS(typeText))
    {
      elementType = elementText.LHS(typeText);
    }
    else
    {
      elementType = typeText;
    }
  }

  void CHTMLElement::insert(CHTMLAttribute &&attr)
  {
    attributes.emplace(attr.attribute(), std::move(attr));
  }

  CHTMLElement::child_pointer CHTMLElement::insert(std::unique_ptr<CHTMLNodeBase> &&ins)
  {
    elementChildren.push_back(std::move(ins));
  }


  void CHTMLElement::insert(std::string const &attr, std::string const &val)
  {
    /* Adding an attribute. */
    attributes.emplace(std::piecewise_construct,
                       std::forward_as_tuple(attr),
                       std::forward_as_tuple(attr, val));

  }

  CHTMLElement::child_pointer CHTMLElement::insert(CHTMLNodeBase *p, std::string const &etype)
  {
    elementChildren.push_back(std::make_unique<CHTMLElement>(p, etype));
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

  std::string CHTMLElement::elementType2string(htmlElements_e type) noexcept
  {
    if (elementText.contains_LHS(type))
    {
      return elementText.RHS(type);
    }
    else
    {
      return std::string();
    }
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

  std::string CHTMLElement::type() const noexcept
  {
    if (std::holds_alternative<htmlElements_e>(elementType))
    {
      return elementType2string(std::get<htmlElements_e>(elementType));
    }
    else
    {
      return std::get<std::string>(elementType);
    }
  }

}
