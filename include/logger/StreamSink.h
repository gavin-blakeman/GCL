//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                CStreamSink
// SUBSYSTEM:           Logging Library
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	boost::chrono
//                      boost::filesystem
//                      boost::thread
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

#ifndef STREAMSINK_H
#define STREAMSINK_H

#ifndef GCL_CONTROL

#include "LoggerCore.h"

#include <ostream>

namespace GCL
{
  namespace logger
  {

    class CStreamSink: public CLoggerSink
    {
      private:
        std::ostream &outputStream;

      protected:
      public:
        CStreamSink(std::ostream &);
        virtual ~CStreamSink() {}
        virtual void write(std::string const &);
    };

  }   // namespace logger
}   // namespace GCL

#endif // GCL_CONTROL

#endif // STREAMSINK_H
