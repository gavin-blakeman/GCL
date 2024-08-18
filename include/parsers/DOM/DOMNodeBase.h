//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// SUBSYSTEM:           Parsers::HTML Parser
// FILE:                htmlNodeBase.h
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
// HISTORY:             2024-08-05 GGB - File Created
//
//**********************************************************************************************************************************/


#ifndef PARSERS_HTML_HTMLNODEBASE_H
#define PARSERS_HTML_HTMLNODEBASE_H

// Standard C++ library header files
#include <list>
#include <memory>
#include <string>

// Parsers library
#include "include/parsers/codePoint.h"
#include "include/parsers/DOM/DOMNodeList.hpp"

namespace GCL::parsers::DOM
{
  class CDOMDocument;

  class CDOMNodeBase
  {
  public:
    using char_type = GCL::parsers::codePoint_t;
    using string_type = std::basic_string<char_type>;
    using child_type = CDOMNodeBase;
    using child_collection = CNodeList;
    using child_pointer = child_type *;
    using const_child_pointer = child_type const *;
    using child_reference = child_type &;
    using child_iterator = CNodeList::iterator;
    using const_child_iterator = CNodeList::const_iterator;
    enum nodeType_e
    {
      ELEMENT_NODE =1, ATTRIBUTE_NODE = 2, TEXT_NODE = 3, CDATA_SECTION_NODE = 4, ENTITY_REFERENCE_NODE = 5,
      ENTITY_NODE = 6, PROCESSING_INSTRUCTION_NODE = 7, COMMENT_NODE = 8, DOCUMENT_NODE = 9, DOCUMENT_TYPE_NODE = 10,
      DOCUMENT_FRAGMENT_NODE = 11, NOTATION_NODE = 12,
    };

    CDOMNodeBase(CDOMNodeBase *parent) : parentNode_(parent) {}
    virtual ~CDOMNodeBase() = default;


    //CDOMNodeBase *getRootNode(GetRootNodeOptions = {});
    CDOMNodeBase const *parentNode() const { return parentNode_; }
    CDOMNodeBase const *parentElement() const {}

    bool hasChildNodes() const noexcept { return !childCollection.empty(); }
    const_child_iterator firstChild() const { return childCollection.begin(); }
    const_child_iterator lastChild() const noexcept { return childCollection.end(); }

    void normalize();

    CDOMNodeBase cloneNode(bool = false);

    bool isEqualNode(CDOMNodeBase const &);
    bool isSameNode(CDOMNodeBase const &);

    /*! @brief       Returns the type of the node.
     *  @returns     The type of the node.
     */
    virtual nodeType_e nodeType() const noexcept = 0;

    bool contains(CDOMNodeBase const &);

    string_type lookupPrefix(string_type nameSpace);
    string_type lookupNamespaceURI(string_type prefix);
    bool isDefaultNamespace(string_type nameSpace);

    CDOMNodeBase *insertBefore(CDOMNodeBase const &, std::unique_ptr<CDOMNodeBase> child);
    child_pointer appendChild(std::unique_ptr<CDOMNodeBase>);
    CDOMNodeBase *replaceChild(CDOMNodeBase const &, std::unique_ptr<CDOMNodeBase> child);
    CDOMNodeBase *removeChild(CDOMNodeBase const &);

  private:
    CDOMNodeBase() = delete;
    CDOMNodeBase(CDOMNodeBase const &) = delete;
    CDOMNodeBase(CDOMNodeBase &&) = delete;
    CDOMNodeBase &operator=(CDOMNodeBase const &) = delete;
    CDOMNodeBase &operator=(CDOMNodeBase &&) = delete;

    string_type nodeName;
    string_type baseURI;
    bool isConnected = false;
    CDOMDocument *ownerDocument = nullptr;
    CDOMNodeBase *parentNode_ = nullptr;
    child_collection childCollection;
    child_type *previousSibling = nullptr;
    child_type *nextSibling = nullptr;
    string_type nodeValue;
    string_type textContent;


  };
}

#endif // PARSERS_HTML_HTMLNODEBASE_H
