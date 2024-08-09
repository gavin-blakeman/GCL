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

#ifndef GCL_PARSERS_HTML_BUFFERENCODER_H
#define GCL_PARSERS_HTML_BUFFERENCODER_H

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
#include "include/parsers/codePoint.h"
#include "include/utf.h"

/* Notes:
 * This class and file are not visible to the HTML API. This class is designed as a setup/teardown class when processing
 * a stream of bytes. The intent is that the buffer can be filled by a different thread to the html processing thread
 * this can make the processing independant of read speed.
 * When the class is created, it needs to determine the stream encoding. (See ...)
 * The buffer stores code points.
 */

namespace GCL::parsers
{
  class CBufferEncoder
  {
  public:
    using value_type = codePoint_t;

    /*! @brief      Constructor.
     *  @param[in]  is: The input stream to parse.
     *  @param[in]  begin: Iterator to the start of the token/string pairs.
     *  @param[in]  end: Iterator to the end of the token/strng pairs.
     *  @param[out] tokens: The container to receiver the tokens.
     */
    CBufferEncoder(std::istream &is) : inputStream(is)
    {
      fillBuffer();
    }

    virtual ~CBufferEncoder() = default;

  protected:
    /*! @brief    Fill the buffer when it falls below the minimum size.
     *  @note     The function needs to take into account the inputStream type (UTF encoding) and the variable byte count nature of the 
     *            UTF8 and UTF16 encodings.
     */
    virtual void fillBuffer()
    {
      while (!buffer.full() && !inputStream.eof())
      {
        switch (streamEncoding)
        {
          case UTF_8:
          {
            try
            {
              utf_t utf;
              char iByte;

              inputStream.get(iByte);
              utf.u8[0] = static_cast<std::uint8_t>(iByte);

              if (utf.u8[0] >= 128)
              {
                std::uint8_t byteCount = utf.u8[0] & 0b01110000;

                std::uint8_t indx = 1;

                while (byteCount)
                {
                  inputStream.get(iByte);
                  utf.u8[indx++] = static_cast<std::uint8_t>(iByte);
                  byteCount = byteCount >> 1;
                }
              }

              buffer.push_back(utf);
            }
            catch(...)
            {
              buffer.push_back(U_EOF);
            }
            break;
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
    value_type consume()
    {
      lastValue = buffer.front();
      buffer.pop();

      return lastValue;
    }

    /*! @brief     Pushes the last returned value  (from consume) back into the buffer.
     *  @note      If reconsume() is called before a call to consume() has been made, the behavior
     *             is undefined.
     */
    void reconsume()
    {
      buffer.push_front(lastValue);
    }

  private:
    CBufferEncoder() = delete;
    CBufferEncoder(CBufferEncoder const &) = delete;
    CBufferEncoder(CBufferEncoder &&) = delete;
    CBufferEncoder &operator=(CBufferEncoder const &) = delete;
    CBufferEncoder &operator=(CBufferEncoder &&) = delete;

    std::istream &inputStream;
    SCL::circular_biBuffer<value_type, 1024, false, 8, false> buffer;
    bool eos = false;
    value_type lastValue;
    utf_e streamEncoding = UTF_8;
  };

} // namespace

#endif // GCL_PARSERS_HTML_BUFFERENCODER_H