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
#include <string>

namespace GCL::parsers::html
{
  enum htmlAttributes_e
  {
    ATTR_ACCESSKEY,
    ATTR_AUTOCAPITALISE,
    ATTR_AUTOFOCUS,
    ATTR_CONTENTEDITABLE,
    ATTR_DIR,
    ATTR_DRAGGABLE,
    ATTR_ENTERKEYHINT,
    ATTR_HIDDEN,
    ATTR_INERT,
    ATTR_INPUTMODE,
    ATTR_IS,
    ATTR_ITEMID,
    ATTR_ITEMPROP,
    ATTR_ITEMREF,
    ATTR_ITEMSCOPE,
    ATTR_ITEMTYPE,
    ATTR_LANG,
    ATTR_NONCE,
    ATTR_POPOVER,
    ATTR_SPELLCHECK,
    ATTR_STYLE,
    ATTR_TABINDEX,
    ATTR_TITLE,
    ATTR_TRANSLATE,
    ATTR_WRITINGSUGGESTIONS,

    ATTR_CLASS,
    ATTR_ID,
    ATTR_SLOT,
  };

  class CHTMLAttribute
  {
    public:
      CHTMLAttribute(htmlAttributes_e, std::string const &);
      CHTMLAttribute(htmlAttributes_e, std::string &&);

      htmlAttributes_e attribute() const noexcept { return attr; }
      std::string const &value() const noexcept { return val; }

    private:
      CHTMLAttribute() = delete;

      htmlAttributes_e attr;
      std::string val;
  };
}

#endif // PARSERS_HTML_HTMLATTRIBUTES_H
