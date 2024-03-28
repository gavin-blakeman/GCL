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
//                      Copyright 2013-2024 Gavin Blakeman.
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
// HISTORY:             2024-03-27 GGB - Added the implment me class to use for unimplemented features that need to be implemented.
//                      2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-09-30 GGB - AIRDAS 2013.09 release.
//                      2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-01-26 GGB - Development of classes for Application AIRDAS
//
//*********************************************************************************************************************************

#ifndef GCL_ERROR_H
#define GCL_ERROR_H

  // Standard C++ library header files

#include <cstdint>
#include <optional>
#include <source_location>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace GCL
{
  namespace logger
  {
    class CLogger;    // Forward declaration needed.
  }

  /// @brief        The CRuntimeError class is used for reporting exceptions and errors. It is integrated with the logger and allows
  ///               exceptions to be thrown and errors reported.
  ///               Exceptions raised by CRuntimeError are not intended to be fatal and allow error management of calling functions.

  class runtime_error : public std::runtime_error
  {
  public:
    using errorType_t = std::uint_least16_t;
    using errorCode_t = std::uint_least16_t;

    runtime_error(std::string const &, errorType_t, errorCode_t, logger::CLogger * = nullptr);
    runtime_error(std::string const &, logger::CLogger * = nullptr);
    virtual ~runtime_error() = default;

    std::string errorMessage() const;

  private:
    errorType_t errorType_ = 0;
    errorCode_t errorCode_ = 0;

    runtime_error() = delete;
    runtime_error(runtime_error const &) = delete;
    runtime_error(runtime_error &&) = delete;
    runtime_error &operator =(runtime_error const &) = delete;
    runtime_error &operator=(runtime_error &&) = delete;
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

    std::string errorMessage(std::string const &, std::size_t) const;

  public:
    explicit CCodeError(std::string fn, size_t ln);
  };
  
  /// @brief        The CImplmentMe class is used to throw exceptions to indicate unimplmented code that needs to 
  ///               be implemented. This has been added to differentiate from CODE_ERRORS (unexpected or impossible branches)  

  class CImplementMe : public std::runtime_error
  {
  public:
    explicit CImplementMe(std::string fn, size_t ln);
    ~CImplementMe() = default;
  
  private:
    CImplementMe(CImplementMe const &) = delete;
    CImplementMe(CImplementMe &&) = delete;
    CImplementMe &operator=(CImplementMe const &) = delete;
    CImplementMe &operator==(CImplementMe &&) = delete;
  
    size_t lineNo;
    std::string fileName;

    std::string errorMessage(std::string const &, std::size_t) const;

  
  };

  /// @brief    The CRuntimeAssert class throws exceptions to indicate assertion failures within a library.
  /// @details  Assertion exceptions are used when parameters to functions, or calculated values within functions are checked for
  ///           correctness. Exceptions are thrown if the assertion fails.
  /// @note     The CodeError class should be used for unreachable code errors.

  class CRuntimeAssert: public std::runtime_error
  {
  public:
    explicit CRuntimeAssert(std::string const &expression, std::string const &message,
                            std::source_location const location = std::source_location::current());
  private:
    std::string errorMessage(std::string const &, std::string const &, std::size_t, std::string const &) const;
  };

  class search_error : public std::runtime_error
  {
  public:
    explicit search_error(std::string const &error) : std::runtime_error(error) {}
    ~search_error() {}
  };

  /// @brief Function to throw a runtime error.

  [[noreturn]] inline void RUNTIME_ERROR(std::string const &message,
                                         runtime_error::errorType_t et,
                                         runtime_error::errorCode_t ec,
                                         logger::CLogger *namedLogger = nullptr)
  {
    throw GCL::runtime_error(message, et, ec, namedLogger);
  }

  /// @brief      Throw a runtime error.
  /// @param[in]  message: The message for the exception.
  /// @param[in]  namedLogger: The logger to use for logging.
  /// @throws
  /// @version    2024-02-08/GGB - Function created.

  [[noreturn]] inline void RUNTIME_ERROR(std::string const &errorString,
                                         logger::CLogger *namedLogger = nullptr)
  {
    throw GCL::runtime_error(errorString, namedLogger);
  }

  [[noreturn]] inline void CODE_ERROR(std::source_location const location = std::source_location::current())
  {
    throw(CCodeError(std::string(location.file_name()), location.line()));
  }
  
  [[noreturn]] inline void IMPLEMENT_ME(std::source_location const location = std::source_location::current())
  {
    throw(CImplementMe(std::string(location.file_name()), location.line()));
  }

  #define RUNTIME_ASSERT(EXPRESSION, MESSAGE) {if (!(EXPRESSION)) { throw GCL::CRuntimeAssert((#EXPRESSION), (MESSAGE)); }}

}	// namespace GCL

#endif // GCL_ERROR_H
