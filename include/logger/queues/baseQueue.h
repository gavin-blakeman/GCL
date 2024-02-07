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

#ifndef GCL_LOGGER_QUEUES_BASEQUEUE_H
#define GCL_LOGGER_QUEUES_BASEQUEUE_H

  // Standard C++ library header files

#include <condition_variable>
#include <memory>
#include <mutex>
#include <shared_mutex>

  // Logger header files

#include "include/logger/records/baseRecord.h"

namespace GCL::logger
{
  class CBaseQueue
  {
    public:
      CBaseQueue() = default;
      virtual ~CBaseQueue() = default;

      CBaseRecord const &front() const;
      void push(std::unique_ptr<CBaseRecord> &&);
      void pop();

      bool empty() const noexcept;

    protected:
      using mutex_type = std::shared_mutex;
      using writeLock = std::unique_lock<mutex_type>;
      using readLock = std::unique_lock<mutex_type>;

      mutable mutex_type queueMutex;
      mutable std::condition_variable_any cvQueueData;

    private:
      CBaseQueue(CBaseQueue const &) = delete;
      CBaseQueue(CBaseQueue &&) = delete;
      CBaseQueue &operator=(CBaseQueue const &) = delete;
      CBaseQueue &operator=(CBaseQueue &&) = delete;

      virtual CBaseRecord const &processFront() const = 0;
      virtual void processPush(std::unique_ptr<CBaseRecord> &&) = 0;
      virtual void processPop() = 0;
      virtual bool processEmpty() const noexcept = 0;
  };
}

#endif // GCL_LOGGER_QUEUES_BASEQUEUE_H
