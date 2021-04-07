//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                alarmCore
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
// OVERVIEW:            This file implements an alarm class.
//
// CLASSES INCLUDED:
//
// HISTORY:             2018-07-06 GGB - File created
//
//*********************************************************************************************************************************

#include "include/alarm/alarmCore.h"

  // Miscellaneous library header files.

#include "include/logger/loggerCore.h"

namespace GCL
{
  namespace alarm
  {

    using GCL::logger::CRITICALMESSAGE;

    /// @brief Constructor for the CAlarmType class.
    /// @param[in[ tz: The time zone to use for the alarm. (+- hours)
    /// @param[in] callbackFunction: The callback function for the alarm.
    /// @param[in] callbackData: The callback data to use with the callback function.
    /// @throws None.
    /// @version 2018-08-11/GGB - Function created.

    CAlarmType::CAlarmType(timezone_t tz, callbackFunction_t callbackFunction, void *callbackData) :
      alarmHandle_(0), callbackFunction_(callbackFunction), callbackData_(callbackData), timeZone_(tz)
    {

    }

    //******************************************************************************************************************************
    //
    // CAlarmCore
    //
    //******************************************************************************************************************************

    /// @brief Constructor for the alarmCore class.
    /// @details Setup the thread to perform the alarm monitoring.
    /// @throws std::runtime_error.
    /// @version 2018-07-06/GGB - Function created.

    CAlarmCore::CAlarmCore()
    {
      alarmThread_ = new std::thread(&CAlarmCore::alarmLoop, this);

      if (!alarmThread_)
      {
        CRITICALMESSAGE("Unable to create alarm thread.");
        throw std::runtime_error("GCL::CAlarmCore::CAlarmCore: Unable to create alarm Thread.");
      };
    }

    /// @brief Destructor for the class.
    /// @throws None.
    /// @version 2018-07-06/GGB - Function created.

    CAlarmCore::~CAlarmCore()
    {
      if (alarmLoopRunning)
      {
        shutDown();
      };
    }

    /// @brief Adds a new alarm to the alarm list.
    /// @param[in] newAlarm: The new alarm to add to the list.
    /// @returns The alarmHandle of the new alarm added.
    /// @throws None.
    /// @version 2018-07-06/GGB - Function created.

    std::pair<alarmHandle_t, CAlarmType *>  CAlarmCore::addAlarm(std::unique_ptr<CAlarmType> newAlarm)
    {
      alarmContainer_.emplace_back(std::move(newAlarm));
      alarmContainer_.back()->alarmHandle(++lastAlarmHandle);
      alarmContainer_.back()->alarmCore(this);

      //return lastAlarmHandle;
    }

    /// @brief The loop function to evaluate whether alarms are required. This is the thread function.
    /// @throws None.
    /// @version 2018-07-07/GGB - Function created.

    void CAlarmCore::alarmLoop()
    {
      bool mustTerminate = false;
      std::tm currentTime;
      std::time_t time;

      while (!mustTerminate)
      {
        std::this_thread::sleep_for(std::chrono::seconds(60));     // Wake up once per minute.

        time = std::time(nullptr);
        if (localTime_)
        {
          currentTime = *std::localtime(&time);
        }
        else
        {
          currentTime = *std::gmtime(&time);
        };

          // Run over each of the alarms and call callback functions as required.

        for (auto &alarm: alarmContainer_)
        {
          alarm->evaluateAlarm(currentTime);
        };

        {
          SharedLock lock(terminateMutex);
          mustTerminate = terminateThread_;
        };

      };    // End while loop.
    }

    /// @brief Hashes a date to allow storage in an unordered set.
    /// @param[in] date: The date to hash.
    /// @throws None.
    /// @note 1: Not all hashes are valid dates, but all valid dates have a unique hash.
    /// @note 2: The hash can be reveresed.
    /// @version 2018-07-07/GGB - Function created.

    CAlarmCore::dateHash_t CAlarmCore::dateHash(std::tm const &date) const noexcept
    {
      dateHash_t dateHash;

      dateHash = date.tm_mday;
      dateHash += date.tm_mon * 32;
      dateHash += date.tm_year * (32 * 12);

      return dateHash;
    }

    /// @brief Checks if a date is a public holiday.
    /// @param[in] date: The date to check.
    /// @returns true - Date is a public holiday.
    /// @returns false - Date is not a public holiday.
    /// @throws None.
    /// @version 2018-07-08/GGB - Function created.

    bool CAlarmCore::isPublicHoliday(std::tm const &date) const
    {
      dateHash_t dh = dateHash(date);
      bool returnValue = false;

      if (publicHolidays_.find(dh) != publicHolidays_.end())
      {
        returnValue = true;
      };

      return returnValue;
    }

    /// @brief Shuts down the alarmLoop. Waits for the thread to join before shutting down.
    /// @throws None.
    /// @note 1: If the thread does not shut down, this is a potential deadlock.
    /// @note 2: It may be better to use boost::threads here as there is better functionality.
    /// @version 2018-07-07/GGB - Function created.

    void CAlarmCore::shutDown()
    {
      {
        UniqueLock writeLock(terminateMutex);
        terminateThread_ = true;
      };

      alarmThread_->join();
    }


  }   // namespace alarm
}   // namespace GCL.
