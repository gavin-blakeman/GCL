//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// SUBSYSTEM:           Parsers::HTML Parser
// FILE:                htmlParser.h
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
// OVERVIEW:            Class that parsers the tokens
//
// CLASSES INCLUDED:
//
// HISTORY:             2024-06-18 GGB - File Created
//
//**********************************************************************************************************************************/

#ifndef GCL_PARSERS_HTML_HTMLPARSER_H
#define GCL_PARSERS_HTML_HTMLPARSER_H

// Standard C++ library header files
#include <istream>
#include <stack>

// GCL header files.
#include "include/parsers/token.h"
#include "include/parsers/html/htmlDocument.h"
#include "include/parsers/html/htmlNodeElement.h"

namespace GCL::parsers::html
{
  /* The lexer breaks the input stream into tokens. The parser then builds the DOM tree from the tokens. This requires knowledge of
   * the HTML5 standard. The output from the parser is the DOM.
   */
  class CHTMLParser
  {
  public:
    CHTMLParser(std::istream &is, CHTMLDocument &d) : inputStream(is), DOM(d) {}
    ~CHTMLParser() = default;

    /*! @brief      Parses the entire document and stores the result in the DOM.
     *  @throws
     */
    void parseDocument();

  protected:
    enum insertionMode_e
    { IM_INITIAL,
    };

    insertionMode_e insertionMode = IM_INITIAL;

  private:
    CHTMLParser() = delete;
    CHTMLParser(CHTMLParser const &) = delete;
    CHTMLParser(CHTMLParser &&) = delete;
    CHTMLParser &operator=(CHTMLParser const &) = delete;
    CHTMLParser &operator=(CHTMLParser &&) = delete;

    using element_ref = CHTMLDocument::value_ref;

    std::istream &inputStream;
    std::vector<GCL::parsers::CToken> tokens;
    CHTMLDocument &DOM;

    /*! @brief      Parses a single token.
     *  @param[in]  token: The token to parse.
     *  @throws
     */
    void parseToken(CToken const &token);

    void parseTokenModeInitial(CToken const &token);

  };
} // namespace



#endif /* GCL_PARSERS_HTML_HTMLPARSER_H */
