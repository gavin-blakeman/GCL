//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                loggerCore.h
// SUBSYSTEM:           Logging Library
// LANGUAGE:						C++
// TARGET OS:						None.
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2014-2026 Gavin Blakeman.
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
// OVERVIEW:            This file implements a logging class to be used to log messages to file.
//                      The class has been designed as a lightweight easy to use class.
//
// CLASSES INCLUDED:    CLogger
//
// HISTORY:             2024-02-05 GGB - Rewrite to a flexible approach to enable additional functionality to be added.
//                      2022-06-09 GGB - Replace macros TRACENETER and TRACEEXIT with functions.
//                      2019-10-22 GGB - Changed Boost::thread to std::thread
//                      2018-08-12 GGB - gnuCash-pud debugging and release.
//                      2015-09-22 GGB - AIRDAS 2015.09 release
//                      2014-07-20 GGB - Development of class for "Observatory Weather System - Service"
//
//*********************************************************************************************************************************

#ifndef GCL_LOGGER_LOGGERCORE_H
#define GCL_LOGGER_LOGGERCORE_H

// Standard C++ libraries

#include <atomic>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <semaphore>
#include <shared_mutex>
#include <source_location>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

// Miscellaneous library header files.

#include "boost/locale.hpp"
#include <fmt/format.h>

// GCL header files

#include "include/common.h"
#include "include/logger/records/baseRecord.h"
#include "include/logger/queues/baseQueue.h"
#include "include/logger/sinks/baseSink.h"

namespace GCL::logger
{
  /// @page page1 Logger
  /// @tableofcontents
  /// The logger classes provide a lightweight logging mechanism for logging errors, debug and diagnostic information, as well
  /// as tracing code execution.
  /// @section sec1 Overview
  /// The logger implementation consists of a core class CLogger, sink classes and a message class.
  /// The logger is multi-threaded allowing the messages to be logged and output asynchronously.
  /// Logger sinks are used to output the log information. Standard sinks are provided for streams (eg, std::cout), files and
  /// also for a Qt text edit control.Any number of sinks can be registered with the logger core.
  /// When a messsage is logged, the core writes the message to all the sinks in turn. This is done on a parallel thread to enable
  /// the main application to continue executing.
  /// @section sec2 Severity
  /// Each message has a severity assigned. The severity determines if the message will be captured by any specific sink. Thus it
  /// is possible to have a sink, sinking to an error file for critical and error messages, and another sink, sinking to a log
  /// file for all other messages except debug and trace messages.
  /// The major components of the logger are
  /// CLogger - The core logging component. One instance is created for each type of log required.
  /// Records - Records are logging records. The record contains the data to be logged. Records are created and added to the log
  ///           queue
  /// Queue - The queue that ensures that the messages are processed in the correct order. The default queue is a simple FIFO queue,
  ///         but it is possible to create queues that process in different orders. (Priority, LIFO, Alarm))
  /// Filter - Filters manage which records are processed and which records are simply discarded. The filter converts the
  ///          information in the record into an output string/message.
  /// Sinks - Sinks present or store the information.
  /// A logging system then consists of the following:
  /// A single CLogger that manages all the records. It contains a single queue and multiple sinks.
  /// Each sink has an filter. The filter converts to records to strings for output/storage by the sink.

  class CLogger
  {
  public:

    /*! @brief      Default constructor for the class.
     *  @param[in]  n: The name of the logger.
     *  @throws     std::bad_alloc
     */
    CLogger(std::string const &n = "LOGGER");
    ~CLogger();

    void addQueue(std::unique_ptr<CBaseQueue> &&);

    void addSink(std::string const &, std::unique_ptr<CBaseSink>);
    bool removeSink(std::string const &);
    CBaseSink &sink(std::string const &) const;

    void logMessage(std::unique_ptr<CBaseRecord> &&);

    void startup();
    void shutDown();

    std::string const &name() const { return name_; }

  protected:
    void writer();

  private:
    CLogger(CLogger const &) = delete;
    CLogger(CLogger &&) = delete;
    CLogger &operator=(CLogger const &) = delete;
    CLogger &operator=(CLogger &&) = delete;

    typedef std::shared_mutex             mutex_type;
    using uniqueLock = std::unique_lock<mutex_type>;
    typedef std::shared_lock<mutex_type>  SharedLock;
    using logSinks_t = std::map<std::string, std::unique_ptr<CBaseSink>>;

    std::string name_;
    std::binary_semaphore messageWaiting;
    std::atomic_flag terminateThread;

    std::unique_ptr<CBaseQueue> messageQueue;

    mutable mutex_type sinkMutex;                 // Mutex protecting the sinks.
    logSinks_t logSinks;

    std::unique_ptr<std::thread> writerThread;
  };

} // namespace GCL::logger

#endif // GCL_LOGGER_LOGGERCORE_H
