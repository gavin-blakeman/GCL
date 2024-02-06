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
// HISTORY:             2024-02-05 GGB - Functions split from loggerCore into seperate files
//
//*********************************************************************************************************************************

#ifndef LOGGER_RECORDS_DEBUGRECORD_H
#define LOGGER_RECORDS_DEBUGRECORD_H

// Standard C++ library

#include <cstdint>

// GCL header files

#include "include/logger/records/baseRecord.h"
#include "include/dateTime.h"

namespace GCL::logger
{
  class CDebugRecord : public CBaseRecord
  {
  public:
    using severity_t = std::uint32_t;

    CDebugRecord(severity_t, std::string const &);
    virtual ~CDebugRecord() = default;

    severity_t severity() const noexcept { return severity_; }
    dateTime_t timeStamp() const noexcept { return timeStamp_; }

  private:
    CDebugRecord() = delete;
    CDebugRecord(CDebugRecord const &) = delete;
    CDebugRecord(CDebugRecord &&) = delete;
    CDebugRecord &operator=(CDebugRecord const &) = delete;
    CDebugRecord &operator=(CDebugRecord &&) = delete;

    severity_t severity_;
    dateTime_t timeStamp_;

  };
} // namespace

#endif // LOGGER_RECORDS_DEBUGRECORD_H
