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
//**********************************************************************************************************************************


#include "include/parsers/html/htmlDocument.h"


// Miscellaneous library header files.
#include <GCL>

namespace GCL::parsers::html
{
  void CHTMLDocument::addAttribute(std::string const &attr, std::string const &val)
  {
    currentElement.get().insert(attr, val);
  }

  CHTMLDocument::iterator CHTMLDocument::find(htmlElements_e element, iterator start) const noexcept
  {
    while (start->elementType() != element) start++;

    return start;
  }

  void CHTMLDocument::openElement(std::string const &element)
  {
    createStack.push_front(currentElement);
    currentElement = currentElement.get().insert(element);
  }

  void CHTMLDocument::setValue(std::string const &val)
  {
    currentElement.get().value(val);
  }


  void CHTMLDocument::closeElement()
  {
    currentElement = createStack.front();
    createStack.pop_front();
  }

  void CHTMLDocument::closeElement(std::string const &element)
  {
    if (currentElement.get().elementType() == CHTMLElement::string2elementType(element))
    {
      closeElement();
    }
    else
    {
      IMPLEMENT_ME();
    }
  }

}
