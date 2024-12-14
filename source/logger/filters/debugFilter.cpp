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

  // Standard C++ libraries
#include <iostream>

// Miscellaneous libraries
#include <fmt/format.h>
#include <fmt/chrono.h>
#include <fmt/std.h>

// GCL header files

#include "include/logger/records/debugRecord.h"

namespace GCL::logger
{
  /// @brief      Class constructor.
  /// @param[in]  cmp: The map of criticalities and text.
  /// @throws
  /// @version    2024-02-06/GGB - Function created.

  CDebugFilter::CDebugFilter(criticalityMap_t const &cm) : criticalityMap(cm.begin(), cm.end())
  {
  }

  /// @brief      Class constructor.
  /// @param[in]  cmp: The map of criticalities and text.
  /// @throws
  /// @version    2024-02-06/GGB - Function created.

  CDebugFilter::CDebugFilter(criticalityMap_t &&cmp) : criticalityMap(std::move(cmp))
  {
  }

  /// @brief      Class constructor.
  /// @param[in]  cmp: The map of criticalities and text.
  /// @param[in]  cmk: The set of criticalities to process
  /// @throws
  /// @version    2024-02-06/GGB - Function created.

  CDebugFilter::CDebugFilter(criticalityMap_t const &cmp, criticalityMask_t const &cm)
    : criticalityMap(cmp.begin(), cmp.end()), criticalityMask(cm.begin(), cm.end())
  {

  }

  /// @brief      Class constructor.
  /// @param[in]  cmp: The map of criticalities and text.
  /// @param[in]  cmk: The set of criticalities to process
  /// @throws
  /// @version    2024-02-06/GGB - Function created.

  CDebugFilter::CDebugFilter(criticalityMap_t &&cmp, criticalityMask_t &&cmk)
      : criticalityMap(std::move(cmp)), criticalityMask(std::move(cmk))
  {
  }

  /// @brief      Adds tests to the mask.
  /// @param[in]  toAdd: The mask set to add
  /// @throws
  /// @version    2024-02-06/GGB - Function created.

  void CDebugFilter::addMask(criticalityMask_t &&toAdd)
  {
    criticalityMask.merge(std::move(toAdd));
  }

  /// @brief      Create the record string.
  /// @param[in]  record: The record to process.
  /// @returns    The text to output.
  /// @throws
  /// @version    2024-02-06/GGB - Function created.

  std::optional<std::string> CDebugFilter::processRecordString(CBaseRecord const &record)
  {
    std::optional<std::string> returnValue;

    try
    {
      CDebugRecord const &debugRecord = dynamic_cast<CDebugRecord const &>(record);

      if (criticalityMask.contains(debugRecord.severity()))
      {
        std::string rv = fmt::format("{:%Y-%m-%d %H:%M:%S} [{:s}] {:s}",
                                     debugRecord.timeStamp().dateTime(),
                                     criticalityMap.at(debugRecord.severity()),
                                     record.text());
        returnValue = rv;
      }
    }
    catch(std::bad_cast &w)
    {
      std::cerr << "Bad Cast in CDebugFilter::processRecordString: " << w.what() << std::endl;
    }
    catch(...)
    {
      std::cerr << "General Exception: " << std::endl;
    }

    return returnValue;
  }
} // namespace
