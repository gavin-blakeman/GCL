//*********************************************************************************************************************************
//
// PROJECT:							General Class Library (GCL)
// FILE:								GCLError.h
// SUBSYSTEM:						Specific GCL error handling
// LANGUAGE:						C++
// TARGET OS:						None.
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2020-2023 Gavin Blakeman.
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
// OVERVIEW:            This file implements exceptions that are used for error reporting by the GCL library.
//
// CLASSES INCLUDED:    CGCLError
//
// HISTORY:             2020-09-20 GGB - File created
//
//*********************************************************************************************************************************

#ifndef GCLERROR_H
#define GCLERROR_H

#include "include/error.h"

namespace GCL
{
  TErrorCode const E_LOGGER_UNABLETOOPENFILE      = 0x0001;
  TErrorCode const E_LOGGER_UNABLETOSTARTTHREAD   = 0x0002;

  TErrorCode const E_SQLWRITER_UNKNOWNDIALECT     = 0x1000;
  TErrorCode const E_SQLWRITER_SYNTAXERROR        = 0x1001;
  TErrorCode const E_SQLWRITER_INVALIDCOMMAND     = 0x1002;
  TErrorCode const E_SQLWRITER_INVALIDTABLENAME   = 0x1003;
  TErrorCode const E_SQLWRITER_NOSELECTFIELDS     = 0x1004;
  TErrorCode const E_SQLWRITER_NOFROMFIELD        = 0x1005;

  TErrorCode const E_PLUGINMANAGER_NODIRECTORY    = 0x1100;
  TErrorCode const E_PLUGINMANAGER_UNABLETOMAP    = 0x1102;
  TErrorCode const E_PLUGINMANAGER_UNABLETOUNLOAD = 0x1103;
}

#endif // GCLERROR_H
