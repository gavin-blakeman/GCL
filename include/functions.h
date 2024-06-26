//*********************************************************************************************************************************
//
// PROJECT:							General Class Library (GCL)
// FILE:								General functions.
// SUBSYSTEM:						General Class Library Exception subsystem
// LANGUAGE:						C++
// TARGET OS:						None.
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2015, 2017-2024 Gavin Blakeman.
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
// OVERVIEW:            This file implements general functions.
//
// CLASSES INCLUDED:    None.
//
// FUNCTIONS INCLUDED:  prettyPrintBytes(...)
//
// HISTORY:             2017-09-24/GGB - Moved sprintf functions from AstroFunctions to this file.
//                      2015-09-22 GGB - AIRDAS 2015.09 release
//                      2015-01-02/GGB - File created.
//
//*********************************************************************************************************************************

#ifndef GCL_FUNCTIONS_H
#define GCL_FUNCTIONS_H

  // Standard libraries

#include <cstdint>
#include <ctime>
#include <iostream>
#include <tuple>
#include <set>
#include <string>

  // GCL Libraries

#include "config.h"

namespace GCL
{
  typedef std::tuple<int, int, int, int> TIPV4;

  std::string prettyPrintBytes(size_t bytes);

  std::string IPtoString(TIPV4 const &);
  TIPV4 stringToIP(std::string const &);

  int sprintfDMS(char *, const double, const bool = true, const int=0);  // ±ddºmm'ss.s"
  std::string sprintfDMS(double const &, bool = true, int = 0);
  int sprintfHMS(char *, FP_t, int=2);
  std::string sprintfHMS(std::uint32_t const &);
  std::string sprintfHMS(FP_t const &, int=2);

  /*! @brief      Performs an include/exclude test.
   *  @details    The two sets provide an inlude and an exclude test.
   *                If the val is in the include test, it is included.
   *                If the include set is empty, the val is included unless it is in the exclude set.
   *                RV = (T IN INCLUDE) || (INCLUDE.empty() && !(T IN EXCLUDE))
   *  @param[in]  include: The set of items to include.
   *  @param[in]  exclude: The set of items to exclude.
   *  @returns    true if the item should be included.
   *  @throws
   */
  template<typename T>
  bool includeExclude(std::set<T> const &include, std::set<T> const &exclude, T const &val)
  {
    /* |---------------|
     * | I | E | IE| O |
     * |---+---+---+---|
     * | F | F | F | F |
     * | F | F | T | T | <-
     * | F | T | X | F |
     * | T | X | F | T | <-
     * | T | X | T | NP|
     * |---------------|
     */

    return ( include.contains(val) || (include.empty() && !exclude.contains(val)) );
  }

}   // namespace GCL

  std::ostream &operator<<(std::ostream &, std::tm const &);

#endif // GCL_FUNCTIONS_H
