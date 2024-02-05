//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                baseSink.h
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

#ifndef GCL_LOGGER_LOGGERSINK_H
#define GCL_LOGGER_LOGGERSINK_H

// GCL header files

#include "include/logger/filters/baseFilter.h"
#include "include/logger/records/baseRecord.h"

namespace GCL::logger
{
  /* Template type is the filter to be used with the sink. */

  class CBaseSink
  {
  public:
    CBaseSink() = default;
    virtual ~CBaseSink() = default;

    virtual void writeRecord(CBaseRecord const *) = 0;
    CBaseFilter *filter() { return filter_.get(); }

  protected:
    T filter_;

  private:
    CBaseSink(CBaseSink const &) = delete;
    CBaseSink(CBaseSink &&) = delete;
    CBaseSink &operator=(CBaseSink const &) = delete;
    CBaseSink &operator=(CBaseSink &&) = delete;

  };
} // namespace

#endif // GCL_LOGGER_LOGGERSINK_H
