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
  static CHTMLTokeniser::string_type S32_DOCTYPE{'d', 'o', 'c', 't', 'y', 'p', 'e'};
  static CHTMLTokeniser::string_type S32_CDATA{'[','C', 'D', 'A', 'T', 'A', '['};
  static CHTMLTokeniser::string_type S32_PUBLIC{'u', 'b', 'l', 'i', 'c'};
  static CHTMLTokeniser::string_type S32_SYSTEM{'y', 's', 't', 'e', 'm'};

  CHTMLTokeniser::token_type CHTMLTokeniser::getToken()
  {
    bool emit = false;
    token_type token;

    while (!emit)
    {
      switch(smState)
      {
        case SM_DATA:
        {
          emit = processData(token);
          break;
        }
        case SM_TAG_OPEN:
        {
          emit = processTagOpen(token);
          break;
        }
        case SM_END_TAG_OPEN:
        {
          emit = processEndTagOpen(token);
          break;
        }
        case SM_TAG_NAME:
        {
          emit = processTagName(token);
          break;
        }
        case SM_RCDATA:
        {
          emit = processRCData(token);
          break;
        }
        case SM_RCDATA_LESSTHAN:
        {
          emit = processRCDataLessThan(token);
          break;
        }
        case SM_RAWTEXT_END_TAG_OPEN:
        {
          emit = processRawTextEndTagOpen(token);
          break;
        }
        case SM_RAWTEXT_END_TAG_NAME:
        {
          emit = processRawTextEndTagName(token);
          break;
        }
        case SM_RCDATA_END_TAG_OPEN:
        {
          emit = processRCDataEndTagOpen(token);
          break;
        }
        case SM_RCDATA_END_TAG_NAME:
        {
          emit = processRCDataEndTagName(token);
          break;
        }
        case SM_RAWTEXT:
        {
          emit = processRawText(token);
          break;
        }
        case SM_RAWTEXT_LESSTHAN:
        {
          emit = processRawTextLessThan(token);
          break;
        }
        case SM_SCRIPT:
        {
          emit = processScript(token);
          break;
        }
        case SM_SCRIPT_LESSTHAN:
        {
          emit = processScriptLessThan(token);
          break;
        }
        case SM_SCRIPT_ESCAPED_END_TAG_NAME:
        {
          emit = processScriptEscapedEndTagName(token);
          break;
        }
        case SM_SCRIPT_END_TAG_OPEN:
        {
          emit = processScriptEndTagOpen(token);
          break;
        }
        case SM_SCRIPT_END_TAG_NAME:
        {
          emit = processScriptEndTagName(token);
          break;
        }
        case SM_SCRIPT_ESCAPED:
        {
          emit = processScriptEscaped(token);
          break;
        }
        case SM_SCRIPT_ESCAPED_DASH:
        {
          emit = processScriptEscapedDash(token);
          break;
        }
        case SM_SCRIPT_ESCAPED_END_TAG_OPEN:
        {
          emit = processScriptEscapedEndTagOpen(token);
          break;
        }
        case SM_SCRIPT_ESCAPED_START_DASH_DASH:
        {
          emit = processScriptEscapedDashDash(token);
          break;
        }
        case SM_SCRIPT_ESCAPED_LESSTHAN:
        {
          emit = processScriptEscapedLessThan(token);
          break;
        }
        case SM_SCRIPT_ESCAPE_START:
        {
          emit = processScriptEscapeStart(token);
          break;
        }
        case SM_SCRIPT_ESCAPE_START_DASH:
        {
          emit = processScriptEscapeStartDash(token);
          break;
        }
        case SM_SCRIPT_DOUBLE_ESCAPE_START:
        {
          emit = processScriptDoubleEscapeStart(token);
          break;
        }
        case SM_SCRIPT_DOUBLE_ESCAPED:
        {
          emit = processScriptDoubleEscaped(token);
          break;
        }
        case SM_SCRIPT_DOUBLE_ESCAPED_DASH: // 13.2.5.28
        {
          emit = processScriptDoubleEscapedDash(token);
          break;
        }
        case SM_SCRIPT_DOUBLE_ESCAPED_DASH_DASH: // 13.2.5.29
        {
          emit = processScriptDoubleEscapedDashDash(token);
          break;
        }
        case SM_PLAINTEXT:
        {
          emit = processPlainText(token);
          break;
        }
        case SM_SCRIPT_DOUBLE_ESCAPED_LESSTHAN: // 13.2.5.30
        {
          emit = processScriptDoubleEscapedLessThan(token);
          break;
        }
        case SM_SCRIPT_DOUBLE_ESCAPE_END: // 13.2.5.31
        {
          emit = processScriptDoubleEscapeEnd(token);
          break;
        }
        case SM_BEFORE_ATTR_NAME: // 13.2.5.32
        {
          emit = processBeforeAttrName(token);
          break;
        }
        case SM_ATTR_NAME:  // 13.2.5.33
        {
          emit = processAttrName(token);
          break;
        }
        case SM_AFTER_ATTR_NAME:  // 13.2.5.34
        {
          emit = processAfterAttrName(token);
          break;
        }
        case SM_BEFORE_ATTR_VALUE:  // 13.2.5.35
        {
          emit = processBeforeAttrValueStart(token);
          break;
        }
        case SM_ATTR_VALUE_DOUBLE_QUOTED:  // 13.2.5.36
        {
          emit = processAttrValueDoubleQuoted(token);
          break;
        }
        case SM_ATTR_VALUE_SINGLE_QUOTED:  // 13.2.5.37
        {
          emit = processAttrValueSingleQuoted(token);
          break;
        }
        case SM_ATTR_VALUE_UNQUOTED:  // 13.2.5.38
        {
          emit = processAttrValueUnquoted(token);
          break;
        }
        case SM_AFTER_ATTR_VALUE_QUOTED: // 13.2.5.39
        {
          emit = processAfterAttrValueQuoted(token);
          break;
        }
        case SM_TAG_SELF_CLOSING_START: // 13.2.5.40
        {
          emit = processSelfClosingStartTag(token);
          break;
        }
        case SM_BOGUS_COMMENT: // 13.2.5.41
        {
          emit = processBogusComment(token);
          break;
        }
        case SM_MARKUP_DECLARATION_OPEN: // 13.2.5.42
        {
          emit = processMarkupDeclarationOpen(token);
          break;
        }
        case SM_COMMENT_START: // 13.2.5.43
        {
          emit = processCommentStart(token);
          break;
        }
        case SM_COMMENT_START_DASH: // 13.2.5.44
        {
          emit = processCommentStartDash(token);
          break;
        }
        case SM_COMMENT: // 13.2.5.45
        {
          emit = processComment(token);
          break;
        }
        case SM_COMMENT_LESSTHAN: // 13.2.5.46
        {
          emit = processCommentLessThan(token);
          break;
        }
        case SM_COMMENT_LESSTHAN_BANG: // 13.2.5.47
        {
          emit = processCommentLessThanBang(token);
          break;
        }
        case SM_COMMENT_LESSTHAN_BANG_DASH: // 13.2.5.48
        {
          emit = processCommentLessThanBangDash(token);
          break;
        }
        case SM_COMMENT_LESSTHAN_BANG_DASH_DASH: // 13.2.5.49
        {
          emit = processCommentLessThanBangDashDash(token);
          break;
        }
        case SM_COMMENT_END_DASH: // 13.2.5.50
        {
          emit = processCommentEndDash(token);
          break;
        }
        case SM_COMMENT_END: // 13.2.5.51
        {
          emit = processCommentEnd(token);
          break;
        }
        case SM_COMMENT_END_BANG: // 13.2.5.52
        {
          emit = processCommentEndBang(token);
          break;
        }
        case SM_DOCTYPE: // 13.2.5.53
        {
          emit = processDocType(token);
          break;
        }
        case SM_BEFORE_DOCTYPE_NAME: // 13.2.5.54
        {
          emit = processBeforeDocTypeName(token);
          break;
        }
        case SM_DOCTYPE_NAME: // 13.2.5.55
        {
          emit = processDocTypeName(token);
          break;
        }
        case SM_AFTER_DOCTYPE_NAME: // 13.2.5.56
        {
          emit = processAfterDocTypeName(token);
          break;
        }
        case SM_AFTER_DOCTYPE_PUBLIC_KEYWORD: // 13.2.5.57
        {
          emit = processAfterDocTypePublicKeyword(token);
          break;
        }
        case SM_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER:  // 13.2.5.58
        {
          emit = processBeforeDocTypePublicIdentifier(token);
          break;
        }
      }
    }
    return token;
  }

  // 13.2.5.1
  bool CHTMLTokeniser::processData(token_type &token)
  {
    bool emit = false;

    value_type v = consume();
    switch(v)
    {
      case U_0026:
      {
        retState = SM_DATA;
        smState = SM_CHARACTER_REFERENCE;
        break;
      }
      case U_003C:
      {
        smState = SM_TAG_OPEN;
        break;
      };
      case EOF:
      {
        token.type(token_type::TT_EOF);
        emit = true;
        break;
      }
      case U_0000:
      {
        PARSE_ERROR("Unexpected null character");
        [[fallthrough]];
      }
      default:
      {
        emit = emitCharacter(token, v);
      }
    }
    return emit;
  }

  bool CHTMLTokeniser::processPlainText(token_type &token)
  {
    bool emit = false;

    value_type v = consume();
    switch(v)
    {
      case U_0000:
      {
        PARSE_ERROR("unexpected null character");
        emit = emitCharacter(token, U_FFFD);
        break;
      }
      case U_EOF:
      {
        token.type(token_type::TT_EOF);
        emit = true;
        break;
      }
      default:
      {
        emit = emitCharacter(token, v);
        break;
      }
    }
    return emit;
  }

  // 13.2.5.3
  bool CHTMLTokeniser::processRawText(token_type &token)
  {
    bool emit = false;
    value_type v = consume();
    switch(v)
    {
      case U_003C:
      {
        smState = SM_RAWTEXT_LESSTHAN;
        break;
      };
      case U_0000:
      {
        PARSE_ERROR("Unexpected null character");
        emit = emitCharacter(token, U_FFFD);
        break;
      }
      case U_EOF:
      {
        token.type(token_type::TT_EOF);
        emit = true;
        break;
      }
      default:
      {
        emit = emitCharacter(token, v);
        break;
      }
    }
    return emit;
  }

  // 13.2.5.14
  bool CHTMLTokeniser::processRawTextEndTagName(token_type &token)
  {
    bool emit = false;

    value_type v = consume();
    switch(v)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      case U_002F:
      {
        if (token.name() == lastStartTag)
        {
          if (v == U_002F)
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
          token.type(token_type::TT_CHARACTER);
          token.appendData({U_003C, U_002F});
          token.appendData(temporaryBuffer);
          temporaryBuffer.clear();
          emit = true;
          reconsume();
          smState = SM_RAWTEXT;
        }
        break;
      }
      case U_003E:
      {
        if (token.name() == lastStartTag)
        {
          emit = true;
          smState = SM_DATA;
        }
        else
        {
          token.type(token_type::TT_CHARACTER);
          token.appendData({U_003C, U_002F});
          token.appendData(temporaryBuffer);
          temporaryBuffer.clear();
          emit = true;
          reconsume();
          smState = SM_RAWTEXT;
        }
        break;
      }
      default:
      {
        if (v.isalpha())
        {
          token.appendName(v.tolower());
          temporaryBuffer.push_back(v);
        }
        else
        {
          token.type(token_type::TT_CHARACTER);
          token.appendData({U_003C, U_002F});
          token.appendData(temporaryBuffer);
          temporaryBuffer.clear();
          emit = true;
          reconsume();
          smState = SM_RAWTEXT;
        }
      }
    }

    return emit;
  }

  // 13.2.5.13
  bool CHTMLTokeniser::processRawTextEndTagOpen(token_type &token)
  {
    bool emit = false;

    value_type v = consume();

    if (v.isalpha())
    {
      token.type(token_type::TT_TAG_END);
      token.appendName(string_type());
      reconsume();
      smState = SM_RAWTEXT_END_TAG_NAME;
    }
    else
    {
      token.type(token_type::TT_CHARACTER);
      token.appendData({U_003C, U_002F});
      emit = true;
      reconsume();
      smState = SM_RAWTEXT;
    }

    return emit;
  }

  // 13.2.5.12
  bool CHTMLTokeniser::processRawTextLessThan(token_type &token)
  {
    bool emit = false;

    value_type v = consume();

    if (v == U_002F)
    {
      temporaryBuffer.clear();
      smState = SM_RAWTEXT_END_TAG_OPEN;
    }
    else
    {
      token.type(token_type::TT_CHARACTER);
      token.appendValue(U_003C);
      emit = true;
      reconsume();
      smState = SM_RAWTEXT;
    }

    return emit;
  }

  // 13.2.5.2
  bool CHTMLTokeniser::processRCData(token_type &token)
  {
    bool emit = false;

    value_type v = consume();
    switch(v)
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
        token.type(token_type::TT_CHARACTER);
        token.appendValue(U_FFFD);
        emit = true;
        break;
      }
      case U_EOF:
      {
        token.type(token_type::TT_EOF);
        emit = true;
        break;
      }
      default:
      {
        token.type(token_type::TT_CHARACTER);
        token.appendValue(v);
        emit = true;
      }
    }
    return emit;
  }

  // 13.2.5.11
  bool CHTMLTokeniser::processRCDataEndTagName(token_type &token)
  {
    bool emit = false;

    value_type v = consume();

    switch(v)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      case U_002F:
      {
        if (token.value() == lastStartTag)
        {
          if (v == U_002F)
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
          token.type(token_type::TT_CHARACTER);
          token.appendValue({U_003C, U_002F});
          token.appendValue(temporaryBuffer);
          temporaryBuffer.clear();
          emit = true;
          reconsume();
          smState = SM_RCDATA;
        }
        break;
      }
      case U_003E:
      {
        if (token.value() == lastStartTag)
        {
          emit = true;
          smState = SM_DATA;
        }
        else
        {
          token.type(token_type::TT_CHARACTER);
          token.appendValue({U_003C, U_002F});
          token.appendValue(temporaryBuffer);
          temporaryBuffer.clear();
          emit = true;
          reconsume();
          smState = SM_RCDATA;
        }
        break;
      }
      default:
      {
        if (v.isalpha())
        {
          token += v.tolower();
          temporaryBuffer.push_back(v);
        }
        else
        {
          token.type(token_type::TT_CHARACTER);
          token.appendValue({U_003C, U_002F});
          token.appendValue(temporaryBuffer);
          temporaryBuffer.clear();
          emit = true;
          reconsume();
          smState = SM_RCDATA;
        }
      }
    }

    return emit;
  }

  // 13.2.5.10
  bool CHTMLTokeniser::processRCDataEndTagOpen(token_type &token)
  {
    bool emit = false;

    value_type v = consume();

    if (v.isalpha())
    {
      token.type(token_type::TT_END_TAG);
      token.appendValue(string_type());
      reconsume();
      smState = SM_RCDATA_END_TAG_NAME;
    }
    else
    {
      token.type(token_type::TT_CHARACTER);
      token.appendValue({U_003C, U_002F});
      smState = SM_RCDATA;
      reconsume();
    }

    return emit;
  }

  // 13.2.5.9
  bool CHTMLTokeniser::processRCDataLessThan(token_type &token)
  {
    bool emit = false;

    value_type v = consume();

    switch(v)
    {
      case U_002F:	// '/'
      {
        temporaryBuffer.clear();
        smState = SM_RCDATA_END_TAG_OPEN;
        break;
      }
      default:
      {
        token.type(token_type::TT_CHARACTER);
        token.appendValue(U_003C);
        smState = SM_RCDATA;
        reconsume();
        break;
      }
    }

    return emit;
  }

  // 12.2.5.4
  bool CHTMLTokeniser::processScript(token_type &token)
  {
    bool emit = false;
    value_type v = consume();
    switch(v)
    {
      case U_003C:
      {
        smState = SM_SCRIPT_LESSTHAN;
        break;
      };
      case U_0000:
      {
        PARSE_ERROR("Unexpected null character");
        token.type(token_type::TT_CHARACTER);
        token.appendValue(U_FFFD);
        emit = true;
        break;
      }
      case U_EOF:
      {
        token.type(token_type::TT_EOF);
        emit = true;
        break;
      }
      default:
      {
        token.type(token_type::TT_CHARACTER);
        token.appendValue(v);
        emit = true;
      }
    }
    return emit;
  }

  // 13.2.5.27
  bool CHTMLTokeniser::processScriptDoubleEscaped(token_type &token)
  {
    bool emit = false;

    value_type v = consume();

    switch (v)
    {
      case U_002D:
      {
        token.type(token_type::TT_CHARACTER);
        token.appendValue(U_002D);
        emit = true;
        smState = SM_SCRIPT_DOUBLE_ESCAPED_DASH;
        break;
      }
      case U_003C:
      {
        token.type(token_type::TT_CHARACTER);
        token.appendValue(U_003C);
        emit = true;
        smState = SM_SCRIPT_DOUBLE_ESCAPED_LESSTHAN;
        break;
      }
      case U_0000:
      {
        PARSE_ERROR("Unexpected null character");
        token.type(token_type::TT_CHARACTER);
        token.appendValue(U_FFFD);
        emit = true;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in script/html/comment like text");
        token.type(token_type::TT_EOF);
        emit = true;
        break;
      }
      default:
      {
        token.type(token_type::TT_CHARACTER);
        token.appendValue(v);
        emit = true;
      }
    }

    return emit;
  }

  // 13.2.5.26
  bool CHTMLTokeniser::processScriptDoubleEscapeStart(token_type &token)
  {
    bool emit = false;

    value_type v = consume();

    switch(v)
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
          token.type(token_type::TT_CHARACTER);
          token.appendValue(v);
          emit = true;
          smState = SM_SCRIPT_ESCAPED;
        }
        break;
      }
      default:
      {
        if (v.isalpha())
        {
          temporaryBuffer.push_back(v.tolower());
          token.type(token_type::TT_CHARACTER);
          token.appendValue(v);
          emit = true;
        }
        else
        {
          reconsume();
          smState = SM_SCRIPT_ESCAPED;
        }
      }
    }

    return emit;
  }
  // 13.2.5.17
  bool CHTMLTokeniser::processScriptEndTagName(token_type &token)
  {
    bool emit = false;

    value_type v = consume();

    switch(v)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      case U_002F:
      {
        if (token.value() == lastStartTag)
        {
          if (v == U_002F)
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
          token.type(token_type::TT_CHARACTER);
          token.appendValue({U_003C, U_002F});
          token += temporaryBuffer;
          temporaryBuffer.clear();
          emit = true;
          reconsume();
          smState = SM_SCRIPT;
        }
        break;
      }
      case 0x003E:
      {
        if (token.value() == lastStartTag)
        {
          emit = true;
          smState = SM_DATA;
        }
        else
        {
          token.type(token_type::TT_CHARACTER);
          token.appendValue({U_003C, U_002F});
          token.appendValue(temporaryBuffer);
          temporaryBuffer.clear();
          emit = true;
          reconsume();
          smState = SM_SCRIPT;
        }
        break;
      }
      default:
      {
        if (std::isalpha(v))
        {
          token += v.tolower();
          temporaryBuffer.push_back(v);
        }
        else
        {
          token.type(token_type::TT_CHARACTER);
          token.appendValue({U_003C, U_002F});
          token.appendValue(temporaryBuffer);
          temporaryBuffer.clear();
          emit = true;
          reconsume();
          smState = SM_SCRIPT;
        }
      }
    }

    return emit;
  }

  // 13.2.5.16
  bool CHTMLTokeniser::processScriptEndTagOpen(token_type &token)
  {
    bool emit = false;

    value_type v = consume();

    if (std::isalpha(v))
    {
      token.type(token_type::TT_END_TAG);
      token.appendValue(string_type());
      reconsume();
      smState = SM_SCRIPT_END_TAG_NAME;
    }
    else
    {
      token.type(token_type::TT_CHARACTER);
      token.appendValue({U_003C, U_002F});
      emit = true;
      reconsume();
      smState = SM_SCRIPT;
    }

    return emit;
  }

  // 13.2.5.20
  bool CHTMLTokeniser::processScriptEscaped(token_type &token)
  {
    bool emit = false;

    value_type v = consume();

    switch (v)
    {
      case U_002D:
      {
        smState = SM_SCRIPT_ESCAPED_DASH;
        token.type(token_type::TT_CHARACTER);
        token.appendValue(U_002D);
        emit = true;
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
        token.type(token_type::TT_CHARACTER);
        token.appendValue(U_FFFD);
        emit = true;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in script, html,comment like text");
        token.type(token_type::TT_EOF);
        emit = true;
        break;
      }
      default:
      {
        token.type(token_type::TT_CHARACTER);
        token.appendValue(v);
        emit = true;
      }
    }

    return emit;
  }

  bool CHTMLTokeniser::processScriptEscapedDash(token_type &token)
  {
    bool emit = false;

    value_type v = consume();

    switch(v)
    {
      case U_002D:
      {
        token.type(token_type::TT_CHARACTER);
        token.appendValue(U_002D);
        emit = true;
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
        smState = SM_SCRIPT_ESCAPED;
        token.type(token_type::TT_CHARACTER);
        token.appendValue(U_FFFD);
        emit = true;
        break;
      }
      case EOF:
      {
        PARSE_ERROR("eof in script, html,comment like text");
        token.type(token_type::TT_EOF);
        emit = true;
        break;
      }
      default:
      {
        token.type(token_type::TT_CHARACTER);
        token.appendValue(v);
        emit = true;
        smState = SM_SCRIPT_ESCAPED;
        break;
      }
    }

    return emit;
  }

  // 13.2.5.22
  bool CHTMLTokeniser::processScriptEscapedDashDash(token_type &token)
  {
    bool emit = false;

    value_type v = consume();

    switch(v)
    {
      case U_002D:
      {
        token.type(token_type::TT_CHARACTER);
        token.appendValue(U_002D);
        emit = true;
        break;
      }
      case U_003C:
      {
        smState = SM_SCRIPT_ESCAPED_LESSTHAN;
        break;
      }
      case U_003E:
      {
        token.type(token_type::TT_CHARACTER);
        token.appendValue(U_003E);
        emit = true;
        smState = SM_SCRIPT;
        break;
      }
      case U_0000:
      {
        PARSE_ERROR("Unexpected null character");
        smState = SM_SCRIPT_ESCAPED;
        token.type(token_type::TT_CHARACTER);
        token.appendValue(U_FFFD);
        emit = true;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in script, html,comment like text");
        token.type(token_type::TT_EOF);
        emit = true;
        break;
      }
      default:
      {
        token.type(token_type::TT_CHARACTER);
        token.appendValue(v);
        emit = true;
        smState = SM_SCRIPT_ESCAPED;
        break;
      }
    }

    return emit;
  }

  // 13.2.5.25
  bool CHTMLTokeniser::processScriptEscapedEndTagName(token_type &token)
  {
    bool emit = false;

    value_type v = consume();

    switch(v)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      case U_002F:
      {
        if (token.value() == lastStartTag)
        {
          if (v == U_002F)
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
          token.type(token_type::TT_CHARACTER);
          token.appendValue({U_003C, U_002F});
          token += temporaryBuffer;
          emit = true;
          reconsume();
          smState = SM_SCRIPT_ESCAPED;
        }
        break;
      }
      case 0x003E:
      {
        if (token.value() == lastStartTag)
        {
          emit = true;
          smState = SM_DATA;
        }
        else
        {
          token.type(token_type::TT_CHARACTER);
          token.appendValue({U_003C, U_002F});
          token += temporaryBuffer;
          emit = true;
          reconsume();
          smState = SM_SCRIPT_ESCAPED;
        }
        break;
      }
      default:
      {
        if (std::isalpha(v))
        {
          token += v.tolower();
          temporaryBuffer.push_back(v);
        }
        else
        {
          token.type(token_type::TT_CHARACTER);
          token.appendValue({U_003C, U_002F});
          token += temporaryBuffer;
          emit = true;
          reconsume();
          smState = SM_SCRIPT_ESCAPED;
        }
      }
    }

    return emit;
  }

  // 13.2.5.24
  bool CHTMLTokeniser::processScriptEscapedEndTagOpen(token_type &token)
  {
    bool emit = false;

    value_type v = consume();

    if (std::isalpha(v))
    {
      token.type(token_type::TT_END_TAG);
      token.appendValue(string_type());
      reconsume();
      smState = SM_SCRIPT_ESCAPED_END_TAG_NAME;
    }
    else
    {
      token.type(token_type::TT_CHARACTER);
      token.appendValue({U_003C, U_002F});
      emit = true;
      reconsume();
      smState = SM_SCRIPT_ESCAPED;
    }

    return emit;
  }

  // 13.2.5.23
  bool CHTMLTokeniser::processScriptEscapedLessThan(token_type &token)
  {
    bool emit = false;

    value_type v = consume();

    switch(v)
    {
      case U_002F:
      {
        temporaryBuffer.clear();
        smState = SM_SCRIPT_ESCAPED_END_TAG_OPEN;
        break;
      }
      default:
      {
        if (std::isalpha(v))
        {
          temporaryBuffer.clear();
          token.type(token_type::TT_CHARACTER);
          token.appendValue(U_003C);
          emit = true;
          reconsume();
          smState = SM_SCRIPT_DOUBLE_ESCAPE_START;
        }
        else
        {
          token.type(token_type::TT_CHARACTER);
          token.appendValue(U_003C);
          emit = true;
          reconsume();
          smState = SM_SCRIPT_ESCAPED;
        }
        break;
      }
    }

    return emit;
  }

  // 13.2.5.18
  bool CHTMLTokeniser::processScriptEscapeStart(token_type &token)
  {
    bool emit = false;

    value_type v = consume();

    if (v == U_002D)
    {
      smState = SM_SCRIPT_ESCAPE_START_DASH;
      token.type(token_type::TT_CHARACTER);
      token.appendValue(U_002D);
      emit = true;
    }
    else
    {
      reconsume();
      smState = SM_SCRIPT;
    }

    return emit;
  }

  // 13.2.5.19
  bool CHTMLTokeniser::processScriptEscapeStartDash(token_type &token)
  {
    bool emit = false;

    value_type v = consume();

    if (v == U_002D)
    {
      smState = SM_SCRIPT_ESCAPED_START_DASH_DASH;
      token.type(token_type::TT_CHARACTER);
      token.appendValue(U_002D);
      emit = true;
    }
    else
    {
      reconsume();
      smState = SM_SCRIPT;
    }

    return emit;
  }

  // 13.2.5.15
  bool CHTMLTokeniser::processScriptLessThan(token_type &token)
  {
    bool emit = false;

    value_type v = consume();

    switch (v)
    {
      case U_002F:
      {
        temporaryBuffer.clear();
        smState = SM_RCDATA_END_TAG_OPEN;
        break;
      }
      case U_0021:
      {
        smState = SM_SCRIPT_ESCAPE_START;
        token.type(token_type::TT_CHARACTER);
        token.appendValue({U_003C, U_0021});
        emit = true;
        break;
      }
      default:
      {
        smState = SM_SCRIPT;
        token.type(token_type::TT_CHARACTER);
        token.appendValue(U_003C);
        reconsume();
        emit = true;
        break;
      }
    }

    return emit;
  }

  // 13.2.5.7
  bool CHTMLTokeniser::processEndTagOpen(token_type &token)
  {
    bool emit = false;

    value_type v = consume();

    if (std::isalpha(v))
    {
      token.type(token_type::TT_END_TAG);
      smState = SM_TAG_NAME;
      reconsume();
    }
    else if (v == U_003E)
    {
      PARSE_ERROR("missing end tag name");
      smState = SM_DATA;
    }
    else if (v == U_EOF)
    {
      PARSE_ERROR("eof before tag name");
      token.type(token_type::TT_EOF);
      token.appendValue({U_003C, U_002F});
    }
    else
    {
      PARSE_ERROR("invalid first character of tag name");
      token.type(token_type::TT_COMMENT);
      smState = SM_BOGUS_COMMENT;
      reconsume();
    }

    return emit;
  }

  // 13.2.5.8
  bool CHTMLTokeniser::processTagName(token_type &token)
  {
    bool emit = false;

    value_type v = consume();

    switch(v)
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
        emit = true;
        smState = SM_DATA;
        break;
      };
      case U_0000:
      {
        PARSE_ERROR("unexpected null character");
        token += U_FFFD;
        break;
      }
      case EOF:
      {
        PARSE_ERROR("eof in tag");
        token.type(token_type::TT_EOF);
        token.value().clear();
        emit = true;
        break;
      }
      default:
      {
        token += v.tolower();
        break;
      }
    };

    return emit;
  };

  // 13.2.5.6
  bool CHTMLTokeniser::processTagOpen(token_type &token)
  {
    bool emit = false;

    value_type v = consume();

    switch (v)
    {
      case U_0021:
      {
        smState = SM_MARKUP_DECLARATION_OPEN;
        break;
      }
      case U_002F:
      {
        smState = SM_END_TAG_OPEN;
        break;
      }
      case U_003F:
      {
        smState = SM_BOGUS_COMMENT;
        token.type(token_type::TT_COMMENT);
        reconsume();
        break;
      }
      case U_EOF:
      {
        throw parse_error("eof before tag name");
        token.type(token_type::TT_EOF);
        token.appendValue(U_003C);
        emit = true;
        break;
      }
      default:
      {
        if (v.isalpha())
        {
          token.type(token_type::TT_START_TAG);
          smState = SM_TAG_NAME;
        }
        else
        {
          token.type(token_type::TT_CHARACTER);
          token.appendValue(U_003C);
          smState = SM_DATA;
          emit = true;
        }
        break;
      }
    }

    return emit;
  }

  // 13.2.5.28
  bool CHTMLTokeniser::processScriptDoubleEscapedDash(token_type &token)
  {
    bool emit = false;

    value_type v = consume();

    switch(v)
    {
      case U_002D:
      {
        token.type(token_type::TT_CHARACTER);
        token.appendValue(U_002D);
        emit = true;
        smState = SM_SCRIPT_DOUBLE_ESCAPED_DASH_DASH;
        break;
      }
      case U_003C:
      {
        token.type(token_type::TT_CHARACTER);
        token.appendValue(U_003C);
        emit = true;
        smState = SM_SCRIPT_DOUBLE_ESCAPED_LESSTHAN;
        break;
      }
      case U_0000:
      {
        PARSE_ERROR("unexpected null character");
        token.type(token_type::TT_CHARACTER);
        token.appendValue(U_FFFD);
        emit = true;
        smState = SM_SCRIPT_DOUBLE_ESCAPED;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in script/html/comment line text");
        token.type(token_type::TT_EOF);
        emit = true;
        break;
      }
      default:
      {
        token.type(token_type::TT_CHARACTER);
        token.appendValue(v);
        emit = true;
        smState = SM_SCRIPT_DOUBLE_ESCAPED;
        break;
      };
    }

    return emit;
  }

  // 13.2.5.29
  bool CHTMLTokeniser::processScriptDoubleEscapedDashDash(token_type &token)
  {
    bool emit = false;

    value_type v = consume();

    switch(v)
    {
      case U_002D:
      {
        token.type(token_type::TT_CHARACTER);
        token.appendValue(U_002D);
        emit = true;
        break;
      }
      case U_003C:
      {
        emit = emitCharacter(token, U_003C);
        smState = SM_SCRIPT_DOUBLE_ESCAPED_LESSTHAN;
        break;
      }
      case U_003E:
      {
        emit = emitCharacter(token, U_003E);
        smState = SM_SCRIPT;
        break;
      }
      case U_0000:
      {
        PARSE_ERROR("unexpected null character");
        emit = emitCharacter(token, U_FFFD);
        smState = SM_SCRIPT_DOUBLE_ESCAPED;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in script/html/comment like text");
        emit = emitEOF(token);
        break;
      }
    }

    return emit;
  }

  // 13.2.5.30
  bool CHTMLTokeniser::processScriptDoubleEscapedLessThan(token_type &token)
  {
    bool emit = false;

    value_type v = consume();

    if (v == U_002F) { emit = emitCharacter(token, U_002F); temporaryBuffer.clear(); smState = SM_SCRIPT_DOUBLE_ESCAPE_END;
    }
    else { smState = SM_SCRIPT_DOUBLE_ESCAPED; reconsume();
    }

    return emit;
  }

  // 13.2.5.31
  bool CHTMLTokeniser::processScriptDoubleEscapeEnd(token_type &token)
  {
    bool emit = false; value_type v = consume();

    switch(v)
    {
      case U_0009:
      case U_000A:
      case U_000C:
      case U_0020:
      case U_002F:
      case U_003E:
      {
        emit = emitCharacter(token, v);
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
        if (v.isalpha())
        {
          temporaryBuffer.push_back(v.tolower());
          emit = emitCharacter(token, v);
        }
        else
        {
          smState = SM_SCRIPT_DOUBLE_ESCAPED;
          reconsume();
        }
      }
    }
    return emit;
  }

  // 13.2.5.32
  bool CHTMLTokeniser::processBeforeAttrName(token_type &token)
  {
    bool emit = false;
    value_type v = consume();

    switch (v)
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
        token.attrStart();
        token.attrConcatName(v);
        smState = SM_ATTR_NAME;
        break;
      }
      default:
      {
        token.attrStart();
        smState = SM_ATTR_NAME;
        reconsume();
        break;
      };
    }

    return emit;
  }

  // 13.2.5.33
  bool CHTMLTokeniser::processAttrName(token_type &token)
  {
    bool emit = false;
    value_type v = consume();

    switch(v)
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
        token.attrConcatName(U_FFFD);
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
        token.attrConcatName(v.tolower());
        break;
      }
    }

    return emit;
  }

  // 13.2.5.34
  bool CHTMLTokeniser::processAfterAttrName(token_type &token)
  {
    bool emit = false;
    value_type v = consume();

    switch (v)
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
        emit = emitEOF(token);
        break;
      }
      default:
      {
        token.attrStart();
        smState = SM_ATTR_NAME;
        reconsume();
        break;
      }
    }

    return emit;
  }

  // 13.2.5.35
  bool CHTMLTokeniser::processBeforeAttrValueStart(token_type &token)
  {
    bool emit = false;
    value_type v = consume();

    switch(v)
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

    return emit;
  }

  // 13.2.5.36
  bool CHTMLTokeniser::processAttrValueDoubleQuoted(token_type &token)
  {
    bool emit = false;
    value_type v = consume();

    switch(v)
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
        token.attrConcatValue(U_FFFD);
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in tag");
        emit = emitEOF(token);
        break;
      }
      default:
      {
        token.attrConcatValue(v);
        break;
      }
    }

    return emit;
  }

  // 13.2.5.37
  bool CHTMLTokeniser::processAttrValueSingleQuoted(token_type &token)
  {
    bool emit = false;
    value_type v = consume();

    switch(v)
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
        token.attrConcatValue(U_FFFD);
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in tag");
        emit = emitEOF(token);
        break;
      }
      default:
      {
        token.attrConcatValue(v);
        break;
      }
    }

    return emit;
  }

  // 13.2.5.38
  bool CHTMLTokeniser::processAttrValueUnquoted(token_type &token)
  {
    bool emit = false;
    value_type v = consume();

    switch(v)
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
        token.attrConcatValue(U_FFFD);
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in tag");
        emit = emitEOF(token);
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
        token.attrConcatValue(v);
        break;
      }
    }

    return emit;
  }

  // 13.2.5.39
  bool CHTMLTokeniser::processAfterAttrValueQuoted(token_type &token)
  {
    bool emit = false;
    value_type v = consume();

    switch(v)
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
        emit = emitEOF(token);
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

    return emit;
  }

  // 13.2.5.40
  bool CHTMLTokeniser::processSelfClosingStartTag(token_type &token)
  {
    bool emit = false;
    value_type v = consume();

    switch(v)
    {
      case U_003E:
      {
        token.selfClosing(true);
        smState = SM_DATA;
        emit = true;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in tag");
        emit = emitEOF(token);
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
    return emit;
  }

  // 13.2.5.41
  bool CHTMLTokeniser::processBogusComment(token_type &token)
  {
    bool emit = false;
    value_type v = consume();

    switch(v)
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
        emit = true;
        smState = SM_DATA;
        reconsume();
        break;
      }
      case U_0000:
      {
        PARSE_ERROR("unexpected null character");
        token.appendValue(U_FFFD);
        break;
      }
      default:
      {
        token.appendValue(v);
        break;
      }
    }

    return emit;
  }

  // 13.2.5.42
  bool CHTMLTokeniser::processMarkupDeclarationOpen(token_type &token)
  {
    bool emit = false;

    if (match(S32_MINUSMINUS, true))
    {
      consume(S32_MINUSMINUS.size());
      token.type(token_type::TT_COMMENT);
      token.value().clear();
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
      token.type(token_type::TT_COMMENT);
      token.value().clear();
      smState = SM_BOGUS_COMMENT;
    }

    return emit;
  }

  // 13.2.5.43
  bool CHTMLTokeniser::processCommentStart(token_type &token)
  {
    bool emit = false;
    value_type v = consume();

    switch (v)
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

    return emit;
  }

  // 13.2.5.44
  bool CHTMLTokeniser::processCommentStartDash(token_type &token)
  {
    bool emit = false;
    value_type v = consume();

    switch(v)
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
        emit = true;
        reconsume();
        break;
      }
      default:
      {
        token.appendValue(U_002D);
        smState = SM_COMMENT;
        reconsume();
        break;
      }
    }

    return emit;
  }

  // 13.2.5.45
  bool CHTMLTokeniser::processComment(token_type &token)
  {
    bool emit = false;
    value_type v = consume();

    switch(v)
    {
      case U_003C:
      {
        token.appendValue(v);
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
        token.appendValue(U_FFFD);
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in comment");
        emit = true;
        reconsume();
        break;
      }
      default:
      {
        token.appendValue(v);
        break;
      }
    }

    return emit;
  }

  // 13/2/5/46
  bool CHTMLTokeniser::processCommentLessThan(token_type &token)
  {
    bool emit = false;
    value_type v = consume();

    switch(v)
    {
      case U_0021:
      {
        token.appendValue(v);
        smState = SM_COMMENT_LESSTHAN_BANG;
        break;
      }
      case U_003C:
      {
        token.appendValue(v);
        break;
      }
      default:
        smState = SM_COMMENT;
        reconsume();
        break;
    }

    return emit;
  }

  // 13.2.5.47
  bool CHTMLTokeniser::processCommentLessThanBang(token_type &token)
  {
    bool emit = false;
    value_type v = consume();

    switch(v)
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

    return emit;
  }

  // 13.2.5.48
  bool CHTMLTokeniser::processCommentLessThanBangDash(token_type &token)
  {
    bool emit = false;
    value_type v = consume();

    switch(v)
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

    return emit;
  }

  // 13.2.5.49
  bool CHTMLTokeniser::processCommentLessThanBangDashDash(token_type &token)
  {
    bool emit = false;
    value_type v = consume();

    switch (v)
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
    return emit;
  }

  // 13.2.5.50
  bool CHTMLTokeniser::processCommentEndDash(token_type &token)
  {
    bool emit = false;
    value_type v = consume();

    switch(v)
    {
      case U_002D:
      {
        smState = SM_COMMENT_END;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in comment");
        emit = true;
        smState = SM_DATA;
        reconsume();
        break;
      }
      default:
      {
        token.appendValue(U_002D);
        smState = SM_COMMENT;
        reconsume();
        break;
      }
    }

    return emit;
  }

  // 13.2.5.51
  bool CHTMLTokeniser::processCommentEnd(token_type &token)
  {
    bool emit = true;
    value_type v = consume();

    switch(v)
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
        token.appendValue(v);
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in comment");
        emit = true;
        smState = SM_DATA;
        reconsume();
        break;
      }
      default:
      {
        token.appendValue({U_002D, U_002D});
        smState = SM_COMMENT;
        reconsume();
        break;
      }
    }

    return emit;
  }

  // 13.2.5.52
  bool CHTMLTokeniser::processCommentEndBang(token_type &token)
  {
    bool emit = false;
    value_type v = consume();

    switch(v)
    {
      case U_002D:
      {
        token.appendValue({U_002D, U_002D, U_0021});
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
        emit = true;
        smState = SM_DATA;
        reconsume();
        break;
      }
      default:
      {
        token.appendValue({U_002D, U_002D, U_0021});
        smState = SM_COMMENT;
        reconsume();
        break;
      }
    }

    return emit;
  }

  // 13.2.5.53
  bool CHTMLTokeniser::processDocType(token_type &token)
  {
    bool emit = false;
    value_type v = consume();

    switch(v)
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
        token.type(token_type::TT_DOCTYPE);
        token.forceQuirks(true);
        emit = true;
        smState = SM_DATA;
        reconsume();
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

    return emit;
  }

  // 13.2.5.54
  bool CHTMLTokeniser::processBeforeDocTypeName(token_type &token)
  {
    bool emit = false;
    value_type v = consume();

    switch(v)
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
        token.type(token_type::TT_DOCTYPE);
        token.value(U_FFFD);
        smState = SM_DOCTYPE_NAME;
        break;
      }
      case U_003E:
      {
        PARSE_ERROR("missing doctype name");
        token.type(token_type::TT_DOCTYPE);
        token.forceQuirks(true);
        smState = SM_DATA;
        emit = true;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in doctype");
        token.type(token_type::TT_DOCTYPE);
        token.forceQuirks(true);
        emit = true;
        smState = SM_DATA;
        reconsume();
        break;
      }
      default:
      {
        token.type(token_type::TT_DOCTYPE);
        token.value(v.tolower());
        smState = SM_DOCTYPE_NAME;
        break;
      }
    }

    return emit;
  }

  // 13.2.5.55
  bool CHTMLTokeniser::processDocTypeName(token_type &token)
  {
    bool emit = false;
    value_type v = consume();

    switch (v)
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
        token.appendValue(U_FFFD);
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in doctype");
        token.forceQuirks(true);
        emit = true;
        smState = SM_DATA;
        reconsume();
        break;
      }
      default:
      {
        token.appendValue(v.tolower());
        break;
      }
    }

    return emit;
  }

  // 13.2.5.56
  bool CHTMLTokeniser::processAfterDocTypeName(token_type &token)
  {
    bool emit = false;
    value_type v = consume();

    switch(v)
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
        token.forceQuirks(true);
        emit = true;
        smState = SM_DATA;
        reconsume();
        break;
      }
      default:
      {
        if ((v.tolower() == 'p') && match(S32_PUBLIC, false))
        {
          consume(S32_PUBLIC.size());
          smState = SM_AFTER_DOCTYPE_PUBLIC_KEYWORD;
          break;
        }
        else if ((v.tolower() == 's') && match(S32_SYSTEM, false))
        {
          consume(S32_SYSTEM.size());
          smState = SM_AFTER_DOCTYPE_SYSTEM_KEYWORD;
          break;
        }
        else
        {
          PARSE_ERROR("invalid character sequence after doctype name");
          token.forceQuirks(true);
          smState = SM_BOGUS_DOCTYPE;
          reconsume();
          break;
        }
      }
    }

    return emit;
  }

  // 13.2.5.57
  bool CHTMLTokeniser::processAfterDocTypePublicKeyword(token_type &token)
  {
    bool emit = false;
    value_type v = consume();

    switch (v)
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
        smState = SM_DOCTYPE_PUBLIC_IDENTIFER_DOUBLE_QUOTED;
        break;
      }
      case U_0027:
      {
        PARSE_ERROR("missing whitespace after doctype public keyword");
        IMPLEMENT_ME();
        smState = SM_DOCTYPE_PUBLIC_IDENTIFER_SINGLE_QUOTED;
        break;
      }
      case U_003E:
      {
        PARSE_ERROR("missing doctype public identifier");
        token.forceQuirks(true);
        emit = true;
        smState = SM_DATA;
        break;
      }
      case U_EOF:
      {
        PARSE_ERROR("eof in doctype");
        token.forceQuirks(true);
        emit = true;
        smState = SM_DATA;
        reconsume();
        break;
      }
      default:
      {
        PARSE_ERROR("missing quote before doctype public identifier");
        token.forceQuirks(true);
        smState = SM_BOGUS_DOCTYPE;
        reconsume();
        break;
      }
    }

    return emit;
  }

  // 13.2.5.58
  bool CHTMLTokeniser::processBeforeDocTypePublicIdentifier(token_type &token)
  {
    bool emit = false;
    value_type v = consume();

    switch (v)
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

      }
    }

    return emit;
  }

}
