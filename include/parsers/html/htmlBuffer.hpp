//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// SUBSYSTEM:           Parsers
// FILE:                bufferEncoder.h
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

#ifndef GCL_PARSERS_HTML_BUFFER_H
#define GCL_PARSERS_HTML_BUFFER_H

// Standard C++ library header files
#include <atomic>
#include <cstdint>
#include <deque>
#include <iostream>
#include <mutex>
#include <semaphore>
#include <shared_mutex>
#include <string>
#include <thread>
#include <vector>

// Miscellaneous library header files

// GCL/parser header files.
#include "include/utf/codePoint.hpp"
#include "include/utf/utf.h"
#include "include/utf/utfString.hpp"

/* Notes:
 * This class and file are not visible to the HTML API. This class is designed as a setup/teardown class when processing
 * a stream of bytes. The intent is that the buffer can be filled by a different thread to the html processing thread
 * this can make the processing independant of read speed.
 * When the class is created, it needs to determine the stream encoding. (See ...)
 * The buffer stores code points.
 */

namespace GCL::parsers::html
{
  class CHTMLBuffer
  {
  public:
    using char_type = codePoint_t;
    using string_type = std::basic_string<char_type>;
    using new_string_type = utf_string<char32_t>;

    /*! @brief      Constructor.
     *  @param[in]  is: The input stream to parse.
     *  @param[in]  begin: Iterator to the start of the token/string pairs.
     *  @param[in]  end: Iterator to the end of the token/strng pairs.
     *  @param[out] tokens: The container to receiver the tokens.
     */
    CHTMLBuffer(std::istream &is) : inputStream(is)
    {
      fillBuffer();
    }

    virtual ~CHTMLBuffer() = default;

  protected:
    char_type currentChar;

    /*! @brief    Fill the buffer when it falls below the minimum size.
     *  @note     The function needs to take into account the inputStream type (UTF encoding) and the variable byte count nature of the
     *            UTF8 and UTF16 encodings.
     */
    virtual void fillBuffer()
    {
      inputStream.exceptions(std::istream::eofbit); // May throw. Throws exception when EOF bit is set.
      while (!inputStream.eof())
      {
        switch (streamEncoding)
        {
          case UTF_8:
          {
          }
          case UTF_16BE:
          case UTF_16LE:
          {
            break;
          }
        }
      }

      if (inputStream.eof())
      {
        eos = true;
        buffer.push_back(U_EOF);
      }
    }

    /*! @brief      Pops and returns the value at the head of the buffer.
     *  @returns    The value at the front of the buffer.
     *  @throws     If  the buffer is empty.
     */
    void consume()
    {
      currentChar = buffer.front();
      buffer.pop_front();
    }

    /*! @brief      Consume multiple characters.
     *  @param[in]  n: number of characters to consume.
     *  @throws
     */
    void consume(std::size_t n)
    {
      while (n != 0)
      {
        consume();
        n--;
      }
    }

    /*! @brief      Matches a string to he current buffer.
     *  @param[in]  s: The string to match.
     *  @param[in]  cs: Case sensitive compare
     *  @returns    true if the string is the next set of characters in the buffer.
     */
    bool match(string_type const &s, bool cs)
    {
      bool bMatch = true;
      for (auto i = 0; i != s.size() && bMatch; i++)
      {
        if (cs)
        {
          bMatch = s[i] == buffer[i];
        }
        else
        {
          bMatch = s[i].tolower() == buffer[i].tolower();
        }
      }

      return bMatch;
    }

    /*! @brief     Pushes the last returned value  (from consume) back into the buffer.
     *  @note      If reconsume() is called before a call to consume() has been made, the behavior
     *             is undefined.
     */
    void reconsume()
    {
      buffer.push_front(currentChar);
    }

  private:
    CHTMLBuffer() = delete;
    CHTMLBuffer(CHTMLBuffer const &) = delete;
    CHTMLBuffer(CHTMLBuffer &&) = delete;
    CHTMLBuffer &operator=(CHTMLBuffer const &) = delete;
    CHTMLBuffer &operator=(CHTMLBuffer &&) = delete;

    std::istream &inputStream;
    std::deque<char_type> buffer;
    bool eos = false;
    utf_e streamEncoding = UTF_8;
  };

} // namespace

#endif // GCL_PARSERS_HTML_BUFFERENCODER_H
