//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                TextEditSink
// SUBSYSTEM:           Logging Library
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	boost::chrono
//                      boost::filesystem
//                      boost::shared_ptr
//                      boost::thread
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2014-2016 Gavin Blakeman.
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
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2014-12-25 GGB - Development of class for "Observatory Weather System - Service"
//
//*********************************************************************************************************************************

#include "include/logger/TextEditSink.h"

#include "include/Error.h"

namespace GCL
{
  namespace logger
  {
    //******************************************************************************************************************************
    //
    // CTextEditSink
    //
    //******************************************************************************************************************************

    /// @brief Default constructor for class.
    /// @param[in] te - The text edit control used to display the log messages.
    /// @throws None.
    /// @version 2014-12-25/GGB - Function created.

    CTextEditSink::CTextEditSink(QTextEdit *te) : CLoggerSink()
    {
      textEdit = te;
    }

    /// @brief Writes the text to the device.
    /// @param[in] s - The log string to display.
    /// @throws GCL::CError(GCL, 0x1002) - LOGGER: Text Edit not assignd.
    /// @version 2014-12-25/GGB - Function created.

    void CTextEditSink::write(std::string const &s)
    {
      if (textEdit)
      {
        textEdit->append(QString::fromStdString(s));
        textEdit->repaint();
      }
      else
      {
        ERROR(GCL, 0x1002);     // LOGGER: Text Edit not assignd.
      };
    }

  }   // namespace logger
}   // namespace GCL
