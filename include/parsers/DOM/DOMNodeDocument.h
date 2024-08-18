//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// SUBSYSTEM:           Parsers::HTML Parser
// FILE:                htmlDocument.h
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
// OVERVIEW:            Class that represents the DOM.
//
// CLASSES INCLUDED:
//
// HISTORY:             2024-06-18 GGB - File Created
//
//**********************************************************************************************************************************/

#ifndef PARSERS_HTML_HTMLDOCUMENT_H
#define PARSERS_HTML_HTMLDOCUMENT_H

// Standard C++ library header files
#include <deque>
#include <istream>
#include <memory>
#include <optional>
#include <stack>

// GCL Header files
#include "include/parsers/DOM/DOMNodeBase.h"
#include "include/parsers/DOM/DOMNodeList.hpp"
#include "include/parsers/DOM/DOMNodeElement.h"
#include "include/parsers/DOM/htmlCollection.hpp"
#include "include/error.h"

/* The HTML document class stores all the data that makes up the document.
 * There should only be one root node in the DOM and this is the docType node. All other nodes fall under the root node.
 * Nodes are stored as children of a higher level node. Any attributes are stored within the node. The node values
 * is stored in the node.
 * All attribute values and node values are stored as text.
 * The stack contains the nodes in order of creation. This stores the current node, and all elements above the current node
 * in the tree. This should allow easy DOM creation. Note: The stack is used during tree creation.
 */

namespace GCL::parsers::DOM
{
  class CDOMNodeDocType;
  class CDOMNodeElement;
  class CDOMComment;

  template<bool isConst>
  class CHTMLDocumentIterator;

  class CDOMDocument : public CDOMNodeBase
  {
  public:
    enum documentReadyState_e { DRS_LOADING, DRS_INTERACTIVE, DRS_COMPLETE};
    enum documentVisibilityState_e { DVS_VISIBLE, DVS_HIDDEN };

    using value_type = CDOMNodeBase;
    using value_ref = std::reference_wrapper<value_type>;
    using reference = value_type &;
    using pointer = value_type *;
    using const_reference = value_type const &;
    using iterator = CHTMLDocumentIterator<false>;
    using const_iterator = CHTMLDocumentIterator<true>;
    using child_iterator = value_type::child_iterator;

    /*! @brief      Default constructs an HTML Document.
     */
    CDOMDocument() = default;

    CDOMDocument(CDOMDocument const &) = default;
    CDOMDocument(CDOMDocument &&) = default;
    CDOMDocument &operator=(CDOMDocument const &) = default;
    CDOMDocument &operator=(CDOMDocument &&) = default;
    ~CDOMDocument() = default;

    virtual nodeType_e nodeType() const noexcept {return DOCUMENT_NODE; }

    CHTMLCollection getElementsByTagName(string_type const &qualifiedName);
    CHTMLCollection getElementsByTagNameNS(string_type const &nameSpace, string_type const &localName);
    CHTMLCollection getElementsByClassName(string_type const &classNames);

    CDOMNodeBase *createElement(string_type const &localName);
    CDOMNodeBase *createElementNS(string_type const &nameSpace, string_type const &qualifiedName);

    CDOMComment *createComment(string_type const &);
    CDOMComment *createComment(string_type &&);

    void insertDocType(string_type const &, string_type const &, string_type const &);

#ifdef TEST
  public:
#else
  private:
#endif
    // Following from DOM standard
    //DOMImplementation implementation;
    string_type URL;
    string_type documentURI;
    string_type compatMode;
    string_type characterSet;
    string_type charSet;
    string_type inputEncoding;
    string_type contentType;
    CDOMNodeDocType *doctype = nullptr;
    std::unique_ptr<CDOMElement> documentElement;
    bool parserCannotChangeMode = false;

    // Following are from the HTML Standard
    CHTMLCollection images;
    CHTMLCollection embeds;
    CHTMLCollection plugins;
    CHTMLCollection links;
    CHTMLCollection forms;
    CHTMLCollection scripts;

    std::unique_ptr<CDOMNodeBase> root;
    std::deque<pointer> openElementsStack;  // Cannot be a stack as we need to acces random elements as well.
    // front = first element in, back = last element in. Push/Pop to back.

    inline pointer currentNode() noexcept { return ( (openElementsStack.empty()) ? nullptr : openElementsStack.back() ); }
    inline void push(pointer p) noexcept { openElementsStack.push_back(p); }
    inline void pop() noexcept { openElementsStack.pop_back(); }
  };

//  template<bool isConst>
//  class CHTMLDocumentIterator
//  {
//    /* Iteration can be any of the iteration orders. In order to iterate, different iteration orders require different variables.
//     * Essentially we need to have a stack to be able to traverse back up the hierarchy. It is also necessary to have an iterator
//     * for the current element's children.
//     * Another question is whether the iteration order is a template parameter or a variable passed to the constructor. In this case
//     * it is probably a variable passed in the constructor.
//     */
//  public:
//    using value_type = CHTMLDocument::value_type;
//    using value_ref = CHTMLDocument::value_ref;
//    using reference = value_type &;
//    using const_reference = value_type const &;
//    using pointer = value_type *;
//    using const_pointer = value_type const *;
//    using self_type = CHTMLDocumentIterator;
//    using child_iterator = CHTMLDocument::child_iterator;
//    using iterator_category = std::forward_iterator_tag;
//
//    CHTMLDocumentIterator(CHTMLDocument &l, bool ie = false) : linked(l), isEnd(ie)
//    {
//      if (!isEnd)
//      {
//        if (linked.root)
//        {
//          if (linked.root->child_begin() != linked.root->child_end())
//          {
//            stack.push(std::make_pair(std::ref(*linked.root.get()), linked.root->child_begin()));
//          }
//          else
//          {
//            isEnd = true;
//          }
//        }
//        else
//        {
//          isEnd = true;
//        }
//      }
//    }
//
//    CHTMLDocumentIterator(self_type const &rhs) : linked(rhs.linked), isEnd(rhs.isEnd), stack(rhs.stack) {}
//    ~CHTMLDocumentIterator() = default;
//
//    reference operator*()
//    {
//      if (!isEnd)
//      {
//        return stack.top().first.get();
//      }
//      else
//      {
//        CODE_ERROR();
//        // Does not return.
//      }
//    }
//
//    pointer operator->()
//    {
//      if (!isEnd)
//      {
//        return &stack.top().first.get();
//      }
//      else
//      {
//        CODE_ERROR();
//        // Does not return.
//      }
//    }
//
//    self_type &operator++()
//          {
//      if (!isEnd)
//      {
//        while (!stack.empty() && (++stack.top().second == stack.top().first.get().child_end()))
//        {
//          stack.pop();
//        }
//        if (stack.empty())
//        {
//          isEnd = true;
//        }
//      }
//      return *this;
//          }
//
//    self_type operator++(int)
//          {
//      if (!isEnd)
//      {
//        self_type temp(*this);
//        ++*this;
//
//        return temp;
//      }
//      else
//      {
//        return *this;
//      }
//          }
//
//  private:
//    CHTMLDocumentIterator() = delete;
//
//    using stack_entry = std::pair<value_ref, child_iterator>;
//
//    CHTMLDocument &linked;
//    bool isEnd = false;
//    std::stack<stack_entry> stack;          // Current element is TOS.
//
//    friend bool operator==(self_type const &, self_type const &) { return false; };
//  };

} // namespace

#endif // PARSERS_HTML_HTMLDOCUMENT_H
