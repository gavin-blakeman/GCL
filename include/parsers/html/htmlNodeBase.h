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

// Parsers header files
#include "include/parsers/html/htmlNodeTypes.h"

// GCL header files
#include "include/parsers/html/htmlNodeTypes.h"

namespace GCL::parsers::html
{
  class CHTMLNodeBase
  {
  public:
    using child_type = CHTMLNodeBase;
    using child_collection = std::list<std::unique_ptr<child_type>>;
    using child_pointer = child_type *;
    using child_iterator = child_collection::iterator;
    using child_const_iterator = child_collection::const_iterator;

    CHTMLNodeBase(CHTMLNodeBase *parent) : parentNode(parent) {}
    virtual ~CHTMLNodeBase() = default;

    child_pointer insert(std::unique_ptr<CHTMLNodeBase> &&ins);
    child_pointer insert(CHTMLNodeBase *p, std::string const &etype);


    child_iterator child_begin() { return childCollection.begin(); }
    child_const_iterator child_begin() const { return childCollection.cbegin(); }
    child_const_iterator child_cbegin() const { return childCollection.cbegin(); }

    child_iterator child_end() { return childCollection.end(); }
    child_const_iterator child_end() const { return childCollection.cend(); }
    child_const_iterator child_cend() const { return childCollection.cend(); }

    /*! @brief       Returns the type of the node.
     *  @returns     The type of the node.
     */
    virtual nodeType_t nodeType() const = 0;

    private:
      CHTMLNodeBase() = delete;
      CHTMLNodeBase(CHTMLNodeBase const &) = delete;
      CHTMLNodeBase(CHTMLNodeBase &&) = delete;
      CHTMLNodeBase &operator=(CHTMLNodeBase const &) = delete;
      CHTMLNodeBase &operator=(CHTMLNodeBase &&) = delete;

      CHTMLNodeBase *parentNode = nullptr;
      child_collection childCollection;


  };
}

#endif // PARSERS_HTML_HTMLNODEBASE_H
