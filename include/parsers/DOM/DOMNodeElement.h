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
//**********************************************************************************************************************************/

#ifndef PARSERS_HTML_HTMLELEMENT_H
#define PARSERS_HTML_HTMLELEMENT_H

// Standard C++ library header files
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <variant>

// GCL header files
#include "include/parsers/DOM/DOMNodeBase.h"
#include "include/parsers/DOM/DOMNodeAttribute.h"
#include "include/parsers/DOM/htmlCollection.hpp"

/* An element corresponds to a tag.
 * An elements can have a list of attributes. The attributes apply to this element (and possible elements below it in the
 * structure.)
 * An element can also have child elements.
 */

namespace GCL::parsers::DOM
{
  class CDOMElement : public CDOMNodeBase
  {
  public:
    using attribute_type = CDOMAttribute;
    using attribute_collection = std::map<std::string, attribute_type>;
    using attribute_iterator = attribute_collection::iterator;
    using attribute_const_iterator = attribute_collection::const_iterator;
    using attribute_reference = attribute_type &;
    using attribute_const_reference = attribute_type const &;
    using attribute_pointer = attribute_type *;
    using attribute_const_pointer = attribute_type const *;

    CDOMElement(CDOMNodeBase *parent, string_type const &);
    ~CDOMElement() = default;

    virtual nodeType_e nodeType() const noexcept override { return ELEMENT_NODE; };

    void setAttribute(string_type const &, string_type const &);
    void setAttributeNS(string_type const &, string_type const &, string_type const &);
    void removeAttribute(string_type const &);
    void removeAttributeNS(string_type const &, string_type const &);
    bool toggleAttribute(string_type const &, bool force = false);
    bool hasAttribute(string_type const &) const noexcept;
    bool hasAttributeNS(string_type const &, string_type const &) const noexcept;

    attribute_const_pointer getAttributeNode(string_type const &) const noexcept;
    attribute_const_pointer getAttributeNodeNS(string_type const &, std::string const &) const noexcept;
    attribute_const_pointer setAttributeNode(attribute_const_pointer);
    attribute_const_pointer setAttributeNodeNS(attribute_const_pointer);

    CHTMLCollection getElementsByTagName(string_type const &) const;
    CHTMLCollection getElementsByTagNameNS(string_type const &, string_type const &) const;
    CHTMLCollection getElementsByClassName(string_type const &) const;
//
//    attribute_iterator attributes_begin() { return attributes.begin(); }
//    attribute_const_iterator attributes_begin() const { return attributes.cbegin(); }
//    attribute_const_iterator attributes_cbegin() const { return attributes.cbegin(); }
//
//    attribute_iterator attributes_end() { return attributes.end(); }
//    attribute_const_iterator attributes_end() const { return attributes.cend(); }
//    attribute_const_iterator attributes_cend() const { return attributes.cend(); }
//
//    attribute_const_reference operator[](std::string const &) const;


  private:
    CDOMElement() = delete;

    string_type namespaceURI;
    string_type prefix;
    string_type localName;
    string_type tagName;
    string_type id;
    string_type className;
    string_type slot;

    attribute_collection attributes;
  };

} // namespace

#endif // PARSERS_HTML_HTMLELEMENT_H
