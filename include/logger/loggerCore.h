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
//                      Copyright 2014-2022 Gavin Blakeman.
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
// HISTORY:             2019-10-22 GGB - Changed Boost::thread to std::thread
//                      2018-08-12 GGB - gnuCash-pud debugging and release.
//                      2015-09-22 GGB - AIRDAS 2015.09 release
//                      2014-07-20 GGB - Development of class for "Observatory Weather System - Service"
//
//*********************************************************************************************************************************

#ifndef GCL_LOGGER_LOGGERCORE_H
#define GCL_LOGGER_LOGGERCORE_H

#ifndef GCL_CONTROL

  // Standard C++ libraries

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <fstream>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <sstream>
#include <string>
#include <thread>
#include <queue>
#include <vector>

  // Miscellaneous library header files.

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

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
      exception,  ///< Used to capture exceptions. These do not need to be actioned as the code may manage them.
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
        bool fException;
        bool fTrace;

        bool allow(ESeverity);
    };

    class CLoggerSink;
    typedef std::shared_ptr<CLoggerSink> PLoggerSink;

    class CLoggerRecord
    {
      typedef std::shared_mutex             mutex_type;
      typedef std::shared_lock<mutex_type>  SharedLock;
      typedef std::unique_lock<mutex_type>  ExclusiveLock;

    private:
      mutable mutex_type recordMutex;                                   ///< Mutex for access control to the record.

    public:
      std::chrono::system_clock::time_point timeStamp;
      ESeverity severity;
      std::string message;

      CLoggerRecord(ESeverity, std::string const &);

      std::string writeRecord(bool ts, bool ss) const;
    };
    typedef std::shared_ptr<CLoggerRecord> PLoggerRecord;

    class CLogger
    {
      private:
        typedef std::shared_mutex             mutex_type;
        typedef std::unique_lock<mutex_type>  UniqueLock;
        typedef std::shared_lock<mutex_type>  SharedLock;
        //typedef std::shared_lock<mutex_type>  UpgradeLock;
        typedef std::vector<PLoggerSink> TSinkContainer;

        mutable mutex_type queueMutex;
        mutable mutex_type terminateMutex;
        bool terminateThread;
        mutable std::condition_variable_any cvQueueData;

        mutable mutex_type sinkMutex;
        TSinkContainer sinkContainer;

        std::unique_ptr<std::thread> writerThread;
        PLoggerSink defaultStreamSink_;                        ///< Stream sink created in constructor to ensure logger always works.

        std::queue<PLoggerRecord> messageQueue;

      protected:
        ESeverity logSeverity;

        virtual void writer();

      public:
        CLogger();
        virtual ~CLogger();

        virtual void addSink(PLoggerSink ls);
        virtual bool removeSink(PLoggerSink ls);

        CLoggerSink *defaultStreamSink() { return defaultStreamSink_.get(); }
        void removeDefaultStreamSink();

        virtual void logMessage(ESeverity, std::string const &);
        virtual void logMessage(ESeverity s, boost::format const &m) { logMessage(s, boost::str(m)); }

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

      // Some inline functions to simplify life

    /// @brief Function to log a message.
    /// @param[in] severity: The severity of the message.
    /// @param[in] message: The message to log.
    /// @throws
    /// @version 2020-06-13/GGB - Converted from macro to function.

    inline void LOGMESSAGE(ESeverity severity, std::string const &message)
    {
      defaultLogger().logMessage(severity, message);
    }

    /// @brief Function to log a critical message.
    /// @param[in] message: The message to log.
    /// @throws
    /// @version 2020-06-13/GGB - Converted from macro to function.

    inline void CRITICALMESSAGE(std::string const &message)
    {
      defaultLogger().logMessage(critical, message);
    }

    /// @brief Function to log a critical message.
    /// @param[in] logger: The logger to use for logging.
    /// @param[in] message: The message to log.
    /// @throws
    /// @version 2020-06-13/GGB - Converted from macro to function.

    inline void CRITICALMESSAGE(CLogger &logger, std::string const &message)
    {
      logger.logMessage(critical, message);
    }

    /// @brief      Function to log an error message.
    /// @param[in]  message: The message to log.
    /// @throws
    /// @version    2020-06-13/GGB - Converted from macro to function.

    inline void ERRORMESSAGE(std::string const &message)
    {
      defaultLogger().logMessage(error, message);
    }

    /// @brief      Function to log an error message.
    /// @param[in]  message: The message to log.
    /// @throws
    /// @version    2020-08-31/GGB - Converted from macro to function.

    inline void ERRORMESSAGE(boost::format const &message)
    {
      ERRORMESSAGE(boost::str(message));
    }

    /// @brief      Function to log an warning message.
    /// @param[in]  message: The message to log.
    /// @throws
    /// @version    2020-06-13/GGB - Converted from macro to function.

    inline void WARNINGMESSAGE(std::string const &message)
    {
      defaultLogger().logMessage(warning, message);
    }

    /// @brief Function to log a notice message.
    /// @param[in] message: The message to log.
    /// @throws
    /// @version 2020-06-13/GGB - Converted from macro to function.

    inline void NOTICEMESSAGE(std::string const &message)
    {
      defaultLogger().logMessage(notice, message);
    }

    /// @brief      Function to log an information message.
    /// @param[in]  message: The message to log.
    /// @throws
    /// @version    2020-06-13/GGB - Converted from macro to function.

    inline void INFOMESSAGE(std::string const &message)
    {
      defaultLogger().logMessage(info, message);
    }

    /// @brief      Function to log an information message.
    /// @param[in]  message: The message to log.
    /// @throws
    /// @version    2020-10-03/GGB - Converted from macro to function.

    inline void INFOMESSAGE(boost::format const &message)
    {
      INFOMESSAGE(boost::str(message));
    }

    /// @brief Function to log an debug message.
    /// @param[in] message: The message to log.
    /// @throws
    /// @version 2020-06-13/GGB - Converted from macro to function.

    inline void DEBUGMESSAGE(std::string const &message)
    {
      defaultLogger().logMessage(debug, message);
    }

    /// @brief      Function to log an debug message.
    /// @param[in]  message: The message to log.
    /// @throws
    /// @version    2020-09-08/GGB - Function created.

    [[deprecated]]
    inline void DEBUGMESSAGE(boost::format const &message)
    {
      DEBUGMESSAGE(boost::str(message));
    }

    /// @brief Function to log a trace function entry point.
    /// @param[in] message: The message to log.
    /// @throws
    /// @version 2020-06-13/GGB - Converted from macro to function.

    inline void TRACEMESSAGE(std::string const &message)
    {
      defaultLogger().logMessage(trace, message);
    }

    /// @brief Function to log an exception.
    /// @param[in] message: The message to log.
    /// @throws
    /// @version 2020-06-14/GGB - Converted from macro to function.

    inline void LOGEXCEPTION(std::string const &message)
    {
      defaultLogger().logMessage(exception, message);
    }

    /* Note the following two macros cannot be changed as they pick up the name of the function on entry and exit. This can
     * AFAIK only be done with the macro compiler and not the standard compiler. */

#define TRACEENTER GCL::logger::defaultLogger().logMessage(GCL::logger::trace, "Entering Function: " + std::string(__PRETTY_FUNCTION__) + ". File: " + std::string(__FILE__) + ". Line: " + std::to_string(__LINE__))
#define TRACEEXIT GCL::logger::defaultLogger().logMessage(GCL::logger::trace, "Exiting Function: " + std::string(__PRETTY_FUNCTION__) + ". File: " + std::string(__FILE__) + ". Line: " + std::to_string(__LINE__))

  } // namespace logger
} // namespace GCL

#else // GCL_CONTROL
#define TRACEENTER
#define TRACEEXIT
#endif    // GCL_CONTROL


#endif // GCL_LOGGER_LOGGERCORE_H
