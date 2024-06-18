//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// SUBSYSTEM:           Parsers
// FILE:                lexer.h
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
// OVERVIEW:            Class provides a generic interface for lexing a stream.
//
// CLASSES INCLUDED:
//
// HISTORY:             2024-06-18 GGB - File Created
//
//**********************************************************************************************************************************


#include "include/parsers/lexer.h"

namespace GCL::parsers
{

  CLexer::CLexer(std::istream &is, std::vector<CToken> &t) : inputStream(is), tokens(t)
  {
    fillBuffer();
  }

  void CLexer::getTokens()
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
