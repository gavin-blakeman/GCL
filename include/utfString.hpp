//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                cutfString.h
// LANGUAGE:            C++
// TARGET OS:           None.
// NAMESPACE:           GCL
// AUTHOR:              Gavin Blakeman (GGB)
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
//
// OVERVIEW:            A UTF string class encapsulating and abstracting UTF8, UTF16, UTF32 strings.
//
// HISTORY:             2024-08-18 GGB - File created.
//
//*********************************************************************************************************************************/

#ifndef GCL_UTFSTRING_HPP
#define GCL_UTFSTRING_HPP

// Standard C++ library header files
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace GCL
{
  template<typename T, typename ... U>
  concept IsAnyOf = (std::same_as<T, U> || ...);

  template<typename T>
  concept UTFChar = IsAnyOf<std::remove_cvref_t<std::remove_pointer_t<std::decay_t<T>>>, char8_t, char16_t, char32_t>;

  /*! @details The utf_string class encapsulates a UTF string of type selected by the programmer (UTF8, UTF16, UTF32). The class stores string in
   *           the selected format. Modifiers/Accessors/comparator functions are provided to make the storage type transparent. IE the class can
   *           use UTF32 as the underlying storage type, be called with a UTF8 constructed and provide a UTF16 string as output. All conversions
   *           between types are done internally.
   *           Transcoding from/to non-utf code-sets are also supported.
   * @tparam   CharT: The character type. [char8_t, char16_t, char32_t]
   * @tparam   EOF: If true, then an EOF character is defined as -1. This EOF character will not cause any errors or problems during string conversions
   *           and will be correctly converted..
   */

  template<typename CharT,
           bool eof = true,
           class Traits = std::char_traits<CharT>,
           class Allocator = std::allocator<CharT>> requires UTFChar<CharT>
  class utf_string
  {
  public:
    using char_type = CharT;
    using string_type = std::basic_string<char_type>;
    using iterator = string_type::iterator;
    using const_iterator = string_type::const_iterator;
    using size_type = string_type::size_type;
    using encoding_type = std::string;
    using optional_encoding = std::optional<encoding_type>;

    class unexpected_eof : public std::runtime_error { unexpected_eof() : std::runtime_error("EOF") {} };
    class bad_codepoint : public std::runtime_error { bad_codepoint() : std::runtime_error("Bad codepoint in stream") {} };

    constexpr utf_string() noexcept(noexcept(Allocator())) = default;
    utf_string(utf_string const &) = default;
    utf_string(utf_string &&) = default;

    /*! @brief      Construct from a string. If no encoding is given, the string will be evaluated as an ascii (8 bit) string during
     *              conversion.
     *  @param[in]  other: The string to construct from.
     *  @param[in]  encoding: The encoding of the string.
     */
    utf_string(std::string const &other, optional_encoding encoding = optional_encoding())
    {
    }

    /*! @brief      Construct from a UTF8 string.
     *  @param[in]  other: The UTF8 string to construct from.
     *  @throws
     */
    utf_string(std::u8string const &other)
    {
      if constexpr(isUTF8)
      {
        stringStorage.assign(other.begin(), other.end());
      }
      else
      {
      }
    }

    /*! @brief      Construct from a UTF16 string.
     *  @param[in]  other: The UTF16 string to construct from.
     *  @throws
     */
    utf_string(std::u16string const &other)
    {
      if constexpr(isUTF16)
      {
        stringStorage.assign(other.begin(), other.end());
      }
      else
      {
      }
    }

    /*! @brief      Construct from a UTF32 string.
     *  @param[in]  other: The UTF32 string to construct from.
     *  @throws
     */
    utf_string(std::u32string const &other)
    {
      if constexpr (isUTF32)
      {
        stringStorage.assign(other.begin(), other.end());
      }
    }

    ~utf_string() = default;

    utf_string &operator=(utf_string const &) = default;
    utf_string &operator=(utf_string &&) = default;

    explicit operator std::u8string();
    explicit operator std::u16string();
    explicit operator std::u32string();

    /*! @brief      Reads a single code_point (if possible) from the stream and appends it to *this.
     *  @param[in]  is: The stream.
     *  @param[in]  encoding: The input stream encoding.
     *  @returns    The stream.
     *  @throws     utf_string::eof on an eof.
     *  @throws     utf_string::bad_codepoint if the code point cannot be converted.
     */
    std::istream &get(std::istream &is, optional_encoding encoding = optional_encoding())
    {
      std::ios::iostate isState = is.exceptions();
      is.exceptions(isState | std::ifstream::eofbit); // May throw. Throws exception when EOF bit is set.

      try
      {
        char_type codePoint;
        std::string munched;
        bool found = false;
        char iByte;

        while( (munched.size() < 4) && !found)
        {
          is.get(iByte);
          munched.push_back(iByte);
          found = isCodePoint(munched, codePoint);
        }
      }
      catch(std::ios_base::failure const &e)
      {
        is.exceptions(isState);
        throw unexpected_eof();
        }
      }
      is.exceptions(isState);

      if (found)
      {
       stringStorage.push_back(transcode);
      }
      else
      {
        throw bad_codepoint();
      }

      return is;
    }

    /*! @brief      Reads up to the number of codePoints specified by maxPoints from the stream and appends it to *this.
     *  @param[in]  is: The stream.
     *  @param[in]  encoding: The input stream encoding.
     *  @returns    The stream.
     *  @throws     utf_string::eof on an eof.
     *  @throws     utf_string::bad_codepoint if the code point cannot be converted.
     */
    std::istream &get(std::istream &is, size_type maxPoints, optional_encoding encoding = optional_encoding())
    {
      std::ios::iostate isState = is.exceptions();
      is.exceptions(isState | std::ifstream::eofbit); // May throw. Throws exception when EOF bit is set.

      size_type indx;
      while (!is.eof() && indx < maxPoints)
      {
        get(is, encoding);
        indx++;
      }
      

      is.exceptions(isState);
      return is;
    }

    void setStoreEOF() noexcept final { eofFlag = true; }
    void clrStoreEOF() noexcept final { eofFlag = false; }

  private:
    static bool constexpr isUTF8 = std::is_same<CharT, char8_t>::value;
    static bool constexpr isUTF16 = std::is_same<CharT, char16_t>::value;
    static bool constexpr isUTF32 = std::is_same<CharT, char32_t>::value;
    string_type stringStorage;
    bool eofFlag = false;

    /*! @brief    Typecast like operator to typecase the the type of UTF string required.
     */
    template<typename U> requires UTFChar<typename U::value_type>
    U const &operator()() const
    {
    }

    /*! @brief      Simple case for string equality when both types have the same underlying storage.
     */
    friend bool operator==(utf_string const &lhs, utf_string const &rhs) { return lhs.stringStorage == rhs.stringStorage; }

    /*! @brief      Equality operator when the strings have different underlying storage.
     */

    template<typename U> requires UTFChar<typename U::value_type>
    friend bool operator==(utf_string const &lhs, U const &rhs)
    {
     // Convert the rhs string using an explicit static_cast.
      return lhs.stringStorage == static_cast<string_type>(rhs);
    }

    /*! @brief      String insertion operator. Always converts to UTF8 before insertion.
     *  @param[in]  os: The output stream.
     *  @param[in]  obj: The object instance to output.
     *  @returns    reference to os.
     *  @throws
     */
    friend std::ostream &operator<<(std::ostream &os, utf_string const &obj)
    {
      os << static_cast<std::u8string>(obj);
      return os;
    }
  };
}


#endif /* GCL_UTFSTRING_HPP */
