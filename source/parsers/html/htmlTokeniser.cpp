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
 * this incurs overhead in having to determine the nunber of bytes.)
 * NOTE: ****IMPORTANT**** The EOF character is defined as 0xFF. This is an ununsed charpoint in the utf8 mapping.
 * The tokeniser then only needs to consider unsigned comparisons of 32 bit integers. This is quick on modern processors.
 * To correctly handle multi-byte sequences, the codePoint_t type is provided. The buffer/encoder always provides a codePoint
 * to the tokeniser. The code-point is freely convertable between the various representations. (Note the codePoint always represents
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

  static CHTMLTokeniser::string_type STR_SCRIPT{'s', 'c', 'r', 'i', 'p', 't'};

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
        }
        case SM_PLAINTEXT:
        {
          emit = processPlainText(token);
          break;
        }
      }
    }

    return token;
  }

  bool CHTMLTokeniser::processData(token_type &token)
  {
    bool emit = false;

    value_type v = consume();
    switch(v)
    {
      case '&':
      {
        retState = SM_DATA;
        smState = SM_CHARACTER_REFERENCE;
        break;
      }
      case '<':
      {
        smState = SM_TAG_OPEN;
        break;
      };
      case '\0':
      {
        PARSE_ERROR("Unexpected null character");
        token.type(token_type::TT_CHARACTER);
        token.value(string_type());
        emit = true;
        break;
      }
      case EOF:
      {
        token.type(token_type::TT_EOF);
        emit = true;
        break;
      }
      default:
      {
        token.type(token_type::TT_CHARACTER);
        token.value(v);
        emit = true;
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
      case '\0':
      {
        PARSE_ERROR("unexpected null character");
        token.type(token_type::TT_CHARACTER);
        token.value(0xFFFD);
        emit = true;
        break;
      }
      case EOF:
      {
        token.type(token_type::TT_EOF);
        emit = true;
        break;
      }
      default:
      {
        token.type(token_type::TT_CHARACTER);
        token.value(v);
        emit = true;
      }
    }
    return emit;
  }

  bool CHTMLTokeniser::processRawText(token_type &token)
  {
    bool emit = false;
    int v = consume();
    switch(v)
    {
      case '<':
      {
        smState = SM_RAWTEXT_LESSTHAN;
        break;
      };
      case '\0':
      {
        PARSE_ERROR("Unexpected null character");
        token.type(token_type::TT_CHARACTER);
        token.value(0xFFFD);
        emit = true;
        break;
      }
      case EOF:
      {
        token.type(token_type::TT_EOF);
        emit = true;
        break;
      }
      default:
      {
        token.type(token_type::TT_CHARACTER);
        token.value(v);
        emit = true;
      }
    }
    return emit;
  }

  bool CHTMLTokeniser::processRawTextEndTagName(token_type &token) // 13.2.5.14
  {
    bool emit = false;

    value_type v = consume();
    switch(v)
    {
      case 0x0009:
      case 0x000A:
      case 0x000C:
      case 0x0020:
      case 0x002F:
      {
        if (token.value() == lastStartTag)
        {
          if (v == 0x0002F)
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
          token.value({U_003C, U_002F});
          token.value() += temporaryBuffer;
          temporaryBuffer.clear();
          emit = true;
          reconsume();
          smState = SM_RAWTEXT;
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
          token.value({U_003C, U_002F});
          token.value() += temporaryBuffer;
          temporaryBuffer.clear();
          emit = true;
          reconsume();
          smState = SM_RAWTEXT;
        }
        break;
      }
      default:
      {
        if (std::isalpha(v))
        {
          v = std::tolower(v);
          token.value() += v;
          temporaryBuffer.push_back(v);
        }
        else
        {
          token.type(token_type::TT_CHARACTER);
          token.value({U_003C, U_002F});
          token.value() += temporaryBuffer;
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

    if (std::isalpha(v))
    {
      token.type(token_type::TT_END_TAG);
      token.value(string_type());
      reconsume();
      smState = SM_RAWTEXT_END_TAG_NAME;
    }
    else
    {
      token.type(token_type::TT_CHARACTER);
      token.value({U_003C, U_002F});
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

    if (v == 0x002F)
    {
      temporaryBuffer.clear();
      smState = SM_RAWTEXT_END_TAG_OPEN;
    }
    else
    {
      token.type(token_type::TT_CHARACTER);
      token.value(U_003C);
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
      case '&':
      {
        retState = SM_RCDATA;
        smState = SM_CHARACTER_REFERENCE;
        break;
      }
      case '<':
      {
        smState = SM_RCDATA_LESSTHAN;
        break;
      };
      case '\0':
      {
        PARSE_ERROR("unexpexted null character");
        token.type(token_type::TT_CHARACTER);
        token.value(0xFFFD);
        emit = true;
        break;
      }
      case EOF:
      {
        token.type(token_type::TT_EOF);
        emit = true;
        break;
      }
      default:
      {
        token.type(token_type::TT_CHARACTER);
        token.value(v);
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
      case 0x0009:
      case 0x000A:
      case 0x000C:
      case 0x0020:
      case 0x002F:
      {
        if (token.value() == lastStartTag)
        {
          if (v == 0x0002F)
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
          token.value({U_003C, U_002F});
          token.value() += temporaryBuffer;
          temporaryBuffer.clear();
          emit = true;
          reconsume();
          smState = SM_RCDATA;
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
          token.value({U_003C, U_002F});
          token.value() += temporaryBuffer;
          temporaryBuffer.clear();
          emit = true;
          reconsume();
          smState = SM_RCDATA;
        }
        break;
      }
      default:
      {
        if (std::isalpha(v))
        {
          v = std::tolower(v);
          token += v;
          temporaryBuffer.push_back(v);
        }
        else
        {
          token.type(token_type::TT_CHARACTER);
          token.value({U_003C, U_002F});
          token.value() += temporaryBuffer;
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

    if (std::isalpha(v))
    {
      token.type(token_type::TT_END_TAG);
      token.value(string_type());
      reconsume();
      smState = SM_RCDATA_END_TAG_NAME;
    }
    else
    {
      token.type(token_type::TT_CHARACTER);
      token.value({U_003C, U_002F});
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
        token.value(U_003C);
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
    int v = consume();
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
        token.value(0xFFFD);
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
        token.value(v);
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
        token.value(U_002D);
        emit = true;
        smState = SM_SCRIPT_DOUBLE_ESCAPED_DASH;
        break;
      }
      case U_003C:
      {
        token.type(token_type::TT_CHARACTER);
        token.value(U_003C);
        emit = true;
        smState = SM_SCRIPT_DOUBLE_ESCAPED_LESSTHAN;
        break;
      }
      case U_0000:
      {
        PARSE_ERROR("Unexpected null character");
        token.type(token_type::TT_CHARACTER);
        token.value(0xFFFD);
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
        token.value(v);
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
        if (temporaryBuffer == STR_SCRIPT)
        {
          smState = SM_SCRIPT_DOUBLE_ESCAPED;
        }
        else
        {
          token.type(token_type::TT_CHARACTER);
          token.value(v);
          emit = true;
          smState = SM_SCRIPT_ESCAPED;
        }
        break;
      }
      default:
      {
        if (std::isalpha(v))
        {
          temporaryBuffer.push_back(std::tolower(v));
          token.type(token_type::TT_CHARACTER);
          token.value(v);
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
          token.value({U_003C, U_002F});
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
          token.value({U_003C, U_002F});
          token += temporaryBuffer;
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
          v = std::tolower(v);
          token += v;
          temporaryBuffer.push_back(v);
        }
        else
        {
          token.type(token_type::TT_CHARACTER);
          token.value({U_003C, U_002F});
          token += temporaryBuffer;
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
      token.value(string_type());
      reconsume();
      smState = SM_SCRIPT_END_TAG_NAME;
    }
    else
    {
      token.type(token_type::TT_CHARACTER);
      token.value({U_003C, U_002F});
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
        token.value(U_002D);
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
        token.value(0xFFFD);
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
        token.value(v);
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
        token.value(U_002D);
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
        token.value(0xFFFD);
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
        token.value(v);
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
        token.value(U_002D);
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
        token.value(U_003E);
        emit = true;
        smState = SM_SCRIPT;
        break;
      }
      case U_0000:
      {
        PARSE_ERROR("Unexpected null character");
        smState = SM_SCRIPT_ESCAPED;
        token.type(token_type::TT_CHARACTER);
        token.value(0xFFFD);
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
        token.value(v);
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
          token.value({U_003C, U_002F});
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
          token.value({U_003C, U_002F});
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
          token += std::tolower(v);
          temporaryBuffer.push_back(v);
        }
        else
        {
          token.type(token_type::TT_CHARACTER);
          token.value({U_003C, U_002F});
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
      token.value(string_type());
      reconsume();
      smState = SM_SCRIPT_ESCAPED_END_TAG_NAME;
    }
    else
    {
      token.type(token_type::TT_CHARACTER);
      token.value({U_003C, U_002F});
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
          token.value(U_003C);
          emit = true;
          reconsume();
          smState = SM_SCRIPT_DOUBLE_ESCAPE_START;
        }
        else
        {
          token.type(token_type::TT_CHARACTER);
          token.value(U_003C);
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
      token.value(U_002D);
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
      token.value(U_002D);
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
        token.value({U_003C, U_0021});
        emit = true;
        break;
      }
      default:
      {
        smState = SM_SCRIPT;
        token.type(token_type::TT_CHARACTER);
        token.value(U_003C);
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
      token.value({U_003C, U_002F});
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
        token += 0xFFFD;
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
        token += std::tolower(v);
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
        token.value(U_003C);
        emit = true;
        break;
      }
      default:
      {
        if (std::isalpha(v))
        {
          token.type(token_type::TT_START_TAG);
          smState = SM_TAG_NAME;
        }
        else
        {
          token.type(token_type::TT_CHARACTER);
          token.value(U_003C);
          smState = SM_DATA;
          emit = true;
        }
        break;
      }
    }

    return emit;
  }
}
