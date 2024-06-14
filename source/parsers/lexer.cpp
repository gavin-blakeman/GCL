#include "include/parsers/lexer.h"

namespace GCL::parsers
{

  CLexer::CLexer(std::istream &is) : inputStream(is)
  {
    fillBuffer();
  }

  std::vector<CToken> CLexer::getTokens()
  {
    tokens.clear();

    while(buffer.front() != EOF)
    {
      next();
      if (!buffer.full() && !eos)
      {
        fillBuffer();
      }
    }

    tokens.push_back(CToken (TT_EOF, std::string(""), row, col));

    return std::move(tokens);
  }

  void CLexer::consume()
  {
    if(buffer.front() == '\n')
    {
      row++;
      col = 1;
    }
    else
    {
      col++;
    }
    buffer.pop();
  }

  void CLexer::consume(int n)
  {
    for(int i = 0; i < n; ++i)
    {
      consume();
    }
  }

  bool CLexer::match(std::string const &str)
  {
    bool rv = true;
    for(std::size_t i = 0; i != str.size(); i++)
    {
      if (buffer[i] != str[i])
      {
        rv = false;
        break;
      }
    }
    return rv;
  }

  bool CLexer::match(char const c)
  {
    return (buffer.front() == c);
  }

  bool CLexer::peek(char c, std::size_t pos)
  {
    if (!buffer.empty())
    {
      return ((buffer[pos] == c));
    }
    else
    {
      return 0;
    }
  }

  void CLexer::next()
  {
    consume();
  }

  void CLexer::fillBuffer()
  {
    while (!buffer.full() && !inputStream.eof())
    {
      buffer.push(inputStream.get());
    }
    if (inputStream.eof())
    {
      eos = true;
      buffer.push(EOF);
    }
  }



} // namespace
