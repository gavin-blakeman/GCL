//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                debugFilter.cpp
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

#include "include/logger/filters/debugFilter.h"

namespace GCL::logger
{

  CDebugFilter::CDebugFilter(criticalityMap_t &&cmp, criticalityMask_t &&cmk)
  : criticalityMap(std::move(cmp)), criticalityMask(std::move(cmk))
  {
  }

  std::optional<std::string> CDebugFilter::processRecordString(CBaseRecord const *record)
  {
    std::optional<std::string> rv;

    if (record.contains("Criticality") and criticalityMask.contains(record.get<criticality_t>("Criticality")))
    {
      rv = fmt::format("{:%Y-%m-%d-%HH:MM:ss.sss"}, record.get<dateTime_t>("TimeStamp"));
      rv += " [" + criticalityMap.at(record.get<criticality_t>("Criticality")) "] ";
      rv += record.get<std::string>("Text");
    }

  return rv;
}
} // namespace
