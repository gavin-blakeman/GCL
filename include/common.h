﻿//*********************************************************************************************************************************
//
// PROJECT:							General Class Library
// FILE:								common
// SUBSYSTEM:						Common definitions
// LANGUAGE:						C++
// TARGET OS:						None.
// LIBRARY DEPENDANCE:	None.
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2015-2018 Gavin Blakeman.
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
//
// OVERVIEW:						Common definitions that can be used by multiple files
//
//
// CLASSES INCLUDED:		None
//
// CLASS HIERARCHY:     None.
//
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2015-06-29 GGB - File created.
//
//*********************************************************************************************************************************

#ifndef GCL_COMMON_H
#define GCL_COMMON_H

#include <string>

namespace GCL
{
  std::string const GCL_LIBNAME       = "GCL";

  std::string getVersionString();
}

#endif // GCL_COMMON_H
