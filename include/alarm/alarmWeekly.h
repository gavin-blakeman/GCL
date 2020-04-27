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
//                      Copyright 2018-2020 Gavin Blakeman.
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

#ifndef ALARMWEEKLY
#define ALARMWEEKLY

  // Standard C++ Library headers

#include <chrono>
#include <ctime>
#include <vector>

  // GCL header files

#include "alarmCore.h"

namespace GCL
{
  namespace alarm
  {
    /// @brief A weekly alarm class.
    /// @details A weekly alarm allows alarms to be generated at a specific time of day over a selection of days of the week.
    ///          Thus it is possible to have an alarm generated at 06:00 every weekday, but not on weekends.

    class CAlarmWeekly : public CAlarmType
    {
    private:
      CAlarmWeekly() = delete;
      std::vector<bool> weekDays_;                ///< Array of days that the alarm applies to. 0-Sunday; 6-Saturday.
      std::tm alarmTime_;

    protected:
    public:
      CAlarmWeekly(callbackFunction_t, void *);
      CAlarmWeekly(callbackFunction_t, void *, std::vector<bool> const &, std::tm const &);
      explicit CAlarmWeekly(CAlarmWeekly const &);

      virtual void evaluateAlarm(std::tm const &);

    };

  } // namespace alarm
} // namespace GCL

#endif // ALARMWEEKLY

