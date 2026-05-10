//**********************************************************************************************************************************
//
// PROJECT:             sURL
// FILE:                base32.hpp
// SUBSYSTEM:           Base32 functions
// LANGUAGE:						C++
// TARGET OS:           LINUX
// LIBRARY DEPENDANCE:	None.
// NAMESPACE:           N/A
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2026 Gavin Blakeman.
//                      This file is part of the sURL Project
//
//                      sURL is free software: you can redistribute it and/or modify it under the terms of the GNU
//                      General Public License as published by the Free Software Foundation, either version 2 of the License, or (at
//                      your option) any later version.
//
//                      sURL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
//                      even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
//                      Public License for more details.
//
//                      You should have received a copy of the GNU General Public License along with sURL.  If not,
//                      see <http://www.gnu.org/licenses/>.
//
// OVERVIEW:						The class that provides the incoming buffer. This is a specialisation of SCL::circularBuffer
//
// HISTORY:             2026-05-10/GGB - File created.
//
//**********************************************************************************************************************************

#include "include/base32.h"

// Miscellaneous library header files.
#include <SCL>

namespace GCL
{
	static const SCL::bimap<char, std::uint8_t> charMapping =
	{
			{'A', 0},  {'B', 1},  {'C', 2},  {'D', 3},  {'E', 4},  {'F', 5},  {'G', 6},  {'H', 7},
			{'I', 8},  {'J', 9},  {'K', 10}, {'L', 11}, {'M', 12}, {'N', 13}, {'O', 14}, {'P', 15},
			{'Q', 16}, {'R', 17}, {'S', 18}, {'T', 19}, {'U', 20}, {'V', 21}, {'W', 22}, {'X', 23},
			{'Y', 24}, {'Z', 25}, {'2', 26}, {'3', 27}, {'4', 28}, {'5', 29}, {'6', 30}, {'7', 31},
	};

	std::uint8_t B32_to_value(char B32Char)
	{
		return charMapping.RHS(B32Char);
	}

	char value_to_B32(std::uint8_t val)
	{
		return charMapping.LHS(val & 0x1F);
	}

	std::uint64_t B32_to_value(std::string_view sv)
	{
		std::uint64_t rv = 0;

		while (!sv.empty())
		{
			rv <<= 5;
			rv |= B32_to_value(sv.front());
			sv.remove_prefix(1);
		}

		return rv;
	}

	std::string value_to_B32(std::uint64_t value)
	{
		static constexpr char B32_ALPHABET[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
		std::string rv;

		while (value > 0)
		{
			rv.push_back(B32_ALPHABET[value & 0x1F]);
			value >>= 5;
		}

		std::reverse(rv.begin(), rv.end());
		return rv;
	}

}	// namespace
