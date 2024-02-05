//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                debugRecord.h
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
// OVERVIEW:
//
// CLASSES INCLUDED:    CDebugRecord
//
// HISTORY:             2024-02-05 GGB - Functions split from loggerCore into separate files
//
//*********************************************************************************************************************************

#include "include/logger/records/debugRecord.h"

namespace GCL::logger
{

  /// @brief      Constructor for the class.
  /// @param[in]  s: The severity of the message.
  /// @param[in]  t: The message to be logged.
  /// @throws     None.
  /// @version    2014-07-20/GGB - Function created.

  CDebugRecord::CDebugRecord(ESeverity s, std::string const &t)
  {
    data_.emplace()
    timeStamp = std::chrono::system_clock::now();
  }

  /// @brief Writes the logger message to the log file.
  /// @param[in] ts: timestamp
  /// @param[in] ss: message string.
  /// @returns A std::string containing the combined timestamp and string.
  /// @throws None.
  /// @version 2020-06-14/GGB - Added [exception] for exception.
  /// @version 2020-04-26/GGB - [Information] changed to [info]
  /// @version 2019-10-23/GGB - Updated to use std::chrono.
  /// @version 2014-07-21/GGB - Function created.

  std::string CDebugRecord::writeRecord(bool ts, bool ss) const
  {
    std::ostringstream os;

    SharedLock recordMutex;

    if (ts)
    {
      std::time_t now_c = std::chrono::system_clock::to_time_t(timeStamp);
      os << "[" << std::put_time(std::localtime(&now_c), "%F %T") << "] ";
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
          os << "[info] ";
          break;
        };
        case debug:
        {
          os << "[debug] ";
          break;
        };
        case exception:
        {
          os << "[exception] ";
          break;
        }
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
} // namespace
