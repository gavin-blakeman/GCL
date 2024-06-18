//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// SUBSYSTEM:           Parsers::HTML Parser
// FILE:                htmlElements.h
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

#ifndef PARSERS_HTML_HTMLELEMENT_H
#define PARSERS_HTML_HTMLELEMENT_H

// Standard C++ library header files
#include <functional>
#include <map>

// GCL header files
#include "include/parsers/html/htmlAttributes.h"

/* An element corresponds to a tag.
 * An elements can have a list of attributes. The attributes apply to this element (and possible elements below it in the
 * structure.)
 * An element can also have child elements.
 */

namespace GCL::parsers::html
{
  enum htmlElements_e
  {
    HTML_UNKNOWN,
    HTML_HTML,
    HTML_HEAD,
    HTML_TITLE,

    HTML_AREA,
    HTML_BASE,
    HTML_BR,
    HTML_COL,
    HTML_EMBED,
    HTML_HR,
    HTML_INPUT,
    HTML_IMG,
    HTML_LINK,
    HTML_META,
    HTML_SOURCE,
    HTML_TRACK,
    HTML_WBR,

    HTML_TEMPLATE,

    HTML_SCRIPT,
    HTML_STYLE,

    HTML_TEXTAREA,

    HTML_LI,
    HTML_DT,
    HTML_DD,
    HTML_RT,
    HTML_RP,
    HTML_TD

  };

  class CHTMLElement
  {
    public:
      using attribute_type = CHTMLAttribute;
      using attribute_collection = std::map<std::string, attribute_type>;
      using attribute_iterator = attribute_collection::iterator;
      using attribute_const_iterator = attribute_collection::const_iterator;
      using attribute_reference = attribute_type &;
      using attribute_const_reference = attribute_type const &;
      using attribute_pointer = attribute_type *;
      using child_type = CHTMLElement;
      using child_collection = std::map<htmlElements_e, CHTMLElement>;
      using child_ref = std::reference_wrapper<child_type>;
      using child_vector = std::vector<child_ref>;
      using child_reference = child_type &;
      using child_const_reference = child_type const &;
      using child_iterator = child_collection::iterator;

      ~CHTMLElement() = default;

      htmlElements_e elementType() const noexcept { return type; }
      std::string value() const noexcept { return elementValue; }
      void value(std::string const &) noexcept;

      void insert(CHTMLAttribute &&);
      void insert(std::string const &attr, std::string const &val);

      child_reference insert(CHTMLElement &&);
      child_reference insert(std::string const &);

      attribute_iterator attributes_begin() { return attributes.begin(); }
      attribute_const_iterator attributes_begin() const { return attributes.cbegin(); }
      attribute_const_iterator attributes_cbegin() const { return attributes.cbegin(); }

      attribute_iterator attributes_end() { return attributes.end(); }
      attribute_const_iterator attributes_end() const { return attributes.cend(); }
      attribute_const_iterator attributes_cend() const { return attributes.cend(); }

      attribute_const_reference operator[](std::string const &) const;

      /*! @brief      Determines if the specified element is a void element. (open/close at the same time.)
       *  @param[in]  element: The element text to check.
       *  @returns    true if the element is a void element.
       */
      static bool isVoid(std::string const &element) noexcept;

      /*! @brief      Determines if the specified element is a void element. (open/close at the same time.)
       *  @param[in]  element: The element text to check.
       *  @returns    true if the element is a void element.
       */
      static bool isVoid(htmlElements_e element) noexcept;

      /*! @brief      Returns the string associated with an element type.
       *  @param[in]  elementName: The string name of the element.
       *  @returns    The element type.
       *  @throws     noexcept
       */
      static htmlElements_e string2elementType(std::string const &) noexcept;

    private:
      CHTMLElement() = delete;

      htmlElements_e type;
      std::string elementValue;
      attribute_collection attributes;
      child_collection children;
  };

} // namespace

#endif // PARSERS_HTML_HTMLELEMENT_H
