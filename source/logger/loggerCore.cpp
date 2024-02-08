//*********************************************************************************************************************************
//
// PROJECT:							General Class Library
// FILE:								loggerCore.cpp
// SUBSYSTEM:						Logging Library
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
// HISTORY:             2018-08-12 GGB - gnuCash-pud debugging and release.
//                      2015-09-22 GGB - AIRDAS 2015.09 release
//                      2014-07-20 GGB - Development of class for "Observatory Weather System - Service"
//
//*********************************************************************************************************************************

#include "include/logger/loggerCore.h"

// Standard C++ library headers

#include <iomanip>
#include <iostream>

// Miscellaneous library headers

#include <fmt/format.h>

// GCL include headers

#include "include/error.h"

namespace GCL::logger
{

  /// @brief Default constructor for the class.
  /// @throws std::bad_alloc
  /// @version 2019-10-22/GGB - 1. Changed writerThread to a std::unique_ptr
  ///                           2. Changed writerThread from a boost::thread to a std::thread
  ///                           3. Changed to use std::chrono from boot::chrono.
  /// @version 2018-08-13/GGB - Bug #141 - Added auto-creation of std::cerr sink.
  /// @version 2014-12-24/GGB - Function created.

  CLogger::CLogger() : terminateThread(false), writerThread(nullptr), messageWaiting(0)
  {
  }

  /// @brief      Destructor for the class.
  /// @throws     None.
  /// @details    Needs too finish the thread and destroy the thread object.
  /// @version    2024-02-07/GGB - Updated to just call shutDown.
  /// @version    2019-10-22/GGB - 1. Changed writerThread to a std::unique_ptr
  ///                              2. Changed writerThread from a boost::thread to a std::thread
  /// @version    2014-12-24/GGB - Function created.

  CLogger::~CLogger()
  {
    shutDown();
  }

  //// @brief     Adds the specified queue to the logger. If there is an existing queue, it will be deleted.
  /// @param[in]  nq: The new queue
  /// @throws
  /// @version    2024-02-06/GGB - Function created.

  void CLogger::addQueue(std::unique_ptr<CBaseQueue> &&nq)
  {
    if (messageQueue)
    {
      messageQueue.reset();
    }
    messageQueue = std::move(nq);
  }

  /// @brief      Adds an output streamm into the container.
  /// @param[in]  ls: The logger sink to add to the logger.
  /// @throws     None.
  /// @version    2015-09-19/GGB - Added locking to the sink container.
  /// @version    2014-12-24/GGB - Function created.

  void CLogger::addSink(std::string const &sinkName, std::unique_ptr<CBaseSink> ls)
  {
    uniqueLock ul{sinkMutex};

    logSinks.emplace(sinkName, std::move(ls));
  }

  /// @brief      Removes a logger sink from the list of logger sinks.
  /// @param[in]  ls: The logger sink to remove.
  /// @returns    true sink found and erased.
  /// @returns    false sink not found.
  /// @throws     None.
  /// @version    2015-09-19/GGB - Added locking to the sink container.
  /// @version    2014-12-25/GGB - Function created.

  bool CLogger::removeSink(std::string const &sink)
  {
    bool returnValue = false;

    uniqueLock ul{sinkMutex};

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
  /// @version    2024-02-07/GGB - Updated to use atomic_flag and semaphores.
  /// @version    2014-12-25/GGB - Changed to create the logger record immediately on entry. The log record is also a smart pointer.
  /// @version    2014-07-20/GGB - Function created.

  void CLogger::logMessage(std::unique_ptr<CBaseRecord> &&record)
  {
    messageQueue->push(std::move(record));
    messageWaiting.release();
  }

  /// @brief      Returns a pointer to the specified sink.
  /// @param[in]  sink: The name of the sink.
  /// @returns    Pointer to the sink, or nullptr if not found.
  /// @throws
  /// @version    2024-02-06/GGB - Function created.

  CBaseSink &CLogger::sink(std::string const &sink) const
  {
    uniqueLock(sinkMutex);

    if (logSinks.contains(sink))
    {
      return *logSinks.at(sink);
    }
    else
    {
      CODE_ERROR();
      // Does not return,
    }
  }

  /// @brief      Starts the logger running.
  /// @throws
  /// @version    2024-02-06/GGB - Function created.

  void CLogger::startup()
  {
    if (!writerThread)
    {
      writerThread = std::make_unique<std::thread>(&CLogger::writer, this);

      if (!writerThread)
      {
        RUNTIME_ERROR(boost::locale::translate("LOGGER: Unable to start thread."));
      };
    }
  }

  /// @brief      Shuts down the writer thread.
  /// @throws     None.
  /// @version    2024-02-07/GGB - Updated to use atomic_flag and semaphores.
  /// @version    2019-10-22/GGB - 1. Changed writerThread to a std::unique_ptr
  ///                              2. Changed writerThread from a boost::thread to a std::thread
  /// @version    2015-09-19/GGB - Added locking to the sink container.
  /// @version    2015-06-01/GGB - Function created.

  void CLogger::shutDown()
  {
    if (writerThread)
    {
      terminateThread.test_and_set();
      messageWaiting.release();         // Ensure the thread unblocks

      writerThread->join();
      writerThread.reset(nullptr);

      while (!messageQueue->empty())
      {
        CBaseRecord const &record = messageQueue->front();

        uniqueLock ul{sinkMutex};
        for (auto &sink : logSinks)
        {
          sink.second->writeRecord(record);
        }

        messageQueue->pop();
      };
    }
  }

  /// @brief      This is the threaded function that executes concurrently.
  /// @details    Whenever a message is added to the queue, this is the function that writes it to the stream.
  /// @note       The writer thread may be called and active before sinks and queues are available.
  /// @throws     None.
  /// @version    2024-02-07/GGB - Updated to use atomic_flag and semaphores.
  /// @version    2016-05-07/GGB - Updated locking strategy to remove a number of errors.
  /// @version    2015-09-19/GGB - Added locking to the sink container.
  /// @version    2014-12-24/GGB - Updated to write to multiple streams.
  /// @version    2014-07-22/GGB - Function created.

  void CLogger::writer()
  {
    while (!terminateThread.test())
    {
      messageWaiting.acquire();

      while (!messageQueue->empty())
      {
        CBaseRecord const &record = messageQueue->front();
        {
          uniqueLock ul{sinkMutex};
          for (auto &sink : logSinks)
          {
            sink.second->writeRecord(record);
          }
        }
        messageQueue->pop();
      };
    };
  }

} // namespace
