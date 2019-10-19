//*********************************************************************************************************************************
//
// PROJECT:							General Class Library (GCL)
// FILE:								Error
// SUBSYSTEM:						Exception/Error subsystem
// LANGUAGE:						C++
// TARGET OS:						All (Standard C++)
// LIBRARY DEPENDANCE:	None.
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2013-2019 Gavin Blakeman.
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

#ifndef GCL_ERROR_H
#define GCL_ERROR_H

  // Standard C++ library header files

#include <cstdint>
#include <stdexcept>
#include <string>
#include <unordered_map>

  // GCL library header files

#include "../include/logger/LoggerCore.h"

#ifndef GCL_CONTROL

namespace GCL
{

  typedef std::uint16_t TErrorCode;

  /// @brief The CError class is used to throw exceptions to indicate errors within a library.
  /// @details Errors can be of any type. The exception allows a library and error number within a library to be specified.
  /// Each error has a library name, an error number and an error description. These can be loaded at runtime.

  class CError : public std::runtime_error
  {
  private:
    struct SErrorEntry
    {
    private:
      SErrorEntry() = delete;

    public:
      std::string library;
      TErrorCode errorCode;
      std::string errorMessage;

      SErrorEntry(std::string l, TErrorCode ec, std::string em) : library(l), errorCode(ec), errorMessage(em) {}
    };

  public:
    typedef std::unordered_map<std::string, SErrorEntry> TErrorStore;

  private:
    std::string library_;
    TErrorCode errorCode_;

    static TErrorStore &errorMessages();

  public:
    inline explicit CError(std::string const & library, TErrorCode error) : std::runtime_error("Error"), library_(library),
      errorCode_(error) {}

    std::string library() const { return library_; }
    TErrorCode errorCode() const { return errorCode_;}
    std::string errorMessage() const;
    void logErrorMessage() const;
    bool isThisError(std::string library, TErrorCode errorCode) const { return ( (library_ == library) && (errorCode_ == errorCode)); }

    static bool addErrorMessage(std::string, TErrorCode, std::string);
  };

  /// @brief The CCodeError class is used to throw exceptions to indicate code errors within a library.
  /// @details Code exceptions are thrown when the running code reaches places that are theoretically impossible to reach.
  /// The exceptions includes data about the file name and line number where the exception occurred in order to allow easy
  /// tracking of where the exception was thrown.

  class CCodeError : public std::runtime_error
  {
  private:
    std::string library_;
    size_t lineNo;
    std::string fileName;
    std::string timeStamp;

  public:
    inline explicit CCodeError(std::string library, std::string file, std::string time, size_t newLine)
      : std::runtime_error("Unreachable Code Error"), library_(library), lineNo(newLine), fileName(file), timeStamp(time)
    { logErrorMessage(); }

    virtual std::string errorMessage() const;
    std::string library() const { return library_; }
    void logErrorMessage() const;
  };

  /// @brief The CRuntimeAssert class throws exceptions to indicate assertion failures within a library.
  /// @details Assertion exceptions are used when parameters to functions, or calculated values within functions are checked for
  /// correctness. Exceptions are thrown if the assertion fails.
  /// @note The CodeError class should be used for unreachable code errors.

  class CRuntimeAssert: public std::runtime_error
  {
  private:
    std::string library_;
    size_t lineNo;
    std::string fileName;
    std::string timeStamp;
    std::string expression;
    std::string message;

  public:
    explicit CRuntimeAssert(std::string library, std::string ne, std::string newFile, std::string newTime, size_t newLine,
                            std::string nm)
      : std::runtime_error("Runtime Assertion"), library_(library), lineNo(newLine), fileName(newFile), timeStamp(newTime),
        expression(ne), message(nm)
    {
      std::string messageString;

      messageString = "Assertion '" + expression + "' failed in library " + library_ + " file '";

      messageString += fileName + "' at line '" + std::to_string(lineNo) + "'.";

      if (!message.empty())
      {
        messageString += " " + message;
      }

      logger::defaultLogger().logMessage(logger::error, messageString);
    }

    std::string library() const { return library_; }
  };

  class search_error : public std::runtime_error
  {
  public:
    explicit search_error(std::string const &error) : std::runtime_error(error) {}
    ~search_error() {}
  };

#define ERROR(LIBRARY, ERROR) (throw(GCL::CError((#LIBRARY), (ERROR))))
#define CODE_ERROR(LIBRARY) (throw(GCL::CCodeError((#LIBRARY),  __FILE__, __TIMESTAMP__, static_cast<size_t>(__LINE__)) ))
#define RUNTIME_ASSERT(LIBRARY, EXPRESSION, MESSAGE) {if (!(EXPRESSION)) { throw GCL::CRuntimeAssert((#LIBRARY), (#EXPRESSION),  __FILE__, __TIMESTAMP__, (size_t) __LINE__, (MESSAGE)); }}

}	// namespace GCL

#else // GCL_CONTROL

#define ERROR(LIBRARY, ERROR)
#define CODE_ERROR(LIBRARY)
#define RUNTIME_ASSERT(LIBRARY, EXPRESSION, MESSAGE)

#endif // GCL_CONTROL

#endif // GCL_ERROR_H
