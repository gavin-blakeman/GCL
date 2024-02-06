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

#include "include/logger/defaultLogger.h"

// GCL header files

#include "include/logger/queues/queueQueue.h"
#include "include/logger/sinks/streamSink.h"
#include "include/logger/filters/debugFilter.h"

namespace GCL::logger
{
  static void initialise(CLogger &);


  /// @brief      Return a reference to the default file logger.
  /// @throws     None.
  /// @version    2014-12-21/GGB - Function created.

  CLogger &defaultLogger()
  {
    static CLogger logger;
    static bool initialised = false;

    if (!initialised)
    {
      initialise(logger);
      initialised = true;
    }

    logger.startup();

    return logger;
  }

  static void initialise(CLogger &logger)
  {
    CDebugFilter::criticalityMap_t criticalityMap =
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

    std::shared_ptr<CDebugFilter> filter = std::make_shared<CDebugFilter>(std::move(criticalityMap));
    std::unique_ptr<CBaseSink> sink = std::make_unique<CStreamSink>(filter, std::cout);
    logger.addQueue(std::make_unique<CQueueQueue>());

    logger.addSink("cout", std::move(sink));
  }

} // namespace
