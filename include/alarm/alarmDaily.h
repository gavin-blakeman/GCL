//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                alarmDaily
// SUBSYSTEM:           Alarm Module
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2018-2023 Gavin Blakeman.
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
// HISTORY:             2018-07-06 GGB - File created
//
//*********************************************************************************************************************************

#ifndef ALARMDAILY
#define ALARMDAILY

#include "alarmCore.h"

namespace GCL
{
  namespace alarm
  {
    class CAlarmDaily : public CAlarmType
    {
    public:
      CAlarmDaily(timezone_t, std::uint32_t, callbackFunction_t, void *callbackData);

      virtual void evaluateAlarm();

    protected:

    private:
      CAlarmDaily() = delete;
      CAlarmDaily(CAlarmDaily const &) = delete;
      CAlarmDaily(CAlarmDaily &&) = delete;
      CAlarmDaily &operator=(CAlarmDaily const &) = delete;
      CAlarmDaily &operator=(CAlarmDaily &&) = delete;

      std::chrono::time_point<std::chrono::system_clock> nextActivation;

    };

  } // namespace alarm
} // namespace GCL

#endif // ALARMDAILY

