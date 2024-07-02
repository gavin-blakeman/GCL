//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                string.hpp
// SUBSYSTEM:           String functions
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
// OVERVIEW:            Functions for operating on strings.
//
// HISTORY:             2024-03-01 GGB - File Created
//*********************************************************************************************************************************

#ifndef GCL_INCLUDE_STRING_HPP
#define GCL_INCLUDE_STRING_HPP

// Standard C++ library header files.

#include <string>
#include <string_view>

namespace GCL
{
  template<typename O>
  void tokeniseString(O &op, std::string const &str, std::string const &seperators = ".,;:")
  {
    std::string_view sv(str);
    using size_type = std::string_view::size_type;

    while (!sv.empty())
    {
      size_type end = sv.find_first_of(",;:.", 0);
      if (end != std::string_view::npos)
      {
        std::string temp(sv, 0, end);
        op.push_back(std::move(temp));
        sv.remove_prefix(end + 1);
      }
      else
      {
        op.emplace_back(sv);
        sv.remove_prefix(sv.length());
      }
    }
  }
  
  template<typename C>
  std::string expandTokens(C const &tokens, std::string const &seperator = ";")
  {
    std::string rv;
    bool first = true;
    
    for (auto const &t: tokens)
    {
      if (!first)
      {
        rv += seperator + " ";
      }
      else
      {
        first = false;
      }
      rv += t;
    }
    
    return rv;
  }
}


#endif /* GCL_INCLUDE_STRING_HPP */
