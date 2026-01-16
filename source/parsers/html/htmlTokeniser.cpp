#include "include/parsers/html/htmlTokeniser.h"

// Standard C++ library header files
#include <cctype>

// parsers include files
#include "include/parsers/html/htmlExceptions.h"

/* Note on Encoding:
 * HTML from the web may be encoded in UTF8 or UTF16. It is also possible that html may be provided as UTF32. The tokeniser needs to
 * take this into account. The buffer/encoder class provides code-points to the tokeniser. The buffer/encoded takes care of converting the
 * input-stream into UTF32. While UTF32 incurs an overhead in conversion, the benefit is that all comparisons are 32bit to 32bit, without
 * having to worry about the number of bytes involved. (If you convert a codepoint to utf8, the output can be a variable number of bytes,
 * this incurs overhead in having to determine the number of bytes.)
 * NOTE: ****IMPORTANT**** The EOF character is defined as 0xFF. This is an unused codepoint in the utf8 mapping.
 * The tokeniser then only needs to consider unsigned comparisons of 32 bit integers. This is quick on modern processors.
 * To correctly handle multi-byte sequences, the codePoint_t type is provided. The buffer/encoder always provides a codePoint
 * to the tokeniser. The code-point is freely convertible between the various representations. (Note the codePoint always represents
 * a single codePoint.)
 * Where necessary, the tokeniser forwards the codePoint onto the tree builder.
 *
 * In summary:
 * The encoder reads the input stream and provides codePoints to the tokeniser.
 * The tokeniser treats all codePoints as UTF32 internally.
 * The tokeniser passes codePoints onto the treeBuilder.
 */

namespace GCL::parsers::html
{
  static CHTMLTokeniser::string_type S32_SCRIPT{'s', 'c', 'r', 'i', 'p', 't'};
  static CHTMLTokeniser::string_type S32_MINUSMINUS{'-', '-'};
  static CHTMLTokeniser::string_type S32_DOCTYPE{'o', 'c', 't', 'y', 'p', 'e'};
  static CHTMLTokeniser::string_type S32_CDATA{'[','C', 'D', 'A', 'T', 'A', '['};
  static CHTMLTokeniser::string_type S32_PUBLIC{'u', 'b', 'l', 'i', 'c'};
  static CHTMLTokeniser::string_type S32_SYSTEM{'y', 's', 't', 'e', 'm'};

  CHTMLTokeniser::token_type CHTMLTokeniser::getToken()
  {
    if (tokenFIFO.empty())
    {
      emit = false;
      while (!emit)
      {
        consume();
        switch(smState)
        {
          case SM_DATA:
          {
            processData();
            break;
          }
          case SM_TAG_OPEN: // 13.2.5.6
          {
            processTagOpen();
            break;
          }
          case SM_END_TAG_OPEN:
          {
            processEndTagOpen();
            break;
          }
          case SM_TAG_NAME:
          {
            processTagName();
            break;
          }
          case SM_RCDATA:
          {
            processRCData();
            break;
          }
          case SM_RCDATA_LESSTHAN:
          {
            processRCDataLessThan();
            break;
          }
          case SM_RAWTEXT_END_TAG_OPEN:
          {
            processRawTextEndTagOpen();
            break;
          }
          case SM_RAWTEXT_END_TAG_NAME:
          {
            processRawTextEndTagName();
            break;
          }
          case SM_RCDATA_END_TAG_OPEN:
          {
            processRCDataEndTagOpen();
            break;
          }
          case SM_RCDATA_END_TAG_NAME:
          {
            processRCDataEndTagName();
            break;
          }
          case SM_RAWTEXT:
          {
            processRawText();
            break;
          }
          case SM_RAWTEXT_LESSTHAN:
          {
            processRawTextLessThan();
            break;
          }
          case SM_SCRIPT:
          {
            processScript();
            break;
          }
          case SM_SCRIPT_LESSTHAN:
          {
            processScriptLessThan();
            break;
          }
          case SM_SCRIPT_ESCAPED_END_TAG_NAME:
          {
            processScriptEscapedEndTagName();
            break;
          }
          case SM_SCRIPT_END_TAG_OPEN:
          {
            processScriptEndTagOpen();
            break;
          }
          case SM_SCRIPT_END_TAG_NAME:
          {
            processScriptEndTagName();
            break;
          }
          case SM_SCRIPT_ESCAPED:
          {
            processScriptEscaped();
            break;
          }
          case SM_SCRIPT_ESCAPED_DASH:
          {
            processScriptEscapedDash();
            break;
          }
          case SM_SCRIPT_ESCAPED_END_TAG_OPEN:
          {
            processScriptEscapedEndTagOpen();
            break;
          }
          case SM_SCRIPT_ESCAPED_START_DASH_DASH:
          {
            processScriptEscapedDashDash();
            break;
          }
          case SM_SCRIPT_ESCAPED_LESSTHAN:
          {
            processScriptEscapedLessThan();
            break;
          }
          case SM_SCRIPT_ESCAPE_START:
          {
            processScriptEscapeStart();
            break;
          }
          case SM_SCRIPT_ESCAPE_START_DASH:
          {
            processScriptEscapeStartDash();
            break;
          }
          case SM_SCRIPT_DOUBLE_ESCAPE_START:
          {
            processScriptDoubleEscapeStart();
            break;
          }
          case SM_SCRIPT_DOUBLE_ESCAPED:
          {
            processScriptDoubleEscaped();
            break;
          }
          case SM_SCRIPT_DOUBLE_ESCAPED_DASH: // 13.2.5.28
          {
            processScriptDoubleEscapedDash();
            break;
          }
          case SM_SCRIPT_DOUBLE_ESCAPED_DASH_DASH: // 13.2.5.29
          {
            processScriptDoubleEscapedDashDash();
            break;
          }
          case SM_PLAINTEXT:
          {
            processPlainText();
            break;
          }
          case SM_SCRIPT_DOUBLE_ESCAPED_LESSTHAN: // 13.2.5.30
          {
            processScriptDoubleEscapedLessThan();
            break;
          }
          case SM_SCRIPT_DOUBLE_ESCAPE_END: // 13.2.5.31
          {
            processScriptDoubleEscapeEnd();
            break;
          }
          case SM_BEFORE_ATTR_NAME: // 13.2.5.32
          {
            processBeforeAttrName();
            break;
          }
          case SM_ATTR_NAME:  // 13.2.5.33
          {
            processAttrName();
            break;
          }
          case SM_AFTER_ATTR_NAME:  // 13.2.5.34
          {
            processAfterAttrName();
            break;
          }
          case SM_BEFORE_ATTR_VALUE:  // 13.2.5.35
          {
            processBeforeAttrValueStart();
            break;
          }
          case SM_ATTR_VALUE_DOUBLE_QUOTED:  // 13.2.5.36
          {
            processAttrValueDoubleQuoted();
            break;
          }
          case SM_ATTR_VALUE_SINGLE_QUOTED:  // 13.2.5.37
          {
            processAttrValueSingleQuoted();
            break;
          }
          case SM_ATTR_VALUE_UNQUOTED:  // 13.2.5.38
          {
            processAttrValueUnquoted();
            break;
          }
          case SM_AFTER_ATTR_VALUE_QUOTED: // 13.2.5.39
          {
            processAfterAttrValueQuoted();
            break;
          }
          case SM_TAG_SELF_CLOSING_START: // 13.2.5.40
          {
            processSelfClosingStartTag();
            break;
          }
          case SM_BOGUS_COMMENT: // 13.2.5.41
          {
            processBogusComment();
            break;
          }
          case SM_MARKUP_DECLARATION_OPEN: // 13.2.5.42
          {
            processMarkupDeclarationOpen();
            break;
          }
          case SM_COMMENT_START: // 13.2.5.43
          {
            processCommentStart();
            break;
          }
          case SM_COMMENT_START_DASH: // 13.2.5.44
          {
            processCommentStartDash();
            break;
          }
          case SM_COMMENT: // 13.2.5.45
          {
            processComment();
            break;
          }
          case SM_COMMENT_LESSTHAN: // 13.2.5.46
          {
            processCommentLessThan();
            break;
          }
          case SM_COMMENT_LESSTHAN_BANG: // 13.2.5.47
          {
            processCommentLessThanBang();
            break;
          }
          case SM_COMMENT_LESSTHAN_BANG_DASH: // 13.2.5.48
          {
            processCommentLessThanBangDash();
            break;
          }
          case SM_COMMENT_LESSTHAN_BANG_DASH_DASH: // 13.2.5.49
          {
            processCommentLessThanBangDashDash();
            break;
          }
          case SM_COMMENT_END_DASH: // 13.2.5.50
          {
            processCommentEndDash();
            break;
          }
          case SM_COMMENT_END: // 13.2.5.51
          {
            processCommentEnd();
            break;
          }
          case SM_COMMENT_END_BANG: // 13.2.5.52
          {
            processCommentEndBang();
            break;
          }
          case SM_DOCTYPE: // 13.2.5.53
          {
            processDocType();
            break;
          }
          case SM_BEFORE_DOCTYPE_NAME: // 13.2.5.54
          {
            processBeforeDocTypeName();
            break;
          }
          case SM_DOCTYPE_NAME: // 13.2.5.55
          {
            processDocTypeName();
            break;
          }
          case SM_AFTER_DOCTYPE_NAME: // 13.2.5.56
          {
            processAfterDocTypeName();
            break;
          }
          case SM_AFTER_DOCTYPE_PUBLIC_KEYWORD: // 13.2.5.57
          {
            processAfterDocTypePublicKeyword();
            break;
          }
          case SM_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER:  // 13.2.5.58
          {
            processBeforeDocTypePublicIdentifier();
            break;
          }
          case SM_DOCTYPE_PUBLIC_IDENTIFIER_DOUBLE_QUOTED: // 13.2.5.59
          {
            processDocTypePublicIdentifierDoubleQuoted();
            break;
          }
          case SM_DOCTYPE_PUBLIC_IDENTIFIER_SINGLE_QUOTED: // 13.2.5.60
          {
            processDocTypePublicIdentifierSingleQuoted();
            break;
          }
          case SM_AFTER_DOCTYPE_PUBLIC_IDENTIFIER: // 13.2.5.61
          {
            processAfterDocTypePublicIdentifier();
            break;
          }
          case SM_BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS: // 13.2.5.62
          {
            processBetweenDocTypePublicSystmeIdentifiers();
            break;
          }
          case SM_AFTER_DOCTYPE_SYSTEM_KEYWORD: // 13.2.5.63
          {
            processAfterDocTypeSystemKeyword();
            break;
          }
          case SM_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER: // 13.2.5.64
          {
            processBeforeDocTypeSystemIdentifier();
            break;
          }
          case SM_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED: // 13.2.5.65
          {
            processDocTypeSystemIdentifierDoubleQuoted();
            break;
          }
          case SM_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED: // 13.2.5.66
          {
            processDocTypeSystemIdentifierSingleQuoted();
            break;
          }
          case SM_AFTER_DOCTYPE_SYSTEM_IDENTIFIER: // 13.2.5.67
          {
            processAfterDocTypeSystemIdentifier();
            break;
          }
          case SM_BOGUS_DOCTYPE: // 13.2.5.68
          {
            processBogusDocType();
            break;
          }
          case SM_CDATA_SECTION: // 13.2.5.69
          {
            processCDataSection();
            break;
          }
          case SM_CDATA_SECTION_BRACKET: // 13.2.5.70
          {
            processCDataSectionBracket();
            break;
          }
          case SM_CDATA_SECTION_END: // 13.2.5.71
          {
            processCDataSectionEnd();
            break;
          }
          case SM_CHARACTER_REFERENCE: // 13.2.5.72
          {
            processCharacterReference();
            break;
          }
          case SM_NAMED_CHARACTER_REFERENCE:  // 13.2.5.73
          {
            processNamedCharacterReference();
            break;
          }
          case SM_AMBIGUOUS_AMPERSAND:  // 13.2.5.74
          {
            processAmbiguousAmpersand();
            break;
          }
          case SM_NUMERIC_CHARACTER_REFERENCE: // 13.2.5.75
          {
            processNumericCharacterReference();
            break;
          }
          case SM_HEXADECIMAL_CHARACTER_REFERENCE_START:  // 13.2.5.76
          {
            processHexadecimalCharacterReferenceStart();
            break;
          }
          case SM_DECIMAL_CHARACTER_REFERENCE_START:  // 13.2.5.77
          {
            processDecimalCharacterReferenceStart();
            break;
          }
          case SM_HEXADECIMAL_CHARACTER_REFERENCE: // 13.2.5.78
          {
            processHexadecimalCharacterReference();
            break;
          }
          case SM_DECIMAL_CHARACTER_REFERENCE:  // 13.2.5.79
          {
            processDecimalCharacterReference();
            break;
          }
          case SM_NUMERIC_CHARACTER_REFERENCE_END: // 13.2.5.80
          {
            processNumericCharacterReferenceEnd();
            break;
          }
        }
      }
    }
    token_type token = tokenFIFO.front();
    tokenFIFO.pop();
    return token;
  }

