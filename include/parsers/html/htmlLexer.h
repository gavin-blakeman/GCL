//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// SUBSYSTEM:           Parsers::HTML Parser
// FILE:                htmlLexer.h
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


#ifndef GCL_PARSERS_HTMLLEXER_H
#define GCL_PARSERS_HTMLLEXER_H

// Standard C++ library header files
#include <utility>

// Parsers library header files.
#include "include/parsers/lexer.h"
#include "include/parsers/html/htmlLanguageTokens.h"

namespace GCL::parsers::html
{
  template<template<typename...> class _Container>
  class CHTMLLexer : public CLexer< _Container >
  {
  public:
    using tokenID_t = CToken::tokenID_t;
    using token_container = CLexer<_Container>::token_container;

    CHTMLLexer(std::istream &is, token_container &t) : CLexer< _Container >(is, tokenStrings.begin(), tokenStrings.end(), t) {}
    virtual ~CHTMLLexer() = default;

  private:
    CHTMLLexer() = delete;
    CHTMLLexer(CHTMLLexer const &) = delete;
    CHTMLLexer(CHTMLLexer &&) = delete;
    CHTMLLexer &operator=(CHTMLLexer const &) = delete;
    CHTMLLexer &operator=(CHTMLLexer &&) = delete;

    inline static std::vector<std::pair<htmlTokenTypes, std::string>> const tokenStrings =
    {
      { L_TAG_OPEN, "<" },
      { L_TAG_CLOSE, "</" },
      { R_TAG_OPEN, ">" },
      { R_TAG_CLOSE, "/>" },
      { L_TAG_DOCTYPE, "<!"},
      { COMMENT_OPEN, "<!---" },
      { COMMENT_CLOSE, "--->" },
      { ASSIGN,  "=" },
      { ID, "ID"},
      { VALUE, "Value"},
      { TEXT, "Text"},
      { ATTRIBUTE, "Attr"},
    };

    virtual void next() override
    {
      if(this->match("/*"))
      {
        comment();
      }
      else if(this->match("<"))
      {
        tag();
      }
      else
      {
        text();
      }
    }

    void attribute()
    {
      int sRow = this->row;
      int sCol = this->col;

      std::string str;

      whitespace();

      while(true)
      {
        if(this->match(" ") || this->match("/>") || this->match('>') || this->match('=') || this->match(EOF))
        {
          break;
        }
        str.push_back(this->buffer.front());
        this->consume();
      }

      if(!str.empty())
      {
        boost::to_lower(str);
        this->tokenContainer.push_back(CToken(this->tokenStringMap, htmlTokenTypes::ATTRIBUTE, str, sRow, sCol));
      }
    }

    void id()
    {
      int sRow = this->row;
      int sCol = this->col;

      std::string str;

      whitespace();

      while(true)
      {
        if(this->match(" ") || this->match("/>") || this->match('>') || this->match('=') || this->match(EOF))
        {
          break;
        }
        str.push_back(this->buffer.front());
        this->consume();
      }

      if(!str.empty())
      {
        boost::to_lower(str);
        this->tokenContainer.push_back(CToken(this->tokenStringMap, htmlTokenTypes::ID, str, sRow, sCol));
      }
    }

    void whitespace()
    {
      while(std::isspace(this->buffer.front()))
      {
        this->consume();
      }
    }

    void tag()
    {
      if(this->peek('!', 1))
      {
        this->tokenContainer.push_back(CToken(this->tokenStringMap, htmlTokenTypes::L_TAG_DOCTYPE, std::string(""), this->row, this->col));
        this->consume(2); // <!
      }
      else if(this->peek('/', 1))
      {
        this->tokenContainer.push_back(CToken(this->tokenStringMap, htmlTokenTypes::L_TAG_CLOSE, std::string(""), this->row, this->col));
        this->consume(2); // </
      }
      else
      {
        this->tokenContainer.push_back(CToken(this->tokenStringMap, htmlTokenTypes::L_TAG_OPEN, std::string(""), this->row, this->col));
        this->consume(); // <
      }

      whitespace();
      id();

      tokenID_t type;

      int sRow;
      int sCol;

      while(true)
      {
        if(this->match("="))
        {
          this->tokenContainer.push_back(CToken(this->tokenStringMap, htmlTokenTypes::ASSIGN, this->row, this->col));
          this->consume();
        }
        else if(this->match('"'))
        {
          value();
        }
        else if(this->match("/>"))
        {
          type = R_TAG_CLOSE;
          sRow = this->row;
          sCol = this->col;
          this->consume(2); // />
          break;
        }
        else if(this->match(">"))
        {
          type = R_TAG_OPEN;
          sRow = this->row;
          sCol = this->col;
          this->consume(); // >
          break;
        }
        else if(this->match(EOF))
        {
          sRow = this->row;
          sCol = this->col;
          break;
        }
        else
        {
          attribute();
        }
      }

      this->tokenContainer.push_back(CToken(this->tokenStringMap, type, this->row, this->col));
    }

    void comment()
    {
      this->tokenContainer.push_back(CToken(this->tokenStringMap, COMMENT_OPEN, this->row, this->col));

      this->consume(4); // <!--

      whitespace();

      int sRow = this->row;
      int sCol = this->col;

      std::string str;

      while(true)
      {
        if(this->match("-->") || this->match(EOF))
        {
          break;
        }

        str.push_back(this->buffer.front());
        this->consume();
      }

      boost::trim(str);

      if(!str.empty())
      {
        this->tokenContainer.push_back(CToken(this->tokenStringMap, htmlTokenTypes::TEXT, str, sRow, sCol));
      }

      this->tokenContainer.push_back(CToken(this->tokenStringMap, htmlTokenTypes::COMMENT_CLOSE, this->row, this->col));

      this->consume(3); // -->
    }

    void value()
    {
      this->consume(); // "

      int sRow = this->row;
      int sCol = this->col;

      std::string str;

      while(true)
      {
        if(this->match('"') || this->match(EOF))
        {
          break;
        }
        str.push_back(this->buffer.front());
        this->consume();
      }

      this->consume(); // "

      if(!str.empty())
      {
        this->tokenContainer.push_back(CToken(this->tokenStringMap, htmlTokenTypes::VALUE, str, this->row, this->col));
      }
    }

    void text()
    {
      int sRow = this->row;
      int sCol = this->col;

      std::string str;

      whitespace();

      while(true)
      {
        if(this->match('<') || this->match(EOF))
        {
          break;
        }

        str.push_back(this->buffer.front());
        this->consume();
      }

      boost::trim(str);

      if(!str.empty())
      {
        this->tokenContainer.push_back(CToken(this->tokenStringMap, htmlTokenTypes::TEXT, str, sRow, sCol));
      }
    }

  };

} // namesapce

#endif
