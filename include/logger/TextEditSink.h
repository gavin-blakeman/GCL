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

#ifndef GCL_TEXTEDITSINK_H
#define GCL_TEXTEDITSINK_H

#include "LoggerCore.h"

#include "Qt/qtextedit.h"

namespace GCL
{
  namespace logger
  {

    /// @brief Logger sink that can be connected to a text edit in order to display text from a logger.
    /// @note This class requires Qt to be included in the compile chain.

    class CTextEditSink : public CLoggerSink
    {
      private:
        QTextEdit *textEdit;

      protected:
        virtual void write(std::string const &);

      public:
        CTextEditSink(QTextEdit *);
        virtual ~CTextEditSink() {}
    };

  }   // namespace logger
}   // namespace GCL

#endif // GCL_TEXTEDITSINK_H
