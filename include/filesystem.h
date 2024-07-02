//*********************************************************************************************************************************
//
// PROJECT:							General Class Library
// FILE:								filesystem
// SUBSYSTEM:           filesystem extensions.
// LANGUAGE:						C++
// TARGET OS:						None.
// NAMESPACE:						boost::filesystem
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2018-2024 Gavin Blakeman.
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

  // Standard C++ library

#include <cstdint>
#include <filesystem>
#include <list>
#include <string_view>

namespace GCL
{
  using fileList_t = std::list<std::filesystem::path>;

  bool file_readable(std::filesystem::path const &);
  void expandFileList(std::string_view &&, fileList_t &);

  std::filesystem::path temporaryFilename(std::uint8_t = 10);
  std::filesystem::path temporaryFilename(std::filesystem::path const &, std::uint8_t = 10);
  
  /*! @brief      A function to expand a file name that has formatting characters build in. Formatting characters are similar
   *              to the use of std::format. The formatting characters are between braces {}. 
   *  @details    The characters recognised are:
   *              %Y - The current year (4 digits)
   *              %m - The current month (2 digits)
   *              %d - The current day. (2 digits)
   *              An example would be "Report {:%Y-%m-%d}". This would create a filepath something like "Report 2024-02-01"
   *  @param[in]  formatString: The formatString
   *  @returns    An expanded filesystem path.
   *  @throws     noexcept. Any errors are ignored. In the worst case, the formatString will be returned.
   */
  std::filesystem::path expandFileName(std::string const &formatString) noexcept;
}

#endif // FILESYSTEM_H

