//*********************************************************************************************************************************
//
// PROJECT:							General Class Library (GCL)
// FILE:								error.cpp
// SUBSYSTEM:						Exception/Error subsystem
// LANGUAGE:						C++
// TARGET OS:						None.
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2013-2023 Gavin Blakeman.
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
// OVERVIEW:            This file implements exceptions that are used for error reporting by the GCL library.
//
// CLASSES INCLUDED:    CGCLError
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-09-30 GGB - AIRDAS 2013.09 release.
//                      2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-01-26 GGB - Development of classes for Application AIRDAS
//
//*********************************************************************************************************************************

#include "include/error.h"

  // Standard C++ library header files.

#include <cstdlib>

  // Miscellaneous library header files.

#include <boost/lexical_cast.hpp>

namespace GCL
{
  /// @brief        Converts the error message to a string.
  /// @param[in]    fileName: The name of the source file having the error.
  /// @param[in]    timeStamp: The build timestamp of the source file.
  /// @param[in]    lineNo: The line number that raises the error.
  /// @returns      String containing the error message details.
  /// @throws       std::bad_alloc
  /// @version      2020-09-08/GGB - Function created

  std::string runtime_error::errorMessage() const
  {
    std::ostringstream o;

    o << "Runtime Error - " << what() << " Error Code: " << errorCode_;

    if (!library_.empty())
    {
      o << " Library: " << library_;
    };

    o << std::endl;

    return o.str();
  }

  /// @brief        Converts the error message to a string.
  /// @param[in]    fileName: The name of the source file having the error.
  /// @param[in]    lineNo: The line number that raises the error.
  /// @returns      String containing the error message details.
  /// @throws       std::bad_alloc
  /// @version      2022-10-12/GGB - Removed parameter timeStamp. (Part of moving to std::source_location.
  /// @version      2020-09-06/GGB - Removed parameter library.
  /// @version      2020-06-14/GGB - Error text updated.
  /// @version      2015-07-28/GGB - Updated message to reflect library.
  /// @version      2014-12-25/GGB - Updated to working code.
  /// @version      2013-01-26/GGB - Function created

  std::string CCodeError::errorMessage(std::string const &fileName, std::size_t lineNo) const
  {
    std::ostringstream o;

    o << "Code Error. (Generally unreachable code)" << std::endl;
    o << "File: " << fileName << " at line: " << lineNo << std::endl;

    return o.str();
  }

  /// @brief        Creates the message string for the runtime assertion.
  /// @param[in]    expression: The expression that failed.
  /// @param[in]    fileName: The name of the source file that generated the error.
  /// @param[in]    timeStamp: The build timestamp.
  /// @param[in]    lineNumber: The line number that created the exception.
  /// @param[in]    message: The message to associate with the exception.
  /// @returns      A string to use for creating a standardised description for the exception.
  /// @throws       std::bad_alloc
  /// @version      2020-09-06/GGB - Removed parameter library.
  /// @version      2020-04-14/GGB - Function created.

  std::string CRuntimeAssert::errorMessage(std::string const &expression,
                                           std::string const &fileName, std::string const &timeStamp,
                                           std::size_t lineNumber, std::string const &message) const
  {
    std::string messageString;

    messageString = "Failed Assertion: '" + expression + "'  failed in file '";

    messageString += fileName + "' at line '" + std::to_string(lineNumber) + "'.";

    if (!message.empty())
    {
      messageString += " " + message;
    }

    return messageString;
  }

}  // namespace GCL
