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

#include "include/parsers/token.h"

// Standard C++ header files

namespace GCL::parsers
{
  CToken::CToken(tokenStringMap_t const &tsm, tokenID_t type, std::string const &val, std::size_t row, std::size_t col)
   : tokenStringMap(tsm), tokenType(type), tokenValue(val), tokenRow(row), tokenCol(col)
  {
  }

  std::string CToken::to_string() const
  {
    return std::string ("(" + std::to_string(tokenRow) + ", " + std::to_string(tokenCol) + ")"
                        + " " + (tokenStringMap.contains_LHS(tokenType) ? tokenStringMap.RHS(tokenType) : std::to_string(tokenType))
                        + " " + tokenValue);
  }

  std::ostream &operator<<(std::ostream &os, CToken const &t)
  {
    os << t.to_string() << std::endl;
    return os;
  }

} // namespace
