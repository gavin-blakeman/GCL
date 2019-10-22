//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                CStreamSink
// SUBSYSTEM:           Logging Library
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	boost::chrono
//                      boost::filesystem
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2014-2018 Gavin Blakeman.
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
// CLASSES INCLUDED:    CStreamSink
//
// CLASS HEIRARCHY:     CLoggerSink
//                        - CStreamSink
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2014-12-28 GGB - Development of class for "Observatory Weather System - Service"
//
//*********************************************************************************************************************************

#include "../../include/logger/StreamSink.h"

namespace GCL
{
  namespace logger
  {
    /// @brief Default class constructor.
    /// @param[in] os: The stream to associate with the sink.
    /// @version 2014-12-28/GGB - Function created.

    CStreamSink::CStreamSink(std::ostream &os) : CLoggerSink(), outputStream(os)
    {
    }

    /// @brief This is the function to write the message to the stream.
    /// @param[in] s: The string to write to the stream.
    /// @version 2014-12-28/GGB - Function created.

    void CStreamSink::write(std::string const &s)
    {
      outputStream << s << std::endl;
    }

  }   // namespace logger
}   // namespace GCL
