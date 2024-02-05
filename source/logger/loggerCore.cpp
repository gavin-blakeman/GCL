//*********************************************************************************************************************************
//
// PROJECT:							General Class Library
// FILE:								loggerCore.cpp
// SUBSYSTEM:						Logging Library
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
// HISTORY:             2018-08-12 GGB - gnuCash-pud debugging and release.
//                      2015-09-22 GGB - AIRDAS 2015.09 release
//                      2014-07-20 GGB - Development of class for "Observatory Weather System - Service"
//
//*********************************************************************************************************************************

#include "include/logger/loggerCore.h"

  // Standard C++ library headers

#include <iomanip>
#include <iostream>

  // Miscellaneous library headers

#include <fmt/format.h>

  // GCL include headers

#include "include/logger/streamSink.h"

namespace GCL
{
  namespace logger
  {

    /// @brief      Return a reference to the default file logger.
    /// @throws     None.
    /// @version    2014-12-21/GGB - Function created.

    CLogger &defaultLogger()
    {
      static CLogger logger;
      return logger;
    }
  } // namespace logger
} // namespace GCL
