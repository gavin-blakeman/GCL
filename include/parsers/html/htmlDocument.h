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
//**********************************************************************************************************************************

#ifndef PARSERS_HTML_HTMLDOCUMENT_H
#define PARSERS_HTML_HTMLDOCUMENT_H

// Standard C++ library header files
#include <deque>
#include <istream>
#include <memory>
#include <optional>
#include <stack>

// GCL Header files
#include "include/parsers/html/htmlNodeElement.h"
#include "include/error.h"

/* The HTML document class stores all the data that makes up the document.
 * There should only be one root element in the DOM and this is the html element. All other elements fall under the root element.
 * Elements are stored as children of a higher level element. Any attributes are stored within the element. The element value
 * is stored in the element.
 * All attribute values and elements values are stored as text.
 * The stack contains the elements in order of creation. This stores the current element, and all elements above the current element
 * in the tree. This should allow easy DOM creation. Note: The stack is used during tree creation.
 */

namespace GCL::parsers::html
{

  template<bool isConst>
  class CHTMLDocumentIterator;

  class CHTMLDocument
  {
    public:
      enum documentReadyState_e { DRS_LOADING, DRS_INTERACTIVE, DRS_COMPLETE};
      enum documentVisibilityState_e { DVS_VISIBLE, DVS_HIDDEN };

      using value_type = CHTMLElement;
      using value_ref = std::reference_wrapper<value_type>;
      using reference = value_type &;
      using pointer = value_type *;
      using const_reference = value_type const &;
      using iterator = CHTMLDocumentIterator<false>;
      using const_iterator = CHTMLDocumentIterator<true>;
      using child_iterator = CHTMLElement::child_iterator;

      /*! @brief      Default constructs an HTML Document.
       */
      CHTMLDocument() = default;

      /*! @brief      Constructs an HTML document from a stream.
       *  @param[in]  inputStream: The stream to use for the HTML.
       */
      CHTMLDocument(std::istream &inputStream);
      CHTMLDocument(CHTMLDocument const &) = default;
      CHTMLDocument(CHTMLDocument &&) = default;
      CHTMLDocument &operator=(CHTMLDocument const &) = default;
      CHTMLDocument &operator=(CHTMLDocument &&) = default;
      ~CHTMLDocument() = default;


      /*! @brief    Searches for the specified element from the starting position.
       *  @param[in]  element: The element to find.
       *  @param[in]  start: The starting iterator.
       *  @throws     noexcept
       */
      const_iterator find(htmlElements_e element, const_iterator start) const noexcept;

      /*! @brief    Searches for the specified element from the starting position.
       *  @param[in]  element: The element to find.
       *  @param[in]  start: The starting iterator.
       *  @throws     noexcept
       */
      const_iterator find(std::string element, const_iterator start) const noexcept;

      /*! @brief      Opens a new element. The element is opened as a child of the current element.
       *  @param[in]  element: The tag name.
       *  @throws
       */
      void openElement(std::string const &element);

      /*! @brief      Closes the specified element. If the specified element is not TOS, then all other elements are closed
       *              implicitly.
       *  @param[in]  element: The name of the element.
       *  @throws
       */
      void closeElement(std::string const &element);

      /*! @brief    Closes the current element. This should only be used in the case that the element is opened and closed
       *            in the same tag.
       */
      void closeElement();

      /*! @brief      Adds the specified attribute to the current element's atributes.
       *  @param[in]  attr: The attribute name.
       *  @param[in]  val: The attributes value.
       */
      void addAttribute(std::string const &attr, std::string const &val);

      /*! @brief      Adds a comment node to the current node.
       *  @param[in]  node: The node to add.
       *  @throws
       */
      void addComment(std::string const &);

      iterator begin();
      const_iterator begin() const;
      const_iterator cbegin() const;

      iterator end();
      const_iterator end() const;
      const_iterator cend() const;

      /*! @brief      Sets the value of the last item inserted.
       *  @param[in]  val: The value to set.
       *  @throws
       */
      void setValue(std::string const &val);

      static CHTMLDocument parseHTMLUnsafe(std::istream &);
      static CHTMLDocument parseHTMLUnsafe(std::string const &);

//      documentReadyState_e readyState() const;

//      std::string dir() const noexcept;
//      CHTMLNodeElement &body();
//      CHTMLNodeElement &head();

//      CHTMLCollection const &images;
//      CHTMLCOllection const &emeds;
//      CHTMLCollection const &plugins;
//      CHTMLCollection const &links;
//      CHTMLCollection const &forms;
//      CHTMLCollection const &scripts;

      //NodeList getElementsByName(std::string elementName);

//      CHTMLContainer policyContainer;
//      CHTMLPermissions permissionsPolicy;
//      CModuleMap moduleMap;

    private:
      std::unique_ptr<CHTMLElement> root;
      std::deque<pointer> createStack;
      pointer currentElement = nullptr;
  };

  template<bool isConst>
  class CHTMLDocumentIterator
  {
    /* Iteration can be any of the iteration orders. In order to iterate, different iteration orders require different variables.
     * Essentially we need to have a stack to be able to traverse back up the hierarchy. It is also necessary to have an iterator
     * for the current element's children.
     * Another question is whether the iteration order is a template parameter or a variable passed to the constructor. In this case
     * it is probably a variable passed in the constructor.
     */
    public:
      using value_type = CHTMLDocument::value_type;
      using value_ref = CHTMLDocument::value_ref;
      using reference = value_type &;
      using const_reference = value_type const &;
      using pointer = value_type *;
      using const_pointer = value_type const *;
      using self_type = CHTMLDocumentIterator;
      using child_iterator = CHTMLDocument::child_iterator;
      using iterator_category = std::forward_iterator_tag;

      CHTMLDocumentIterator(CHTMLDocument &l, bool ie = false) : linked(l), isEnd(ie)
      {
        if (!isEnd)
        {
          if (linked.root)
          {
            if (linked.root->child_begin() != linked.root->child_end())
            {
              stack.push(std::make_pair(std::ref(*linked.root.get()), linked.root->child_begin()));
            }
            else
            {
              isEnd = true;
            }
          }
          else
          {
            isEnd = true;
          }
        }
      }

      CHTMLDocumentIterator(self_type const &rhs) : linked(rhs.linked), isEnd(rhs.isEnd), stack(rhs.stack) {}
      ~CHTMLDocumentIterator() = default;

      reference operator*()
      {
        if (!isEnd)
        {
          return stack.top().first.get();
        }
        else
        {
          CODE_ERROR();
          // Does not return.
        }
      }

      pointer operator->()
      {
        if (!isEnd)
        {
          return &stack.top().first.get();
        }
        else
        {
          CODE_ERROR();
          // Does not return.
        }
      }

      self_type &operator++()
      {
        if (!isEnd)
        {
          while (!stack.empty() && (++stack.top().second == stack.top().first.get().child_end()))
          {
            stack.pop();
          }
          if (stack.empty())
          {
            isEnd = true;
          }
        }
        return *this;
      }

      self_type operator++(int)
      {
        if (!isEnd)
        {
          self_type temp(*this);
          ++*this;

          return temp;
        }
        else
        {
          return *this;
        }
      }

    private:
      CHTMLDocumentIterator() = delete;

      using stack_entry = std::pair<value_ref, child_iterator>;

      CHTMLDocument &linked;
      bool isEnd = false;
      std::stack<stack_entry> stack;          // Current element is TOS.

      friend bool operator==(self_type const &, self_type const &) { return false; };
  };

} // namespace

#endif // PARSERS_HTML_HTMLDOCUMENT_H
