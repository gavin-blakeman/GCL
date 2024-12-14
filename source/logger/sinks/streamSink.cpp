//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                streamSink.cpp
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
// CLASSES INCLUDED:    CStreamSink
//
// CLASS HEIRARCHY:     CLoggerSink
//                        - CStreamSink
//
// HISTORY:             2024-02-05 GGB - Rewrite to a flexible approach to enable additional functionality to be added.
//                      2015-09-22 GGB - AIRDAS 2015.09 release
//                      2014-12-28 GGB - Development of class for "Observatory Weather System - Service"
//
//*********************************************************************************************************************************

#include "include/logger/sinks/streamSink.h"

// Standard C++ library header files
#include <iostream>

// GCL Header files

#include "include/logger/records/baseRecord.h"

namespace GCL::logger
{
  /// @brief      Default class constructor.
  /// @param[in]  os: The stream to associate with the sink.
  /// @version  2014-12-28/GGB - Function created.

  CStreamSink::CStreamSink(std::shared_ptr<CBaseFilter> filt, std::ostream &os) : CBaseSink(std::move(filt)), outputStream(os)
  {
  }

  /// @brief      This is the function to write the message to the stream.
  /// @param[in]  record: The record to write.
  /// @version    2024-02-06/GGB - Changed parameter to CBaseRecord.
  /// @version    2014-12-28/GGB - Function created.

  void CStreamSink::writeRecord(CBaseRecord const &record)
  {
    std::optional<std::string> os = filter_->recordString(record);

    if (os)
    {
      outputStream << *os << std::endl;
    };


  }

}   // namespace
