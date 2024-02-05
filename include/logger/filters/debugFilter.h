//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                debugFilter.h
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

#ifndef GCL_LOGGER_FILTERS_DEBUGFILTER_H
#define GCL_LOGGER_FILTERS_DEBUGFILTER_H

// Standard C++ header files

#include <cstdint>
#include <initializer_list>
#include <map>
#include <set>

// GCL header files

#include "include/logger/filters/baseFilter.h"
#include "include/logger/records/baseRecord.h"

namespace GCL::logger
{
  class CDebugFilter : public CBaseFilter
  {
  public:
    using criticality_t = std::uint32_t;
    using criticalityMap_t = std::map<criticality_t, std::string>;
    using criticalityMask_t = std::set<criticality_t>;

    CDebugFilter(criticalityMap_t &&, criticalityMask_t &&);

    void clearMask() noexcept { criticalityMask.clear(); }
    void addMask(std::initializer_list<criticality_t>);

  private:
    CDebugFilter() = delete;
    CDebugFilter(CDebugFilter const &) = delete;
    CDebugFilter(CDebugFilter &&) = delete;
    CDebugFilter &operator=(CDebugFilter const &) = delete;
    CDebugFilter &operator=(CDebugFilter &&) = delete;

    criticalityMap_t const criticalityMap;
    criticalityMask_t criticalityMask;

    virtual std::optional<std::string> processRecordString(CBaseRecord const *) = 0;
  };
} // namespace

#endif //GCL_LOGGER_FILTERS_DEBUGFILTER_H
