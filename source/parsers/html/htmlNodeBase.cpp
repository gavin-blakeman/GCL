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

#include "include/parsers/html/htmlNodeBase.h"

namespace GCL::parsers::html
{
  CHTMLNodeBase::child_pointer CHTMLNodeBase::insert(std::unique_ptr<CHTMLNodeBase> &&ins)
  {
    //elementChildren.push_back(std::move(ins));
  }

  CHTMLNodeBase::child_pointer CHTMLNodeBase::insert(CHTMLNodeBase *p, std::string const &etype)
  {
    //elementChildren.push_back(std::make_unique<CHTMLElement>(p, etype));
  }
}
