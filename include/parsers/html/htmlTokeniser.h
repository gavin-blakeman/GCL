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
//**********************************************************************************************************************************/

#ifndef PARSERS_HTML_HTMLTOKENISER_H
#define PARSERS_HTML_HTMLTOKENISER_H

// Standard C++ library header files
#include <queue>
#include <utility>

// Parsers library header files.
#include "include/parsers/html/htmlBuffer.hpp"
#include "include/parsers/html/htmlTokens.h"

namespace GCL::parsers::html
{
  class CHTMLTokeniser : public CHTMLBuffer
  {
  public:
    using char_type = CHTMLBuffer::char_type;
    using token_type = CHTMLToken;
    using string_type = CHTMLToken::string_type;

    CHTMLTokeniser(std::istream &is) : CHTMLBuffer(is) {}
    virtual ~CHTMLTokeniser() = default;

    /*! @brief      Gets the next token from the stream.
     *  @returns    A single token.
     *  @note       1. html-standard 13.2.5 states that each token needs to be handled by the tree construction stage. As the tree construction
     *                 stage may affect the tokenisation stage, and may insert new characters into the stream.
     *              2. Some tokenisation steps result in more than one token being emitted. This function will only return the next token from
     *                 FIFO.
     */
    token_type getToken();

#ifdef TEST
  protected:
#else
  private:
#endif
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

      SM_CHARACTER_REFERENCE, SM_NUMERIC_CHARACTER_REFERENCE, SM_NAMED_CHARACTER_REFERENCE, SM_AMBIGUOUS_AMPERSAND,
      SM_HEXADECIMAL_CHARACTER_REFERENCE_START, SM_DECIMAL_CHARACTER_REFERENCE_START, SM_HEXADECIMAL_CHARACTER_REFERENCE,
      SM_DECIMAL_CHARACTER_REFERENCE, SM_NUMERIC_CHARACTER_REFERENCE_END,

      SM_TAG_OPEN, SM_TAG_NAME, SM_END_TAG_OPEN, SM_TAG_SELF_CLOSING_START,
      SM_MARKUP_DECLARATION_OPEN,

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
      SM_BOGUS_COMMENT,

      SM_DOCTYPE, SM_BEFORE_DOCTYPE_NAME, SM_DOCTYPE_NAME, SM_AFTER_DOCTYPE_NAME, SM_BOGUS_DOCTYPE,
      SM_AFTER_DOCTYPE_PUBLIC_KEYWORD, SM_AFTER_DOCTYPE_SYSTEM_KEYWORD, SM_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER,
      SM_DOCTYPE_PUBLIC_IDENTIFIER_DOUBLE_QUOTED, SM_DOCTYPE_PUBLIC_IDENTIFIER_SINGLE_QUOTED, SM_AFTER_DOCTYPE_PUBLIC_IDENTIFIER,
      SM_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED, SM_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED,
      SM_BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS, SM_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER, SM_AFTER_DOCTYPE_SYSTEM_IDENTIFIER,

      SM_CDATA_SECTION, SM_CDATA_SECTION_BRACKET, SM_CDATA_SECTION_END,
    };

    std::queue<token_type> tokenFIFO;
    smState_e smState = SM_DATA;
    smState_e retState = SM_NONE;
    string_type temporaryBuffer;
    string_type lastStartTag;
    std::uint32_t charRefCode;
    bool emit = false;

