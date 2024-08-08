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
//**********************************************************************************************************************************/


#ifndef GCL_PARSERS_LEXER_H
#define GCL_PARSERS_LEXER_H

// Standard C++ library header files
#include <atomic>
#include <cstdint>
#include <istream>
#include <mutex>
#include <semaphore>
#include <shared_mutex>
#include <string>
#include <thread>
#include <vector>

// Miscellaneous library header files
#include <SCL>

// GCL/parser header files.
#include "include/parsers/token.h"
#include "include/parsers/languageTokens.h"

namespace GCL::parsers
{
  class CLexer
  {
  public:
    using tokenID_t = CToken::tokenID_t;
    using tokenStringMap_t = CToken::tokenStringMap_t;

    /*! @brief      Constructor.
     *  @param[in]  is: The input stream to parse.
     *  @param[in]  begin: Iterator to the start of the token/string pairs.
     *  @param[in]  end: Iterator to the end of the token/strng pairs.
     *  @param[out] tokens: The container to receiver the tokens.
     */
    CLexer(std::istream &is) : inputStream(is)
    {
      fillBuffer();
    }

    virtual ~CLexer() = default;

  protected:
    std::size_t lineNo = 0;
    std::size_t linePos = 0;
    SCL::circularBuffer<int, 1024, false, false> buffer;

    int front()
    {
      return buffer.front();
    }

    /*! @brief      Checks if the next character in the stream matches the parameter.
     *  @param[in]  c: The character to test.
     *  @param[in]  pos: The position in the stream to check.
     *  @returns    true if the c matches the next character in the stream.
     */
    bool peek(char c, std::size_t pos)
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

    void consume(int n)
    {
      while (n)
      {
        if(buffer.front() == '\n')
        {
          lineNo++;
          linePos = 0;
        }
        else
        {
          linePos++;
        }
        buffer.pop();
        n--;
      }
    }

    bool match(std::string const &str)
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

    bool match(char const c)
    {
      return (buffer.front() == c);
    }

    /*! @brief    Fill the buffer when it falls below the minimum size.
     */
    virtual void fillBuffer()
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

    void consume()
    {
      if(buffer.front() == '\n')
      {
        lineNo++;
        linePos = 0;
      }
      else
      {
        linePos++;
      }
      buffer.pop();
    }

    int pop()
    {
      int rv = front();
      consume();
      return rv;
    }

  private:
    CLexer() = delete;
    CLexer(CLexer const &) = delete;
    CLexer(CLexer &&) = delete;
    CLexer &operator=(CLexer const &) = delete;
    CLexer &operator=(CLexer &&) = delete;

    std::istream &inputStream;
    bool eos = false;

  friend std::ostream &operator<<(std::ostream &os, CToken const &);  // This needs to be here as it needs the token map.
  };

} // namespace

#endif // GCL_PARSERS_LEXER_H
