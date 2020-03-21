//*********************************************************************************************************************************
//
// PROJECT:							General Class Library
// FILE:								CLogger
// SUBSYSTEM:						Logging Library
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	boost::filesystem
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2014-2020 Gavin Blakeman.
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

#include "../../include/logger/LoggerCore.h"

  // Standard C++ library headers

#include <iomanip>
#include <iostream>

  // Miscellaneous library headers



  // GCL include headers

#include "../../include/error.h"
#include "../../include/logger/StreamSink.h"

namespace GCL
{
  namespace logger
  {

    /// @brief Return a reference to the default file logger.
    /// @note Optionally create the logger.
    /// @version 2014-12-21/GGB - Function created.

    CLogger &defaultLogger()
    {
      static CLogger logger;
      return logger;
    }

    //******************************************************************************************************************************
    //
    // CSeverity
    //
    //******************************************************************************************************************************

    /// @brief Check if the criticality is allowed
    /// @param[in] s: The severity to check.
    /// @returns true if the severity is allowed
    /// @returns false otherwise
    /// @throws None.
    /// @version 2016-05-06/GGB - Updated to allow critical errors.

    bool CSeverity::allow(ESeverity s)
    {
      bool returnValue = false;

      switch (s)
      {
      case trace:
        returnValue = fTrace;
        break;
      case debug:
        returnValue = fDebug;
        break;
      case info:
        returnValue = fInfo;
        break;
      case notice:
        returnValue = fNotice;
        break;
      case warning:
        returnValue = fWarning;
        break;
      case error:
        returnValue = fError;
        break;
        case critical:
        {
          returnValue = fCritical;
          break;
        }
      default:
        returnValue = false;
        break;
      }

      return returnValue;
    }

    //******************************************************************************************************************************
    //
    // CLoggerSink
    //
    //******************************************************************************************************************************

    /// @brief Default constructor
    /// @throws None.
    /// @version 2018-08-14/GGB - Changed defaults on severity to not include trace and debug.
    /// @version 2014-12-25/GGB - Function created.

    CLoggerSink::CLoggerSink() : timeStamp_(true), severityStamp_(true), logSeverity{true, true, true, true, true, false, false}
    {
    }

    /// @brief Function to set the log level of the logger.
    /// @throws None.
    /// @version 2014-12-24/GGB - Function created.

    void CLoggerSink::setLogLevel(CSeverity severity)
    {
      logSeverity = severity;
    }

    /// @brief Write the logger record to the sink.
    /// @param[in] lr - The record to be written.
    /// @throws None.
    /// @version 2014-12-24/GGB - Function created.

    void CLoggerSink::writeRecord(PLoggerRecord &lr)
    {
        // Check that the message should be logged.

      if (logSeverity.allow(lr->severity))
      {
        write(lr->writeRecord(timeStamp_, severityStamp_));
      }

    }

    //******************************************************************************************************************************
    //
    // CLoggerRecord
    //
    //******************************************************************************************************************************

    /// @brief Constructor for the class.
    /// @param[in] s: The severity of the message.
    /// @param[in] t: The message to be logged.
    /// @throws None.
    /// @version 2014-07-20/GGB - Function created.

    CLoggerRecord::CLoggerRecord(ESeverity s, std::string const &t) : severity(s), message(t)
    {
      timeStamp = std::chrono::system_clock::now();
    }

    /// @brief Writes the logger message to the log file.
    /// @param[in] ts: timestamp
    /// @param[in] ss: message string.
    /// @returns A std::string containing the combined timestamp and string.
    /// @throws None.
    /// @version 2019-10-23/GGB - Updated to use std::chrono. The timestamp output has not been tested and will likely
    ///                           need to be fine tuned.
    /// @version 2014-07-21/GGB - Function created.

