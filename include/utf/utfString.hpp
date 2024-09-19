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
#include <algorithm>
#include <iostream>
#include <optional>
#include <string>

// GCL Library
#include "include/concepts.hpp"
#include "include/utf/utf.h"

namespace GCL
{
  enum encoding_type { UTF8, UTF16, UTF32 };

  /*! @details The utf_string class encapsulates a UTF string of type selected by the programmer (UTF8, UTF16, UTF32). The class
   *           stores string in the selected format. Modifiers/Accessors/comparator functions are provided to make the storage type
   *           transparent. IE the class can use UTF32 as the underlying storage type, be called with a UTF8 constructed and provide
   *           a UTF16 string as output. All conversions between types are done internally.
   * @tparam   CharT: The character type. [char8_t, char16_t, char32_t]
   * @tparam   EOF: If true, then an EOF character is defined as -1. This EOF character will not cause any errors or problems during
   *           string conversions and will be correctly converted.
   */

  template<typename CharT,
  bool eof = true,
  class Traits = std::char_traits<CharT>,
  class Allocator = std::allocator<CharT>> requires UTFChar<CharT>
  class utf_string
  {
  public:
    using value_type = CharT;
    using string_type = std::basic_string<value_type>;
    using iterator = string_type::iterator;
    using const_iterator = string_type::const_iterator;
    using size_type = string_type::size_type;
    using optional_encoding = std::optional<encoding_type>;

    constexpr utf_string() noexcept(noexcept(Allocator())) = default;
    utf_string(utf_string const &) = default;
    utf_string(utf_string &&) = default;

    /*! @brief      Construct from a string. If no encoding is given, the string will be evaluated as an ASCII (8 bit) string during
     *              conversion.
     *  @param[in]  other: The string to construct from.
     *  @param[in]  encoding: The encoding of the string.
     */
    utf_string(std::string const &other, optional_encoding encoding = optional_encoding())
    {
      if (!encoding)
      {

      }
      else
      {
        switch (*encoding)
        {
          case UTF8:
          {
            if constexpr (isUTF8)
            {
              stringStorage.assign(other.begin(), other.end());
            }
            else
            {
            }
            break;
          }
          case UTF16:
          {
            if constexpr (isUTF16)
            {
              stringStorage.assign(other.begin(), other.end());
            }
            else
            {

            }
            break;
          }
          case UTF32:
          {
            if constexpr (isUTF32)
            {
              stringStorage.assign(other.begin(), other.end());
            }
            break;
          }
        }
      }
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
        std::uint32_t codePoint;

        auto iter = other.begin();

        while (iter != other.end())
        {
          iter = decodeUTF(iter, other.end(), codePoint);
          encodeUTF(codePoint, stringStorage);
        }
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
        std::uint32_t codePoint;

        auto iter = other.begin();

        while (iter != other.end())
        {
          iter = decodeUTF(iter, other.end(), codePoint);
          encodeUTF(codePoint, stringStorage);
        }
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
      else
      {
        std::uint32_t codePoint;

        auto iter = other.begin();

        while (iter != other.end())
        {
          iter = decodeUTF(iter, other.end(), codePoint);
          encodeUTF(codePoint, stringStorage);
        }
      }
    }

    template<typename T> requires UTFChar<T>
    utf_string(size_type count, T c) : stringStorage(count, c)
    {
    }


    /*! @brief      Construct from an input stream.
     *  @param[in]  inStrm: The input stream
     *  @param[in]  encoding: The stream encoding.
     *  @param[in]  count: The number of code point to read.
     *  @returns    inStrm
     *  @throws
     */
    utf_string(std::istream &inStrm, utf_e encoding, size_type count)
    {
      while (count)
      {
        decodeUTF<char8_t>(inStrm, stringStorage, encoding);
        count--;
      }
    }

    ~utf_string() = default;

    utf_string &operator=(utf_string const &) = default;
    utf_string &operator=(utf_string &&) = default;

    template<typename U>
    explicit operator U()
    {
      U resultStr;
      std::uint32_t codePoint;
      auto Iter = stringStorage.begin();

      while (Iter != stringStorage.end())
      {
        Iter  = decodeUTF(Iter, stringStorage.begin(), codePoint);
        encodeUTF(codePoint, resultStr);
      }

      return resultStr;
    }

    void setStoreEOF() noexcept { eofFlag = true; }
    void clrStoreEOF() noexcept { eofFlag = false; }

    template<typename T> requires UTFChar<T>
    void push_back(T c)
    {
      stringStorage.push_back(static_cast<value_type>(c));
    }

    void to_upper() noexcept
    {
      for (auto &c : stringStorage)
      {
        c = GCL::toupper(c);
      }
    }

    iterator begin() { return stringStorage.begin(); }
    const_iterator begin() const { return stringStorage.cbegin(); }
    const_iterator cbegin() const { return stringStorage.cbegin(); }
    iterator end() { return stringStorage.end(); }
    const_iterator end() const { return stringStorage.cend(); }
    const_iterator cend() const { return stringStorage.cend(); }

    void clear() noexcept { stringStorage.clear(); }
    size_type size() const noexcept { return stringStorage.size(); }

    value_type const &operator[](size_type indx) const { return stringStorage[indx]; }
    value_type &operator[](size_type indx) { return stringStorage[indx]; }

    utf_string &append(utf_string const &str)
    {
      stringStorage.append(str.stringStorage);
      return *this;
    }

  private:

    static bool constexpr isUTF8 = std::is_same<CharT, char8_t>::value;
    static bool constexpr isUTF16 = std::is_same<CharT, char16_t>::value;
    static bool constexpr isUTF32 = std::is_same<CharT, char32_t>::value;
    string_type stringStorage;
    bool eofFlag = false;

    template<typename Iter, typename T>
    void UTF16toUTF8(Iter inBegin, Iter inEnd, T &out)
    {
    }

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

    template<typename U> requires UTFChar<typename U::value_type>
    friend utf_string operator+(utf_string lhs, U const &rhs)
    {
      lhs.stringStorage += static_cast<decltype(lhs)>(rhs).stringStorage;
      return lhs;
    }

    template<typename T> requires UTFChar<T>
    friend utf_string operator+(utf_string lhs, T rhs)
    {
      lhs.push_back(rhs);
      return lhs;
    }

    /*! @brief      String insertion operator. Always converts to UTF8 before insertion.
     *  @param[in]  os: The output stream.
     *  @param[in]  obj: The object instance to output.
     *  @returns    reference to os.
     *  @throws
     */
    friend std::ostream &operator<<(std::ostream &os, utf_string const &obj)
    {
      //os << static_cast<std::u8string>(obj);
      return os;
    }
  };
}


#endif /* GCL_UTFSTRING_HPP */
