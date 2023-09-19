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
//                      Copyright 2018-2023 Gavin Blakeman.
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

#include "include/filesystem.h"

namespace std::filesystem
{
  /// @brief      Checks if the specified filename is readable. If the file does not exist, or there is any other error, the function
  ///             returns false.
  /// @param[in]  p: The filename (or path) to check.
  /// @returns    true - if the filename or path is readable.
  /// @returns    false - if the filename or path is not readable, or does not exist.
  /// @throws     None.
  /// @version    2023-09-19/GGB - Moved to std::filesystem.
  /// @version    2018-05-20/GGB - Function created.

  bool file_readable(path const &p)
  {
    bool returnValue = false;

    if (exists(p))
    {

      file_status fileStatus = status(p);

      perms permissions = fileStatus.permissions();

      returnValue = ((permissions & std::filesystem::perms::owner_read) |
                    (permissions & std::filesystem::perms::group_read) |
                    (permissions & std::filesystem::perms::others_read)) != std::filesystem::perms::none;
    };
    return returnValue;
  }

} // namespace filesystem
