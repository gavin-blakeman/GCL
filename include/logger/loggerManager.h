//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                defaultLogger.h
// SUBSYSTEM:           Logging Functions
// LANGUAGE:            C++20
// TARGET OS:           None - Standard C++
// NAMESPACE:           GCL
// AUTHOR:              Gavin Blakeman.
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
// OVERVIEW:            The default logger is provided as a migration path from the original logger.
//
// CLASSES INCLUDED:
//
// HISTORY:             2024-02-05 GGB - Functions split from loggerCore into seperate files
//
//*********************************************************************************************************************************

#ifndef GCL_LOGGER_DEFAULTLOGGER_H
#define GCL_LOGGER_DEFAULTLOGGER_H

// Standard C++ library

#include <map>
#include <memory>
#include <optional>
#include <string>

// GCL library

#include "include/logger/loggerCore.h"
#include "include/logger/records/debugRecord.h"

namespace GCL::logger
{
  std::string const DEFAULT_LOGGER{"DEFAULT"};

  class CLoggerManager
  {
  public:
    static void addLogger(std::string const &, std::unique_ptr<CLogger>);
    static bool hasLogger(std::string const &) noexcept;
    static void removeLogger(std::string const &);
    static void setDefaultLogger(std::string const & = DEFAULT_LOGGER);

    static CLogger &defaultLogger();
    static CLogger &namedLogger(std::string const &);

    static void terminateAllLoggers();

  private:
    CLoggerManager() = delete;
    ~CLoggerManager() = delete;
    CLoggerManager(CLoggerManager const &) = delete;
    CLoggerManager(CLoggerManager &&) = delete;
    CLoggerManager &operator=(CLoggerManager const &) = delete;
    CLoggerManager &operator=(CLoggerManager &&) = delete;

    static std::map<std::string, std::unique_ptr<CLogger>> availableLoggers;
    static std::pair<std::string, CLogger *> activeLogger;

    static void createDefaultLogger();
  };

  enum severity_e
  {
    s_critical,
    s_error,
    s_warning,
    s_notice,
    s_information,
    s_debug,
    s_trace,
    s_exception,
  };

  // Some inline functions to simplify life

  /// @brief      Function to log a message.
  /// @param[in]  severity: The severity of the message.
  /// @param[in]  message: The message to log.
  /// @throws
  /// @version    2020-06-13/GGB - Converted from macro to function.

  inline void LOGMESSAGE(severity_e severity, std::string const &message)
  {
    CLoggerManager::defaultLogger().logMessage(std::make_unique<CDebugRecord>(severity, message));
  }

  /// @brief Function to log a critical message.
  /// @param[in] message: The message to log.
  /// @throws
  /// @version 2020-06-13/GGB - Converted from macro to function.

  inline void CRITICALMESSAGE(std::string const &message)
  {
    CLoggerManager::defaultLogger().logMessage(std::make_unique<CDebugRecord>(s_critical, message));
  }

  /// @brief      Function to log an error message.
  /// @param[in]  message: The message to log.
  /// @throws
  /// @version    2020-06-13/GGB - Converted from macro to function.

  inline void ERRORMESSAGE(std::string const &message, CLogger *namedLogger = nullptr)
  {
    if (namedLogger != nullptr)
    {
      namedLogger->logMessage(std::make_unique<CDebugRecord>(s_error, message));
    }
    else
    {
      CLoggerManager::defaultLogger().logMessage(std::make_unique<CDebugRecord>(s_error, message));
    };
  }

  /// @brief      Function to log an warning message.
  /// @param[in]  message: The message to log.
  /// @throws
  /// @version    2020-06-13/GGB - Converted from macro to function.

  inline void WARNINGMESSAGE(std::string const &message)
  {
    CLoggerManager::defaultLogger().logMessage(std::make_unique<CDebugRecord>(s_warning, message));
  }

  /// @brief Function to log a notice message.
  /// @param[in] message: The message to log.
  /// @throws
  /// @version 2020-06-13/GGB - Converted from macro to function.