    void processData();
    void processTagOpen();                                // 13.2.5.6
    void processEndTagOpen();                             // 13.2.5.7
    void processTagName();
    void processPlainText();
    void processRawText();
    void processRawTextLessThan();
    void processRawTextEndTagOpen();
    void processRawTextEndTagName();
    void processRCData();
    void processRCDataLessThan();
    void processRCDataEndTagOpen();
    void processRCDataEndTagName();
    void processScript();
    void processScriptLessThan();
    void processScriptEndTagOpen();
    void processScriptEndTagName();
    void processScriptEscapeStart();
    void processScriptEscapeStartDash();
    void processScriptEscaped();
    void processScriptEscapedDash();
    void processScriptEscapedDashDash();
    void processScriptEscapedLessThan();
    void processScriptEscapedEndTagOpen();
    void processScriptEscapedEndTagName();
    void processScriptDoubleEscapeStart();
    void processScriptDoubleEscaped();                    // 13.2.5.27
    void processScriptDoubleEscapedDash();                // 13.2.5.28
    void processScriptDoubleEscapedDashDash();            // 13.2.5.29
    void processScriptDoubleEscapedLessThan();            // 13.2.5.30
    void processScriptDoubleEscapeEnd();                  // 13.2.5.31
    void processBeforeAttrName();                         // 13.2.5.32
    void processAttrName();                               // 13.2.5.33
    void processAfterAttrName();                          // 13.2.5.34
    void processBeforeAttrValueStart();                   // 13.2.5.35
    void processAttrValueDoubleQuoted();                  // 13.2.5.36
    void processAttrValueSingleQuoted();                  // 13.2.5.37
    void processAttrValueUnquoted();                      // 13.2.5.38
    void processAfterAttrValueQuoted();                   // 13.2.5.39
    void processSelfClosingStartTag();                    // 13.2.5.40
    void processBogusComment();                           // 13.2.5.41
    void processMarkupDeclarationOpen();                  // 13.2.5.42
    void processCommentStart();                           // 13.2.5.43
    void processCommentStartDash();                       // 13.2.5.44
    void processComment();                                // 13.2.5.45
    void processCommentLessThan();                        // 13.2.5.46
    void processCommentLessThanBang();                    // 13.2.5.47
    void processCommentLessThanBangDash();                // 13.2.5.48
    void processCommentLessThanBangDashDash();            // 13.2.5.49
    void processCommentEndDash();                         // 13.2.5.50
    void processCommentEnd();                             // 13.2.5.51
    void processCommentEndBang();                         // 13.2.5.52
    void processDocType();                                // 13.2.5.53
    void processBeforeDocTypeName();                      // 13.2.5.54
    void processDocTypeName();                            // 13.2.5.55
    void processAfterDocTypeName();                       // 13.2.5.56
    void processAfterDocTypePublicKeyword();              // 13.2.5.57
    void processBeforeDocTypePublicIdentifier();          // 13.2.5.58
    void processDocTypePublicIdentifierDoubleQuoted();    // 13.2.5.59
    void processDocTypePublicIdentifierSingleQuoted();    // 13.2.5.60
    void processAfterDocTypePublicIdentifier();           // 13.2.5.61
    void processBetweenDocTypePublicSystmeIdentifiers();  // 13.2.5.62
    void processAfterDocTypeSystemKeyword();              // 13.2.5.63
    void processBeforeDocTypeSystemIdentifier();          // 13.2.5.64
    void processDocTypeSystemIdentifierDoubleQuoted();    // 13.2.5.65
    void processDocTypeSystemIdentifierSingleQuoted();    // 13.2.5.66
    void processAfterDocTypeSystemIdentifier();           // 13.2.5.67
    void processBogusDocType();                           // 13.2.5.68
    void processCDataSection();                           // 13.2.5.69
    void processCDataSectionBracket();                    // 13.2.5.70
    void processCDataSectionEnd();                        // 13.2.5.71
    void processCharacterReference();                     // 13.2.5.72
    void processNamedCharacterReference();                // 13.2.5.73
    void processAmbiguousAmpersand();                     // 13.2.5.74
    void processNumericCharacterReference();              // 13.2.5.75
    void processHexadecimalCharacterReferenceStart();     // 13.2.5.76
    void processDecimalCharacterReferenceStart();         // 13.2.6.77
    void processHexadecimalCharacterReference();          // 13.2.6.78
    void processDecimalCharacterReference();              // 13.2.6.79
    void processNumericCharacterReferenceEnd();           // 13.2.6.80

    inline void emitCharacter(char_type c)
    {
      tokenFIFO.emplace(token_type::TT_CHARACTER, c);
      emit = true;
    }

    inline void emitEOF()
    {
      tokenFIFO.emplace(token_type::TT_EOF);
      emit = true;
    }

    inline void createTokenTagEnd()
    {
      tokenFIFO.emplace(token_type::TT_TAG_END);
    }

    inline void createTokenTagStart()
    {
      tokenFIFO.emplace(token_type::TT_TAG_START);
    }

    inline void createTokenComment()
    {
      tokenFIFO.emplace(token_type::TT_COMMENT);
    }

    inline void createTokenDocType(bool qm)
    {
      tokenFIFO.emplace(token_type::TT_DOCTYPE);
      tokenFIFO.back().forceQuirks(qm);
    }

    inline void emitCharacter(string_type const &s)
    {
      for (auto const &c: s)
      {
        emitCharacter(c);
      }
    }

    inline void emitCharacterTemporaryBuffer()
    {
      emitCharacter(temporaryBuffer);
      temporaryBuffer.clear();
    }

  };

} // namesapce

#endif // PARSERS_HTML_HTMLTOKENISER_H
