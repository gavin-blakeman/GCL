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

#include "../error.h"


namespace GCL::logger
{

  /// @brief Default constructor for the class.
  /// @throws std::bad_alloc
  /// @version 2019-10-22/GGB - 1. Changed writerThread to a std::unique_ptr
  ///                           2. Changed writerThread from a boost::thread to a std::thread
  ///                           3. Changed to use std::chrono from boot::chrono.
  /// @version 2018-08-13/GGB - Bug #141 - Added auto-creation of std::cerr sink.
  /// @version 2014-12-24/GGB - Function created.

  CLogger::CLogger() : terminateThread(false), writerThread(nullptr)
  {
  }

  /// @brief      Destructor for the class.
  /// @throws     None.
  /// @details    Needs too finish the thread and destroy the thread object.
  /// @version    2019-10-22/GGB - 1. Changed writerThread to a std::unique_ptr
  ///                              2. Changed writerThread from a boost::thread to a std::thread
  /// @version    2014-12-24/GGB - Function created.

  CLogger::~CLogger()
  {
    if (writerThread)
    {
      // Request all threads to terminate.
      {
        uniqueLock lock(terminateMutex);
        terminateThread = true;
      }
      cvQueueData.notify_one();

      writerThread->join();
      writerThread.reset(nullptr);

      // There should not be any messages, but empty the queue to be sure.

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
    };
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
  /// @version    2014-12-25/GGB - Changed to create the logger record immediately on entry. The log record is also a smart pointer.
  /// @version    2014-07-20/GGB - Function created.

  void CLogger::logMessage(std::unique_ptr<CBaseRecord> &&record)
  {
    {
      uniqueLock lock(queueMutex);

      messageQueue->push(std::move(record));
    };
    cvQueueData.notify_one();
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
        RUNTIME_ERROR(boost::locale::translate("LOGGER: Unable to start thread."), E_LOGGER_UNABLETOSTARTTHREAD, LIBRARYNAME);
      };
    }
  }

  /// @brief      Shuts down the writer thread.
  /// @throws     None.
  /// @version    2019-10-22/GGB - 1. Changed writerThread to a std::unique_ptr
  ///                              2. Changed writerThread from a boost::thread to a std::thread
  /// @version    2015-09-19/GGB - Added locking to the sink container.
  /// @version    2015-06-01/GGB - Function created.

  void CLogger::shutDown()
  {
    // Obtain a unique lock on the terminate mutex to write the value.

    {
      uniqueLock writeLock(terminateMutex);
      terminateThread = true;
    }  // Lock released at this point.
    cvQueueData.notify_one();   // Notify the queue to run.

    writerThread->join();
    writerThread.reset(nullptr);

    // There should not be any messages, but empty the queue to be sure. In this case the queue does not need to be locked as
    // the thread has been terminated.

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

  /// @brief      This is the threaded function that executes concurrently.
  /// @details    Whenever a message is added to the queue, this is the function that writes it to the stream.
  /// @note       The writer thread may be called and active before sinks and queues are available.
  /// @throws     None.
  /// @version    2016-05-07/GGB - Updated locking strategy to remove a number of errors.
  /// @version    2015-09-19/GGB - Added locking to the sink container.
  /// @version    2014-12-24/GGB - Updated to write to multiple streams.
  /// @version    2014-07-22/GGB - Function created.

  void CLogger::writer()
  {
    bool mustTerminate = false;

    while (!mustTerminate)
    {
      // Waiting part of the writer. Implement using a shared lock. The shared lock will be released once data is put in the
      // queue and notify is called.
      {
        SharedLock readLock(queueMutex);
        while (messageQueue->empty())
        {
          cvQueueData.wait(readLock);             // Wait for notify from adding data. NB. This also unlocks the mutex!
        }
      }    // The shared lock is released on exit from the code block.

      while (!messageQueue->empty())
      {
        CBaseRecord const &record = messageQueue->front();

        uniqueLock ul{sinkMutex};
        for (auto &sink : logSinks)
        {
          sink.second->writeRecord(record);
        }

        // Need to have write access to the queue. Request a Unique lock.

        {
          uniqueLock writeLock(queueMutex);
          messageQueue->pop();
        };  // Unique lock released on exit from code block.
      };

      // Lock the terminateMutex to get the value of terminateThread.
      {
        SharedLock lock(terminateMutex);
        mustTerminate = terminateThread;
      }
    };
  }

} // namespace
