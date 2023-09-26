//*********************************************************************************************************************************
//
// PROJECT:							General Class Library
// FILE:								string.cpp
// SUBSYSTEM:						Database library
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	None.
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2013-2023 Gavin Blakeman.
//                      This file is part of the General Class Library (GCL)
//
//                      GCL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
//                      License as published by the Free Software Foundation, either version 2 of the License, or (at your option)
//                      any later version.
//
//                      GCL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
//                      warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
//                      more details.
//
//                      You should have received a copy of the GNU General Public License along with GCL.  If not,
//                      see <http://www.gnu.org/licenses/>.
//
// OVERVIEW:            This file provides string support that is missing in the windows API.
//
// CLASSES INCLUDED:    None
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-01-26 GGB - Development of class for application AIRDAS
//
//*********************************************************************************************************************************

// Added to enable functions

#include <string.h>

#ifdef _MSC_VER

int __cdecl strncasecmp(const char * _Str1, const char * _Str2, size_t _MaxCount)
{
  return strncmp(_Str1, _Str2, _MaxCount);
};

int __cdecl strcasecmp(const char *str1, const char *str2)
{
  size_t maxLen = strlen(str1);
  if (strlen(str2) > maxLen)
    maxLen = strlen(str2);

  return strncmp(str1, str2, maxLen);
};

#endif
