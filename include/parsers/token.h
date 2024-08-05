//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// SUBSYSTEM:           Parsers
// FILE:                token.h
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
// OVERVIEW:            Class provides a generic token used for lexing files.
//
// CLASSES INCLUDED:
//
// HISTORY:             2024-06-18 GGB - File Created
//
//**********************************************************************************************************************************

#ifndef PARSERS_TOKEN_H
#define PARSERS_TOKEN_H

// Standard C++ library header files
#include <cstdint>
#include <iostream>
#include <string>

// Miscellaneous library header files.
#include <SCL>

namespace GCL::parsers
{
  class CToken
  {
    public:
      using tokenID_t = std::uint_fast8_t;
      using tokenStringMap_t = SCL::bimap<tokenID_t, std::string>;

      CToken(tokenStringMap_t const &tsm, tokenID_t type, std::size_t row, std::size_t col);
      CToken(tokenStringMap_t const &tsm, tokenID_t type, std::string const &val, std::size_t row, std::size_t col);
      std::string to_string() const;

      tokenID_t type() const noexcept { return tokenType; }
      std::string value() const noexcept { return tokenValue; }

    private:
      tokenStringMap_t const &tokenStringMap;
      tokenID_t tokenType;
      std::string tokenValue;
      std::size_t tokenRow;
      std::size_t tokenCol;

  friend std::ostream &operator<<(std::ostream &os, CToken const &);
  };

} // namespace

#endif // PARSERS_TOKEN_H