    std::string CLoggerRecord::writeRecord(bool ts, bool ss) const
    {
      std::ostringstream os;

      //os << boost::chrono::time_fmt(boost::chrono::timezone::local);

      SharedLock(recordMutex);

      if (ts)
      {
        std::time_t now_c = std::chrono::system_clock::to_time_t(timeStamp);
        os << "[" << std::put_time(std::localtime(&now_c), "%F%T%") << "] ";
      };

      if (ss)
      {
        switch (severity)
        {
          case critical:
          {
            os << "[critical] ";
            break;
          }
          case error:
          {
            os << "[error] ";
            break;
          };
          case warning:
          {
            os << "[warning] ";
            break;
          };
          case notice:
          {
            os << "[notice] ";
            break;
          };
          case info:
          {
            os << "[information] ";
            break;
          };
          case debug:
          {
            os << "[debug] ";
            break;
          };
          case trace:
          {
            os << "[trace] ";
            break;
          };
          default:
            break;
        };
      };

      os << message << std::flush;

      return ( os.str() );
    }

    //******************************************************************************************************************************
    //
    // CLogger
    //
    //******************************************************************************************************************************

    /// @brief Default constructor for the class.
    /// @throws GCL::CError(GCL, 0x1001) - LOGGER: Unable to start thread.
    /// @throws std::bad_alloc
    /// @throws GCL::CError(GCL, 0x1001)
    /// @version 2019-10-22/GGB - 1. Changed writerThread to a std::unique_ptr
    ///                           2. Changed writerThread from a boost::thread to a std::thread
    ///                           3. Changed to use std::chrono from boot::chrono.
    /// @version 2018-08-13/GGB - Bug #141 - Added auto-creation of std::cerr sink.
    /// @version 2014-12-24/GGB - Function created.

    CLogger::CLogger() : terminateThread(false), writerThread(nullptr), defaultStreamSink(std::make_shared<CStreamSink>(std::cerr)),
      logSeverity(warning)
    {
      writerThread = std::make_unique<std::thread>(&CLogger::writer, this);

      if (!writerThread)
      {
        ERROR(GCL, 0x1001);  // LOGGER: Unable to start thread.
      };

      defaultStreamSink->setLogLevel(GCL::logger::CSeverity{true, true, true, true, true, false, false});

      addSink(defaultStreamSink);
    }

    /// @brief Destructor for the class.
    /// @throws None.
    /// @details Needs too finish the thread and destroy the thread object.
    /// @version 2019-10-22/GGB - 1. Changed writerThread to a std::unique_ptr
    ///                           2. Changed writerThread from a boost::thread to a std::thread
    /// @version 2014-12-24/GGB - Function created.

    CLogger::~CLogger()
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
        //while (!writerThread->try_join_for(boost::chrono::milliseconds(1000)) )     // Wait for the writer thread to complete
        //{
        //}
        writerThread.reset(nullptr);

          // There should not be any messages, but empty the queue to be sure.

