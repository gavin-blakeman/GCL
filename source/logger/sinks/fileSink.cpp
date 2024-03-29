﻿//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                fileSink.cpp
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
//                      2023-11-16 GGB - Removed useage of boost::format.
//                      2018-01-27 GGB - Removed all compression support. Do not see any way to make this truly system independent.
//                      2018-01-26 GGB - Remove CFileSink default constructor.
//                      2015-09-22 GGB - AIRDAS 2015.09 release
//                      2014-12-25 GGB - Development of class for "Observatory Weather System - Service"
//
//*********************************************************************************************************************************

#include "include/logger/sinks/fileSink.h"

// Standard C++ library header files

#include <iostream>
#include <cmath>

// Miscellaneous library header files

#include "boost/locale.hpp"
#include <fmt/format.h>
#include <fmt/chrono.h>

// GCL Library header files.

#include "include/common.h"
#include "include/error.h"

namespace GCL::logger
{
  //******************************************************************************************************************************
  //
  // CFileLogger
  //
  //******************************************************************************************************************************

  /// @brief      Constructor for the class.
  /// @param[in]  filt: The filter to associate with the sink.
  /// @param[in]  lfp: Log file path.
  /// @param[in]  lfn: Log file name.
  /// @param[in]  lfe: Log File Extension. The extension to use. <".log">
  /// @throws     None.
  /// @version    2022-07-10/GGB - Changed boost::filesystem to std::filesystem
  /// @version    2018-08-18/GGB - Split log file name over three variables.
  /// @version    2017-01-26/GGB - Use a single variable for storing the path and name.
  /// @version    2014-07-22/GGB - Function created.

  CFileSink::CFileSink(std::shared_ptr<CBaseFilter> filt, std::filesystem::path const &lfp, std::filesystem::path const &lfn, std::filesystem::path const &lfe)
      : CBaseSink(std::move(filt)), logFilePath(lfp), logFileName(lfn), logFileExt(lfe), logFile()
  {
  }

  /// @brief      Destructor for the class. Only needs to close the file. The parent takes care of the closing of the thread.
  /// @throws     None.
  /// @version    2014-07-22/GGB - Function created.

  CFileSink::~CFileSink()
  {
    if ( logFile.is_open() )
    {
      logFile.close();
    };
  }

  /// @brief    Creates and opens the log file.
  /// @throws   0x1000 - LOGGER: Unable to open log file.
  /// @version  2018-01-18/GGB - Added class member logFileSize.
  /// @version  2014-07-22/GGB - Function created.

  void CFileSink::openLogFile()
  {
    std::filesystem::path logFileFullName = logFilePath / logFileName;

    switch (rotationMethod)
    {
      case days:
      {
        break;
      };
      case daily:
      {
        date_t currentDate;   // Default initialises to now.

        logFileFullName += fmt::format("{:%Y%m%d}", currentDate.date());
        logFileFullName += logFileExt;
        break;
      };
      case size:
      {
        logFileFullName += logFileExt;
        break;
      }
      case use:
      {
        logFileFullName += logFileExt;

        if ( std::filesystem::exists(logFileFullName) )
        {
          // Need to roll files.

          rollFiles();
        };

        break;
      };
      default:
      {
        CODE_ERROR();
        break;
      }
    };

    logFile.open(logFileFullName.string(), std::ios_base::out | std::ios_base::app);

    if (!logFile.good() || !logFile.is_open())
    {
      RUNTIME_ERROR(boost::locale::translate("LOGGER: Unable to open log file."));
    }
    else
    {
      openLogFileName = logFileFullName;
    };
  }

  /// @brief      Function to roll the files. IE move the files down the numbering order by 1.
  /// @pre        1. The variable "logFileFullName" needs to be fully assigned.
  /// @throws
  /// @version    2023-11-16/GGB - Removed use of boost::format.
  /// @version    2018-01-26/GGB - Function created.

  void CFileSink::rollFiles(void)
  {
    std::filesystem::path logFileFullName = logFilePath / logFileName;
    logFileFullName += logFileExt;

    if (maxCopies > 0)
    {
      std::uint16_t copyIndex = maxCopies;
      std::uint8_t digits = static_cast<std::uint8_t>(std::log10(maxCopies) + 1);     // Number of digits in copy number.
      std::string number;
      std::filesystem::path fnOld;
      std::filesystem::path fnNew;

      // Delete the last file if necessary.

      number = fmt::format("{:2d}", copyIndex);
      fnNew = openLogFileName;
      fnNew += "." + number;

      if (std::filesystem::exists(fnNew) )
      {
        std::filesystem::remove(fnNew);
      };

      copyIndex--;

      // rename all files.

      for (; copyIndex > 0; copyIndex--)
      {
        number = fmt::format("{:2d}", copyIndex);
        fnOld = logFileFullName;
        fnOld += "." + number;

        if (std::filesystem::exists(fnOld) )
        {
          std::filesystem::rename(fnOld, fnNew);
        };

        fnNew = fnOld;
      };

      // Now rename the base file to fnNew

      if (std::filesystem::exists(logFileFullName))
      {
        std::filesystem::rename(logFileFullName, fnNew);
      };
    }
    else
    {
      if ( std::filesystem::exists(logFileFullName) )
      {
        std::filesystem::remove(logFileFullName);
      };
    }
  }

