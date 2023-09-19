//*********************************************************************************************************************************
//
// PROJECT:							General Class Library
// SUBSYSTEM:						SQL ORM library
// FILE:								index.h
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

#ifndef INDEX_H
#define INDEX_H

  // Standard C++ library

#include <string>
#include <vector>


namespace GCL::ORM
{
  enum indexType_t
  {
    IT_NONE = 0,
    IT_PRIMARY,
    IT_UNIQUE,
    IT_INDEX,
  };

  class index_t
  {
  public:
  private:
    std::string name;
    indexType_t type;
    std::vector<std::string> columnNames;

  };
}

#endif // INDEX_H
