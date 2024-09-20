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

#include "include/parsers/DOM/DOMNodeDocument.h"

// Miscellaneous library header files.
#include "include/error.h"

// Parsers header files
#include "include/parsers/DOM/DOMNodeComment.h"
#include "include/parsers/DOM/DOMNodeElement.h"
#include "include/parsers/DOM/DOMNodeDocType.h"
#include "include/parsers/DOM/DOMNodeList.hpp"

namespace GCL::parsers::DOM
{
  void CDOMDocument::docType(CDOMDocument::string_type const &name,
                                CDOMDocument::string_type const &publicIdentifier,
                                CDOMDocument::string_type const &systemIdentifier)
  {
    CNodeList::value_type val = std::make_unique<CDOMNodeDocType>(this, name, publicIdentifier, systemIdentifier);
    doctype = dynamic_cast<CDOMNodeDocType *>(val.get());
    childCollection.push_back(std::move(val));
  }

}
