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
//                      Copyright 2015, 2018 Gavin Blakeman.
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

  // Standard libraries

#include <iomanip>
#include <sstream>
#include <string>
#include <utility>

  // Boost Library

#include <boost/algorithm/string.hpp>

  // GCL Include files

#include "../include/Error.h"

namespace GCL
{

  unsigned int const GCL_MAJORVERSION = 2018;           ///< The Major version number (yyyy)
  unsigned int const GCL_MINORVERSION = 9;              ///< The Minor version number (mm)
  unsigned int const GCL_BUILDNUMBER  = 0x0059;         ///< The build number - incremented on each build.

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

  /// @brief Loads the error code for the library into the error class.
  /// @throws None.

  void loadErrorMessages()
  {
    std::vector<std::pair<TErrorCode, std::string>> errors =
    {
      {0x0001, std::string("MAPPED SQL WRITER: Invalid Map file name.")},
      {0x0002, std::string("MAPPED SQL WRITER: Syntax Error.")},
      {0x0003, std::string("MAPPED SQL WRITER: Invalid Command.")},
      {0x0004, std::string("MAPPED SQL WRITER: Invalid Table Name.")},
      {0x0005, std::string("MAPPED SQL WRITER: Invalid Column Name.")},
      {0x0006, std::string("MAPPED SQL WRITER: Invalid ORDER BY direction (ACS, DESC)")},
      {0x0007, std::string("MAPPED SQL WRITER: No Select fields in select clause.")},
      {0x0008, std::string("MAPPED SQL WRITER: No from fields in select clause.")},
      {0x0009, "MAPPED SQL WRITER: No fields defined for set statement." },
      {0x1000, std::string("LOGGER: Unable to open log file.")},
      {0x1001, std::string("LOGGER: Unable to start thread.")},
      {0x1002, std::string("LOGGER: Text Edit not assignd.")}
    };

    std::for_each(errors.begin(), errors.end(),
                  [] (std::pair<TErrorCode, std::string> p) { CError::addErrorMessage("GCL", p.first, p.second); });

  }
}