  // 13.2.5.1
  void CHTMLTokeniser::processData()
  {
    switch(currentChar)
    {
      case U_0026:  // '&'
      {
        retState = SM_DATA;
        smState = SM_CHARACTER_REFERENCE;
        break;
      }
      case U_003C: // '<'
      {
        smState = SM_TAG_OPEN;
        break;
      };
      case U_EOF:
      {
        emitEOF();
        break;
      }
      case U_0000:
      {
        PARSE_ERROR("Unexpected null character");
        [[fallthrough]];
      }
      default:
      {
        emitCharacter(currentChar);
      }
    }
  }

  void CHTMLTokeniser::processPlainText()
  {
    switch(currentChar)
    {
      case U_0000:
      {
        PARSE_ERROR("unexpected null character");
        emitCharacter(U_FFFD);
        break;
      }
      case U_EOF:
      {
        emitEOF();
        break;
      }
      default:
      {
        emitCharacter(currentChar);
        break;
      }
    }
  }

  // 13.2.5.3
  void CHTMLTokeniser::processRawText()
  {
    switch(currentChar)
    {
      case U_003C:
      {
        smState = SM_RAWTEXT_LESSTHAN;
        break;
      };
      case U_0000:
      {
        PARSE_ERROR("Unexpected null character");
        emitCharacter(U_FFFD);
        break;
      }
      case U_EOF:
      {
        emitEOF();
        break;
      }
      default:
      {
        emitCharacter(currentChar);
        break;
      }
    }
  }

