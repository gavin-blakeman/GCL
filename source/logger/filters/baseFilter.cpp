//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                baseFilter.h
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

#include "include/logger/filters/baseFilter.h"

// Standard C++ library header files
#include <iostream>

namespace GCL::logger
{
  /// @brief      Basic filter string
  /// @param[in]  record: The record to output.
  /// @returns    The record text.
  /// @throws
  /// @version    2024-02-06/GGB - Function created.

  std::optional<std::string> CBaseFilter::processRecordString(CBaseRecord const &record)
  {
    std::optional<std::string> returnValue = record.text();

    return returnValue;
  }
} // namespace
