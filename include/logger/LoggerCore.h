//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                CLogger
// SUBSYSTEM:           Logging Library
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	boost::chrono
//                      boost::filesystem>
//                      boost::thread
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2014-2018 Gavin Blakeman.
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
//                      CLoggerRecord
//                      CLoggerSink
//
// HISTORY:             2018-08-12 GGB - gnuCash-pud debugging and release.
//                      2015-09-22 GGB - AIRDAS 2015.09 release
//                      2014-07-20 GGB - Development of class for "Observatory Weather System - Service"
//
//*********************************************************************************************************************************

#ifndef GCL_LOGGER_LOGGERCORE_H
#define GCL_LOGGER_LOGGERCORE_H

#ifndef GCL_CONTROL

  // Standard C++ libraries

#include <cstdint>
#include <fstream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <queue>
#include <vector>

#ifndef GCL_NOBOOST
  #include "boost/chrono/include.hpp"
  #include "boost/filesystem.hpp"
  #include "boost/thread.hpp"
#endif  // GCL_NOBOOST

namespace GCL
{
  namespace logger
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

    enum ESeverity
    {
      trace,      ///< Lowest level log severity. Used for tracing program execution through function entry/exit.
      debug,      ///< Used to display debugging messages.
      info,       ///< Information messages.
      notice,     ///< Legal notices etc.
      warning,    ///< Failure of action. Normally built into program logic. (Time outs etc)
      error,      ///< Failure of program function. May be recoverable.
      critical,   ///< Non-recoverable failure.
    };

    struct CSeverity
    {
      public:
        bool fCritical;
        bool fError;
        bool fWarning;
        bool fNotice;
        bool fInfo;
        bool fDebug;
        bool fTrace;

        bool allow(ESeverity);
    };

    class CLoggerSink;
    typedef std::shared_ptr<CLoggerSink> PLoggerSink;

    class CLoggerRecord
    {
      typedef boost::shared_mutex             mutex_type;
      typedef boost::shared_lock<mutex_type>  SharedLock;
      typedef boost::unique_lock<mutex_type>  ExclusiveLock;

    private:
      mutable mutex_type recordMutex;                                   ///< Mutex for access control to the record.

    public:
      boost::chrono::system_clock::time_point timeStamp;
      ESeverity severity;
      std::string message;

      CLoggerRecord(ESeverity, std::string const &);

      std::string writeRecord(bool ts, bool ss) const;
    };
    typedef std::shared_ptr<CLoggerRecord> PLoggerRecord;

    class CLogger
    {
      private:
        typedef boost::upgrade_mutex            mutex_type;
        typedef boost::unique_lock<mutex_type>  UniqueLock;
        typedef boost::shared_lock<mutex_type>  SharedLock;
        typedef boost::upgrade_lock<mutex_type> UpgradeLock;
        typedef std::vector<PLoggerSink> TSinkContainer;

        mutable mutex_type queueMutex;
        mutable mutex_type terminateMutex;
        bool terminateThread;
        mutable boost::condition_variable_any cvQueueData;

        mutable mutex_type sinkMutex;
        TSinkContainer sinkContainer;

        boost::thread *writerThread;
        PLoggerSink defaultStreamSink;                        ///< Stream sink created in constructor to ensure logger always works.

        std::queue<PLoggerRecord> messageQueue;

      protected:
        ESeverity logSeverity;

        virtual void writer();

      public:
        CLogger();
        virtual ~CLogger();

        virtual void addSink(PLoggerSink ls);
        virtual bool removeSink(PLoggerSink ls);
        void removeDefaultStreamSink();

        virtual void logMessage(ESeverity, std::string const &);

        virtual void shutDown();
    };

    class CLoggerSink
    {
      private:
        bool timeStamp_     : 1;
        bool severityStamp_ : 1;
        CSeverity logSeverity;

      protected:
        virtual void write(std::string const &) = 0;

      public:
        CLoggerSink();
        void setLogLevel(CSeverity severity);

        void timeStamp(bool nts) { timeStamp_ = nts;}
        void severityStamp(bool nss) { severityStamp_ = nss;}

        void trace(bool f) { logSeverity.fTrace = f; }
        void debug(bool f) { logSeverity.fDebug = f; }
        void info(bool f) { logSeverity.fInfo = f; }
        void notice(bool f) { logSeverity.fNotice = f; }
        void warning(bool f) { logSeverity.fWarning = f; }

        virtual void writeRecord(PLoggerRecord &);
    };

    CLogger &defaultLogger();

      // Some macros to simplify life

#define LOGMESSAGE(severity, message) GCL::logger::defaultLogger().logMessage(GCL::logger::severity, message)
#define CRITICALMESSAGE(message) GCL::logger::defaultLogger().logMessage(GCL::logger::critical, message)
#define ERRORMESSAGE(message) GCL::logger::defaultLogger().logMessage(GCL::logger::error, message)
#define WARNINGMESSAGE(message) GCL::logger::defaultLogger().logMessage(GCL::logger::warning, message)
#define NOTICEMESSAGE(message) GCL::logger::defaultLogger().logMessage(GCL::logger::info, message)
#define INFOMESSAGE(message) GCL::logger::defaultLogger().logMessage(GCL::logger::info, message)
#define DEBUGMESSAGE(message) GCL::logger::defaultLogger().logMessage(GCL::logger::debug, message)
#define TRACEMESSAGE(message) GCL::logger::defaultLogger().logMessage(GCL::logger::trace, message)
#define TRACEENTER GCL::logger::defaultLogger().logMessage(GCL::logger::trace, "Entering Function: " + std::string(__PRETTY_FUNCTION__) + ". File: " + std::string(__FILE__) + ". Line: " + std::to_string(__LINE__))
#define TRACEEXIT GCL::logger::defaultLogger().logMessage(GCL::logger::trace, "Exiting Function: " + std::string(__PRETTY_FUNCTION__) + ". File: " + std::string(__FILE__) + ". Line: " + std::to_string(__LINE__))
      } // namespace logger
} // namespace GCL

#else // GCL_CONTROL
#define LOGMESSAGE(severity, message)
#define DEBUGMESSAGE(message)
#define TRACEMESSAGE(message)
#define TRACEENTER
#define TRACEEXIT
#endif    // GCL_CONTROL


#endif // GCL_LOGGER_LOGGERCORE_H
