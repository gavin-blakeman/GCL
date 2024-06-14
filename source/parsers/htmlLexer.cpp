#include "include/parsers/htmlLexer.h"

#include <iostream>
#include <utility>
#include <vector>

#include "include/parsers/htmlTokenType.h"

namespace GCL::parsers
{
  static std::vector<std::pair<token_id, std::string>> tokenStrings =
  {
    { L_TAG_OPEN, "<" },
    { L_TAG_CLOSE, "</" },
    { R_TAG_OPEN, ">" },
    { R_TAG_CLOSE, "/>" },
    { L_TAG_DOCTYPE, "<!"},
    { COMMENT_OPEN, "<!---" },
    { COMMENT_CLOSE, "--->" },
    { ASSIGN,  "=" },
    { ID, "ID" },
    { VALUE, "Value" },
    { TEXT, "Text" },
  };

  CHTMLLexer::CHTMLLexer(std::istream &is) : CLexer(is)
  {
    CToken::tokenStrings.insert(tokenStrings.begin(), tokenStrings.end());
  }


  void CHTMLLexer::next()
  {
    if(match("/*"))
    {
      comment();
    }
    else if(match("<"))
    {
      tag();
    }
    else
    {
      text();
    }
  }

  void CHTMLLexer::whitespace()
  {
    while(std::isspace(buffer.front()))
    {
      consume();
    }
  }

  void CHTMLLexer::tag()
  {
    if(peek('!', 1))
    {
      tokens.push_back(CToken (htmlTokenTypes::L_TAG_DOCTYPE, std::string(""), row, col));
      consume(2); // <!
    }
    else if(peek('/', 1))
    {
      tokens.push_back(CToken (htmlTokenTypes::L_TAG_CLOSE, std::string(""), row, col));
      consume(2); // </
    }
    else
    {
      tokens.push_back(CToken (htmlTokenTypes::L_TAG_OPEN, std::string(""), row, col));
      consume(); // <
    }

    whitespace();
    id();

    token_id type;

    int sRow;
    int sCol;

    while(true)
    {
      if(match("="))
      {
        tokens.push_back(CToken (htmlTokenTypes::ASSIGN, "", row, col));
        consume();
      }
      else if(match(EOF))
      {
        value();
      }
      else if(match("/>"))
      {
        type = htmlTokenTypes::R_TAG_CLOSE;
        sRow = row;
        sCol = col;
        consume(2); // />
        break;
      }
      else if(match(">"))
      {
        type = htmlTokenTypes::R_TAG_OPEN;
        sRow = row;
        sCol = col;
        consume(); // >
        break;
      }
      else if(match(EOF))
      {
        sRow = row;
        sCol = col;
        break;
      }
      else
      {
        id();
      }
    }

    tokens.push_back(CToken (type, "", row, col));
  }

  void CHTMLLexer::comment()
  {
    tokens.push_back(CToken (htmlTokenTypes::COMMENT_OPEN, "", row, col));

    consume(4); // <!--

    whitespace();

    int sRow = row;
    int sCol = col;

    std::string str;

    while(true)
    {
      if(match("-->") || match(EOF))
      {
        break;
      }

      str.push_back(buffer.front());
      consume();
    }

    boost::trim(str);

    if(!str.empty())
    {
      tokens.push_back(CToken (htmlTokenTypes::TEXT, str, sRow, sCol));
    }

    tokens.push_back(CToken (htmlTokenTypes::COMMENT_CLOSE, "", row, col));

    consume(3); // -->
  }

  void CHTMLLexer::id()
  {
    int sRow = row;
    int sCol = col;

    std::string str;

    whitespace();

    while(true)
    {
      if(match(" ") || match("/>") || match('>') || match('=') || match(EOF))
      {
        break;
      }
      str.push_back(buffer.front());
      consume();
    }

    if(!str.empty())
    {
      tokens.push_back(CToken (htmlTokenTypes::ID, str, sRow, sCol));
    }
  }

  void CHTMLLexer::value()
  {
    consume(); // "

    int sRow = row;
    int sCol = col;

    std::string str;

    while(true)
    {
      if(match('"') || match(EOF))
      {
        break;
      }
      str.push_back(buffer.front());
      consume();
    }

    consume(); // "

    if(!str.empty())
    {
      tokens.push_back(CToken (htmlTokenTypes::VALUE, str, row, col));
    }
  }

  void CHTMLLexer::text()
  {
    int sRow = row;
    int sCol = col;

    std::string str;

    whitespace();

    while(true)
    {
      if(match('<') || match(EOF))
      {
        break;
      }

      str.push_back(buffer.front());
      consume();
    }

    boost::trim(str);

    if(!str.empty())
    {
      tokens.push_back(CToken (htmlTokenTypes::TEXT, str, sRow, sCol));
    }
  }



} // namespace
