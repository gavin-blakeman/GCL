//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// SUBSYSTEM:           Parsers::HTML Parser
// FILE:                htmlLexer.cpp
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
// OVERVIEW:            Class that lexes the html stream
//
// CLASSES INCLUDED:
//
// HISTORY:             2024-06-18 GGB - File Created
//
//**********************************************************************************************************************************


#include "include/parsers/html/htmlLexer.h"

// Standard C++ library header files
#include <iostream>
#include <utility>
#include <vector>

namespace GCL::parsers::html
{
  static std::vector<std::pair<CHTMLLexer::tokenID_t, std::string>> const tokenStrings =
  {
    { CHTMLLexer::L_TAG_OPEN, "<" },
    { CHTMLLexer::L_TAG_CLOSE, "</" },
    { CHTMLLexer::R_TAG_OPEN, ">" },
    { CHTMLLexer::R_TAG_CLOSE, "/>" },
    { CHTMLLexer::L_TAG_DOCTYPE, "<!"},
    { CHTMLLexer::COMMENT_OPEN, "<!---" },
    { CHTMLLexer::COMMENT_CLOSE, "--->" },
    { CHTMLLexer::ASSIGN,  "=" },
    { CHTMLLexer::ID, "ID: " },
    { CHTMLLexer::VALUE, "Value: " },
    { CHTMLLexer::TEXT, "Text: " },
    { CHTMLLexer::ATTRIBUTE, "Attribute: "},
  };

  CHTMLLexer::CHTMLLexer(std::istream &is, token_container &t)
    : CLexer(is, tokenStrings.begin(), tokenStrings.end(), t) {}

  void CHTMLLexer::attribute()
    {
      int sRow = row;
      int sCol = col;

      std::string str;

      whitespace();

      while(true)
      {
        if(match(" ") || match("/>") || match('>') || match('=') || match(EOF))
        {
          break;
        }
        str.push_back(buffer.front());
        consume();
      }

      if(!str.empty())
      {
        boost::to_lower(str);
        tokenContainer.push_back(CToken(tokenStringMap, htmlTokenTypes::ATTRIBUTE, str, sRow, sCol));
      }
    }

  void CHTMLLexer::next()
  {
    if(match("/*"))
    {
      comment();
    }
    else if(match("<"))
    {
      tag();
    }
    else
    {
      text();
    }
  }

  void CHTMLLexer::whitespace()
  {
    while(std::isspace(buffer.front()))
    {
      consume();
    }
  }

  void CHTMLLexer::tag()
  {
    if(peek('!', 1))
    {
      tokenContainer.push_back(CToken(tokenStringMap, htmlTokenTypes::L_TAG_DOCTYPE, std::string(""), row, col));
      consume(2); // <!
    }
    else if(peek('/', 1))
    {
      tokenContainer.push_back(CToken(tokenStringMap, htmlTokenTypes::L_TAG_CLOSE, std::string(""), row, col));
      consume(2); // </
    }
    else
    {
      tokenContainer.push_back(CToken(tokenStringMap, htmlTokenTypes::L_TAG_OPEN, std::string(""), row, col));
      consume(); // <
    }

    whitespace();
    id();

    tokenID_t type;

    int sRow;
    int sCol;

    while(true)
    {
      if(match("="))
      {
        tokenContainer.push_back(CToken(tokenStringMap, htmlTokenTypes::ASSIGN, "", row, col));
        consume();
      }
      else if(match('"'))
      {
        value();
      }
      else if(match("/>"))
      {
        type = R_TAG_CLOSE;
        sRow = row;
        sCol = col;
        consume(2); // />
        break;
      }
      else if(match(">"))
      {
        type = R_TAG_OPEN;
        sRow = row;
        sCol = col;
        consume(); // >
        break;
      }
      else if(match(EOF))
      {
        sRow = row;
        sCol = col;
        break;
      }
      else
      {
        attribute();
      }
    }

    tokenContainer.push_back(CToken(tokenStringMap, type, "", row, col));
  }

  void CHTMLLexer::comment()
  {
    tokenContainer.push_back(CToken(tokenStringMap, htmlTokenTypes::COMMENT_OPEN, "", row, col));

    consume(4); // <!--

    whitespace();

    int sRow = row;
    int sCol = col;

    std::string str;

    while(true)
    {
      if(match("-->") || match(EOF))
      {
        break;
      }

      str.push_back(buffer.front());
      consume();
    }

    boost::trim(str);

    if(!str.empty())
    {
      tokenContainer.push_back(CToken(tokenStringMap, htmlTokenTypes::TEXT, str, sRow, sCol));
    }

    tokenContainer.push_back(CToken(tokenStringMap, htmlTokenTypes::COMMENT_CLOSE, "", row, col));

    consume(3); // -->
  }

  void CHTMLLexer::id()
  {
    int sRow = row;
    int sCol = col;

    std::string str;

    whitespace();

    while(true)
    {
      if(match(" ") || match("/>") || match('>') || match('=') || match(EOF))
      {
        break;
      }
      str.push_back(buffer.front());
      consume();
    }

    if(!str.empty())
    {
      boost::to_lower(str);
      tokenContainer.push_back(CToken(tokenStringMap, htmlTokenTypes::ID, str, sRow, sCol));
    }
  }

  void CHTMLLexer::value()
  {
    consume(); // "

    int sRow = row;
    int sCol = col;

    std::string str;

    while(true)
    {
      if(match('"') || match(EOF))
      {
        break;
      }
      str.push_back(buffer.front());
      consume();
    }

    consume(); // "

    if(!str.empty())
    {
      tokenContainer.push_back(CToken(tokenStringMap, htmlTokenTypes::VALUE, str, row, col));
    }
  }

  void CHTMLLexer::text()
  {
    int sRow = row;
    int sCol = col;

    std::string str;

    whitespace();

    while(true)
    {
      if(match('<') || match(EOF))
      {
        break;
      }

      str.push_back(buffer.front());
      consume();
    }

    boost::trim(str);

    if(!str.empty())
    {
      tokenContainer.push_back(CToken(tokenStringMap, htmlTokenTypes::TEXT, str, sRow, sCol));
    }
  }



} // namespace
