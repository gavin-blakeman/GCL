//*********************************************************************************************************************************
//
// PROJECT:							General Class Library
// FILE:								common
// SUBSYSTEM:						Common definitions
// LANGUAGE:						C++
// TARGET OS:						None.
// LIBRARY DEPENDANCE:	None.
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2015, 2018-2020 Gavin Blakeman.
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
// OVERVIEW:						Common definitions that can be used by multiple files
//
//
// CLASSES INCLUDED:		None
//
// CLASS HIERARCHY:     None.
//
// FUNCTIONS INCLUDED:  std::string getVersionString()
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2015-06-29 GGB - File created.
//
//*********************************************************************************************************************************

#include "../include/common.h"

  // Standard C++ library header files.

#include <iomanip>
#include <sstream>
#include <string>
#include <utility>

  // Miscellaneous library header files.

#include <boost/algorithm/string.hpp>

  // GCL Include files

#include "include/error.h"

namespace GCL
{

  unsigned int const GCL_MAJORVERSION = 2019;           ///< The Major version number (yyyy)
  unsigned int const GCL_MINORVERSION = 9;              ///< The Minor version number (mm)
  unsigned int const GCL_BUILDNUMBER  = 0x01AF;         ///< The build number - incremented on each build.

  /// @brief Returns the library version and build number
  /// @returns A string with the application name, version, build.
  /// @throws Nothing.
  /// @version 2015-06-29/GGB - Function created.

  std::string getVersionString()
  {
    std::stringstream s;

    s << GCL_MAJORVERSION << "." << std::setfill('0') << std::setw(2) << GCL_MINORVERSION << std::setw(4) << std::hex << GCL_BUILDNUMBER << std::endl;

    std::string returnValue = s.str();
    boost::algorithm::to_upper(returnValue);

    return returnValue;
  }
}
