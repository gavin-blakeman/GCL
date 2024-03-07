//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                tempFileManager.h
// SUBSYSTEM:           Data File Parser
// LANGUAGE:            C++
// TARGET OS:           None.
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
// OVERVIEW:            Manages temporary files and ensures they are deleted.
//
// CLASSES INCLUDED:
//
// HISTORY:             2024-03-06 GGB - File Created
//
//**********************************************************************************************************************************

#ifndef GCL_INCLUDE_TEMPFILEMANAGER_H_
#define GCL_INCLUDE_TEMPFILEMANAGER_H_

// Standard C++ library header files

#include <cstdint>
#include <filesystem>
#include <set>

namespace GCL
{
  class CTempFileManager
  {
  public:
    CTempFileManager(std::filesystem::path const & = std::filesystem::path());
    ~CTempFileManager();

    void registerFile(std::filesystem::path const &);
    void registerFile(std::filesystem::path &&);

    std::filesystem::path temporaryFilename(std::uint8_t = 10);

  protected:
    std::filesystem::path tempDirectory;
    bool deletePathOnExit = false;
    std::set<std::filesystem::path> temporaryFiles;

  private:
    CTempFileManager() = default;
    CTempFileManager(CTempFileManager const &) = delete;
    CTempFileManager(CTempFileManager &&) =delete;
    CTempFileManager &operator=(CTempFileManager const &) = delete;
    CTempFileManager &operator=(CTempFileManager &&) = delete;

  };
}



#endif /* GCL_INCLUDE_TEMPFILEMANAGER_H_ */
