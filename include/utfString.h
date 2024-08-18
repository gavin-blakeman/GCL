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
//*********************************************************************************************************************************

#ifndef GCL_UTFSTRING_H
#define GCL_UTFSTRING_H

// Standard C++ library header files
#include <string>


namespace GCL
{
  template<class T>
  class utfString
  {
    static_assert(std::is_same_v<T, std::u8string, "T must be a defined string type.");
  };
}


#endif /* GCL_UTFSTRING_H */
