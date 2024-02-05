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
//                      Copyright 2014-2024 Gavin Blakeman.
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

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <mutex>
#include <queue>
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
//#include "include/error.h"
#include "include/GCLError.h"
#include "include/logger/records/baseRecord.h"
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

  class CLogger
  {
  public:
    /// @brief Default constructor for the class.
    /// @throws GCL::CError(GCL, 0x1001) - LOGGER: Unable to start thread.
    /// @throws std::bad_alloc
    /// @throws GCL::CError(GCL, 0x1001)
    /// @version 2019-10-22/GGB - 1. Changed writerThread to a std::unique_ptr
    ///                           2. Changed writerThread from a boost::thread to a std::thread
    ///                           3. Changed to use std::chrono from boot::chrono.
    /// @version 2018-08-13/GGB - Bug #141 - Added auto-creation of std::cerr sink.
    /// @version 2014-12-24/GGB - Function created.

    CLogger() : terminateThread(false), writerThread(nullptr)
    {
      writerThread = std::make_unique<std::thread>(&CLogger::writer, this);

      if (!writerThread)
      {
        RUNTIME_ERROR(boost::locale::translate("LOGGER: Unable to start thread."), E_LOGGER_UNABLETOSTARTTHREAD, LIBRARYNAME);
      };
    }

    /// @brief Destructor for the class.
    /// @throws None.
    /// @details Needs too finish the thread and destroy the thread object.
    /// @version 2019-10-22/GGB - 1. Changed writerThread to a std::unique_ptr
    ///                           2. Changed writerThread from a boost::thread to a std::thread
    /// @version 2014-12-24/GGB - Function created.

    virtual ~CLogger()
    {
      if (writerThread)
      {
        // Request all threads to terminate.

        {
          UniqueLock lock(terminateMutex);
          terminateThread = true;
        }
        cvQueueData.notify_one();

        writerThread->join();
        writerThread.reset(nullptr);

        // There should not be any messages, but empty the queue to be sure.

        while (!queue.empty())
        {
          std::unique_ptr<CBaseRecord> record = queue.pop_back();

          UniqueLock ul{sinkMutex};
          for (auto &sink : logSinks)
          {
            sink.writeRecord(record.get());
          }

          record.reset();
        };
      };
    }


    /// @brief      Adds an output streamm into the container.
    /// @param[in]  ls: The logger sink to add to the logger.
    /// @throws     None.
    /// @version    2015-09-19/GGB - Added locking to the sink container.
    /// @version    2014-12-24/GGB - Function created.

    virtual void addSink(std::string const &sinkName, std::unique_ptr<CBaseSink> ls)
    {
      UniqueLock ul{sinkMutex};

      sinkContainer.emplace(sinkName, std::move(ls));
    }

    /// @brief Removes a logger sink from the list of logger sinks.
    /// @param[in] ls: The logger sink to remove.
    /// @returns true sink found and erased.
    /// @returns false sink not found.
    /// @throws None.
    /// @version 2015-09-19/GGB - Added locking to the sink container.
    /// @version 2014-12-25/GGB - Function created.

    virtual bool removeSink(std::string const &sink)
    {
      bool returnValue = false;

      UniqueLock ul{sinkMutex};

      if (logSinks.contains(sink))
      {
        logSinks.erase(sink);
        returnValue = true;
      };
      return returnValue;
    }

    /// @brief      Creates a log message in place on the queue.
    /// @param[in]  record: The message to log.
    /// @throws     std::bad_alloc
    /// @version    2014-12-25/GGB - Changed to create the logger record immediately on entry. The log record is also a smart pointer.
    /// @version    2014-07-20/GGB - Function created.

    virtual void logMessage(std::unique_ptr<CBaseRecord> &&record)
    {
      {
        UniqueLock lock(queueMutex);

        messageQueue.push_front(std::move(record));
      };
    cvQueueData.notify_one();
    }


    /// @brief Shuts down the writer thread.
    /// @throws None.
    /// @version 2019-10-22/GGB - 1. Changed writerThread to a std::unique_ptr
    ///                           2. Changed writerThread from a boost::thread to a std::thread
    /// @version 2015-09-19/GGB - Added locking to the sink container.
    /// @version 2015-06-01/GGB - Function created.

    virtual void shutDown()
    {
      // Obtain a unique lock on the terminate mutex to write the value.

      {
        UniqueLock writeLock(terminateMutex);
        terminateThread = true;
      };  // Lock released at this point.
      cvQueueData.notify_one();   // Notify the queue to run.

      writerThread->join();
      writerThread.reset(nullptr);

      // There should not be any messages, but empty the queue to be sure. In this case the queue does not need to be locked as
      // the thread has been terminated.

      while (!queue.empty())
      {
        std::unique_ptr<CRecord> record = queue.pop_back();

        UniqueLock ul{sinkMutex};
        for (auto &sink : logSinks)
        {
          sink.writeRecord(record.get());
        }

        record.reset();
      };
    }

  private:
    CLogger(CLogger const &) = delete;
    CLogger(CLogger &&) = delete;
    CLogger &operator=(CLogger const &) = delete;
    CLogger &operator=(CLogger &&) = delete;


    typedef std::shared_mutex             mutex_type;
    typedef std::unique_lock<mutex_type>  UniqueLock;
    typedef std::shared_lock<mutex_type>  SharedLock;
    typedef std::map<std::string, CLoggerSink> logSinks_t;

    mutable mutex_type queueMutex;              // Protects the message queue
    mutable mutex_type terminateMutex;
    bool terminateThread;
    mutable std::condition_variable_any cvQueueData;

    T queue;

    mutable mutex_type sinkMutex;
    logSinks_t logSinks;

    std::unique_ptr<std::thread> writerThread;

  protected:
    /// @brief This is the threaded function that executes concurrently.
    /// @details Whenever a message is added to the queue, this is the function that writes it to the stream.
    /// @throws None.
    /// @version 2016-05-07/GGB - Updated locking strategy to remove a number of errors.
    /// @version 2015-09-19/GGB - Added locking to the sink container.
    /// @version 2014-12-24/GGB - Updated to write to multiple streams.
    /// @version 2014-07-22/GGB - Function created.

    virtual void writer()
    {
      bool mustTerminate = false;

      while (!mustTerminate)
      {
        // Waiting part of the writer. Implement using a shared lock. The shared lock will be released once data is put in the
        // queue and notify is called.
        {
          SharedLock readLock(queueMutex);
          while (messageQueue.empty())
          {
            cvQueueData.wait(readLock);             // Wait for notify from adding data. NB. This also unlocks the mutex!
          };
        };    // The shared lock is released on exit from the code block.

      while (!messageQueue.empty())
      {
        std::unique_ptr<CRecord> record = queue.pop_back();

        UniqueLock ul{sinkMutex};
        for (auto &sink : logSinks)
        {
          sink.writeRecord(record.get());
        }

        record.reset();

        // Need to have write access to the queue. Request a Unique lock.

        {
          UniqueLock writeLock(queueMutex);
          messageQueue.pop();
        };  // Unique lock released on exit from code block.
      };

      // Lock the terminateMutex to get the value of terminateThread.
      {
        SharedLock lock(terminateMutex);
        mustTerminate = terminateThread;
      }
      };
    }
  };

} // namespace GCL::logger

#endif // GCL_LOGGER_LOGGERCORE_H
