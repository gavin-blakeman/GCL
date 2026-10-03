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
//                      Copyright 2024-2026 Gavin Blakeman.
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
  concept HasPushBack = requires(C c, T t) { c.push_back(t); };

  template<typename C, typename T>
  concept HasPush = requires(C c, T t) { c.push(t); };

  template<typename C, typename T>
  concept HasEmplaceBack = requires(C c, T t) { c.emplace_back(t); };

  template<class ContainerType>
  concept Container = requires(ContainerType a, const ContainerType b)
  {
    requires std::regular<ContainerType>;
    requires std::swappable<ContainerType>;
    requires std::destructible<typename ContainerType::value_type>;
    requires std::same_as<typename ContainerType::reference, typename ContainerType::value_type &>;
    requires std::same_as<typename ContainerType::const_reference, const typename ContainerType::value_type &>;
    requires std::forward_iterator<typename ContainerType::iterator>;
    requires std::forward_iterator<typename ContainerType::const_iterator>;
    requires std::signed_integral<typename ContainerType::difference_type>;
    requires std::same_as<typename ContainerType::difference_type, typename std::iterator_traits<typename ContainerType::iterator>::difference_type>;
    requires std::same_as<typename ContainerType::difference_type, typename std::iterator_traits<typename ContainerType::const_iterator>::difference_type>;
    { a.begin() } -> std::same_as<typename ContainerType::iterator>;
    { a.end() } -> std::same_as<typename ContainerType::iterator>;
    { b.begin() } -> std::same_as<typename ContainerType::const_iterator>;
    { b.end() } -> std::same_as<typename ContainerType::const_iterator>;
    { a.cbegin() } -> std::same_as<typename ContainerType::const_iterator>;
    { a.cend() } -> std::same_as<typename ContainerType::const_iterator>;
    { a.size() } -> std::same_as<typename ContainerType::size_type>;
    { a.max_size() } -> std::same_as<typename ContainerType::size_type>;
    { a.empty() } -> std::same_as<bool>;
  };

  template <typename T>
  concept IsAtomicStorageIntegral = requires(T a, typename T::value_type v, typename T::value_type* p, std::memory_order order) 
  {
    // 1. Must define an integral value_type (e.g., std::uint32_t)
    typename T::value_type;
    requires std::is_integral_v<typename T::value_type>;

    // 2. Must support standard construction/copy properties
    requires !std::copy_constructible<T>; // Atomics are non-copyable

    // 3. Mandatory method signatures matching std::atomic interface
    { a.load(order) } -> std::same_as<typename T::value_type>;
    { a.store(v, order) } -> std::same_as<void>;
    { a.exchange(v, order) } -> std::same_as<typename T::value_type>;
    { a.fetch_add(v, order) } -> std::same_as<typename T::value_type>;
    { a.fetch_or(v, order) } -> std::same_as<typename T::value_type>;
    { a.compare_exchange_strong(*p, v, order, order) } -> std::same_as<bool>;
    { a.compare_exchange_weak(*p, v, order, order) } -> std::same_as<bool>;
  };

}




#endif /* GCL_INCLUDE_CONCEPTS_HPP_ */
