//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                alarmDaily
// SUBSYSTEM:           Alarm Module
// LANGUAGE:						C++
// TARGET OS:						None.
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
// CLASSES INCLUDED:
//
// HISTORY:             2018-07-06 GGB - File created
//
//*********************************************************************************************************************************

#include "include/alarm/alarmDaily.h"

namespace GCL
{
  namespace alarm
  {

    CAlarmDaily::CAlarmDaily(timezone_t tz, std::uint32_t, callbackFunction_t callbackFunction, void *callbackData) :
      CAlarmType(tz, callbackFunction, callbackData)
    {

    }

    void CAlarmDaily::evaluateAlarm()
    {
//      if (weekDays_[currentTime.tm_wday])
//      {
//          // The alarm is valid for this weekday.

//        if ( (alarmTime_.tm_hour == currentTime.tm_hour) &&
//             (alarmTime_.tm_min == currentTime.tm_min) &&
//             (alarmTime_.tm_sec == currentTime.tm_sec) )
//        {
//          if (publicHolidayDisable())
//          {
//            if (!alarmCore()->isPublicHoliday(currentTime))
//            {
//              executeCallback();
//            };
//          }
//          else
//          {
//            executeCallback();
//          }
//        };
//      };
    }

  } // namespace alarm
} // namespace GCL
