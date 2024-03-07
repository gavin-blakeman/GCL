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

#include "include/tempFileManager.h"

// GCL header files

#include "../include/filesystem.h"

namespace GCL
{
  /// @brief      Constructor. Sets the path for the temporary files. If no path given, then the default path /tmp is used,
  ///             The tempPath is concatenated to the /tmp path.
  /// @param[in]  tempPath: The path to use for the temporary file storage.
  /// @throws
  /// @version    2024-03-06/GGB - Function created.

  CTempFileManager::CTempFileManager(std::filesystem::path const &tempPath) : tempDirectory(tempPath)
  {
    if (!std::filesystem::exists(tempDirectory))
    {
      deletePathOnExit = true;
      std::filesystem::create_directory(tempDirectory);
    }
  }


  /// @brief      Destructor, ensures that all temporary files have been deleted.
  /// @throws
  /// @version    2024-03-06/GGB - Function created.

  CTempFileManager::~CTempFileManager()
  {
    for (auto const &file : temporaryFiles)
    {
      try
      {
        std::filesystem::remove(file);
      }
      catch(...) {}
    }
    temporaryFiles.clear();

    // Delete the temporary directory (if created) and any remaining files.

    if (deletePathOnExit)
    {
      std::filesystem::remove_all(tempDirectory);
    }
  }

  /// @brief      Registers a file for deletion on destruction. The file does not need to be stored in the tempDirectory.
  /// @param[in]  tempFile: The temporary file that needs to be deleted.
  /// @throws
  /// @version    2024-03-06/GGB - Function created.

  void CTempFileManager::registerFile(std::filesystem::path const &tempFile)
  {
    temporaryFiles.insert(tempFile);
  }

  /// @brief      Registers a file for deletion on destruction. The file does not need to be stored in the tempDirectory.
  /// @param[in]  tempFile: The temporary file that needs to be deleted.
  /// @throws
  /// @version    2024-03-06/GGB - Function created.

  void CTempFileManager::registerFile(std::filesystem::path &&tempFile)
  {
    temporaryFiles.insert(std::move(tempFile));
  }

  /// @brief      Creates a random temporary filename. Also checks that the filename does not exist in the tempDirecory.
  /// @param[in]  fnLength: Number of characters to use for the filename. (Minimum 8 digits)
  /// @returns    A termporary filename. The full path is returned.

  std::filesystem::path CTempFileManager::temporaryFilename(std::uint8_t fnLength)
  {
    std::filesystem::path rv;

    do
    {
      rv = GCL::temporaryFilename(tempDirectory, fnLength);
    }
    while (temporaryFiles.contains(rv) || std::filesystem::exists(rv));
    temporaryFiles.insert(rv);

    return rv;
  }
}
