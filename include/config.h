//*********************************************************************************************************************************
//
// PROJECT:							General Class Library
// FILE:								Config
// SUBSYSTEM:						Configuration file
// LANGUAGE:						C++
// TARGET OS:						None.
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2016-2022 Gavin Blakeman.
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
// OVERVIEW:						Configuration values and typedefs for the library.
//
//
// CLASSES INCLUDED:		None
//
// CLASS HIERARCHY:     None.
//
//
// HISTORY:             2016-01-15 GGB - File created.
//
//*********************************************************************************************************************************

#ifndef GCL_CONFIG_H
#define GCL_CONFIG_H

#include <cstdint>

#ifdef EMBEDDED
  #ifdef __XC32
    #define GCL_NOBOOST
    #define GCL_NOMT
    #define GCL_REALTIME
    #define GCL_NOSTRINGS
    #define GCL_CONTROL
  #endif  // __XC32
#else   // EMBEDDED
  #undef GCL_NOBOOST
  #undef GCL_NOMT
  #undef GCL_REALTIME
  #undef GCL_NOSTRINGS
  #undef GCL_CONTROL
#endif  // EMBEDDED

typedef double FP_t;
typedef std::uint32_t indexID_t;

#endif // GCL_CONFIG_H
