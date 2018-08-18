//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                alarmDaily
// SUBSYSTEM:           Alarm Module
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2018 Gavin Blakeman.
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

#include "../../include/alarm/alarmWeekly.h"

namespace GCL
{
  namespace alarm
  {

    /// @brief Constructor for a weekly alarm.
    /// @param[in] callbackFunction: The callback function to associate with the alarm.
    /// @param[in] callbackData: The callback data to use with the callback function.
    /// @throws None.
    /// @version 2018-08-11/GGB - Function created.

    CAlarmWeekly::CAlarmWeekly(callbackFunction_t callbackFunction, void *callbackData) : CAlarmType(callbackFunction, callbackData)
    {
    }

    /// @brief Constructor for a weekly alarm.
    /// @param[in] callbackFunction: The callback function to associate with the alarm.
    /// @param[in] callbackData: The callback data to use with the callback function.
    /// @param[in] weekDays: Array of weekdays that the alarm should apply to.
    /// @param[in] alarmTime: The time the alarm should occur.
    /// @throws None.
    /// @note 1: Only the hours, minutes and seconds fields of the std::tm are used.
    /// @version 2018-07-07/GGB - Function created.

    CAlarmWeekly::CAlarmWeekly(callbackFunction_t callbackFunction, void *callbackData,
                               std::vector<bool> const &weekDays, std::tm const &alarmTime) :
      CAlarmType(callbackFunction, callbackData), weekDays_(std::move(weekDays)), alarmTime_(std::move(alarmTime))
    {

    }

    /// @brief Evaluates the currentTime and determines if an alarm is required. The callback function is used if a callback is
    ///        required.
    /// @param[in] currentTime: The current time stored in a std::tm structure.
    /// @throws None.
    /// @version 2018-07-07/GGB - Function created.

    void CAlarmWeekly::evaluateAlarm(std::tm const &currentTime)
    {
      if (weekDays_[currentTime.tm_wday])
      {
          // The alarm is valid for this weekday.

        if ( (alarmTime_.tm_hour == currentTime.tm_hour) &&
             (alarmTime_.tm_min == currentTime.tm_min) &&
             (alarmTime_.tm_sec == currentTime.tm_sec) )
        {
          if (publicHolidayDisable())
          {
            if (!alarmCore()->isPublicHoliday(currentTime))
            {
              executeCallback();
            };
          }
          else
          {
            executeCallback();
          }
        };
      };
    }

  } // namespace alarm
} // namespace GCL




