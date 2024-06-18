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

#include "include/parsers/lexer.h"

namespace GCL::parsers::html
{
  class CHTMLLexer : public CLexer
  {
    public:
      CHTMLLexer(std::istream &is, std::vector<CToken> &tokens);
      virtual ~CHTMLLexer() = default;

    private:
      CHTMLLexer() = delete;
      CHTMLLexer(CHTMLLexer const &) = delete;
      CHTMLLexer(CHTMLLexer &&) = delete;
      CHTMLLexer &operator=(CHTMLLexer const &) = delete;
      CHTMLLexer &operator=(CHTMLLexer &&) = delete;

      virtual void next() override;

      void attribute();
      void id();
      void whitespace();
      void tag();
      void comment();
      void value();
      void text();

  };

} // namesapce

#endif
