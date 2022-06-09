//*********************************************************************************************************************************
//
// PROJECT:							General Class Library (GCL)
// FILE:								error.h
// SUBSYSTEM:						Exception/Error subsystem
// LANGUAGE:						C++
// TARGET OS:						None.
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2013-2022 Gavin Blakeman.
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

#include "include/logger/loggerCore.h"

#ifndef GCL_CONTROL

namespace GCL
{
  using GCL::logger::LOGEXCEPTION;

  typedef std::uint16_t TErrorCode;

  /// @brief The CError class is used to throw exceptions to indicate errors within a library.
  /// @details Errors can be of any type. The exception allows a library and error number within a library to be specified.
  /// Each error has a library name, an error number and an error description. These can be loaded at runtime.

  class [[deprecated]] CError : public std::runtime_error
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

  /// @brief        The CRuntimeError class is used for reporting exceptions and errors. It is integrated with the logger and allows
  ///               exceptions to be thrown and errors reported.
  ///               Exceptions raised by CRuntimeError are not intended to be fatal and allow error management of calling functions.

  class runtime_error : public std::runtime_error
  {
  private:
    TErrorCode errorCode_;
    std::string library_;

    runtime_error() = delete;
    runtime_error(runtime_error const &) = delete;
    runtime_error(runtime_error &&) = delete;
    runtime_error operator =(runtime_error const &) = delete;

    std::string errorMessage() const;

  public:
    runtime_error(std::string const &errorString, TErrorCode errorCode = 0, std::string const &library = "") :
      std::runtime_error(errorString), errorCode_(errorCode), library_(library)
    {
      LOGEXCEPTION(errorMessage());
    }

  };

  /// @brief        The CCodeError class is used to throw exceptions to indicate code errors within a library.
  /// @details      Code exceptions are thrown when the running code reaches places that are theoretically impossible to reach.
  ///               The exceptions includes data about the file name and line number where the exception occurred in order to allow
  ///               easy tracking of where the exception was thrown.

  class CCodeError : public std::runtime_error
  {
  private:
    size_t lineNo;
    std::string fileName;
    std::string timeStamp;

    std::string errorMessage(std::string const &, std::string const &, std::size_t) const;

  public:
    inline explicit CCodeError(std::string fileName, std::string timeStamp, size_t lineNumber)
      : std::runtime_error("Code Error.")
    {
      LOGEXCEPTION(errorMessage(fileName, timeStamp, lineNumber));
    }
  };

  /// @brief    The CRuntimeAssert class throws exceptions to indicate assertion failures within a library.
  /// @details  Assertion exceptions are used when parameters to functions, or calculated values within functions are checked for
  ///           correctness. Exceptions are thrown if the assertion fails.
  /// @note     The CodeError class should be used for unreachable code errors.

  class CRuntimeAssert: public std::runtime_error
  {
  private:
    std::string errorMessage(std::string const &, std::string const &, std::string const &, std::size_t, std::string const &) const;

  public:
    explicit CRuntimeAssert(std::string const &expression, std::string const &fileName,
                            std::string const &timeStamp, size_t lineNumber, std::string const &message)
      : std::runtime_error("Runtime Assert")
    {
      LOGEXCEPTION(errorMessage(expression, fileName, timeStamp, lineNumber, message));
    }
  };

  class search_error : public std::runtime_error
  {
  public:
    explicit search_error(std::string const &error) : std::runtime_error(error) {}
    ~search_error() {}
  };

  /// @brief Function to throw a runtime error.

  inline void RUNTIME_ERROR(std::string const &errorString, TErrorCode errorCode = 0, std::string const &library = "")
  {
    throw GCL::runtime_error(errorString, errorCode, library);
  }

#define ERROR(LIBRARY, ERROR) (throw(GCL::CError((#LIBRARY), (ERROR))))
#define CODE_ERROR (throw(GCL::CCodeError( __FILE__, __TIMESTAMP__, static_cast<size_t>(__LINE__)) ))
#define RUNTIME_ASSERT(EXPRESSION, MESSAGE) {if (!(EXPRESSION)) { throw GCL::CRuntimeAssert((#EXPRESSION),  __FILE__, __TIMESTAMP__, (size_t) __LINE__, (MESSAGE)); }}

}	// namespace GCL

#else // GCL_CONTROL

#define ERROR(LIBRARY, ERROR)
#define CODE_ERROR
#define RUNTIME_ASSERT(EXPRESSION, MESSAGE)

#endif // GCL_CONTROL

#endif // GCL_ERROR_H
