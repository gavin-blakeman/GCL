//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// SUBSYSTEM:           Parsers::HTML Parser
// FILE:                htmlParser.cpp
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
// OVERVIEW:            Class that parses the tokens.
//
// CLASSES INCLUDED:
//
// HISTORY:             2024-06-18 GGB - File Created
//
//**********************************************************************************************************************************/

#include "include/parsers/html/htmlParser.h"

// Standard C++ library header files

// GCL library header files
#include "include/parsers/html/htmlTokens.h"
#include "include/parsers/html/htmlTokeniser.h"

namespace GCL::parsers::html
{
  void CHTMLParser::parseDocument()
  {
    using namespace GCL::parsers;

    CHTMLTokeniser lexer(inputStream);

    for(CToken token = lexer.getToken(); token != TT_EOF; token = lexer.getToken())
    {
      parseToken(token);
    }
  }

  void CHTMLParser::parseToken(CToken const &token)
  {
    switch (insertionMode)
    {
      case IM_INITIAL:
      {
        parseTokenModeInitial(token);
        break;
      }
    }
  }

  void CHTMLParser::parseTokenModeInitial(CToken const &token)
  {
    switch(token.type())
    {
      case TT_CHARACTER:
      {

        break;
      }
        case TT_COMMENT:
        {
          DOM.insertComment(token.value());
          break;
        }
        case ASSIGN:
        case VALUE:
        case ID:
        {
          // Malformed HTML file. Simply discard.
          CODE_ERROR();
          break;
        }
        case L_TAG_DOCTYPE:
        {
          break;
        }
        case TEXT:
        {
   //       DOM.setValue(tokenIterator->value());
          break;
        }
        case TT_EOF:
        {
           break;
        }
        default:
        {
          /* These will generally be tags that are out of order. IE closing tags. Closing tags should be picked
           * up by the open functions. So these would be out-of-order or repeated closings. */
          break;
        }
      }
  }

} // namespace
