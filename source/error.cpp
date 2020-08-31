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
//                      Copyright 2013-2020 Gavin Blakeman.
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
  class [[deprecated]] CLoader
  {
  private:
    void loadErrorMessages();
  public:
    CLoader();
  };

  static CLoader executeLoader;

  CLoader::CLoader()
  {
    loadErrorMessages();
  }

  /// @brief Function to add an error message to the error message list.
  /// @throws None.
  /// @version 2018-09-21/GGB - Updated to use new errorMessages() function.
  /// @version 2015-07-28/GGB - Function created.

  bool CError::addErrorMessage(std::string library, TErrorCode code, std::string message)
  {
    errorMessages().emplace(library + std::to_string(code), SErrorEntry(library, code, message));

    return true;
  }

  /// @brief Returns the string of the error message.
  /// @returns The error message.
  /// @version 2018-09-21/GGB - Updated to use new errorMessage() function.
  /// @version 2013-01-26/GGB - Function created.

  std::string CError::errorMessage() const
  {
    TErrorStore::const_iterator errorData;

    if ((errorData = errorMessages().find(library_ + std::to_string(errorCode_))) == errorMessages().end() )
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

  /// @brief Construct and store the errorMessages map.
  /// @returns Reference to the errorMessagesMap.
  /// @note This uses the construct on first use idiom.
  /// @throws None.
  /// @version 2018-09-21/GGB - Function created.

  CError::TErrorStore &CError::errorMessages()
  {
    static TErrorStore errorMessages_;

    return errorMessages_;
  }

  /// @brief Function to write the error message to a logFile.
  /// @details This is not automatically done in the library when an exception is thrown as the library may be able to recover from
  ///          the exception without having to terminate.
  /// @version 2018-09-21/GGB - Updated to use new errorMessage() function.
  /// @version  2014-12-24/GGB - Function created.

  void CError::logErrorMessage() const
  {
    TErrorStore::const_iterator errorData;

    if ((errorData = errorMessages().find(library_ + std::to_string(errorCode_))) == errorMessages().end() )
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
  /// @param[in] library: The name of the library. (May be an empty string)
  /// @param[in] fileName: The name of the source file having the error.
  /// @param[in] timeStamp: The build timestamp of the source file.
  /// @param[in] lineNo: The line number that raises the error.
  /// @returns String containing the error message details.
  /// @throws None.
  /// @version 2020-06-14/GGB - Error text updated.
  /// @version 2015-07-28/GGB - Updated message to reflect library.
  /// @version 2014-12-25/GGB - Updated to working code.
  /// @version 2013-01-26/GGB - Function created

  std::string CCodeError::errorMessage(std::string const &library, std::string const &fileName,
                                       std::string const &timeStamp, std::size_t lineNo) const
  {
    std::ostringstream o;

    o << "Code Error. (Generally unreachable code)" << std::endl;
    if (!library.empty())
    {
      o << "Library: " << library << " ";
    };
    o << "File: " << fileName << " dated: " << timeStamp << " at line: " << lineNo << std::endl;

    return o.str();
  }

  /// @brief Creates the message string for the runtime assertion.
  /// @param[in] library: The name of the library. (May be an empty string)
  /// @param[in] expression: The expression that failed.
  /// @param[in] fileName: The name of the source file that generated the error.
  /// @param[in] timeStamp: The build timestamp.
  /// @param[in] lineNumber: The line number that created the exception.
  /// @param[in] message: The message to associate with the exception.
  /// @returns A string to use for creating a standardised description for the exception.
  /// @throws std::bad_alloc
  /// @version 2020-04-14/GGB - Function created.

  std::string CRuntimeAssert::errorMessage(std::string const &library, std::string const &expression,
                                           std::string const &fileName, std::string const &timeStamp,
                                           std::size_t lineNumber, std::string const &message) const
  {
    std::string messageString;

    messageString = "Failed Assertion: '" + expression + "'  failed in ";

    if (!library.empty())
    {
      messageString += " library " + library + ", ";
    };

    messageString += " file '";

    messageString += fileName + "' at line '" + std::to_string(lineNumber) + "'.";

    if (!message.empty())
    {
      messageString += " " + message;
    }

    return messageString;
  }

  //********************************************************************************************************************************
  //
  // CLoader
  //
  //********************************************************************************************************************************

  /// @brief Loads the error code for the library into the error class.
  /// @throws None.
  /// @version 2018-09-20/GGB - Function created.

  void CLoader::loadErrorMessages()
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

}  // namespace GCL
