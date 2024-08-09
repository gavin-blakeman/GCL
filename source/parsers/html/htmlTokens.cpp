//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// SUBSYSTEM:           Parsers
// FILE:                htmlTokens.cpp
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
// OVERVIEW:            Class provides a generic token used for lexing files.
//
// CLASSES INCLUDED:
//
// HISTORY:             2024-06-18 GGB - File Created
//
//**********************************************************************************************************************************

#include "include/parsers/html/htmlTokens.h"

// Standard C++ header files

namespace GCL::parsers::html
{
  CHTMLToken::tokenStringMap_t const tokenStrings =
  {
    { CHTMLToken::TT_START_TAG, "Start Tag" },
    { CHTMLToken::TT_END_TAG, "End Tag" },
    { CHTMLToken::TT_CHARACTER, "Character"},
    { CHTMLToken::TT_DOCTYPE, "DocType" },
    { CHTMLToken::TT_COMMENT, "Comment" },
    { CHTMLToken::TT_EOF, "EOF" },
  };

  CHTMLToken::CHTMLToken() : tokenStringMap(tokenStrings) {}

  CHTMLToken &CHTMLToken::operator=(CHTMLToken const &other)
  {
    if (this != &other)
    {
      tokenType = other.tokenType;
      tokenValue = other.tokenValue;
    }

    return *this;
  }

  std::ostream &operator<<(std::ostream &os, CHTMLToken const &t)
  {
    os << std::endl;
    return os;
  }

} // namespace
