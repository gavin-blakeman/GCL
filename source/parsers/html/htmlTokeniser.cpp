#include "include/parsers/html/htmlTokeniser.h"

// Standard C++ library header files
#include <cctype>

// parsers include files
#include "include/parsers/html/htmlExceptions.h"
#include "include/parsers/html/htmlLanguageTokens.h"

namespace GCL::parsers::html
{

  CToken CHTMLTokeniser::getToken()
  {
    bool emit = false;
    CToken token(tokenStrings);

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
        case SM_PLAINTEXT:
        {
          emit = processPlainText(token);
          break;
        }
      }
    }

    return token;
  }

  bool CHTMLTokeniser::processData(CToken &token)
  {
    bool emit = false;
    int v = consume();
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
        token.type(TT_CHARACTER);
        token.value("");
        emit = true;
        break;
      }
      case EOF:
      {
        token.type(TT_EOF);
        emit = true;
        break;
      }
      default:
      {
        token.type(TT_CHARACTER);
        token.value(static_cast<char>(v));
        emit = true;
      }
    }
    return emit;
  }

  bool CHTMLTokeniser::processPlainText(CToken &token)
  {
    bool emit = false;
    int v = consume();
    switch(v)
    {
      case '\0':
      {
        PARSE_ERROR("unexpected null character");
        token.type(TT_CHARACTER);
        token.value(0xFFFD);
        emit = true;
        break;
      }
      case EOF:
      {
        token.type(TT_EOF);
        emit = true;
        break;
      }
      default:
      {
        token.type(TT_CHARACTER);
        token.value(static_cast<char>(v));
        emit = true;
      }
    }
    return emit;
  }

  bool CHTMLTokeniser::processRawText(CToken &token)
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
        token.type(TT_CHARACTER);
        token.value(0xFFFD);
        emit = true;
        break;
      }
      case EOF:
      {
        token.type(TT_EOF);
        emit = true;
        break;
      }
      default:
      {
        token.type(TT_CHARACTER);
        token.value(static_cast<char>(v));
        emit = true;
      }
    }
    return emit;
  }

  bool CHTMLTokeniser::processRawTextEndTagName(CToken &token)
  {
    bool emit = false;

    int v = consume();
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
          token.type(TT_CHARACTER);
          token.value("</");
          token += temporaryBuffer;
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
          token.type(TT_CHARACTER);
          token.value("</");
          token += temporaryBuffer;
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
          token += v;
          temporaryBuffer.push_back(v);
        }
        else
        {
          token.type(TT_CHARACTER);
          token.value("</");
          token += temporaryBuffer;
          temporaryBuffer.clear();
          emit = true;
          reconsume();
          smState = SM_RAWTEXT;
        }
      }
    }

    return emit;
  }

  bool CHTMLTokeniser::processRawTextEndTagOpen(CToken &token)
  {
    bool emit = false;

    int v = consume();

    if (std::isalpha(v))
    {
      token.type(TT_END_TAG);
      token.value("");
      reconsume();
      smState = SM_RAWTEXT_END_TAG_NAME;
    }
    else
    {
      token.type(TT_CHARACTER);
      token.value("</");
      emit = true;
      reconsume();
      smState = SM_RAWTEXT;
    }

    return emit;
  }

  bool CHTMLTokeniser::processRawTextLessThan(CToken &token)
  {
    bool emit = false;

    int v = consume();

    if (v == 0x002F)
    {
      temporaryBuffer.clear();
      smState = SM_RAWTEXT_END_TAG_OPEN;
    }
    else
    {
      token.type(TT_CHARACTER);
      token.value("<");
      emit = true;
      reconsume();
      smState = SM_RAWTEXT;
    }

    return emit;
  }

  bool CHTMLTokeniser::processRCData(CToken &token)
  {
    bool emit = false;
    int v = consume();
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
        token.type(TT_CHARACTER);
        token.value(0xFFFD);
        emit = true;
        break;
      }
      case EOF:
      {
        token.type(TT_EOF);
        emit = true;
        break;
      }
      default:
      {
        token.type(TT_CHARACTER);
        token.value(static_cast<char>(v));
        emit = true;
      }
    }
    return emit;
  }

  bool CHTMLTokeniser::processRCDataEndTagName(CToken &token)
  {
    bool emit = false;

    int v = consume();

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
          token.type(TT_CHARACTER);
          token.value("</");
          token += temporaryBuffer;
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
          token.type(TT_CHARACTER);
          token.value("</");
          token += temporaryBuffer;
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
          token.type(TT_CHARACTER);
          token.value("</");
          token += temporaryBuffer;
          temporaryBuffer.clear();
          emit = true;
          reconsume();
          smState = SM_RCDATA;
        }
      }
    }

    return emit;
  }

  bool CHTMLTokeniser::processRCDataEndTagOpen(CToken &token)
  {
    bool emit = false;

    int v = consume();

    if (std::isalpha(v))
    {
      token.type(TT_END_TAG);
      token.value("");
      reconsume();
      smState = SM_RCDATA_END_TAG_NAME;
    }
    else
    {
      token.type(TT_CHARACTER);
      token.value("</");
      smState = SM_RCDATA;
      reconsume();
    }

    return emit;
  }

  bool CHTMLTokeniser::processRCDataLessThan(CToken &token)
  {
    bool emit = false;

    switch(consume())
    {
      case 0x002F:	// '/'
      {
        temporaryBuffer.clear();
        smState = SM_RCDATA_END_TAG_OPEN;
        break;
      }
      default:
      {
        token.type(TT_CHARACTER);
        token.value(0x003C);
        smState = SM_RCDATA;
        reconsume();
        break;
      }
    }

    return emit;
  }

  bool CHTMLTokeniser::processScript(CToken &token)
  {
    bool emit = false;
    int v = consume();
    switch(v)
    {
      case '<':
      {
        smState = SM_SCRIPT_LESSTHAN;
        break;
      };
      case '\0':
      {
        PARSE_ERROR("Unexpected null character");
        token.type(TT_CHARACTER);
        token.value(0xFFFD);
        emit = true;
        break;
      }
      case EOF:
      {
        token.type(TT_EOF);
        emit = true;
        break;
      }
      default:
      {
        token.type(TT_CHARACTER);
        token.value(static_cast<char>(v));
        emit = true;
      }
    }
    return emit;
  }

  bool CHTMLTokeniser::processScriptEndTagName(CToken &token)
  {
    bool emit = false;

    int v = consume();

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
          token.type(TT_CHARACTER);
          token.value("</");
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
          token.type(TT_CHARACTER);
          token.value("</");
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
          token.type(TT_CHARACTER);
          token.value("</");
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

  bool CHTMLTokeniser::processScriptEndTagOpen(CToken &token)
  {
    bool emit = false;

    int v = consume();

    if (std::isalpha(v))
    {
      token.type(TT_END_TAG);
      token.value("");
      reconsume();
      smState = SM_SCRIPT_END_TAG_NAME;
    }
    else
    {
      token.type(TT_CHARACTER);
      token.value("</");
      emit = true;
      reconsume();
      smState = SM_SCRIPT;
    }

    return emit;
  }

  bool CHTMLTokeniser::processScriptEscaped(CToken &token)
  {
    bool emit = false;

    int v = consume();

    switch (v)
    {
      case 0x002D:
      {
        smState = SM_SCRIPT_ESCAPED_DASH;
        token.type(TT_CHARACTER);
        token.value("-");
        emit = true;
        break;
      }
      case 0x003C:
      {
        smState = SM_SCRIPT_ESCAPED_LESSTHAN;
        break;
      }
      case 0x0000:
      {
        PARSE_ERROR("Unexpected null character");
        token.type(TT_CHARACTER);
        token.value(0xFFFD);
        emit = true;
        break;
      }
      case EOF:
      {
        PARSE_ERROR("eof in script, html,comment like text");
        token.type(TT_EOF);
        emit = true;
        break;
      }
      default:
      {
        token.type(TT_CHARACTER);
        token.value(v);
        emit = true;
      }
    }

    return emit;
  }

  bool CHTMLTokeniser::processScriptEscapedDash(CToken &token)
  {
    bool emit = false;

    int v = consume();

    switch(v)
    {
      case 0x002D:
      {
        token.type(TT_CHARACTER);
        token.value("-");
        emit = true;
        smState = SM_SCRIPT_ESCAPED_START_DASH_DASH;
        break;
      }
      case 0x003C:
      {
        smState = SM_SCRIPT_ESCAPED_LESSTHAN;
        break;
      }
      case 0x0000:
      {
        PARSE_ERROR("Unexpected null character");
        smState = SM_SCRIPT_ESCAPED;
        token.type(TT_CHARACTER);
        token.value(0xFFFD);
        emit = true;
        break;
      }
      case EOF:
      {
        PARSE_ERROR("eof in script, html,comment like text");
        token.type(TT_EOF);
        emit = true;
        break;
      }
      default:
      {
        token.type(TT_CHARACTER);
        token.value(v);
        emit = true;
        smState = SM_SCRIPT_ESCAPED;
        break;
      }
    }

    return emit;
  }

  bool CHTMLTokeniser::processScriptEscapedDashDash(CToken &token)
  {
    bool emit = false;

    int v = consume();

    switch(v)
    {
      case 0x002D:
      {
        token.type(TT_CHARACTER);
        token.value("-");
        emit = true;
        break;
      }
      case 0x003C:
      {
        smState = SM_SCRIPT_ESCAPED_LESSTHAN;
        break;
      }
      case 0x003E:
      {
        token.type(TT_CHARACTER);
        token.value(">");
        emit = true;
        smState = SM_SCRIPT;
        break;
      }
      case 0x0000:
      {
        PARSE_ERROR("Unexpected null character");
        smState = SM_SCRIPT_ESCAPED;
        token.type(TT_CHARACTER);
        token.value(0xFFFD);
        emit = true;
        break;
      }
      case EOF:
      {
        PARSE_ERROR("eof in script, html,comment like text");
        token.type(TT_EOF);
        emit = true;
        break;
      }
      default:
      {
        token.type(TT_CHARACTER);
        token.value(v);
        emit = true;
        smState = SM_SCRIPT_ESCAPED;
        break;
      }
    }

    return emit;
  }

  bool CHTMLTokeniser::processScriptEscapedEndTagName(CToken &token)
  {
    bool emit = false;

    int v = consume();

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
          if (v == 0x002F)
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
          token.type(TT_CHARACTER);
          token.value("</");
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
          token.type(TT_CHARACTER);
          token.value("</");
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
          token.type(TT_CHARACTER);
          token.value("</");
          token += temporaryBuffer;
          emit = true;
          reconsume();
          smState = SM_SCRIPT_ESCAPED;
        }
      }
    }

    return emit;
  }

  bool CHTMLTokeniser::processScriptEscapedEndTagOpen(CToken &token)
  {
    bool emit = false;

    int v = consume();

    if (std::isalpha(v))
    {
      token.type(TT_END_TAG);
      token.value("");
      reconsume();
      smState = SM_SCRIPT_ESCAPED_END_TAG_NAME;
    }
    else
    {
      token.type(TT_CHARACTER);
      token.value("</");
      emit = true;
      reconsume();
      smState = SM_SCRIPT_ESCAPED;
    }

    return emit;
  }

  bool CHTMLTokeniser::processScriptEscapedLessThan(CToken &token)
  {
    bool emit = false;

    int v = consume();

    switch(v)
    {
      case 0x002F:
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
          token.type(TT_CHARACTER);
          token.value("<");
          emit = true;
          reconsume();
          smState = SM_SCRIPT_DOUBLE_ESCAPE_START;
        }
        else
        {
          token.type(TT_CHARACTER);
          token.value("<");
          emit = true;
          reconsume();
          smState = SM_SCRIPT_ESCAPED;
        }
        break;
      }
    }

    return emit;
  }

  bool CHTMLTokeniser::processScriptEscapeStart(CToken &token)
  {
    bool emit = false;

    int v = consume();

    if (v == 0x002D)
    {
      smState = SM_SCRIPT_ESCAPE_START_DASH;
      token.type(TT_CHARACTER);
      token.value("-");
      emit = true;
    }
    else
    {
      reconsume();
      smState = SM_SCRIPT;
    }

    return emit;
  }

  bool CHTMLTokeniser::processScriptEscapeStartDash(CToken &token)
  {
    bool emit = false;

    int v = consume();

    if (v == 0x002D)
    {
      smState = SM_SCRIPT_ESCAPED_START_DASH_DASH;
      token.type(TT_CHARACTER);
      token.value("-");
      emit = true;
    }
    else
    {
      reconsume();
      smState = SM_SCRIPT;
    }

    return emit;
  }

  bool CHTMLTokeniser::processScriptLessThan(CToken &token)
  {
    bool emit = false;

    int v = consume();

    switch (v)
    {
      case 0x002F:
      {
        temporaryBuffer.clear();
        smState = SM_RCDATA_END_TAG_OPEN;
        break;
      }
      case 0x0021:
      {
        smState = SM_SCRIPT_ESCAPE_START;
        token.type(TT_CHARACTER);
        token.value("<!");
        emit = true;
        break;
      }
      default:
      {
        smState = SM_SCRIPT;
        token.type(TT_CHARACTER);
        token.value("<");
        reconsume();
        emit = true;
        break;
      }
    }

    return emit;
  }

  bool CHTMLTokeniser::processEndTagOpen(CToken &token)
  {
    bool emit = false;

    int v = front();
    if (std::isalpha(v))
    {
      token.type(TT_END_TAG);
      smState = SM_TAG_NAME;
    }
    else if (v == '>')
    {
      consume();
      PARSE_ERROR("missing end tag name");
      smState = SM_DATA;
    }
    else if (v == EOF)
    {
      consume();
      PARSE_ERROR("eof before tag name");
      token.type(TT_EOF);
      token.value("</");
    }
    else
    {
      PARSE_ERROR("invalid first character of tag name");
      token.type(TT_COMMENT);
      smState = SM_BOGUS_COMMENT;
    }

    return emit;
  }

  bool CHTMLTokeniser::processTagName(CToken &token)
  {
    bool emit = false;

    int v = consume();

    switch(v)
    {
      case '\t':
      case '\n':
      case '\f':
      case ' ':
      {
        smState = SM_BEFORE_ATTR_NAME;
        break;
      }
      case '/':
      {
        smState = SM_TAG_SELF_CLOSING_START;
        break;
      }
      case '>':
      {
        emit = true;
        smState = SM_DATA;
        break;
      };
      case '\0':
      {
        PARSE_ERROR("unexpected null character");
        token += 0xFFFD;
        break;
      }
      case EOF:
      {
        PARSE_ERROR("eof in tag");
        token.type(TT_EOF);
        token.value("");
        emit = true;
        break;
      }
      default:
      {
        token += std::tolower(static_cast<char>(v));
        break;
      }
    };

    return emit;
  };

  bool CHTMLTokeniser::processTagOpen(CToken &token)
  {
    bool emit = false;

    int c = front();
    switch (c)
    {
      case '!':
      {
        smState = SM_MARKUP_DECLARATION_OPEN;
        consume();
        break;
      }
      case '/':
      {
        smState = SM_END_TAG_OPEN;
        consume();
        break;
      }
      case '?':
      {
        smState = SM_BOGUS_COMMENT;
        token.type(TT_COMMENT);
        break;
      }
      case EOF:
      {
        consume();
        throw parse_error("eof before tag name");
        token.type(TT_EOF);
        token.value("<");
        emit = true;
        break;
      }
      default:
      {
        if (std::isalpha(c))
        {
          token.type(TT_START_TAG);
          smState = SM_TAG_NAME;
        }
        else
        {
          token.type(TT_CHARACTER);
          token.value("<");
          smState = SM_DATA;
          emit = true;
        }
        break;
      }
    }

    return emit;
  }
}
