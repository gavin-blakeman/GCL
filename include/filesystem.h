//*********************************************************************************************************************************
//
// PROJECT:							General Class Library
// FILE:								filesystem
// SUBSYSTEM:						std::filesystem or boost::filesystem extensions.
// LANGUAGE:						C++
// TARGET OS:						None.
// LIBRARY DEPENDANCE:	boost::filesystem
// NAMESPACE:						boost::filesystem
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2018 Gavin Blakeman.
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
// OVERVIEW:						Some extension functions to boost::filesystem. When std::filesystem is in common use, this will be changed
//                      to support std::filesystem.
//                      Note: The functions defined are set in the boost::filesystem namespace.
//
// CLASSES INCLUDED:		None
//
// CLASS HIERARCHY:     None.
//
// HISTORY:             2018-05-20 GGB - File created.
//
//*********************************************************************************************************************************

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <boost/filesystem.hpp>

namespace boost
{
  namespace filesystem
  {

    bool file_readable(path const &);

  } // namespace filesystem
} // namespace boost

#endif // FILESYSTEM_H

