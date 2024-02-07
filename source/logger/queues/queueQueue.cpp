//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                queueQueue.h
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
// CLASSES INCLUDED:    CQueueQueue
//
// HISTORY:             2024-02-05 GGB - Functions split from loggerCore into seperate files
//
//*********************************************************************************************************************************

#include "include/logger/queues/queueQueue.h"

  // GCL header files

#include "include/error.h"

namespace GCL::logger
{

  /// @brief      Moves a new item into the queue
  /// @param[in]  r: The record to be moved to the queue.
  /// @throws
  /// @version    2024-02-06/GGB - Function created.

  void CQueueQueue::processPush(std::unique_ptr<CBaseRecord> &&r)
  {
    queue.push(std::move(r));
  }

  /// @brief      Pops the first item off the queue.
  /// @throws
  /// @version    2024-02-06/GGB - Function created.

  void CQueueQueue::processPop()
  {
    if (!queue.empty())
    {
      queue.pop();
    };
  }

  /// @brief      Determines if the queue is empty.
  /// @returns    true if the queue is empty.
  /// @throws
  /// @version    2024-02-06/GGB - Function created.

  bool CQueueQueue::processEmpty() const noexcept
  {
    return queue.empty();
  }

  /// @brief      Returns the front item in the queue.
  /// @returns    The first item in the queue.
  /// @throws
  /// @version    2024-02-06/GGB - Function created.

  CBaseRecord const &CQueueQueue::processFront() const
  {
    if (!queue.empty())
    {
      return *queue.front();
    }
    else
    {
      CODE_ERROR();
      // Does not return.
    }
  }


};  // namespace