  /// @brief Rotates the logfile.
  /// @throws None.
  /// @version 2014-12-21/GGB - Function created.

  void CFileSink::rotateLogFile()
  {
    bool reopen = false;

    // If the file is open close it and record that it needs to be reopened.

    if (logFile.is_open())
    {
      reopen = true;
      logFile.close();
    };

    // Now roll the logfiles.

    rollFiles();

    // Reopen the log file if required.

    if (reopen)
    {
      openLogFile();
    }
  }

  /// @brief Function to change the path and file name that the logger writes logs to.
  /// @param[in] filePath - The new file path.
  /// @param[in] fileName - The new logfile name.
  /// @param[in] fileExt - The new logfile extension.
  /// @throws None.
  /// @details If the log file is already open, it is closed and then reopened.
  /// @version 2014-12-21/GGB - Function created.

  void CFileSink::setLogFileName(std::string const &filePath, std::string const &fileName, std::string const &fileExt)
  {
    bool reOpen = false;

    // If the file is open close it and record that it needs to be reopened.

    if (logFile.is_open())
    {
      reOpen = true;
      logFile.close();
    };

    logFilePath = filePath;
    logFileName = fileName;
    logFileExt = fileExt;

    // Reopen the log file if required.

    if (reOpen)
    {
      openLogFile();
    }
  }

  /// @brief      Function to set the rotation policy to a filesize rotation policy.
  /// @param[in]  copies - The maximum number of log files to keep.
  /// @param[in]  maxSize - The maximum allowed size of a log file
  /// @throws     None.
  /// @version    2018-01-26/GGB - Removed all compression support
  /// @version    2018-01-19/GGB - Function created.

  void CFileSink::setRotationPolicySize(std::uint16_t copies, std::uintmax_t maxSize)
  {
    rotationMethod = size;
    maxCopies = copies;
    rotationSize = maxSize;
  }

  /// @brief Function to set a daily rotation policy.
  /// @param[in] copies - The maximum number of log files to keep.
  /// @throws None.
  /// @version 2018-01-26/GGB - Removed all compression support
  /// @version 2018-01-19/GGB - Function created.

  void CFileSink::setRotationPolicyDaily(std::uint16_t copies)
  {
    rotationMethod = daily;
    maxCopies = copies;
  }

  /// @brief      Sets the rotation policy to a maximum number of days.'
  /// @param[in]  copies: The number of copies of the log file to keep.
  /// @param[in]  maxDays: The number of days before rotation.
  /// @throws     None.
  /// @version    2018-01-26/GGB - Removed all compression support
  /// @version    2018-01-19/GGB - Function created.

  void CFileSink::setRotationPolicyDays(std::uint16_t copies, std::uint16_t maxDays)
  {
    rotationMethod = days;
    maxCopies = copies;
    rotationDays = maxDays;
  }

  /// @brief      Sets the rotation policy to a new one every startup (use) of the application.
  /// @param[in]  copies: The number of copies of the log file to keep.
  /// @returns    *this
  /// @throws     None.
  /// @version    2022-12-01/GGB - Changed return type.
  /// @version    2018-01-26/GGB - Removed all compression support
  /// @version    2018-01-19/GGB - Function created.

  CFileSink &CFileSink::setRotationPolicyUse(std::uint16_t copies)
  {
    rotationMethod = use;
    maxCopies = copies;

    return *this;
  }

  /// @brief      Function to write the message to the logFile.
  /// @param[in]  record: The record to write.
  /// @throws     None.
  /// @version    2015-06-01/GGB - Added functionality to rotate based on date.
  /// @version    2014-12-24/GGB - Function created.

  void CFileSink::writeRecord(CBaseRecord const &record)
  {
    if (!logFile.is_open())
    {
      openLogFile();
    };

    if (rotationMethod == daily)
    {
      std::time_t currentTime;
    };

    std::optional<std::string> os = filter_->recordString(record);

    if (os)
    {
      logFile << *os << std::endl;
    };

    if (rotationMethod == size)
    {
      std::uintmax_t fileSize = std::filesystem::file_size(openLogFileName);
      if (fileSize >= rotationSize)
      {
        rotateLogFile();
      };
    }
    else if (rotationMethod == days)
    {

    };
  }

} // namespace
