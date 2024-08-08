//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// SUBSYSTEM:           Parsers::HTML Parser
// FILE:                htmlTokeniser.h
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
      SM_NONE,
      SM_DATA,
      SM_RCDATA, SM_RCDATA_LESSTHAN, SM_RCDATA_END_TAG_OPEN, SM_RCDATA_END_TAG_NAME,
      SM_CHARACTER_REFERENCE,
      SM_TAG_OPEN, SM_TAG_NAME, SM_END_TAG_OPEN, SM_TAG_SELF_CLOSING_START,
      SM_MARKUP_DECLARATION_OPEN,
      SM_BOGUS_COMMENT,
      SM_BEFORE_ATTR_NAME,
      SM_RAWTEXT, SM_RAWTEXT_LESSTHAN, SM_RAWTEXT_END_TAG_OPEN, SM_RAWTEXT_END_TAG_NAME,
      SM_SCRIPT, SM_SCRIPT_LESSTHAN, SM_SCRIPT_END_TAG_OPEN, SM_SCRIPT_END_TAG_NAME,
      SM_SCRIPT_ESCAPE_START, SM_SCRIPT_ESCAPE_START_DASH, SM_SCRIPT_ESCAPED,
      SM_SCRIPT_ESCAPED_DASH, SM_SCRIPT_ESCAPED_START_DASH_DASH, SM_SCRIPT_ESCAPED_LESSTHAN, SM_SCRIPT_ESCAPED_END_TAG_OPEN,
      SM_SCRIPT_DOUBLE_ESCAPE_START, SM_SCRIPT_ESCAPED_END_TAG_NAME,
      SM_PLAINTEXT,
    };

    smState_e smState = SM_DATA;
    smState_e retState = SM_NONE;

    std::string temporaryBuffer;
    std::string lastStartTag;

    bool processData(CToken &);
    bool processTagOpen(CToken &);
    bool processEndTagOpen(CToken &);
    bool processTagName(CToken &);
    bool processPlainText(CToken &);
    bool processRawText(CToken &);
    bool processRawTextLessThan(CToken &);
    bool processRawTextEndTagOpen(CToken &);
    bool processRawTextEndTagName(CToken &);
    bool processRCData(CToken &);
    bool processRCDataLessThan(CToken &);
    bool processRCDataEndTagOpen(CToken &);
    bool processRCDataEndTagName(CToken &);
    bool processScript(CToken &);
    bool processScriptLessThan(CToken &);
    bool processScriptEndTagOpen(CToken &);
    bool processScriptEndTagName(CToken &);
    bool processScriptEscapeStart(CToken &);
    bool processScriptEscapeStartDash(CToken &);
    bool processScriptEscaped(CToken &);
    bool processScriptEscapedDash(CToken &);
    bool processScriptEscapedDashDash(CToken &);
    bool processScriptEscapedLessThan(CToken &);
    bool processScriptEscapedEndTagOpen(CToken &);
    bool processScriptEscapedEndTagName(CToken &);
  };

} // namesapce

#endif // PARSERS_HTML_HTMLTOKENISER_H
