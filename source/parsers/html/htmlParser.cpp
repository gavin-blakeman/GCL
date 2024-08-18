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
#include "include/parsers/html/htmlExceptions.h"

namespace GCL::parsers::html
{
  static CHTMLParser::string_type S32_HTML{'h', 't', 'm', 'l'};

  void CHTMLParser::parseDocument()
  {
    using namespace GCL::parsers;
    CHTMLTokeniser tokeniser(inputStream);

    parseToken(tokeniser.getToken());

  }

  void CHTMLParser::parseToken(CHTMLToken const &token)
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

  // 13.2.6.4.1
  void CHTMLParser::processInitial(CHTMLToken const &token)
  {
    switch(token.type())
    {
      case TT_CHARACTER:
      {
        switch (token.data())
        {
          case U_0009:
          case U_000A:
          case U_000C:
          case U_000D:
          case U_0020:
          {
            // ignore the character.
            break;
          }
        }
        break;
      }
      case TT_COMMENT:
      {
        DOM.createComment(std::move(token.value()));
        break;
      }
      case TT_DOCTYPE:
      {
        if ( (token.name() == S32_HTML) || docTypeValid(token.name()) )
        {
         DOM.insertDocType(token.name(),token.publicIdentifier(), token.systemIdentifier());
         insertionMode = IM_BEFORE_HTML;
        }
        else
        {
          PARSE_ERROR("");
        }
        break;
      }
      default:
      {
        insertionMode = IM_BEFORE_HTML;
        break;
      }
      }
  }

  // 13.2.6.4.2
  void CHTMLParser::processBeforeHTML(CHTMLToken const &)
  {
    switch(token.type())
    {
      case TT_DOCTYPE:
      {
        PARSE_ERROR("");
        break;
      }
      case TT_COMMENT:
      {
        DOM.createComment(std::move(token.value()));
        break;
      }
      case TT_CHARACTER:
      {
        switch (token.data())
        {
          case U_0009:
          case U_000A:
          case U_000C:
          case U_000D:
          case U_0020:
          {
            // ignore the character.
            break;
          }
        }
        break;
      }
      case TT_TAG_START:
      {

      }
    }
  }

} // namespace
