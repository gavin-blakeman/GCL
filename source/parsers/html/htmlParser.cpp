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
#include "include/parsers/html/htmlLexer.h"
#include "include/parsers/html/htmlLanguageTokens.h"

namespace GCL::parsers::html
{
  void CHTMLParser::parseDocument()
  {
    CHTMLLexer<std::vector> lexer(inputStream, tokens);
    lexer.getTokens();

    parseTokens();
  }

  void CHTMLParser::parseTokens()
  {
    auto iter = tokens.begin();

    while (iter != tokens.end())
    {
      switch(iter++->type())
      {
        case L_TAG_OPEN:  // "<"
        {
          // Starting a new tag.
          std::string tagName = iter++->value();
          DOM.openElement(tagName);
          while (iter->type ()== ATTRIBUTE)
          {
            // Capture all the attributes.
            std::string temp = iter++->value();
            if (iter->type() != ASSIGN)
            {
              DOM.addAttribute(temp, "");
            }
            else
            {
              DOM.addAttribute(temp, (++iter)->value());
            };
            iter++;

          }
          if (iter->type() == R_TAG_OPEN )
          {
            if (CHTMLElement::isVoid(tagName))
            {
              // Tag is complete and can be added to the DOM
              DOM.closeElement();
            }
          }
          else if (iter->type() == R_TAG_CLOSE)
          {
            // Tag is complete and can be added to the DOM
            DOM.closeElement();
          }
          else
          {
            // Error in file format.
            IMPLEMENT_ME();
          }
          break;
        }
        case L_TAG_CLOSE:   // "</"
        {
          if ((++iter)->type() == ID)
          {
            DOM.closeElement();
          }
          else
          {
            // Malformed file.
            IMPLEMENT_ME();
          }
          break;
        }
        case COMMENT_OPEN:
        {
          // Simply discard all tags until the COMMENT_CLOSE is found.
          DOM.addComment((++iter)->value());
          if (((++iter)->type() != COMMENT_CLOSE))
          {
            // Malformed file.
            IMPLEMENT_ME();
          }
          break;
        }
        case COMMENT_CLOSE:
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
          // This is used to indicate an HTML5 file if it is just html
          break;
        }
        case TEXT:
        {
          DOM.setValue(iter->value());
          break;
        }
        default:
        {
          break;
        }
      }
      ++iter; // Move to the next token.
    }
  }

} // namespace
