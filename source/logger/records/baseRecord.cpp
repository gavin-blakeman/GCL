//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                baseRecord.h
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
// CLASSES INCLUDED:    CBaseRecord
//
// HISTORY:             2024-02-05 GGB - Functions split from loggerCore into seperate files
//
//*********************************************************************************************************************************

#include "include/logger/records/baseRecord.h"

namespace GCL::logger
{
  /// @brief      Writes a record to stream.
  /// @param[in]  os: The stream to writ to.
  /// @param[in]  record: The record to write.
  /// @returns    The ostrean
  /// @throws
  /// @version    2024-02-06/GGB - Function created.

  std::ostream& operator<<(std::ostream& os, CBaseRecord const &record)
  {
    os << record.text();
    return os;
  }

} // namespace
