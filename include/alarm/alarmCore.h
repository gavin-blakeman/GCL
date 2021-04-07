//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                alarmCore
// SUBSYSTEM:           Alarm Module
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
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

#ifndef GCL_ALARMCORE
#define GCL_ALARMCORE

  // C++ Library include files.

#include <chrono>
#include <cstdint>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_set>
#include <utility>

  // Miscellaneous library header files.


namespace GCL
{
  namespace alarm
  {
    typedef std::uint32_t alarmHandle_t;
    typedef std::function<void(alarmHandle_t, void *)> callbackFunction_t;      // first parameter is the alarmHandle.
                                                                                // The second is user defined data.
    using timezone_t = std::int8_t;

    class CAlarmCore;

    class CAlarmType
    {
    private:
      CAlarmCore *alarmCore_ = nullptr;
      alarmHandle_t alarmHandle_;
      callbackFunction_t callbackFunction_;
      void *callbackData_;
      bool publicHolidayDisable_ = true;
      timezone_t timeZone_;

        // Remove the undesirable constructors.

      CAlarmType() = delete;
      CAlarmType(CAlarmType const &) = delete;
      CAlarmType(CAlarmType &&) = delete;
      CAlarmType &operator =(CAlarmType const &) = delete;

    protected:
      virtual void executeCallback() const { callbackFunction_(alarmHandle_, callbackData_); }

    public:
      CAlarmType(timezone_t, callbackFunction_t callbackFunction, void *callbackData);
      virtual ~CAlarmType() {}

      void alarmStart() {}
      void alarmStop() {}

      void alarmHandle(alarmHandle_t alarmHandle) { alarmHandle_ = alarmHandle; }
      alarmHandle_t alarmHandle() const noexcept { return alarmHandle_; }

      void alarmCore(CAlarmCore *alarmCore) { alarmCore_ = alarmCore; }
      CAlarmCore const *alarmCore() const { return alarmCore_; }

      void publicHolidayDisable(bool phd) { publicHolidayDisable_ = phd; }
      bool publicHolidayDisable() const noexcept { return publicHolidayDisable_; }

      void timeZone(std::int8_t tz) { timeZone_ = tz; }
      std::int8_t timeZone() { return timeZone_; }

      virtual void evaluateAlarm(std::tm const &) = 0;

    };

    class CAlarmCore
    {
    private:
      typedef std::uint64_t dateHash_t;
      typedef std::shared_mutex             mutex_type;
      typedef std::unique_lock<mutex_type>  UniqueLock;
      typedef std::shared_lock<mutex_type>  SharedLock;

      typedef std::list<std::unique_ptr<CAlarmType>> TAlarmContainer;

      alarmHandle_t lastAlarmHandle = 0;                  ///< The last alarm handle used.
      mutable mutex_type terminateMutex;
      bool terminateThread_ = false;
      TAlarmContainer alarmContainer_;
      std::thread *alarmThread_;
      bool alarmLoopRunning = false;
      bool localTime_ = true;
      std::unordered_set<dateHash_t> publicHolidays_;

      CAlarmCore(CAlarmCore const &) = delete;    // Remove potential for copy constructor.
      dateHash_t dateHash(std::tm const &) const noexcept;

    protected:
      void alarmLoop();

    public:
      CAlarmCore();
      ~CAlarmCore();

       std::pair<alarmHandle_t, CAlarmType *> addAlarm(std::unique_ptr<CAlarmType>);
       bool removeAlarm(alarmHandle_t);

       void stopAlarmCore();
       void startAlarmCore();

       void shutDown();

       void addPublicHoliday(std::tm const &ph) { publicHolidays_.insert(dateHash(ph)); }
       bool isPublicHoliday(std::tm const &) const;
    };


  }   // namespace alarm

}   // namespace GCL.

#endif // GCL_ALARMCORE