  // 13.2.5.14
  void CHTMLTokeniser::processRawTextEndTagName()
  {
    switch(currentChar)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      case U_002F:
      {
        if (tokenFIFO.back().name() == lastStartTag)
        {
          if (currentChar == U_002F)
          {
            smState = SM_TAG_SELF_CLOSING_START;
          }
          else
          {
            smState = SM_BEFORE_ATTR_NAME;
          }
        }
        else
        {
          emitCharacter({U_003C, U_002F});
          emitCharacterTemporaryBuffer();
          smState = SM_RAWTEXT;
          reconsume();
        }
        break;
      }
      case U_003E:
      {
        if (tokenFIFO.back().name() == lastStartTag)
        {
          emit = true;
          smState = SM_DATA;
        }
        else
        {
          emitCharacter(U_003C);
          emitCharacter(U_002F);
          emitCharacterTemporaryBuffer();
          smState = SM_RAWTEXT;
          reconsume();
        }
        break;
      }
      default:
      {
        if (currentChar.isalpha())
        {
          tokenFIFO.back().appendName(currentChar.tolower());
          temporaryBuffer.push_back(currentChar);
        }
        else
        {
          emitCharacter(U_003C);
          emitCharacter(U_002F);
          emitCharacterTemporaryBuffer();
          smState = SM_RAWTEXT;
          reconsume();
        }
      }
    }
  }

  // 13.2.5.13
  void CHTMLTokeniser::processRawTextEndTagOpen()
  {
    if (currentChar.isalpha())
    {
      createTokenTagEnd();
      reconsume();
      smState = SM_RAWTEXT_END_TAG_NAME;
    }
    else
    {
      emitCharacter(U_003C);
      emitCharacter(U_002F);
      smState = SM_RAWTEXT;
      reconsume();
    }
  }

  // 13.2.5.12
  void CHTMLTokeniser::processRawTextLessThan()
  {
    if (currentChar == U_002F)
    {
      temporaryBuffer.clear();
      smState = SM_RAWTEXT_END_TAG_OPEN;
    }
    else
    {
      emitCharacter(U_003C);
      smState = SM_RAWTEXT;
      reconsume();
    }
  }

  // 13.2.5.2
  void CHTMLTokeniser::processRCData()
  {
    switch(currentChar)
    {
      case U_0026:
      {
        retState = SM_RCDATA;
        smState = SM_CHARACTER_REFERENCE;
        break;
      }
      case U_003C:
      {
        smState = SM_RCDATA_LESSTHAN;
        break;
      };
      case U_0000:
      {
        PARSE_ERROR("unexpexted null character");
        emitCharacter(U_FFFD);
        break;
      }
      case U_EOF:
      {
        emitEOF();
        break;
      }
      default:
      {
        emitCharacter(currentChar);
        break;
      }
    }
  }

  // 13.2.5.11
  void CHTMLTokeniser::processRCDataEndTagName()
  {
    switch(currentChar)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      case U_002F:
      {
        if (tokenFIFO.back().name() == lastStartTag)
        {
          if (currentChar == U_002F)
          {
            smState = SM_TAG_SELF_CLOSING_START;
          }
          else
          {
            smState = SM_BEFORE_ATTR_NAME;
          }
        }
        else
        {
          emitCharacter(U_003C);
          emitCharacter(U_002F);
          emitCharacterTemporaryBuffer();
          reconsume();
          smState = SM_RCDATA;
        }
        break;
      }
      case U_003E:
      {
        if (tokenFIFO.back().name() == lastStartTag)
        {
          emit = true;
          smState = SM_DATA;
        }
        else
        {
          emitCharacter(U_003C);
          emitCharacter(U_002F);
          emitCharacterTemporaryBuffer();
          reconsume();
          smState = SM_RCDATA;
        }
        break;
      }
      default:
      {
        if (currentChar.isalpha())
        {
          tokenFIFO.back().appendName(currentChar.tolower());
          temporaryBuffer.push_back(currentChar);
        }
        else
        {
          emitCharacter(U_003C);
          emitCharacter(U_002F);
          emitCharacterTemporaryBuffer();
          smState = SM_RCDATA;
          reconsume();
        }
      }
    }
  }

  // 13.2.5.10
  void CHTMLTokeniser::processRCDataEndTagOpen()
  {
    if (currentChar.isalpha())
    {
      createTokenTagEnd();
      reconsume();
      smState = SM_RCDATA_END_TAG_NAME;
    }
    else
    {
      emitCharacter(U_003C);
      emitCharacter(U_002F);
      smState = SM_RCDATA;
      reconsume();
    }
  }

  // 13.2.5.9
  void CHTMLTokeniser::processRCDataLessThan()
  {
    switch(currentChar)
    {
      case U_002F:	// '/'
      {
        temporaryBuffer.clear();
        smState = SM_RCDATA_END_TAG_OPEN;
        break;
      }
      default:
      {
        emitCharacter(U_003C);
        smState = SM_RCDATA;
        reconsume();
        break;
      }
    }
  }

  // 12.2.5.4
  void CHTMLTokeniser::processScript()
  {
    switch(currentChar)
    {
      case U_003C:
      {
        smState = SM_SCRIPT_LESSTHAN;
        break;
      };
      case U_0000:
      {
        PARSE_ERROR("Unexpected null character");
        emitCharacter(U_FFFD);
        break;
      }
      case U_EOF:
      {
        emitEOF();
        break;
      }
      default:
      {
        emitCharacter(currentChar);
        break;
      }
    }
  }

  // 13.2.5.27
  void CHTMLTokeniser::processScriptDoubleEscaped()
  {
    switch (currentChar)
    {
      case U_002D:
      {
        emitCharacter(U_002D);
        smState = SM_SCRIPT_DOUBLE_ESCAPED_DASH;
        break;
      }
      case U_003C:
      {
        emitCharacter(U_003C);
        smState = SM_SCRIPT_DOUBLE_ESCAPED_LESSTHAN;
        break;
      }
      case U_0000:
      {
        PARSE_ERROR("Unexpected null character");
        emitCharacter(U_FFFD);
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in script/html/comment like text");
        emitEOF();
        break;
      }
      default:
      {
        emitCharacter(currentChar);
        break;
      }
    }
  }

  // 13.2.5.26
  void CHTMLTokeniser::processScriptDoubleEscapeStart()
  {
    switch(currentChar)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      case U_002F:
      case U_003E:
      {
        if (temporaryBuffer == S32_SCRIPT)
        {
          smState = SM_SCRIPT_DOUBLE_ESCAPED;
        }
        else
        {
          emitCharacter(currentChar);
          smState = SM_SCRIPT_ESCAPED;
        }
        break;
      }
      default:
      {
        if (currentChar.isalpha())
        {
          emitCharacter(currentChar);
          temporaryBuffer.push_back(currentChar.tolower());
        }
        else
        {
          reconsume();
          smState = SM_SCRIPT_ESCAPED;
        }
        break;
      }
    }
  }
  // 13.2.5.17
  void CHTMLTokeniser::processScriptEndTagName()
  {
    switch(currentChar)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      case U_002F:
      {
        if (tokenFIFO.back().name() == lastStartTag)
        {
          if (currentChar == U_002F)
          {
            smState = SM_TAG_SELF_CLOSING_START;
          }
          else
          {
            smState = SM_BEFORE_ATTR_NAME;
          }
        }
        else
        {
          emitCharacter(U_003C);
          emitCharacter(U_002F);
          emitCharacterTemporaryBuffer();
          smState = SM_SCRIPT;
          reconsume();
        }
        break;
      }
      case 0x003E:
      {
        if (tokenFIFO.back().name() == lastStartTag)
        {
          emit = true;
          smState = SM_DATA;
        }
        else
        {
          emitCharacter({ U_003C, U_002F });
          emitCharacterTemporaryBuffer();
          smState = SM_SCRIPT;
          reconsume();
        }
        break;
      }
      default:
      {
        if (std::isalpha(currentChar))
        {
          tokenFIFO.back().appendName(currentChar.tolower());
          temporaryBuffer.push_back(currentChar);
        }
        else
        {
          emitCharacter({U_003C, U_002F});
          emitCharacterTemporaryBuffer();
          smState = SM_SCRIPT;
          reconsume();
        }
      }
    }
  }

  // 13.2.5.16
  void CHTMLTokeniser::processScriptEndTagOpen()
  {
    if (std::isalpha(currentChar))
    {
      createTokenTagEnd();
      smState = SM_SCRIPT_END_TAG_NAME;
      reconsume();
    }
    else
    {
      emitCharacter({U_003C, U_002F});
      smState = SM_SCRIPT;
      reconsume();
    }
  }

  // 13.2.5.20
  void CHTMLTokeniser::processScriptEscaped()
  {
    switch (currentChar)
    {
      case U_002D:
      {
        smState = SM_SCRIPT_ESCAPED_DASH;
        emitCharacter(U_002D);
        break;
      }
      case U_003C:
      {
        smState = SM_SCRIPT_ESCAPED_LESSTHAN;
        break;
      }
      case U_0000:
      {
        PARSE_ERROR("Unexpected null character");
        emitCharacter(U_FFFD);
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in script, html,comment like text");
        emitEOF();
        break;
      }
      default:
      {
        emitCharacter(currentChar);
        break;
      }
    }
  }

  void CHTMLTokeniser::processScriptEscapedDash()
  {
    switch(currentChar)
    {
      case U_002D:
      {
        emitCharacter(U_002D);
        smState = SM_SCRIPT_ESCAPED_START_DASH_DASH;
        break;
      }
      case U_003C:
      {
        smState = SM_SCRIPT_ESCAPED_LESSTHAN;
        break;
      }
      case 0x0000:
      {
        PARSE_ERROR("Unexpected null character");
        emitCharacter(U_FFFD);
        smState = SM_SCRIPT_ESCAPED;
        break;
      }
      case 0xFFFF:  // EOF
      {
        PARSE_ERROR("eof in script, html,comment like text");
        emitEOF();
        break;
      }
      default:
      {
        emitCharacter(currentChar);
        smState = SM_SCRIPT_ESCAPED;
        break;
      }
    }
  }

  // 13.2.5.22
  void CHTMLTokeniser::processScriptEscapedDashDash()
  {
    switch(currentChar)
    {
      case U_002D:
      {
        emitCharacter(U_002D);
        break;
      }
      case U_003C:
      {
        smState = SM_SCRIPT_ESCAPED_LESSTHAN;
        break;
      }
      case U_003E:
      {
        emitCharacter(U_003E);
        smState = SM_SCRIPT;
        break;
      }
      case U_0000:
      {
        PARSE_ERROR("Unexpected null character");
        smState = SM_SCRIPT_ESCAPED;
        emitCharacter(U_FFFD);
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in script, html,comment like text");
        emitEOF();
        break;
      }
      default:
      {
        emitCharacter(currentChar);
        smState = SM_SCRIPT_ESCAPED;
        break;
      }
    }
  }

  // 13.2.5.25
  void CHTMLTokeniser::processScriptEscapedEndTagName()
  {
    switch(currentChar)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      case U_002F:
      {
        if (tokenFIFO.back().name() == lastStartTag)
        {
          if (currentChar == U_002F)
          {
            smState = SM_TAG_SELF_CLOSING_START;
          }
          else
          {
            smState = SM_BEFORE_ATTR_NAME;
          }
        }
        else
        {
          emitCharacter({U_003C, U_002F});
          emitCharacterTemporaryBuffer();
          smState = SM_SCRIPT_ESCAPED;
          reconsume();
        }
        break;
      }
      case 0x003E:
      {
        if (tokenFIFO.back().name() == lastStartTag)
        {
          emit = true;
          smState = SM_DATA;
        }
        else
        {
          emitCharacter({U_003C, U_002F});
          emitCharacterTemporaryBuffer();
          smState = SM_SCRIPT_ESCAPED;
          reconsume();
        }
        break;
      }
      default:
      {
        if (std::isalpha(currentChar))
        {
          tokenFIFO.back().appendName(currentChar.tolower());
          temporaryBuffer.push_back(currentChar);
        }
        else
        {
          emitCharacter({U_003C, U_002F});
          emitCharacterTemporaryBuffer();
          smState = SM_SCRIPT_ESCAPED;
          reconsume();
        }
      }
    }
  }

  // 13.2.5.24
  void CHTMLTokeniser::processScriptEscapedEndTagOpen()
  {
    if (std::isalpha(currentChar))
    {
      createTokenTagEnd();
      smState = SM_SCRIPT_ESCAPED_END_TAG_NAME;
      reconsume();
    }
    else
    {
      emitCharacter({U_003C, U_002F});
      smState = SM_SCRIPT_ESCAPED;
      reconsume();
    }
  }

  // 13.2.5.23
  void CHTMLTokeniser::processScriptEscapedLessThan()
  {
    switch(currentChar)
    {
      case U_002F:
      {
        temporaryBuffer.clear();
        smState = SM_SCRIPT_ESCAPED_END_TAG_OPEN;
        break;
      }
      default:
      {
        if (std::isalpha(currentChar))
        {
          temporaryBuffer.clear();
          emitCharacter(U_003C);
          smState = SM_SCRIPT_DOUBLE_ESCAPE_START;
          reconsume();
        }
        else
        {
          emitCharacter(U_003C);
          smState = SM_SCRIPT_ESCAPED;
          reconsume();
        }
        break;
      }
    }
  }

  // 13.2.5.18
  void CHTMLTokeniser::processScriptEscapeStart()
  {
    if (currentChar == U_002D)
    {
      emitCharacter(U_002D);
      smState = SM_SCRIPT_ESCAPE_START_DASH;
    }
    else
    {
      smState = SM_SCRIPT;
      reconsume();
    }
  }

  // 13.2.5.19
  void CHTMLTokeniser::processScriptEscapeStartDash()
  {
    if (currentChar == U_002D)
    {
      emitCharacter(U_002D);
      smState = SM_SCRIPT_ESCAPED_START_DASH_DASH;
    }
    else
    {
      smState = SM_SCRIPT;
      reconsume();
    }
  }

  // 13.2.5.15
  void CHTMLTokeniser::processScriptLessThan()
  {
    switch (currentChar)
    {
      case U_002F:
      {
        temporaryBuffer.clear();
        smState = SM_RCDATA_END_TAG_OPEN;
        break;
      }
      case U_0021:
      {
        emitCharacter({U_003C, U_0021});
        smState = SM_SCRIPT_ESCAPE_START;
        break;
      }
      default:
      {
        emitCharacter(U_003C);
        smState = SM_SCRIPT;
        reconsume();
        break;
      }
    }
  }

  // 13.2.5.7
  void CHTMLTokeniser::processEndTagOpen()
  {
    if (std::isalpha(currentChar))
    {
      createTokenTagEnd();
      smState = SM_TAG_NAME;
      reconsume();
    }
    else if (currentChar == U_003E) // '>'
    {
      PARSE_ERROR("missing end tag name");
      smState = SM_DATA;
    }
    else if (currentChar == U_EOF)
    {
      PARSE_ERROR("eof before tag name");
      emitEOF();
      emitCharacter({U_003C, U_002F});
    }
    else
    {
      PARSE_ERROR("invalid first character of tag name");
      createTokenComment();
      smState = SM_BOGUS_COMMENT;
      reconsume();
    }
  }

  // 13.2.5.8
  void CHTMLTokeniser::processTagName()
  {
    switch(currentChar)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      {
        smState = SM_BEFORE_ATTR_NAME;
        break;
      }
      case U_002F:  // '/'
      {
        smState = SM_TAG_SELF_CLOSING_START;
        break;
      }
      case U_003E:
      {
        emit = true;
        smState = SM_DATA;
        break;
      };
      case U_0000:
      {
        PARSE_ERROR("unexpected null character");
        tokenFIFO.back().appendName(U_FFFD);
        break;
      }
      case 0xFFFF:  // EOF
      {
        PARSE_ERROR("eof in tag");
        emitEOF();
        break;
      }
      default:
      {
        tokenFIFO.back().appendName(currentChar.tolower());
        break;
      }
    };
  };

  // 13.2.5.6
  void CHTMLTokeniser::processTagOpen()
  {
    switch (currentChar)
    {
      case U_0021:  // '!'
      {
        smState = SM_MARKUP_DECLARATION_OPEN;
        break;
      }
      case U_002F:  // '/'
      {
        smState = SM_END_TAG_OPEN;
        break;
      }
      case U_003F:  // '?/
      {
        smState = SM_BOGUS_COMMENT;
        createTokenComment();
        reconsume();
        break;
      }
      case U_EOF:
      {
        throw parse_error("eof before tag name");
        emitCharacter(U_003C);
        emitEOF();
        break;
      }
      default:
      {
        if (currentChar.isalpha())
        {
          createTokenTagStart();
          smState = SM_TAG_NAME;
        }
        else
        {
          emitCharacter(U_003C);
          smState = SM_DATA;
        }
        reconsume();
        break;
      }
    }
  }

  // 13.2.5.28
  void CHTMLTokeniser::processScriptDoubleEscapedDash()
  {
    switch(currentChar)
    {
      case U_002D:
      {
        emitCharacter(U_002D);
        smState = SM_SCRIPT_DOUBLE_ESCAPED_DASH_DASH;
        break;
      }
      case U_003C:
      {
        emitCharacter(U_003C);
        smState = SM_SCRIPT_DOUBLE_ESCAPED_LESSTHAN;
        break;
      }
      case U_0000:
      {
        PARSE_ERROR("unexpected null character");
        emitCharacter(U_FFFD);
        smState = SM_SCRIPT_DOUBLE_ESCAPED;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in script/html/comment line text");
        emitEOF();
        break;
      }
      default:
      {
        emitCharacter(currentChar);
        smState = SM_SCRIPT_DOUBLE_ESCAPED;
        break;
      };
    }
  }

  // 13.2.5.29
  void CHTMLTokeniser::processScriptDoubleEscapedDashDash()
  {
    switch(currentChar)
    {
      case U_002D:
      {
        emitCharacter(U_002D);
        break;
      }
      case U_003C:
      {
        emitCharacter(U_003C);
        smState = SM_SCRIPT_DOUBLE_ESCAPED_LESSTHAN;
        break;
      }
      case U_003E:
      {
        emitCharacter(U_003E);
        smState = SM_SCRIPT;
        break;
      }
      case U_0000:
      {
        PARSE_ERROR("unexpected null character");
        emitCharacter(U_FFFD);
        smState = SM_SCRIPT_DOUBLE_ESCAPED;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in script/html/comment like text");
        emitEOF();
        break;
      }
    }
  }

  // 13.2.5.30
  void CHTMLTokeniser::processScriptDoubleEscapedLessThan()
  {
    if (currentChar == U_002F)
    {
      emitCharacter(U_002F);
      temporaryBuffer.clear();
      smState = SM_SCRIPT_DOUBLE_ESCAPE_END;
    }
    else
    {
      smState = SM_SCRIPT_DOUBLE_ESCAPED;
      reconsume();
    }
  }

  // 13.2.5.31
  void CHTMLTokeniser::processScriptDoubleEscapeEnd()
  {
    switch(currentChar)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      case U_002F:
      case U_003E:
      {
        emitCharacter(currentChar);
        if (temporaryBuffer == S32_SCRIPT)
        {
          smState = SM_SCRIPT_ESCAPED;
        }
        else
        {
          smState = SM_SCRIPT_DOUBLE_ESCAPED;
        }
        break;
      }
      default:
      {
        if (currentChar.isalpha())
        {
          temporaryBuffer.push_back(currentChar.tolower());
          emitCharacter(currentChar);
        }
        else
        {
          smState = SM_SCRIPT_DOUBLE_ESCAPED;
          reconsume();
        }
      }
    }
  }

  // 13.2.5.32
  void CHTMLTokeniser::processBeforeAttrName()
  {
    switch (currentChar)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      {
        // Ignore the character.
        break;
      }
      case U_002F:
      case U_003E:
      case U_EOF:
      {
        smState = SM_AFTER_ATTR_NAME;
        reconsume();
        break;
      }
      case U_003D:
      {
        PARSE_ERROR("unexpected equals sign before attribute name");
        tokenFIFO.back().attrStart();
        tokenFIFO.back().attrConcatName(currentChar);
        smState = SM_ATTR_NAME;
        break;
      }
      default:
      {
        tokenFIFO.back().attrStart();
        smState = SM_ATTR_NAME;
        reconsume();
        break;
      };
    }
  }

  // 13.2.5.33
  void CHTMLTokeniser::processAttrName()
  {
    switch(currentChar)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      case U_002F:
      case U_003E:
      case U_EOF:
      {
        smState = SM_AFTER_ATTR_NAME;
        reconsume();
        break;
      }
      case U_003D:
      {
        smState = SM_BEFORE_ATTR_VALUE;
        break;
      }
      case U_0000:
      {
        PARSE_ERROR("unexpected null character");
        tokenFIFO.back().attrConcatName(U_FFFD);
        break;
      }
      case U_0022:
      case U_0027:
      case U_003C:
      {
        PARSE_ERROR("unexpected character in attribute name");
        [[fallthrough]];
      }
      default:
      {
        tokenFIFO.back().attrConcatName(currentChar.tolower());
        break;
      }
    }
  }

  // 13.2.5.34
  void CHTMLTokeniser::processAfterAttrName()
  {
    switch (currentChar)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      {
        // Ignore the character
        break;
      }
      case U_002F:
      {
        smState = SM_TAG_SELF_CLOSING_START;
        break;
      }
      case U_003D:
      {
        smState = SM_BEFORE_ATTR_VALUE;
        break;
      }
      case U_003E:
      {
        emit = true;
        smState = SM_DATA;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in tag");
        emitEOF();
        break;
      }
      default:
      {
        tokenFIFO.back().attrStart();
        smState = SM_ATTR_NAME;
        reconsume();
        break;
      }
    }
  }

  // 13.2.5.35
  void CHTMLTokeniser::processBeforeAttrValueStart()
  {
    switch(currentChar)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      {
        // Ignore the character.
        break;
      }
      case U_0022:
      {
        smState = SM_ATTR_VALUE_DOUBLE_QUOTED;
        break;
      }
      case U_0027:
      {
        smState = SM_ATTR_VALUE_SINGLE_QUOTED;
        break;
      }
      case U_003E:
      {
        PARSE_ERROR("missing attribute value");
        emit = true;
        smState = SM_DATA;
        break;
      }
      default:
      {
        smState = SM_ATTR_VALUE_UNQUOTED;
        reconsume();
        break;
      }
    }
  }

  // 13.2.5.36
  void CHTMLTokeniser::processAttrValueDoubleQuoted()
  {
    switch(currentChar)
    {
      case U_0022:
      {
        smState = SM_AFTER_ATTR_VALUE_QUOTED;
        break;
      }
      case U_0026:
      {
        retState = smState;
        smState = SM_CHARACTER_REFERENCE;
        break;
      }
      case U_0000:
      {
        PARSE_ERROR("unexpected null character");
        tokenFIFO.back().attrConcatValue(U_FFFD);
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in tag");
        emitEOF();
        break;
      }
      default:
      {
        tokenFIFO.back().attrConcatValue(currentChar);
        break;
      }
    }
  }

  // 13.2.5.37
  void CHTMLTokeniser::processAttrValueSingleQuoted()
  {
    switch(currentChar)
    {
      case U_0027:
      {
        smState = SM_AFTER_ATTR_VALUE_QUOTED;
        break;
      }
      case U_0026:
      {
        retState = smState;
        smState = SM_CHARACTER_REFERENCE;
        break;
      }
      case U_0000:
      {
        PARSE_ERROR("unexpected null character");
        tokenFIFO.back().attrConcatValue(U_FFFD);
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in tag");
        emitEOF();
        break;
      }
      default:
      {
        tokenFIFO.back().attrConcatValue(currentChar);
        break;
      }
    }
  }

  // 13.2.5.38
  void CHTMLTokeniser::processAttrValueUnquoted()
  {
    switch(currentChar)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      {
        smState = SM_BEFORE_ATTR_NAME;
        break;
      }
      case U_0026:
      {
        retState = smState;
        smState = SM_CHARACTER_REFERENCE;
        break;
      }
      case U_003E:
      {
        smState = SM_DATA;
        emit = true;
        break;
      }
      case U_0000:
      {
        PARSE_ERROR("unexpected null character");
        tokenFIFO.back().attrConcatValue(U_FFFD);
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in tag");
        emitEOF();
        break;
      }
      case U_0022:
      case U_0027:
      case U_003C:
      case U_003D:
      case U_0060:
      {
        PARSE_ERROR("unexpected character in unquoted attribute value");
        [[fallthrough]];
      }
      default:
      {
        tokenFIFO.back().attrConcatValue(currentChar);
        break;
      }
    }
  }

  // 13.2.5.39
  void CHTMLTokeniser::processAfterAttrValueQuoted()
  {
    switch(currentChar)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      {
        smState = SM_BEFORE_ATTR_NAME;
        break;
      }
      case U_002F:
      {
        smState = SM_TAG_SELF_CLOSING_START;
        break;
      }
      case U_003E:
      {
        smState = SM_DATA;
        emit = true;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in tag");
        emitEOF();
        break;
      }
      default:
      {
        PARSE_ERROR("missing whitespace between attributes");
        smState = SM_BEFORE_ATTR_NAME;
        reconsume();
        break;
      }
    }
  }

  // 13.2.5.40
  void CHTMLTokeniser::processSelfClosingStartTag()
  {
    switch(currentChar)
    {
      case U_003E:  // '>'
      {
        tokenFIFO.back().selfClosing(true);
        smState = SM_DATA;
        emit = true;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in tag");
        emitEOF();
        break;
      }
      default:
      {
        PARSE_ERROR("unexpected solidus in tag");
        smState = SM_BEFORE_ATTR_NAME;
        reconsume();
        break;
      }
    }
  }

  // 13.2.5.41
  void CHTMLTokeniser::processBogusComment()
  {
    switch(currentChar)
    {
      case U_003E:
      {
        smState = SM_DATA;
        emit = true;
        break;
      }
      case U_EOF:
      {
        // This appears to be the best way to handle the double emit required.
        emitEOF();
        break;
      }
      case U_0000:
      {
        PARSE_ERROR("unexpected null character");
        tokenFIFO.back().appendData(U_FFFD);
        break;
      }
      default:
      {
        tokenFIFO.back().appendData(currentChar);
        break;
      }
    }
  }

  // 13.2.5.42
  void CHTMLTokeniser::processMarkupDeclarationOpen()
  {
    if (match(S32_MINUSMINUS, true))
    {
      consume(S32_MINUSMINUS.size());
      createTokenComment();
      smState = SM_COMMENT_START;
    }
    else if (match(S32_DOCTYPE, false))
    {
      consume(S32_DOCTYPE.size());
      smState = SM_DOCTYPE;
    }
    else if (match(S32_CDATA, true))
    {
      IMPLEMENT_ME();
    }
    else
    {
      PARSE_ERROR("incorrectly opened comment");
      createTokenComment();
      smState = SM_BOGUS_COMMENT;
    }
  }

  // 13.2.5.43
  void CHTMLTokeniser::processCommentStart()
  {
    switch (currentChar)
    {
      case U_002D:
      {
        smState = SM_COMMENT_START_DASH;
        break;
      }
      case U_003E:
      {
        PARSE_ERROR("abrupt closing of empty comment");
        smState = SM_DATA;
        emit = true;
        break;
      }
      default:
      {
        smState = SM_COMMENT;
        reconsume();
        break;
      }
    }
  }

  // 13.2.5.44
  void CHTMLTokeniser::processCommentStartDash()
  {
    switch(currentChar)
    {
      case U_002D:
      {
        smState = SM_COMMENT_END;
        break;
      }
      case U_003E:
      {
        PARSE_ERROR("abrupt closing of empty comment");
        smState = SM_DATA;
        emit = true;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in comment");
        emitEOF();
        break;
      }
      default:
      {
        tokenFIFO.back().appendData(U_002D);
        smState = SM_COMMENT;
        reconsume();
        break;
      }
    }
  }

  // 13.2.5.45
  void CHTMLTokeniser::processComment()
  {
    switch(currentChar)
    {
      case U_003C:
      {
        tokenFIFO.back().appendData(currentChar);
        smState = SM_COMMENT_LESSTHAN;
        break;
      }
      case U_002D:
      {
        smState = SM_COMMENT_END_DASH;
        break;
      }
      case U_0000:
      {
        PARSE_ERROR("unexpected null character");
        tokenFIFO.back().appendData(U_FFFD);
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in comment");
        emitEOF();
        break;
      }
      default:
      {
        tokenFIFO.back().appendData(currentChar);
        break;
      }
    }
  }

  // 13/2/5/46
  void CHTMLTokeniser::processCommentLessThan()
  {
    switch(currentChar)
    {
      case U_0021:
      {
        tokenFIFO.back().appendData(currentChar);
        smState = SM_COMMENT_LESSTHAN_BANG;
        break;
      }
      case U_003C:
      {
        tokenFIFO.back().appendData(currentChar);
        break;
      }
      default:
        smState = SM_COMMENT;
        reconsume();
        break;
    }
  }

  // 13.2.5.47
  void CHTMLTokeniser::processCommentLessThanBang()
  {
    switch(currentChar)
    {
      case U_002D:
      {
        smState = SM_COMMENT_LESSTHAN_BANG_DASH;
        break;
      }
      default:
      {
        smState = SM_COMMENT;
        reconsume();
      }
    }
  }

  // 13.2.5.48
  void CHTMLTokeniser::processCommentLessThanBangDash()
  {
    switch(currentChar)
    {
      case U_002D:
      {
        smState = SM_COMMENT_LESSTHAN_BANG_DASH_DASH;
        break;
      }
      default:
      {
        smState = SM_COMMENT_END_DASH;
        reconsume();
        break;
      }
    }
  }

  // 13.2.5.49
  void CHTMLTokeniser::processCommentLessThanBangDashDash()
  {
    switch (currentChar)
    {
      case U_003E:
      case U_EOF:
      {
        smState = SM_COMMENT_END;
        reconsume();
        break;
      }
      default:
      {
        PARSE_ERROR("nested comment");
        smState = SM_COMMENT_END;
        reconsume();
        break;
      }
    }
  }

  // 13.2.5.50
  void CHTMLTokeniser::processCommentEndDash()
  {
    switch(currentChar)
    {
      case U_002D:
      {
        smState = SM_COMMENT_END;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in comment");
        emitEOF();
        break;
      }
      default:
      {
        tokenFIFO.back().appendName(U_002D);
        smState = SM_COMMENT;
        reconsume();
        break;
      }
    }
  }

  // 13.2.5.51
  void CHTMLTokeniser::processCommentEnd()
  {
    switch(currentChar)
    {
      case U_003E:
      {
        emit = true;
        smState = SM_DATA;
        break;
      }
      case U_0021:
      {
        smState = SM_COMMENT_END_BANG;
        break;
      }
      case U_002D:
      {
        tokenFIFO.back().appendData(currentChar);
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in comment");
        emitEOF();
        break;
      }
      default:
      {
        tokenFIFO.back().appendData({U_002D, U_002D});
        smState = SM_COMMENT;
        reconsume();
        break;
      }
    }
  }

  // 13.2.5.52
  void CHTMLTokeniser::processCommentEndBang()
  {
    switch(currentChar)
    {
      case U_002D:
      {
        tokenFIFO.back().appendData({U_002D, U_002D, U_0021});
        smState = SM_COMMENT_END_DASH;
        break;
      }
      case U_003E:
      {
        PARSE_ERROR("incorrectly closed comment");
        smState = SM_DATA;
        emit = true;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in comment");
        emitEOF();
        break;
      }
      default:
      {
        tokenFIFO.back().appendData({U_002D, U_002D, U_0021});
        smState = SM_COMMENT;
        reconsume();
        break;
      }
    }
  }

  // 13.2.5.53
  void CHTMLTokeniser::processDocType()
  {
    switch(currentChar)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      {
        smState = SM_BEFORE_DOCTYPE_NAME;
        break;
      }
      case U_003E:
      {
        smState = SM_BEFORE_DOCTYPE_NAME;
        reconsume();
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in doctype");
        createTokenDocType(true);
        emitEOF();
        break;
      }
      default:
      {
        PARSE_ERROR("missing whitespace before doctype name");
        smState = SM_BEFORE_DOCTYPE_NAME;
        reconsume();
        break;
      }
    }
  }

  // 13.2.5.54
  void CHTMLTokeniser::processBeforeDocTypeName()
  {
    switch(currentChar)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      {
        // Ignore character.
        break;
      }
      case U_0000:
      {
        PARSE_ERROR("unexpected null character");
        createTokenDocType(false);
        tokenFIFO.back().appendName(U_FFFD);
        smState = SM_DOCTYPE_NAME;
        break;
      }
      case U_003E:
      {
        PARSE_ERROR("missing doctype name");
        createTokenDocType(true);
        smState = SM_DATA;
        emit = true;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in doctype");
        createTokenDocType(true);
        emitEOF();
        break;
      }
      default:
      {
        createTokenDocType(false);
        tokenFIFO.back().appendName(currentChar.tolower());
        smState = SM_DOCTYPE_NAME;
        break;
      }
    }
  }

  // 13.2.5.55
  void CHTMLTokeniser::processDocTypeName()
  {
    switch (currentChar)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      {
        smState = SM_AFTER_DOCTYPE_NAME;
        break;
      }
      case U_003E:
      {
        emit = true;
        smState = SM_DATA;
        break;
      }
      case U_0000:
      {
        PARSE_ERROR("unexpected null character");
        tokenFIFO.back().appendName(U_FFFD);
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in doctype");
        tokenFIFO.back().forceQuirks(true);
        emitEOF();
        reconsume();
        break;
      }
      default:
      {
        tokenFIFO.back().appendName(currentChar.tolower());
        break;
      }
    }
  }

  // 13.2.5.56
  void CHTMLTokeniser::processAfterDocTypeName()
  {
    switch(currentChar)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      {
        // Ignore character.
        break;
      }
      case U_003E:
      {
        emit = true;
        smState = SM_DATA;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in doctype");
        tokenFIFO.back().forceQuirks(true);
        emitEOF();
        break;
      }
      default:
      {
        if ((currentChar.tolower() == 'p') && match(S32_PUBLIC, false))
        {
          consume(S32_PUBLIC.size());
          smState = SM_AFTER_DOCTYPE_PUBLIC_KEYWORD;
          break;
        }
        else if ((currentChar.tolower() == 's') && match(S32_SYSTEM, false))
        {
          consume(S32_SYSTEM.size());
          smState = SM_AFTER_DOCTYPE_SYSTEM_KEYWORD;
          break;
        }
        else
        {
          PARSE_ERROR("invalid character sequence after doctype name");
          tokenFIFO.back().forceQuirks(true);
          smState = SM_BOGUS_DOCTYPE;
          reconsume();
          break;
        }
      }
    }
  }

  // 13.2.5.57
  void CHTMLTokeniser::processAfterDocTypePublicKeyword()
  {
    switch (currentChar)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      {
        smState = SM_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER;
        break;
      }
      case U_0022:
      {
        PARSE_ERROR("missing whitespace after doctype public keyword");
        IMPLEMENT_ME();
        smState = SM_DOCTYPE_PUBLIC_IDENTIFIER_DOUBLE_QUOTED;
        break;
      }
      case U_0027:
      {
        PARSE_ERROR("missing whitespace after doctype public keyword");
        IMPLEMENT_ME();
        smState = SM_DOCTYPE_PUBLIC_IDENTIFIER_SINGLE_QUOTED;
        break;
      }
      case U_003E:
      {
        PARSE_ERROR("missing doctype public identifier");
        tokenFIFO.back().forceQuirks(true);
        emit = true;
        smState = SM_DATA;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in doctype");
        tokenFIFO.back().forceQuirks(true);
        emitEOF();
        break;
      }
      default:
      {
        PARSE_ERROR("missing quote before doctype public identifier");
        tokenFIFO.back().forceQuirks(true);
        smState = SM_BOGUS_DOCTYPE;
        reconsume();
        break;
      }
    }
  }

  // 13.2.5.58
  void CHTMLTokeniser::processBeforeDocTypePublicIdentifier()
  {
    switch (currentChar)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      {
        // Ignore the character.
        break;
      }
      case U_0022:
      {
        tokenFIFO.back().setPublicIdentifierEmpty();
        smState = SM_DOCTYPE_PUBLIC_IDENTIFIER_DOUBLE_QUOTED;
        break;
      }
      case U_0027:
      {
        tokenFIFO.back().setPublicIdentifierEmpty();
        smState = SM_DOCTYPE_PUBLIC_IDENTIFIER_SINGLE_QUOTED;
        break;
      }
      case U_003E:
      {
        PARSE_ERROR("missing doctype public identifier");
        tokenFIFO.back().forceQuirks(true);
        emit = true;
        smState = SM_DATA;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in doctype");
        tokenFIFO.back().forceQuirks(true);
        emitEOF();
        break;
      }
      default:
      {
        PARSE_ERROR("missing quote before doctype public identifier");
        tokenFIFO.back().forceQuirks(true);
        smState = SM_BOGUS_DOCTYPE;
        reconsume();
        break;
      }
    }
  }

  // 13.2.5.59
  void CHTMLTokeniser::processDocTypePublicIdentifierDoubleQuoted()
  {
    switch (currentChar)
    {
      case U_0022:
      {
        smState = SM_AFTER_DOCTYPE_PUBLIC_IDENTIFIER;
        break;
      }
      case U_0000:
      {
        PARSE_ERROR("unexpected null character");
        tokenFIFO.back().appendPublicIdentifier(U_FFFD);
        break;
      }
      case U_003E:
      {
        PARSE_ERROR("abrupt doctype public identifier");
        tokenFIFO.back().forceQuirks(true);
        emit = true;
        smState = SM_DATA;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in doctype");
        tokenFIFO.back().forceQuirks(true);
        emitEOF();
        break;
      }
      default:
      {
        tokenFIFO.back().appendPublicIdentifier(currentChar);
        break;
      }
    }
  }

  // 13.2.5.60
  void CHTMLTokeniser::processDocTypePublicIdentifierSingleQuoted()
  {
    switch (currentChar)
    {
      case U_0027:
      {
        smState = SM_AFTER_DOCTYPE_PUBLIC_IDENTIFIER;
        break;
      }
      case U_0000:
      {
        PARSE_ERROR("unexpected null character");
        tokenFIFO.back().appendPublicIdentifier(U_FFFD);
        break;
      }
      case U_003E:
      {
        PARSE_ERROR("abrupt doctype public identifier");
        tokenFIFO.back().forceQuirks(true);
        emit = true;
        smState = SM_DATA;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in doctype");
        tokenFIFO.back().forceQuirks(true);
        emitEOF();
        break;
      }
      default:
      {
        tokenFIFO.back().appendPublicIdentifier(currentChar);
        break;
      }
    }
  }

  // 12.2.5.61
  void CHTMLTokeniser::processAfterDocTypePublicIdentifier()
  {
    switch(currentChar)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      {
        smState = SM_BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS;
        break;
      }
      case U_003E:
      {
        emit = true;
        smState = SM_DATA;
        break;
      }
      case U_0022:
      {
        PARSE_ERROR("missing whitespace between doctype public and system identifiers");
        tokenFIFO.back().setSystemIdentifierEmpty();
        smState = SM_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED;
        break;
      }
      case U_0027:
      {
        PARSE_ERROR("missing whitespace between doctype public and system identifiers");
        tokenFIFO.back().setSystemIdentifierEmpty();
        smState = SM_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in doctype");
        tokenFIFO.back().forceQuirks(true);
        emitEOF();
        break;
      }
      default:
      {
        PARSE_ERROR("missing quote before doctype system identifier");
        tokenFIFO.back().forceQuirks(true);
        smState = SM_BOGUS_DOCTYPE;
        reconsume();
        break;
      }
    }
  }

  // 13.2.5.62
  void CHTMLTokeniser::processBetweenDocTypePublicSystmeIdentifiers()
  {
    switch (currentChar)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      {
        // Ignore the character.
        break;
      }
      case U_003E:
      {
        emit = true;
        smState = SM_DATA;
        break;
      }
      case U_0022:
      {
        smState = SM_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED;
        break;
      }
      case U_0027:
      {
        smState = SM_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in doctype");
        tokenFIFO.back().forceQuirks(true);
        emitEOF();
        break;
      }
      default:
      {
        PARSE_ERROR("missing quote before doctype system identifier");
        tokenFIFO.back().forceQuirks(true);
        smState = SM_BOGUS_DOCTYPE;
        reconsume();
        break;
      }
    }
  }

  // 13.2.5.63
  void CHTMLTokeniser::processAfterDocTypeSystemKeyword()
  {
    switch(currentChar)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      {
        smState = SM_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER;
        break;
      }
      case U_0022:
      {
        PARSE_ERROR("missing whitespace after doctype system keyyword");
        tokenFIFO.back().setSystemIdentifierEmpty();
        smState = SM_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED;
        break;
      }
      case U_0027:
      {
        PARSE_ERROR("missing whitespace after doctype system keyyword");
        tokenFIFO.back().setSystemIdentifierEmpty();
        smState = SM_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED;
        break;
      }
      case U_003E:
      {
        PARSE_ERROR("missing doctype system identifier");
        tokenFIFO.back().forceQuirks(true);
        emit = true;
        smState = SM_DATA;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in doctype");
        tokenFIFO.back().forceQuirks(true);
        emitEOF();
        break;
      }
      default:
      {
        PARSE_ERROR("missing quote before doctype system identifier");
        tokenFIFO.back().forceQuirks(true);
        smState = SM_BOGUS_DOCTYPE;
        reconsume();
      }
    }
  }

  void CHTMLTokeniser::processBeforeDocTypeSystemIdentifier()
  {
    switch (currentChar)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      {
        // Ignore the character.
        break;
      }
      case U_0022:
      {
        tokenFIFO.back().setSystemIdentifierEmpty();
        smState = SM_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED;
        break;
      }
      case U_0027:
      {
        tokenFIFO.back().setSystemIdentifierEmpty();
        smState = SM_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED;
        break;
      }
      case U_003E:
      {
        PARSE_ERROR("missing doctype system identifier");
        tokenFIFO.back().forceQuirks(true);
        smState = SM_DATA;
        emit = true;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in doctype");
        tokenFIFO.back().forceQuirks(true);
        emitEOF();
        break;
      }
      default:
      {
        PARSE_ERROR("missing quote before doctype system identifier");
        tokenFIFO.back().forceQuirks(true);
        smState = SM_BOGUS_DOCTYPE;
        break;
      }
    }
  }

  // 13.2.5.65
  void CHTMLTokeniser::processDocTypeSystemIdentifierDoubleQuoted()
  {
    switch (currentChar)
    {
      case U_0022:
      {
        smState = SM_AFTER_DOCTYPE_SYSTEM_IDENTIFIER;
        break;
      }
      case U_0000:
      {
        PARSE_ERROR("unexpected null character");
        tokenFIFO.back().appendSystemIdentifier(U_FFFD);
        break;
      }
      case U_003E:
      {
        PARSE_ERROR("abrupt doctype system identifier");
        tokenFIFO.back().forceQuirks(true);
        smState = SM_DATA;
        emit = true;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in doctype");
        tokenFIFO.back().forceQuirks(true);
        emitEOF();
        break;
      }
      default:
      {
        tokenFIFO.back().appendSystemIdentifier(currentChar);
        break;
      }
    }
  }

  // 13.2.5.66
  void CHTMLTokeniser::processDocTypeSystemIdentifierSingleQuoted()
  {
    switch (currentChar)
    {
      case U_0027:
      {
        smState = SM_AFTER_DOCTYPE_SYSTEM_IDENTIFIER;
        break;
      }
      case U_0000:
      {
        PARSE_ERROR("unexpected null character");
        tokenFIFO.back().appendSystemIdentifier(U_FFFD);
        break;
      }
      case U_003E:
      {
        PARSE_ERROR("abrupt doctype system identifier");
        tokenFIFO.back().forceQuirks(true);
        smState = SM_DATA;
        emit = true;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in doctype");
        tokenFIFO.back().forceQuirks(true);
        emitEOF();
        break;
      }
      default:
      {
        tokenFIFO.back().appendSystemIdentifier(currentChar);
        break;
      }
    }
  }

  // 13.2.5.67
  void CHTMLTokeniser::processAfterDocTypeSystemIdentifier()
  {
    switch (currentChar)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      {
        // Ignore the character.
        break;
      }
      case U_003E:
      {
        smState = SM_DATA;
        emit = true;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in doctype");
        tokenFIFO.back().forceQuirks(true);
        emitEOF();
        break;
      }
      default:
      {
        PARSE_ERROR("unexpected character after doctype system identifier");
        smState = SM_BOGUS_DOCTYPE;
        reconsume();
        break;
      }
    }
  }

  // 13.2.5.68
  void CHTMLTokeniser::processBogusDocType()
  {
    switch(currentChar)
    {
      case U_003E:
      {
        smState = SM_DATA;
        emit = true;
        break;
      }
      case U_0000:
      {
        PARSE_ERROR("unexpected null character");
        break;
      }
      case U_EOF:
      {
        emitEOF();
        break;
      }
      default:
      {
        // Ignore the character.
        break;
      }
    }
  }

  // 13.2.5.69
  void CHTMLTokeniser::processCDataSection()
  {
    switch (currentChar)
    {
      case U_005D:
      {
        smState = SM_CDATA_SECTION_BRACKET;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in cdata");
        emitEOF();
        break;
      }
      default:
      {
        emitCharacter(currentChar);
        break;
      }
    }
  }

  // 13.2.5.70
  void CHTMLTokeniser::processCDataSectionBracket()
  {
    switch(currentChar)
    {
      case U_005D:
      {
        smState = SM_CDATA_SECTION_END;
        break;
      }
      default:
      {
        emitCharacter(U_005D);
        smState = SM_CDATA_SECTION;
        reconsume();
        break;
      }
    }
  }

  // 13.2.5.71
  void CHTMLTokeniser::processCDataSectionEnd()
  {
    switch(currentChar)
    {
      case U_005D:
      {
        emitCharacter(U_005D);
        break;
      }
      case U_003E:
      {
        smState = SM_DATA;
        break;
      }
      default:
      {
        emitCharacter(U_005D);
        emitCharacter(U_005D);
        smState = SM_CDATA_SECTION;
        reconsume();
        break;
      }
    }
  }

  // 13.2.5.72
  void CHTMLTokeniser::processCharacterReference()
  {
    temporaryBuffer.clear();
    temporaryBuffer.push_back(U_0026);
    switch (currentChar)
    {
      case U_0023:
      {
        temporaryBuffer.push_back(currentChar);
        smState = SM_NUMERIC_CHARACTER_REFERENCE;
        break;
      }
      default:
      {
        if (currentChar.isalphanumeric())
        {
          smState = SM_NAMED_CHARACTER_REFERENCE;
          reconsume();
        }
        else
        {
          IMPLEMENT_ME();
          smState = retState;
          reconsume();
        }
      }
    }
  }

  // 13.2.5.73
  void CHTMLTokeniser::processNamedCharacterReference()
  {
    IMPLEMENT_ME();
  }

  // 13.2.5.74
  void CHTMLTokeniser::processAmbiguousAmpersand()
  {
    if (currentChar.isalphanumeric())
    {
      IMPLEMENT_ME();
    }
    else if (currentChar == U_003B)
    {
      PARSE_ERROR("unknown named character reference");
      smState = retState;
      reconsume();
    }
    else
    {
      smState = retState;
      reconsume();
    }
  }

  // 13.2.5.75
  void CHTMLTokeniser::processNumericCharacterReference()
  {
    charRefCode = 0;

    switch (currentChar)
    {
      case U_0078:
      case U_0058:
      {
        temporaryBuffer.push_back(currentChar);
        smState = SM_HEXADECIMAL_CHARACTER_REFERENCE_START;
        break;
      }
      default:
      {
        smState = SM_DECIMAL_CHARACTER_REFERENCE_START;
        reconsume();
        break;
      }
    }
  }

  // 13.2.5.76
  void CHTMLTokeniser::processHexadecimalCharacterReferenceStart()
  {
    if (currentChar.isHexDigit())
    {
      smState = SM_HEXADECIMAL_CHARACTER_REFERENCE;
      reconsume();
    }
    else
    {
      PARSE_ERROR("absence of digits in numeric character reference");
      IMPLEMENT_ME();
      smState = retState;
      reconsume();
    }
  }

  // 13.2.5.77
  void CHTMLTokeniser::processDecimalCharacterReferenceStart()
  {
    if (currentChar.isnumeric())
    {
      smState = SM_DECIMAL_CHARACTER_REFERENCE;
      reconsume();
    }
    else
    {
      PARSE_ERROR("absence of digits in numeric character reference");
      IMPLEMENT_ME();
      smState = retState;
      reconsume();
    }
  }

  // 13.2.5.78
  void CHTMLTokeniser::processHexadecimalCharacterReference()
  {
    if (currentChar.isHexDigit())
    {
      charRefCode *= 16;
      IMPLEMENT_ME();
    }
    else if (currentChar == U_003B)
    {
      smState = SM_NUMERIC_CHARACTER_REFERENCE_END;
    }
    else
    {
      PARSE_ERROR("missing semicolon after character reference.");
      smState = SM_NUMERIC_CHARACTER_REFERENCE_END;
      reconsume();
    }
  }

  // 13.2.5.79
  void CHTMLTokeniser::processDecimalCharacterReference()
  {
    if (currentChar.isnumeric())
    {
      charRefCode *= 10;
      IMPLEMENT_ME();
    }
    else if (currentChar == U_003B)
    {
      smState = SM_NUMERIC_CHARACTER_REFERENCE_END;
    }
    else
    {
      PARSE_ERROR("missing semicolon after character reference.");
      smState = SM_NUMERIC_CHARACTER_REFERENCE_END;
      reconsume();
    }
  }

  // 13.2.5.80
  void CHTMLTokeniser::processNumericCharacterReferenceEnd()
  {
    if (charRefCode == 0)
    {
      PARSE_ERROR("null character reference");
      charRefCode = U_FFFD;
    }
    else if (charRefCode > 0x10FFFF)
    {
      PARSE_ERROR("character reference outside unicode range");
      charRefCode = U_FFFD;
    }
    else
    {
      IMPLEMENT_ME();
    }
  }


}