  inline void NOTICEMESSAGE(std::string const &message)
  {
    CLoggerManager::defaultLogger().logMessage(std::make_unique<CDebugRecord>(s_notice, message));
  }

  /// @brief      Function to log an information message.
  /// @param[in]  message: The message to log.
  /// @throws
  /// @version    2020-06-13/GGB - Converted from macro to function.

  inline void INFOMESSAGE(std::string const &message, CLogger *namedLogger = nullptr)
  {
    if (namedLogger != nullptr)
    {
      namedLogger->logMessage(std::make_unique<CDebugRecord>(s_information, message));
    }
    else
    {
      CLoggerManager::defaultLogger().logMessage(std::make_unique<CDebugRecord>(s_information, message));
    }
  }

  /// @brief Function to log an debug message.
  /// @param[in] message: The message to log.
  /// @throws
  /// @version 2020-06-13/GGB - Converted from macro to function.

  inline void DEBUGMESSAGE(std::string const &message)
  {
    CLoggerManager::defaultLogger().logMessage(std::make_unique<CDebugRecord>(s_debug, message));
  }

  /// @brief Function to log a trace function entry point.
  /// @param[in] message: The message to log.
  /// @throws
  /// @version 2020-06-13/GGB - Converted from macro to function.

  inline void TRACEMESSAGE(std::string const &message)
  {
    CLoggerManager::defaultLogger().logMessage(std::make_unique<CDebugRecord>(s_trace, message));
  }

  /// @brief Function to log an exception.
  /// @param[in] message: The message to log.
  /// @throws
  /// @version 2020-06-14/GGB - Converted from macro to function.

  inline void LOGEXCEPTION(std::string const &message, CLogger *namedLogger = nullptr)
  {
    if (namedLogger != nullptr)
    {
      namedLogger->logMessage(std::make_unique<CDebugRecord>(s_exception, message));
    }
    else
    {
      CLoggerManager::defaultLogger().logMessage(std::make_unique<CDebugRecord>(s_exception, message));
    };
  }

  inline void TRACE_ENTER(std::source_location const location = std::source_location::current())
  {
    CLoggerManager::defaultLogger().logMessage(std::make_unique<CDebugRecord>(s_trace,
                               "Entering Function: " + std::string(location.function_name()) +
                               ". File: " + std::string(location.file_name()) +
                               ". Line: " + std::to_string(location.line())));
  }

  inline void TRACE_EXIT(std::source_location const location = std::source_location::current())
  {
    CLoggerManager::defaultLogger().logMessage(std::make_unique<CDebugRecord>(s_trace,
                               "Exiting Function: " + std::string(location.function_name()) +
                               ". File: " + std::string(location.file_name()) +
                               ". Line: " + std::to_string(location.line())));
  }


  /// @brief      Trace function to trace a line.
  /// @param[in]  location: Source code location.
  /// @throws
  /// @version    2023-10-16/GGB - Function created.

  inline void TRACE_LINE(std::source_location const location = std::source_location::current())
  {
    CLoggerManager::defaultLogger().logMessage(std::make_unique<CDebugRecord>(s_trace,
                               "Passing Line: " + std::string(location.function_name()) +
                               ". File: " + std::string(location.file_name()) +
                               ". Line: " + std::to_string(location.line())));
  }

  /// @brief      Trace function to flag an unexpected condition.
  /// @param[in]  location: Source code location.
  /// @throws
  /// @version    2023-10-16/GGB - Function created.

  inline void TRACE_UNEXPECTED(std::source_location const location = std::source_location::current())
  {
    CLoggerManager::defaultLogger().logMessage(std::make_unique<CDebugRecord>(s_trace,
                               "Unexpected branch: " + std::string(location.function_name()) +
                               ". File: " + std::string(location.file_name()) +
                               ". Line: " + std::to_string(location.line())));
  }

} // namespace

#endif // GCL_LOGGER_DEFAULTLOGGER_H
