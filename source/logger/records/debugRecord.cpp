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

  CDebugRecord::CDebugRecord(severity_t s, std::string const &t) : CBaseRecord(t), severity_(s), timeStamp_(std::chrono::system_clock::now())
  {
  }

} // namespace
