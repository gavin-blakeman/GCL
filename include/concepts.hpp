//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                concepts.hpp
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
// OVERVIEW:            Shared concepts.
//
// HISTORY:             2024-08-20 GGB - File Created
//
//*********************************************************************************************************************************/

#ifndef GCL_INCLUDE_CONCEPTS_HPP_
#define GCL_INCLUDE_CONCEPTS_HPP_

// Standard C++ library
#include <cstdint>
#include <type_traits>

namespace GCL
{
  template<typename T, typename ... U>
  concept IsAnyOf = (std::same_as<T, U> || ...);

  template<typename T>
  concept UTFChar = IsAnyOf<std::remove_cvref_t<std::remove_pointer_t<std::decay_t<T>>>, char8_t, char16_t, char32_t>;

  template<typename T>
  concept isUTF8Char = IsAnyOf<std::remove_cvref_t<std::remove_pointer_t<std::decay_t<T>>>, char8_t, std::uint8_t>;

  template<typename T>
  concept isUTF16Char = IsAnyOf<std::remove_cvref_t<std::remove_pointer_t<std::decay_t<T>>>, char16_t, std::uint16_t>;

  template<typename T>
  concept isUTF32Char = IsAnyOf<std::remove_cvref_t<std::remove_pointer_t<std::decay_t<T>>>, char32_t, std::uint32_t>;

  template<typename C, typename T>
  concept HasPushBack = requires(C c, T t) { {c.push_back(t) } -> std::same_as<void>; };

  template<typename C, typename T>
  concept HasPush = requires(C c, T t) { {c.push(t) } -> std::same_as<void>; };
}




#endif /* GCL_INCLUDE_CONCEPTS_HPP_ */
