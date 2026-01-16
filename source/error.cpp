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
//                      Copyright 2013-2026 Gavin Blakeman.
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
// HISTORY:             2024-03-27 GGB - Added the implement me class to use for unimplemented features that need to be implemented.
//                      2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-09-30 GGB - AIRDAS 2013.09 release.
//                      2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-01-26 GGB - Development of classes for Application AIRDAS
//
//*********************************************************************************************************************************

#include "include/error.h"

  // Standard C++ library header files.

#include <cstdlib>
#include <iostream>

  // Miscellaneous library header files.

#include <boost/lexical_cast.hpp>
#include <fmt/format.h>
#include <fmt/std.h>

  // GCL library header files

#include "include/logger/loggerManager.h"

namespace GCL
{

  using logger::LOGEXCEPTION;

  runtime_error::runtime_error(std::string const &m, errorType_t et, errorCode_t ec, logger::CLogger *namedLogger)
      : std::runtime_error(m), errorType_(et), errorCode_(ec)
  {
    LOGEXCEPTION(errorMessage(), namedLogger);
  }

  /// @brief      Constructor
  /// @param[in]  message: The error message.
  /// @param[in]  namedLogger: The named logger to use.
  /// @throws
  /// @version    2024-02-08/GGB - Function created.

  runtime_error::runtime_error(std::string const &message, logger::CLogger *namedLogger)
  : std::runtime_error(message)
  {
    LOGEXCEPTION(errorMessage(), namedLogger);
  }

  /// @brief        Converts the error message to a string.
  /// @param[in]    fileName: The name of the source file having the error.
  /// @param[in]    timeStamp: The build timestamp of the source file.
  /// @param[in]    lineNo: The line number that raises the error.
  /// @returns      String containing the error message details.
  /// @throws       std::bad_alloc
  /// @version      2020-09-08/GGB - Function created

  std::string runtime_error::errorMessage() const
  {
    if (errorType_ && errorCode_)
    {
      return fmt::format("Runtime Error: Type: {:Ld}, Code: {:Ld} - {:s}", errorType_, errorCode_, what());
    }
    else
    {
      return fmt::format("Runtime Error: {:s}", what());
    }

  }

  CCodeError::CCodeError(std::string fn, size_t ln) : std::runtime_error("Code Error."), fileName(fn),lineNo(ln)
  {
    LOGEXCEPTION(errorMessage(fileName, lineNo));
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

  /// @brief      Class constructor.
  /// @param[in]  fn: File name
  /// @param[in]  ln: Line number.
  /// @throws
  /// @version      2024-03-27/GGB - Function created

  CImplementMe::CImplementMe(std::string fn, size_t ln) : std::runtime_error("Implement Me."), fileName(fn),lineNo(ln)
  {
    LOGEXCEPTION(errorMessage(fileName, lineNo));
    std::cout << errorMessage(fileName, lineNo) << std::endl;
  }

  /// @brief        Converts the error message to a string.
  /// @param[in]    fileName: The name of the source file having the error.
  /// @param[in]    lineNo: The line number that raises the error.
  /// @returns      String containing the error message details.
  /// @throws       std::bad_alloc
  /// @version      2024-03-27/GGB - Function created

  std::string CImplementMe::errorMessage(std::string const &fileName, std::size_t lineNo) const
  {
    std::ostringstream o;

    o << "Implement ME. Code must still be implemented." << std::endl;
    o << "File: " << fileName << " at line: " << lineNo << std::endl;

    return o.str();
  }

  /// @brief      Constructor for runtime assert. Also raises an exception.
  /// @param[in]  expression: The expression that failed.
  /// @param[in]  message: The message associated with the expression.
  /// @param[in]  location: The location where the error occurred.
  /// @throws
  /// @version      2020-04-14/GGB - Function created.

  CRuntimeAssert::CRuntimeAssert(std::string const &expression, std::string const &message, std::source_location const location)
        : std::runtime_error("Runtime Assert")
  {
    LOGEXCEPTION(errorMessage(expression, message, location));
  }

  /// @brief        Creates the message string for the runtime assertion.
  /// @param[in]    expression: The expression that failed.
  /// @param[in]    message: The message to associate with the exception.
  /// @param[in]    location: The source location where the error occurred.
  /// @returns      A string to use for creating a standardised description for the exception.
  /// @throws       std::bad_alloc
  /// @version      2024-04-02/GGB - Changed parameters to include the source_location.
  /// @version      2023-11-30.GGB - Changed to reflect move to std::source_location
  /// @version      2020-09-06/GGB - Removed parameter library.
  /// @version      2020-04-14/GGB - Function created.

  std::string CRuntimeAssert::errorMessage(std::string const &expression,
                                           std::string const &message,
                                           std::source_location const &location) const
  {
    return fmt::format("Failed Assertation: '{}:{}', File: {}, Function: {}",
                       expression,
                       message,
                       std::string(location.file_name()),
                       std::string(location.function_name()));
  }

}  // namespace GCL
