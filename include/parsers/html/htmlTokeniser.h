//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// SUBSYSTEM:           Parsers::HTML Parser
<<<<<<<< HEAD:include/parsers/html/htmlTokeniser.h
// FILE:                htmlLexer.h
========
// FILE:                htmlTokens.h
>>>>>>>> 3e5bc50 (Html parsers dev (#3)):include/parsers/html/htmlLanguageTokens.h
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
<<<<<<<< HEAD:include/parsers/html/htmlTokeniser.h
// OVERVIEW:            Class that lexes the html stream
//
// CLASSES INCLUDED:
//
// HISTORY:             2024-06-18 GGB - File Created
//
//**********************************************************************************************************************************


#ifndef PARSERS_HTML_HTMLTOKENISER_H
#define PARSERS_HTML_HTMLTOKENISER_H

// Standard C++ library header files
#include <utility>

// Parsers library header files.
#include "include/parsers/lexer.hpp"
#include "include/parsers/html/htmlLanguageTokens.h"

namespace GCL::parsers::html
{
  class CHTMLTokeniser : public CLexer
  {
  public:
    using tokenID_t = CToken::tokenID_t;

    CHTMLTokeniser(std::istream &is) : CLexer(is) {}
    virtual ~CHTMLTokeniser() = default;

    CToken getToken();

  private:
    CHTMLTokeniser() = delete;
    CHTMLTokeniser(CHTMLTokeniser const &) = delete;
    CHTMLTokeniser(CHTMLTokeniser &&) = delete;
    CHTMLTokeniser &operator=(CHTMLTokeniser const &) = delete;
    CHTMLTokeniser &operator=(CHTMLTokeniser &&) = delete;

    enum smState_e
    {
      SM_DATA,
      SM_RCDATA, SM_RCDATA_LESSTHAN,
      SM_CHARACTER_REFERENCE,
      SM_TAG_OPEN, SM_TAG_NAME, SM_END_TAG_OPEN, SM_TAG_SELF_CLOSING_START,
      SM_MARKUP_DECLARATION_OPEN,
      SM_BOGUS_COMMENT,
      SM_BEFORE_ATTR_NAME,
      SM_RAWTEXT, SM_RAWTEXT_LESSTHAN,
      SM_SCRIPT, SM_SCRIPT_LESSTHAN,
      SM_PLAINTEXT,
    };

    smState_e smState = SM_DATA;
    smState_e retState;

    bool processData(CToken &);
    bool processTagOpen(CToken &);
    bool processEndTagOpen(CToken &);
    bool processTagName(CToken &);
    bool processPlainText(CToken &);
    bool processRawText(CToken &);
    bool processRCData(CToken &);
    bool processRCDataLessThan(CToken &);
    bool processScript(CToken &);
  };

} // namesapce

#endif // PARSERS_HTML_HTMLTOKENISER_H
========
// OVERVIEW:            Class that parsers the tokens
//
// CLASSES INCLUDED:
//
// HISTORY:             2024-08-05 GGB - File Created
//
//**********************************************************************************************************************************/

#ifndef PARSERS_HTML_TOKENS_H
#define PARSERS_HTML_TOKENS_H

// parsers library
#include "include/parsers/token.h"
#include "include/parsers/languageTokens.h"

namespace GCL::parsers::html
{
  enum htmlTokenTypes : CToken::tokenID_t
  {
    L_TAG_OPEN = TT_NEXT,     // <
    L_TAG_CLOSE,              // </
    R_TAG_OPEN,               // >
    R_TAG_CLOSE,              // />
    L_TAG_DOCTYPE,            // <!
    COMMENT_OPEN,             // <!---
    COMMENT_CLOSE,            // --->
    ASSIGN,                   // =
    ID,
    VALUE,
    TEXT,
    ATTRIBUTE,
  };

}

#endif // PARSERS_HTML_TOKENS_H
>>>>>>>> 3e5bc50 (Html parsers dev (#3)):include/parsers/html/htmlLanguageTokens.h
