//**********************************************************************************************************************************
//
// PROJECT:							General Class Library
// FILE:								hash.hpp
// SUBSYSTEM:						Hashing functions
// LANGUAGE:						C++
// TARGET OS:						None.
// NAMESPACE:						SCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2026 Gavin Blakeman.
//                      This file is part of the Storage Class Library (SCL)
//
//                      SCL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
//                      License as published by the Free Software Foundation, either version 2 of the License, or (at your option)
//                      any later version.
//
//                      SCL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
//                      warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
//                      more details.
//
//                      You should have received a copy of the GNU General Public License along with SCL.  If not, see
//                      <http://www.gnu.org/licenses/>.
//
// OVERVIEW:            Hashing functions.
//
// HISTORY:             2020-02-16 GGB - File Created
//
//**********************************************************************************************************************************

#ifndef GCL_HASH_HPP
#define GCL_HASH_HPP

#include <cstdint>

namespace GCL
{
	struct murmurHash3
	{
		static constexpr std::uint32_t operator()(std::uint32_t x) noexcept
		{
			x ^= x >> 16;
			x *= 0x85ebca6b;
			x ^= x >> 13;
			x *= 0xc2b2ae35;
			x ^= x >> 16;

			return x;
		}
	};

	struct hash32
	{
		static constexpr std::uint32_t operator()(std::uint32_t x) noexcept
		{
			x = (x + 0x7ed55d16) + (x << 12);
			x = (x ^ 0xc761c23c) ^ (x >> 19);
			x = (x + 0x165667b1) + (x << 5);
			x = (x + 0xd3a2646c) ^ (x << 9);
			x = (x + 0xfd7046c5) + (x << 3);
			x = (x ^ 0xb55a4f09) ^ (x >> 16);

			return x;
		}
	};


}

#endif /* GCL_HASH_HPP */
