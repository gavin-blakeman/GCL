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
#include <queue>
#include <utility>

// Parsers library header files.
#include "include/parsers/html/bufferEncoder.hpp"
#include "include/parsers/html/htmlTokens.h"

namespace GCL::parsers::html
{
  class CHTMLTokeniser : public CBufferEncoder
  {
  public:
    using value_type = CBufferEncoder::value_type;
    using token_type = CHTMLToken;
    using string_type = CHTMLToken::string_type;

    CHTMLTokeniser(std::istream &is) : CBufferEncoder(is) {}
    virtual ~CHTMLTokeniser() = default;

    token_type getToken();

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
      SM_BEFORE_ATTR_NAME, SM_ATTR_NAME, SM_AFTER_ATTR_NAME, SM_BEFORE_ATTR_VALUE, SM_ATTR_VALUE_DOUBLE_QUOTED,
      SM_ATTR_VALUE_SINGLE_QUOTED, SM_ATTR_VALUE_UNQUOTED, SM_AFTER_ATTR_VALUE_QUOTED,
      SM_RAWTEXT, SM_RAWTEXT_LESSTHAN, SM_RAWTEXT_END_TAG_OPEN, SM_RAWTEXT_END_TAG_NAME,
      SM_SCRIPT, SM_SCRIPT_LESSTHAN, SM_SCRIPT_END_TAG_OPEN, SM_SCRIPT_END_TAG_NAME,
      SM_SCRIPT_ESCAPE_START, SM_SCRIPT_ESCAPE_START_DASH, SM_SCRIPT_ESCAPED,
      SM_SCRIPT_ESCAPED_DASH, SM_SCRIPT_ESCAPED_START_DASH_DASH, SM_SCRIPT_ESCAPED_LESSTHAN, SM_SCRIPT_ESCAPED_END_TAG_OPEN,
      SM_SCRIPT_DOUBLE_ESCAPE_START, SM_SCRIPT_DOUBLE_ESCAPED, SM_SCRIPT_DOUBLE_ESCAPED_DASH, SM_SCRIPT_DOUBLE_ESCAPED_LESSTHAN,
      SM_SCRIPT_ESCAPED_END_TAG_NAME, SM_SCRIPT_DOUBLE_ESCAPED_DASH_DASH, SM_SCRIPT_DOUBLE_ESCAPE_END,
      SM_PLAINTEXT,
      SM_COMMENT_START, SM_COMMENT_START_DASH, SM_COMMENT, SM_COMMENT_END, SM_COMMENT_END_DASH, SM_COMMENT_LESSTHAN,
      SM_COMMENT_LESSTHAN_BANG, SM_COMMENT_LESSTHAN_BANG_DASH, SM_COMMENT_LESSTHAN_BANG_DASH_DASH, SM_COMMENT_END_BANG,
      SM_DOCTYPE, SM_BEFORE_DOCTYPE_NAME, SM_DOCTYPE_NAME, SM_AFTER_DOCTYPE_NAME, SM_BOGUS_DOCTYPE,
      SM_AFTER_DOCTYPE_PUBLIC_KEYWORD, SM_AFTER_DOCTYPE_SYSTEM_KEYWORD, SM_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER,
      SM_DOCTYPE_PUBLIC_IDENTIFER_SINGLE_QUOTED,
    };

    smState_e smState = SM_DATA;
    smState_e retState = SM_NONE;

    string_type temporaryBuffer;
    string_type lastStartTag;

    bool processData(token_type &);
    bool processTagOpen(token_type &);
    bool processEndTagOpen(token_type &);
    bool processTagName(token_type &);
    bool processPlainText(token_type &);
    bool processRawText(token_type &);
    bool processRawTextLessThan(token_type &);
    bool processRawTextEndTagOpen(token_type &);
    bool processRawTextEndTagName(token_type &);
    bool processRCData(token_type &);
    bool processRCDataLessThan(token_type &);
    bool processRCDataEndTagOpen(token_type &);
    bool processRCDataEndTagName(token_type &);
    bool processScript(token_type &);
    bool processScriptLessThan(token_type &);
    bool processScriptEndTagOpen(token_type &);
    bool processScriptEndTagName(token_type &);
    bool processScriptEscapeStart(token_type &);
    bool processScriptEscapeStartDash(token_type &);
    bool processScriptEscaped(token_type &);
    bool processScriptEscapedDash(token_type &);
    bool processScriptEscapedDashDash(token_type &);
    bool processScriptEscapedLessThan(token_type &);
    bool processScriptEscapedEndTagOpen(token_type &);
    bool processScriptEscapedEndTagName(token_type &);
    bool processScriptDoubleEscapeStart(token_type &);
    bool processScriptDoubleEscaped(token_type &);            // 13.2.5.27
    bool processScriptDoubleEscapedDash(token_type &);        // 13.2.5.28
    bool processScriptDoubleEscapedDashDash(token_type &);    // 13.2.5.29
    bool processScriptDoubleEscapedLessThan(token_type &);    // 13.2.5.30
    bool processScriptDoubleEscapeEnd(token_type &);          // 13.2.5.31
    bool processBeforeAttrName(token_type &);                 // 13.2.5.32
    bool processAttrName(token_type &);                       // 13.2.5.33
    bool processAfterAttrName(token_type &);                  // 13.2.5.34
    bool processBeforeAttrValueStart(token_type &);           // 13.2.5.35
    bool processAttrValueDoubleQuoted(token_type &);          // 13.2.5.36
    bool processAttrValueSingleQuoted(token_type &);          // 13.2.5.37
    bool processAttrValueUnquoted(token_type &);              // 13.2.5.38
    bool processAfterAttrValueQuoted(token_type &);           // 13.2.5.39
    bool processSelfClosingStartTag(token_type &);            // 13.2.5.40
    bool processBogusComment(token_type &);                   // 13.2.5.41
    bool processMarkupDeclarationOpen(token_type &);          // 13.2.5.42
    bool processCommentStart(token_type &);                   // 13.2.5.43
    bool processCommentStartDash(token_type &);               // 13.2.5.44
    bool processComment(token_type &);                        // 13.2.5.45
    bool processCommentLessThan(token_type &);                // 13.2.5.46
    bool processCommentLessThanBang(token_type &);            // 13.2.5.47
    bool processCommentLessThanBangDash(token_type &);        // 13.2.5.48
    bool processCommentLessThanBangDashDash(token_type &);    // 13.2.5.49
    bool processCommentEndDash(token_type &);                 // 13.2.5.50
    bool processCommentEnd(token_type &);                     // 13.2.5.51
    bool processCommentEndBang(token_type &);                 // 13.2.5.52
    bool processDocType(token_type &);                        // 13.2.5.53
    bool processBeforeDocTypeName(token_type &);              // 13.2.5.54
    bool processDocTypeName(token_type &);                    // 13.2.5.55
    bool processAfterDocTypeName(token_type &);               // 13.2.5.56
    bool processAfterDocTypePublicKeyword(token_type &);      // 13.2.5.57
    bool processBeforeDocTypePublicIdentifier(token_type &);  // 13.2.5.58

    inline bool emitCharacter(token_type &t, codePoint_t cp)
    {
      t.type(token_type::TT_CHARACTER);
      t.appendData(cp);
      return true;
    }
    inline bool emitEOF(token_type &t) { t.type(token_type::TT_EOF); return true; }
  };

} // namesapce

#endif // PARSERS_HTML_HTMLTOKENISER_H
