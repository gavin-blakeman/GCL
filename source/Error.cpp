//*********************************************************************************************************************************
//
// PROJECT:							General Class Library (GCL)
// FILE:								CGCLError
// SUBSYSTEM:						General Class Library Exception subsystem
// LANGUAGE:						C++
// TARGET OS:						All (Standard C++)
// LIBRARY DEPENDANCE:	None.
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2013-2017 Gavin Blakeman.
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

#include "../include/Error.h"

#include <cstdlib>

#include "boost/lexical_cast.hpp"

namespace GCL
{
  CError::TErrorStore CError::errorMessages;

  /// @brief Fuction to add an error message to the error message list.
  /// @throws None.
  /// @version 2015-07-28/GGB - Function created.

  bool CError::addErrorMessage(std::string library, TErrorCode code, std::string message)
  {
    errorMessages.emplace(library + std::to_string(code), SErrorEntry(library, code, message));

    return true;
  }

  /// @brief Returns the string of the error message.
  /// @returns The error message.
  /// @version 2013-01-26/GGB - Function created.

  std::string CError::errorMessage() const
  {
    TErrorStore::const_iterator errorData;

    if ((errorData = errorMessages.find(library_ + std::to_string(errorCode_))) == errorMessages.end() )
    {
        // Error message not found - Error code has not been defined.
        // This is a non-recoverable error.

      exit(0xFFFE);		// Non-recoverable error within an error
    }
    else
    {
      return (errorData->second.errorMessage);
    };
  }

  /// @brief Function to write the error message to a logFile.
  /// @details This is not automatically done in the library when an exception is thrown as the library may be able to recover from
  /// the exception without having to terminate.
  /// @version  2014-12-24/GGB - Function created.

  void CError::logErrorMessage() const
  {
    TErrorStore::const_iterator errorData;

    if ((errorData = errorMessages.find(library_ + std::to_string(errorCode_))) == errorMessages.end() )
    {
        // Error message not found - Error code has not been defined.
        // This is a non-recoverable error.

      GCL::logger::defaultLogger().logMessage(GCL::logger::error,
                                           "Non recoverable error within an error in " + library_ + " library. Error Code: " +
                                           boost::lexical_cast<std::string>(errorCode_) + ". Terminating");
      exit(0xFFFE);		// Non-recoverable error within an error
    }
    else
    {
      GCL::logger::defaultLogger().logMessage(GCL::logger::warning,
                                              "Library " + library_ + " error Code: " +
                                              boost::lexical_cast<std::string>(errorData->second.errorCode) +
                                           " - " + errorData->second.errorMessage);
    };
  }

  //********************************************************************************************************************************
  //
  // CCodeError
  //
  //********************************************************************************************************************************

  /// @brief Converts the error message to a string.
  /// @returns String containing the error message details.
  /// @throws None.
  /// @version 2015-07-28/GGB - Updated message to reflect library.
  /// @version 2014-12-25/GGB - Updated to working code.
  /// @version 2013-01-26/GGB - Function created

  std::string CCodeError::errorMessage() const
  {
    std::ostringstream o;

    o << "A code error has occurred in the " << library_ << " Library." << std::endl;
    o << "In file: " << fileName << " dated: " << timeStamp << " at line: " << lineNo << std::endl;

    return o.str();
  }

  /// @brief Function to write the error message to a logFile.
  /// @details This is not automatically done in the library when an exception is thrown as the library may be able to recover from
  /// the exception without having to terminate.
  /// @throws None.
  /// @version 2014-12-24/GGB - Function created.

  void CCodeError::logErrorMessage() const
  {
    GCL::logger::defaultLogger().logMessage(GCL::logger::error,
                                            "A Code Error has occurred in the " + library_ + " library. In file: " + fileName +
                                         ". At line: " + boost::lexical_cast<std::string>(lineNo));
  }

}  // namespace GCL
