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
// OVERVIEW:						Functions for converting to/from Base32 values. See RFS4648
//
// HISTORY:             2026-05-03/GGB - File created.
//
//**********************************************************************************************************************************

#ifndef INCLUDE_BASE32_HPP
#define INCLUDE_BASE32_HPP

// Standard C++ library header files
#include <cstdint>
#include <string_view>

namespace GCL
{

	std::uint8_t B32_to_value(char B32Char);
	char value_to_B32(std::uint8_t val);
	std::uint64_t B32_to_value(std::string_view sv);
	std::string value_to_B32(std::uint64_t v);


}	// namespace

#endif /* INCLUDE_BASE32_HPP */
