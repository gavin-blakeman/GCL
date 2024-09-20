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
  static CHTMLParser::string_type S32_HTML{"html"};
  static CHTMLParser::string_type S32_ABOUT_LEGACY{"about:legacy-compat"};
  static CHTMLParser::string_type S32_BR{"br"};
  static CHTMLParser::string_type S32_HEAD{"head"};
  static CHTMLParser::string_type S32_BODY{"body"};
  static CHTMLParser::string_type S32_BASE{"base"};
  static CHTMLParser::string_type S32_BASEFONT{"basefont"};
  static CHTMLParser::string_type S32_BGSOUND{"bgsound"};
  static CHTMLParser::string_type S32_LINK{"link"};

  // See 13.2.6.4.1
  bool CHTMLParser::docTypeValid(CHTMLParser::string_type const &name,
                                 CHTMLParser::string_type const &publicID,
                                 CHTMLParser::string_type const &systemID)
  {
    bool returnValue = true;

    if (name != S32_HTML)
    {
      returnValue = false;
    }
    else if (!publicID.empty())
    {
      returnValue = false;
    }
    else if (!systemID.empty())
    {
      if (systemID != S32_ABOUT_LEGACY)
      {
        returnValue = false;
      }
    }
    else
    {
      returnValue = true;
    }

    return returnValue;
  }

  void CHTMLParser::parseDocument()
  {
    using namespace GCL::parsers;
    CHTMLTokeniser tokeniser(inputStream);

    constructTree(tokeniser.getToken());

  }

  void CHTMLParser::constructTree(CHTMLToken const &token)
  {
    switch (insertionMode)
    {
      case IM_INITIAL:
      {
        processModeInitial(token);
        break;
      }
      case IM_BEFORE_HTML:
      {
        processBeforeHTML(token);
        break;
      }
      case IM_BEFORE_HEAD:
      {
        processBeforeHead(token);
        break;
      }
    }
  }

  void CHTMLParser::insertElement(CHTMLParser::string_type const &ns, CHTMLParser::string_type const &tn)
  {

  }

  // 13.2.6.4.1
  void CHTMLParser::processModeInitial(CHTMLToken const &token)
  {
    switch(token.type())
    {
      case CHTMLToken::TT_CHARACTER:
      {
        switch (token.character())
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
      case CHTMLToken::TT_COMMENT:
      {
        DOM.insertComment(token.comment());
        break;
      }
      case CHTMLToken::TT_DOCTYPE:
      {
        if ( docTypeValid(token.name(), token.publicIdentifier(), token.systemIdentifier()) )
        {
          DOM.docType(token.name(),token.publicIdentifier(), token.systemIdentifier());
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
        PARSE_ERROR("");
        insertionMode = IM_BEFORE_HTML;
        break;
      }
    }
  }

  // 13.2.6.4.2
  void CHTMLParser::processBeforeHTML(CHTMLToken const &token)
  {
    switch(token.type())
    {
      case CHTMLToken::TT_DOCTYPE:
      {
        PARSE_ERROR("");
        break;
      }
      case CHTMLToken::TT_COMMENT:
      {
        DOM.insertComment(token.comment());
        break;
      }
      case CHTMLToken::TT_CHARACTER:
      {
        switch (token.character())
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
      case CHTMLToken::TT_TAG_START:
      {
        if (token.name() == S32_HTML)
        {
          insertElement(S32_HTML, token.name());
          openElements.push(token.name());
          insertionMode = IM_BEFORE_HEAD;
        }
        break;
      }
      case CHTMLToken::TT_TAG_END:
      {
        if ( (token.name() != S32_HEAD) &&
             (token.name() != S32_BODY) &&
             (token.name() != S32_HTML) &&
             (token.name() != S32_BR) )
        {
          PARSE_ERROR("");
        }
        else
        {
          insertElement(S32_HTML, token.name());
          openElements.push(token.name());
          insertionMode = IM_BEFORE_HEAD;
          constructTree(token);
        }
        break;
      }
    }
  }

  // 13.2.6.4.3
  void CHTMLParser::processBeforeHead(CHTMLToken const &token)
  {
    switch(token.type())
    {
      case CHTMLToken::TT_CHARACTER:
      {
        switch (token.character())
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
      case CHTMLToken::TT_COMMENT:
      {
        DOM.insertComment(token.comment());
        break;
      }
      case CHTMLToken::TT_DOCTYPE:
      {
        PARSE_ERROR("");
        break;
      }
      case CHTMLToken::TT_TAG_START:
      {
        if (token.name() == S32_HTML)
        {

        }
        else if (token.name() == S32_HEAD)
        {
          insertElement(S32_HTML, token.name());
          insertionMode = IM_IN_HEAD;
        }
        else
        { // anything else
          insertElement(S32_HTML, S32_HEAD);
          insertionMode = IM_IN_HEAD;
          constructTree(token);
        }
        break;
      }
      case CHTMLToken::TT_TAG_END:
      {
        if ( (token.name() == S32_HEAD) ||
             (token.name() == S32_BODY) ||
             (token.name() == S32_HTML) ||
             (token.name() == S32_BR) )
        {
          insertElement(S32_HTML, token.name());
          insertionMode = IM_IN_HEAD;
          constructTree(token);
        }
        else
        {
          PARSE_ERROR("");
        }
        break;
      }
      default:
      {
        insertElement(S32_HTML, S32_HEAD);
        insertionMode = IM_IN_HEAD;
        constructTree(token);
        break;
      }
    }
  }

} // namespace
