//*********************************************************************************************************************************
//
// PROJECT:							General Class Library
// SUBSYSTEM:						SQL ORM library
// FILE:								table.h
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

#ifndef TABLE_H
#define TABLE_H

#include "include/ORM/column.h"
#include "include/ORM/index.h"

  // Standard C++ library

#include <map>
#include <string>
#include <vector>

namespace GCL::ORM
{
  class table_t
  {
  public:
  private:
    std::string name;
    std::map<std::uint32_t, column_t> columns;
    std::vector<index_t> indexes;
  };
}


#endif // TABLE_H
