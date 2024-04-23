//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                utf.h
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
// OVERVIEW:            UTF constants and strings.
//
// HISTORY:             2024-04-23 GGB - File Created
//
//*********************************************************************************************************************************

#ifndef GCL_INCLUDE_UTF_H_
#define GCL_INCLUDE_UTF_H_

// Standard C++ library
#include <cstdint>
#include <istream>

namespace GCL
{
  enum utf_e
  {
    UTF_NONE,
    UTF_8,
    UTF_16BE,
    UTF_16LE,
  };

  utf_e getBOM(std::istream &);

}

#endif /* GCL_INCLUDE_UTF_H_ */