        while (!messageQueue.empty())
        {
          TSinkContainer::iterator sinkIterator;

          for (sinkIterator = sinkContainer.begin(); sinkIterator != sinkContainer.end(); sinkIterator++)
          {
            (*sinkIterator)->writeRecord(messageQueue.front());
          };

          messageQueue.pop();
        };
      };
    }

    /// @brief Adds an output streamm into the container.
    /// @param[in] ls: The logger sink to add to the logger.
    /// @throws None.
    /// @version 2015-09-19/GGB - Added locking to the sink container.
    /// @version 2014-12-24/GGB - Function created.

    void CLogger::addSink(PLoggerSink ls)
    {
      UniqueLock(sinkMutex);

      sinkContainer.push_back(ls);
    }

    /// @brief Creates a log message in place on the queue.
    /// @param[in] s: The severity of the message.
    /// @param[in] m: The text of the message.
    /// @throws std::bad_alloc
    /// @version 2014-12-25/GGB - Changed to create the logger record immediately on entry. The log record is also a smart pointer.
    /// @version 2014-07-20/GGB - Function created.

    void CLogger::logMessage(ESeverity s, std::string const &m)
    {
      PLoggerRecord newRecord(new CLoggerRecord(s, m));
      {
        UniqueLock(queueMutex);

        messageQueue.push(newRecord);
      };
      cvQueueData.notify_one();
    }

    /// @brief Function to remove the default stream sink if it is not needed. The default stream sink is created in the
    ///        constructor to ensure that the logger will always have an output.
    /// @throws None.
    /// @version 2018-08-13/GGB - Function created.

    void CLogger::removeDefaultStreamSink()
    {
      removeSink(defaultStreamSink);
    }

    /// @brief Removes a logger sink from the list of logger sinks.
    /// @param[in] ls: The logger sink to remove.
    /// @returns true sink found and erased.
    /// @returns false sink not found.
    /// @throws None.
    /// @version 2015-09-19/GGB - Added locking to the sink container.
    /// @version 2014-12-25/GGB - Function created.

    bool CLogger::removeSink(PLoggerSink ls)
    {
      UniqueLock(sinkMutex);

      TSinkContainer::iterator iter = sinkContainer.begin();
      bool returnValue = false;

      while ( ((*iter) != ls) && (iter != sinkContainer.end()) )
      {
        iter++;
      };

      if ( (*iter) == ls )
      {
        sinkContainer.erase(iter);
        returnValue = true;
      };

      return returnValue;
    }

    /// @brief Shuts down the writer thread.
    /// @throws None.
    /// @version 2019-10-22/GGB - 1. Changed writerThread to a std::unique_ptr
    ///                           2. Changed writerThread from a boost::thread to a std::thread
    /// @version 2015-09-19/GGB - Added locking to the sink container.
    /// @version 2015-06-01/GGB - Function created.

    void CLogger::shutDown()
    {
        // Obtain a unique lock on the terminate mutex to write the value.

      {
        UniqueLock writeLock(terminateMutex);
        terminateThread = true;
      };  // Lock released at this point.
      cvQueueData.notify_one();   // Notify the queue to run.

      writerThread->join();
//      while (!writerThread->try_join_for(boost::chrono::milliseconds(1000)) )     // Wait for the writer thread to complete
//      {
//        try
//        {
//          writerThread->interrupt();            // Force exit if thread does not join by itself.
//        }
//        catch(...)
//        {
//        }
//      };
      writerThread.reset(nullptr);

        // There should not be any messages, but empty the queue to be sure. In this case the queue does not need to be locked as
        // the thread has been terminated.

      while (!messageQueue.empty())
      {
        UniqueLock(sinkMutex);

        TSinkContainer::iterator sinkIterator;

        for (sinkIterator = sinkContainer.begin(); sinkIterator != sinkContainer.end(); sinkIterator++)
        {
          (*sinkIterator)->writeRecord(messageQueue.front());
        };

        messageQueue.pop();
      };
    }

    /// @brief This is the threaded function that executes concurrently.
    /// @details Whenever a message is added to the queue, this is the function that writes it to the stream.
    /// @throws None.
    /// @version 2016-05-07/GGB - Updated locking strategy to remove a number of errors.
    /// @version 2015-09-19/GGB - Added locking to the sink container.
    /// @version 2014-12-24/GGB - Updated to write to multiple streams.
    /// @version 2014-07-22/GGB - Function created.

    void CLogger::writer()
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
          SharedLock(sinkMutex);              // Lock the sinks while writing.

          TSinkContainer::iterator sinkIterator;

          /// @todo Rewrite using std::for_each() and lambdas or range-based for loop.

          for (sinkIterator = sinkContainer.begin(); sinkIterator != sinkContainer.end(); sinkIterator++)
          {
            (*sinkIterator)->writeRecord(messageQueue.front());
          };

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

  } // namespace logger
} // namespace GCL
