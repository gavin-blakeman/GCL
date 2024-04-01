//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                functionTrace.h
// SUBSYSTEM:           Database library
// LANGUAGE:            C++20
// TARGET OS:           None - Standard C++
// NAMESPACE:           GCL
// AUTHOR:              Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2024 Gavin Blakeman.
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
// OVERVIEW:
//
// CLASSES INCLUDED:
//
// HISTORY:             2024-03-30 GGB - File Created.
//
//*********************************************************************************************************************************

#include "include/functionTrace.h"

namespace GCL
{
  std::filesystem::path CFunctionTrace::outputPath;
  CFunctionTrace::fileMap_t CFunctionTrace::fileMap;

  /// @brief      Registers a hit on a function. (A function being executed,
  /// @param[in]  file: The file the function is contained in.
  /// @param[in]  function: The function called.
  /// @throws
  /// @version    2024-03-30/GGB - Function created.

  void CFunctionTrace::registerHit(std::string const &file, std::string const &function)
  {
    if (!fileMap.contains(file))
    {
      fileMap[file][function] = 1;
    }
    if (!fileMap.at(file).contains(function))
    {
      fileMap.at(file)[function] = 1;
    }
    else
    {
      fileMap[file][function]++;
    };
  }

  /// @brief      Writes the data to disk.
  /// @throws     None.
  /// @version    2024-03-30/GGB - Function created.

  void CFunctionTrace::writeData()
  {
    std::ofstream ofs;
    std::filesystem::path outputName(outputPath);
    outputName = outputName / "functionUsage.dat";
    ofs.open(outputName, std::ios_base::out | std::ios_base::trunc);

    for (auto const &file: fileMap)
    {
      ofs << file.first << '\n';
      for (auto const &function: file.second)
      {
        ofs << "  " << function.first << ": " << function.second << '\n';
      }
    }

    ofs.close();
  }
}
