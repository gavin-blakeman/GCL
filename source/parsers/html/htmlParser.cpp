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
<<<<<<< HEAD
#include "include/parsers/html/htmlLexer.hpp"
=======
#include "include/parsers/token.h"
#include "include/parsers/html/htmlTokeniser.h"
>>>>>>> KararaMining-htmlParsers-dev
#include "include/parsers/html/htmlLanguageTokens.h"

namespace GCL::parsers::html
{
  void CHTMLParser::parseDocument()
  {
<<<<<<< HEAD
    CHTMLLexer<std::vector> lexer(inputStream, tokens);
    lexer.getTokens();
    parseTokens();
  }

  void CHTMLParser::parseLTagOpen()
  {
    // Starting a new tag.
    std::string tagName = tokenIterator++->value();
    DOM.openElement(tagName);
    while (tokenIterator->type ()== ATTRIBUTE)
    {
      // Capture all the attributes.
      std::string temp = tokenIterator++->value();
      if (tokenIterator->type() != ASSIGN)
      {
        DOM.addAttribute(temp, "");
      }
      else
      {
        DOM.addAttribute(temp, (++tokenIterator)->value());
      };
      tokenIterator++;
    }
    if (tokenIterator->type() == R_TAG_OPEN )
    {
      if (CHTMLElement::isVoid(tagName))
      {
        // Tag is complete and can be added to the DOM
        DOM.closeElement();
      }
    }
    else if (tokenIterator->type() == R_TAG_CLOSE)
    {
      // Tag is complete and can be added to the DOM
      DOM.closeElement();
    }
    else
    {
      // Error in file format.
      IMPLEMENT_ME();
    }
  }

  void CHTMLParser::parseLTagClose()
  {
    if ((++tokenIterator)->type() == ID)
    {
      DOM.closeElement();
    }
    else
    {
      // Malformed file.
      IMPLEMENT_ME();
    }
  }

  void CHTMLParser::parseLTagDocType()
  {
    /* Parse to the closing tag. (R_TAG_OPEN)
     * Values that need to be supported are
     * html -> HTML5
     * <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd"> -> HTML 4.01
     * <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd"> -> XHTML 1.1
    */

    

  }

  void CHTMLParser::parseCommentOpen()
  {
    // Simply discard all tags until the COMMENT_CLOSE is found.
    DOM.addComment((++tokenIterator)->value());
    if (((++tokenIterator)->type() != COMMENT_CLOSE))
    {
      // Malformed file.
      IMPLEMENT_ME();
    }
  }

  void CHTMLParser::parseTokens()
  {
    tokenIterator = tokens.begin();

    while (tokenIterator != tokens.end())
    {
      switch(tokenIterator++->type())
      {
        case L_TAG_OPEN:  // "<"
        {
          parseLTagOpen();
          break;
        }
        case L_TAG_CLOSE:   // "</"
        {
          parseLTagClose();
          break;
        }
        case COMMENT_OPEN:
        {
          parseCommentOpen();
          break;
        }
=======
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
>>>>>>> KararaMining-htmlParsers-dev
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
<<<<<<< HEAD
          parseLTagDocType();
=======
>>>>>>> KararaMining-htmlParsers-dev
          break;
        }
        case TEXT:
        {
<<<<<<< HEAD
          DOM.setValue(tokenIterator->value());
=======
   //       DOM.setValue(tokenIterator->value());
>>>>>>> KararaMining-htmlParsers-dev
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
<<<<<<< HEAD
    }
=======
>>>>>>> KararaMining-htmlParsers-dev
  }

} // namespace
