//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                baseQueue.h
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
// CLASSES INCLUDED:    CBaseQueue
//
// HISTORY:             2024-02-05 GGB - Functions split from loggerCore into seperate files
//
//*********************************************************************************************************************************

#include "include/logger/queues/baseQueue.h"


namespace GCL::logger
{

  /// @brief      Returns the next record for processing.
  /// @returns    Constant reference to the next record for processing.
  /// @throws
  /// @version    2024-02-06/GGB - Function created.

  CBaseRecord const &CBaseQueue::front() const
  {
    sharedLock sl(queueMutex);

    return processFront();
  }

  /// @brief      Pushes a record into the queue.
  /// @param[in]  r: The record to push. (The record is moved)
  /// @throws
  /// @version    2024-02-06/GGB - Function created.

  void CBaseQueue::push(std::unique_ptr<CBaseRecord> &&r)
  {
    uniqueLock ul(queueMutex);

    processPush(std::move(r));
  }

  /// @brief      Pops a record from the queue.
  /// @throws
  /// @version    2024-02-06/GGB - Function created.

  void CBaseQueue::pop()
  {
    uniqueLock ul(queueMutex);

    processPop();
  }

  bool CBaseQueue::empty() const noexcept
  {
    uniqueLock ul(queueMutex);

    return processEmpty();
  }


}
