//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                fileSink.h
// SUBSYSTEM:           Logging Library
// LANGUAGE:						C++
// TARGET OS:						None.
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2014-2024 Gavin Blakeman.
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
// OVERVIEW:            This file implements a logging class to be used to log messages to file.
//                      The class has been designed as a lightweight easy to use class.
//
// CLASSES INCLUDED:    CLogger
//
// HISTORY:             2024-02-05 GGB - Rewrite to a flexible approach to enable additional functionality to be added.
//                      2023-11-01 GGB - ADD deletion of unsed constructors. (Move and operator =)
//                      2015-09-22 GGB - AIRDAS 2015.09 release
//                      2014-12-25 GGB - Development of class for "Observatory Weather System - Service"
//
//*********************************************************************************************************************************

#ifndef GCL_LOGGER_SINKS_FILESINK_H
#define GCL_LOGGER_SINKS_FILESINK_H

  // Standard C++ library header files.

#include <cstdint>

  // GCL header files

#include "include/logger/records/baseRecord.h"
#include "include/logger/sinks/baseSink.h"
#include "include/logger/loggerCore.h"
#include "include/dateTime.h"

namespace GCL::logger
{
  class CFileSink : public CBaseSink
  {
  public:
    enum ERotationMethod
    {
      days,               ///< A new log file is created every rotationDays days
      size,               ///< A new log file is created once the old one is above a specified size.
      daily,              ///< A new log file is created every day. (includes date in filename)
      use                 ///< A new log file is created every use (includes date and time in filename)
    };

    CFileSink(std::filesystem::path const &, std::filesystem::path const &, std::filesystem::path const & =".log");
    virtual ~CFileSink();

    void openLogFile();

    void setLogFileName(std::string const &filePath, std::string const &fileName, std::string const &fileExt = ".log");
    void setRotationPolicySize(std::uint16_t copies, std::uintmax_t maxSize = 10 * 1024 * 1024);
    void setRotationPolicyDaily(std::uint16_t copies);
    void setRotationPolicyDays(std::uint16_t copies, std::uint16_t maxDays);
    CFileSink &setRotationPolicyUse(std::uint16_t copies);

  protected:
    std::ofstream logFile;

    virtual void rotateLogFile();
    virtual void writeRecord(CBaseRecord const &);

  private:
    CFileSink() = delete;
    CFileSink(CFileSink const &) = delete;
    CFileSink(CFileSink &&) = delete;
    CFileSink &operator=(CFileSink const &) = delete;
    CFileSink &operator=(CFileSink &&) = delete;

    std::filesystem::path logFilePath;
    std::filesystem::path logFileName;
    std::filesystem::path logFileExt;
    std::filesystem::path openLogFileName;

    // Daily based rotation variables

    date_t fileCreationDate ;

    ERotationMethod rotationMethod = daily;
    std::uint16_t maxAge = 7;                       ///< Maximum age of
    std::uint16_t maxCopies = 10;                   ///< Number of logfile copies to keep. +ve = number, 0 = none.
    std::uintmax_t rotationSize = 10 * 1024 * 1024; ///< Maximum allowable log file size
    std::uint16_t rotationDays = 8;                 ///< Number of days between rotations.
    bool useUTC = true;                             ///< Use UTC for determining start of days.



    void rollFiles(void);

  };

}   // namespace

#endif // GCL_LOGGER_SINKS_FILESINK_H
