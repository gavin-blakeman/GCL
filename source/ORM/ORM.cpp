//*********************************************************************************************************************************
//
// PROJECT:							General Class Library
// SUBSYSTEM:						SQL ORM library
// FILE:								orm.h
// LANGUAGE:						C++
// TARGET OS:						None - Standard C++
// NAMESPACE:						GCL::ORM
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2023 Gavin Blakeman.
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
// OVERVIEW:
//
// HISTORY:             2023-09-19 GGB - File Created.
//
//*********************************************************************************************************************************

#include "include/ORM/ORM.h"

  // Standard C++ library

#include <map>
#include <string>
#include <variant>
#include <vector>

namespace GCL::ORM
{
  enum tokenType_t
  {
    T_NONE,
    T_INSERT,
    T_EXTRACT,
    T_EQ,
    T_NEQ,
    T_GT,
    T_GTE,
    T_LT,
    T_LTE,
    T_AND,
    T_OR,
    T_XOR,
    T_LIKE,
    T_IN,
    T_JOIN,
    T_TABLEREAD,
    T_TABLECREATE,
    T_TABLEDELETE,
    T_TABLEUPDATE,
    T_COLUMN,
    T_VALUE,
  };

  using tokenTypeMap_t = std::map<std::string, tokenType_t>;
  static tokenTypeMap_t tokens =
  {
    { "<<", T_INSERT },
    { ">>", T_EXTRACT },
    { "==", T_EQ },
    { "<>", T_NEQ },
    { "><", T_JOIN },
    { "|+", T_TABLECREATE },
    { "|-", T_TABLEDELETE },
    { "|?", T_TABLEREAD },
    { "|:", T_TABLEUPDATE },
  };

  using tokenValue_t = std::variant<float, double>;
  struct token_t
  {
    tokenType_t tokenType;
    tokenValue_t tokenValue;
  };
  using tokenStream_t = std::vector<token_t>;

  // |?TBL(ID>>, ST>>, FK><|?TBL_FK(ID><, ST>>, PV>>))
}
