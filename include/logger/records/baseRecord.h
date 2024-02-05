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

#ifndef GCL_LOGGER_BASERECORD_H
#define GCL_LOGGER_BASERECORD_H

  // Standard C++ library header

#include <any>
#include <chrono>
#include <string>

namespace GCL::logger
{
  class CBaseRecord
  {
  public:
    CBaseRecord() = default;
    virtual ~CBaseRecord() = default;

    virtual std::string to_string(std::string const &) const noexcept = 0;

  private:
    CBaseRecord(CBaseRecord const &) = delete;
    CBaseRecord(CBaseRecord &&) = delete;
    CBaseRecord &operator=(CBaseRecord const &) = delete;
    CBaseRecord &operator=(CBaseRecord &&) = delete;
  };
} // namespace

#endif // GCL_LOGGER_BASERECORD_H
