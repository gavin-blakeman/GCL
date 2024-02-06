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

#ifndef GCL_LOGGER_QUEUES_QUEUEQUEUE_H
#define GCL_LOGGER_QUEUES_QUEUEQUEUE_H

// Standard C++ library

#include <queue>

// GCL header files

#include "include/logger/queues/baseQueue.h"

namespace GCL::logger
{
  class CQueueQueue : public CBaseQueue
  {
  public:
    CQueueQueue() = default;
    virtual ~CQueueQueue() = default;

  private:
    CQueueQueue(CQueueQueue const &) = delete;
    CQueueQueue(CQueueQueue &&) = delete;
    CQueueQueue &operator=(CQueueQueue const &) = delete;
    CQueueQueue &operator=(CQueueQueue &&) = delete;

    std::queue<std::unique_ptr<CBaseRecord>> queue;

    virtual CBaseRecord const &processFront() const override;
    virtual void processPush(std::unique_ptr<CBaseRecord> &&) override;
    virtual void processPop() override;
    virtual bool processEmpty() const noexcept override;
  };

}

#endif // GCL_LOGGER_QUEUES_QUEUEQUEUE_H
