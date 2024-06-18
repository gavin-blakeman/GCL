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
#include <functional>
#include <deque>
#include <optional>
#include <stack>

// GCL Header files
#include "include/parsers/html/htmlElement.h"

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
      using value_type = CHTMLElement;
      using value_ref = std::reference_wrapper<value_type>;
      using reference = value_type &;
      using pointer = value_type *;
      using const_reference = value_type const &;
      using iterator = CHTMLDocumentIterator<false>;
      using const_iterator = CHTMLDocumentIterator<true>;
      using child_iterator = CHTMLElement::child_iterator;

      enum order_e : int { PREORDER, POSTORDER, PARENTORDER, INORDER, LEFT, RIGHT, NATURAL };

      /*! @brief    Searches for the specified element from the starting position.
       *  @param[in]  element: The element to find.
       *  @param[in]  start: The starting iterator.
       *  @throws     noexcept
       */
      iterator find(htmlElements_e element, iterator start) const noexcept;


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

      void addAttribute(std::string const &attr, std::string const &val);

      iterator begin();
      const_iterator begin() const;
      const_iterator cbegin() const;

      iterator end();
      const_iterator end() const;
      const_iterator cend() const;

      /*! @brief      Sets the value of the last item inserted.
       *  @param[in]  The value to set.
       *  @throws
       */
      void setValue(std::string const &);


    private:
      CHTMLElement root;
      std::deque<value_ref> createStack;
      value_ref currentElement;
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
      using iteration_order = CHTMLDocument::order_e;
      using child_iterator = CHTMLDocument::child_iterator;


      reference operator*()
      {
        return stack.top().get();
      }

      pointer operator->()
      {
        return &stack.top().get();
      }

      self_type &operator++()
      {
        return *this;
      }

      self_type operator++(int)
      {
        self_type temp;

        return temp;
      }

    private:
      iteration_order iterationOrder;
      std::stack<value_ref> stack;          // Current element is TOS.
      child_iterator currentChildIterator;
  };

} // namespace

#endif // PARSERS_HTML_HTMLDOCUMENT_H
