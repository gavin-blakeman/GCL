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
// OVERVIEW:            The default logger is provided as a migration path from the original logger.
//
// CLASSES INCLUDED:
//
// HISTORY:             2024-02-05 GGB - Functions split from loggerCore into separate files
//
//*********************************************************************************************************************************

#include "include/logger/loggerManager.h"

  // Standard C++ library

#include <iostream>

// GCL header files

#include "include/error.h"

#include "include/logger/queues/queueQueue.h"
#include "include/logger/sinks/streamSink.h"
#include "include/logger/filters/debugFilter.h"

namespace GCL::logger
{
  std::map<std::string, std::unique_ptr<CLogger>> CLoggerManager::availableLoggers;
  std::pair<std::string, CLogger *> CLoggerManager::activeLogger = {"", nullptr};

  /// @brief      Adds a new logger into the map of available loggers.
  /// @param[in]  name: The name of the logger.
  /// @param[in]  logger: The logger to add.
  /// @throws
  /// @version    2024-02-06/GGB - Function created.

  void CLoggerManager::addLogger(std::string const &name, std::unique_ptr<CLogger> logger)
  {
    availableLoggers.emplace(name, std::move(logger));
  }

  /// @brief      Creates the default logger.
  /// @throws
  /// @version    2024-02-06/GGB - Function created.

  void CLoggerManager::createDefaultLogger()
  {
    CDebugFilter::criticalityMap_t const criticalityMap =
    {
        { s_critical, "CRITICAL"},
        { s_error, "ERROR"},
        { s_warning, "WARNING"},
        { s_notice, "NOTICE"},
        { s_information, "INFO"},
        { s_debug, "DEBUG"},
        { s_trace, "TRACE"},
        { s_exception, "EXCEPTION"},
    };

    GCL::logger::CDebugFilter::criticalityMask_t criticalityMask{s_critical, s_error, s_warning, s_notice, s_information, s_debug, s_exception, s_trace};
    std::unique_ptr<CLogger> logger = std::make_unique<CLogger>();
    std::shared_ptr<CDebugFilter> filter = std::make_shared<CDebugFilter>(criticalityMap, criticalityMask);
    std::unique_ptr<CBaseSink> sink = std::make_unique<CStreamSink>(filter, std::cout);

    logger->addQueue(std::make_unique<CQueueQueue>());
    logger->addSink("cout", std::move(sink));
    logger->startup();
    addLogger("DEFAULT", std::move(logger));

    if (activeLogger.second == nullptr)
    {
      setDefaultLogger(DEFAULT_LOGGER);
    };
  }

  /// @brief      Returns the currently selected logger
  /// @returns    Reference to the current logger.
  /// @throws
  /// @version    2024-02-06/GGB - Function created.

  CLogger &CLoggerManager::namedLogger(std::string const &name)
  {
    if (availableLoggers.contains(name))
    {
      return *availableLoggers.at(name);
    }
    else
    {
      CODE_ERROR();
      // Does not return.
    }
    return *activeLogger.second;
  }

  CLogger &CLoggerManager::defaultLogger()
  {
    if (activeLogger.second == nullptr)
    {
      if (!availableLoggers.contains(DEFAULT_LOGGER))
      {
        createDefaultLogger();
      }
      setDefaultLogger();
    }

    return *activeLogger.second;
  }

  /// @brief      Tests if a specific logger exists.
  /// @param[in]  name: The name of the logger.
  /// @returns    true if the loggere exists.
  /// @throws     None.
  /// @version    2024-02-06/GGB - Function created.

  bool CLoggerManager::hasLogger(std::string const &name) noexcept
  {
    return availableLoggers.contains(name);
  }

  /// @brief      Removes a logger from the map of available loggers.
  /// @param[in]  name: The name of the logger.
  /// @throws
  /// @version    2024-02-06/GGB - Function created.

  void CLoggerManager::removeLogger(std::string const &name)
  {
    if ( (name != "DEFAULT") && availableLoggers.contains(name))
    {
      if (activeLogger.first == name)
      {
        setDefaultLogger();
      }
      availableLoggers.erase(name);
    }
  }

  /// @brief      Sets the current logger.
  /// @param[in]  name: The logger to make current.
  /// @throws
  /// @version    2024-02-06/GGB - Function created.

  void CLoggerManager::setDefaultLogger(std::string const &name)
  {
    if (availableLoggers.contains(name))
    {
      if (activeLogger.first != name)
      {
        activeLogger.first = name;
        activeLogger.second = availableLoggers.at(name).get();
      };
    }
    else
    {
      CODE_ERROR();
      // Does not return.
    }
  }

  /// @brief      Shuts down and deletes all loggers.
  /// @throws
  /// @version    2024-02-06/GGB - Function created.

  void CLoggerManager::terminateAllLoggers()
  {
    for (auto &logger: availableLoggers)
    {
      logger.second->shutDown();      // This should flush the queue. If not, the next step does flush the queues.
    }
    availableLoggers.clear();
  }

} // namespace
