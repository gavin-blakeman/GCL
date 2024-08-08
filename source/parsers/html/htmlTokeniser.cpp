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
        case SM_RAWTEXT:
        {
          emit = processRawText(token);
          break;
        }
        case SM_SCRIPT:
        {
          emit = processScript(token);
          break;
        }
        case SM_PLAINTEXT:
        {
          emit = processPlainText(token);
          break;
        }
        case SM_RCDATA_LESSTHAN:
        {
          emit = processRCDataLessThan(token);
          break;
        }
      }
    }

    return token;
  }

  bool CHTMLTokeniser::processData(CToken &token)
  {
    bool emit = false;
    int v = pop();
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
        PARSE_ERROR("unexpexted null character");
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
    int v = pop();
    switch(v)
    {
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

  bool CHTMLTokeniser::processRawText(CToken &token)
  {
    bool emit = false;
    int v = pop();
    switch(v)
    {
      case '<':
      {
        smState = SM_RAWTEXT_LESSTHAN;
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

  bool CHTMLTokeniser::processRCData(CToken &token)
  {
    bool emit = false;
    int v = pop();
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

  bool CHTMLTokeniser::processRCDataLessThan(CToken &token)
  {
    bool emit = false;

    switch(front())
    {
      case 0x002F:	// '/'
      {
        consume();
        temporaryBuffer.clear();
        smState = SM_RCDATA_END_TAG_OPEN;
        break;
      }
      default:
      {
        token.type(TT_CHARACTER);
        token.value(0x003C);
        smState = SM_RCDATA;
        break;
      }
    }

    return emit;
  }

  bool CHTMLTokeniser::processScript(CToken &token)
  {
    bool emit = false;
    int v = pop();
    switch(v)
    {
      case '<':
      {
        smState = SM_SCRIPT_LESSTHAN;
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

    int v = pop();
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
