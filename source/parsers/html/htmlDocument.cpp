//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// SUBSYSTEM:           Parsers::HTML Parser
// FILE:                htmlDocument.cpp
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


#include "include/parsers/html/htmlDocument.h"

// Miscellaneous library header files.
#include "include/error.h"

// Parsers header files
#include "include/parsers/html/htmlParser.h"
#include "include/parsers/html/htmlNodeComment.h"
#include "include/parsers/html/htmlNodeElement.h"

namespace GCL::parsers::html
{
  CHTMLDocument::CHTMLDocument(std::istream &is)
  {
    CHTMLParser parser(is, *this);
  }

  void CHTMLDocument::addAttribute(std::string const &attr, std::string const &val)
  {
    if (currentNode()->nodeType() == NT_ELEMENT)
    {
      dynamic_cast<CHTMLNodeElement *>(currentNode())->insert(attr, val);
    }
    else
    {
      CODE_ERROR();
    }
  }

  void CHTMLDocument::addComment(std::string const &comment)
  {
    //currentNode()->insert(std::make_unique<CHTMLNodeComment>(currentElement, comment));
  }

//  CHTMLDocument::const_iterator CHTMLDocument::find(htmlElements_e element, const_iterator start) const noexcept
//  {
    /* Start at the start point and search till the end. */

//    while (start->type() != element) start++;

//    return start;
//  }

  CHTMLDocument::const_iterator CHTMLDocument::find(std::string element, const_iterator start) const noexcept
  {
    /* Start at the start point and search till the end. */

//    while (start->type() != element) start++;

    return start;
  }

  void CHTMLDocument::insertComment(std::string const &c)
  {
    push(currentNode()->insert(std::make_unique<CHTMLNodeComment>(currentNode(), c)));
  }

  void CHTMLDocument::openElement(std::string const &element)
  {
    if (root)
    {
//      createStack.push_front(currentElement);
  //    currentNode()->insert(std::make_unique<CHTMLNodeElement>(currentElement, element));
    }
    else
    {
//      root = std::make_unique<CHTMLNodeElement>(currentElement, element);
    }
  }

  void CHTMLDocument::setValue(std::string const &val)
  {
  //  currentElement->value(val);
  }

  void CHTMLDocument::closeElement()
  {
//    currentElement = createStack.front();
//    createStack.pop_front();
  }

  void CHTMLDocument::closeElement(std::string const &element)
  {
//    if (currentElement->nodeType() == CHTMLElement::string2elementType(element))
//    {
//      closeElement();
//    }
//    else
//    {
//      IMPLEMENT_ME();
//    }
  }

  CHTMLDocument CHTMLDocument::parseHTMLUnsafe(std::istream &is)
  {
    return CHTMLDocument(is);
  }

  CHTMLDocument CHTMLDocument::parseHTMLUnsafe(std::string const &)
  {

  }

  CHTMLDocument::iterator CHTMLDocument::begin()
  {

  }
  CHTMLDocument::const_iterator CHTMLDocument::begin() const
  {

  }
  CHTMLDocument::const_iterator CHTMLDocument::cbegin() const
  {

  }

  CHTMLDocument::iterator CHTMLDocument::end()
  {

  }
  CHTMLDocument::const_iterator CHTMLDocument::end() const
  {

  }
  CHTMLDocument::const_iterator CHTMLDocument::cend() const
  {

  }

}
