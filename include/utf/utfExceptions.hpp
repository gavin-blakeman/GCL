//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                utfExceptions.hpp
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
// OVERVIEW:            UTF Exceptions
//
// HISTORY:             2024-08-20 GGB - File Created
//
//*********************************************************************************************************************************

#ifndef GCL_INCLUDE_UTF_UTFEXCEPTIONS_HPP_
#define GCL_INCLUDE_UTF_UTFEXCEPTIONS_HPP_

// Standard C++ library.
#include <stdexcept>

namespace GCL
{
  class unexpected_eof : public std::runtime_error
  {
  public:
    unexpected_eof() : std::runtime_error("EOF") {}
  };

  class bad_codepoint : public std::runtime_error
  {
  public:
    bad_codepoint() : std::runtime_error("Bad codepoint in stream") {}
  };

  class bad_utf8 : public std::runtime_error
  {
  public:
    bad_utf8() : std::runtime_error("Bad codepoint in stream") {}
  };
}

#endif /* GCL_INCLUDE_UTF_UTFEXCEPTIONS_HPP_ */
