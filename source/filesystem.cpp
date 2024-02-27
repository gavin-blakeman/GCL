//*********************************************************************************************************************************
//
// PROJECT:							General Class Library
// FILE:								filesystem
// SUBSYSTEM:						filesystem extensions.
// LANGUAGE:						C++
// TARGET OS:						None.
// LIBRARY DEPENDANCE:
// NAMESPACE:
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

#include "include/filesystem.h"

// Standard C++ library header files

#include <cstdio>
#include <random>

  // Miscellaneous library header files.

#include <boost/algorithm/string.hpp>
#include <boost/utility/string_view.hpp>

  // GCL header files

#include "include/error.h"

namespace GCL
{
  /// @brief      Expands a comma or semi-colon delimited list of files into a list of paths.
  /// @param[in]  svList: A string_view of the list of files.
  /// @param[out] fileList: The list of files.

  void expandFileList(std::string_view &&svList, fileList_t &fileList)
  {
    std::size_t tokenEnd;
    std::string szToken;

    while (!svList.empty())
    {
      svList = boost::trim_copy(svList);

      if (svList[0] == '"')
      {
          // Filename demarkated with a string. Find the end of the string.

        tokenEnd = svList.find('"', 1);
        fileList.emplace_back(svList.substr(1, tokenEnd-1));
        if (tokenEnd == std::string_view::npos)
        {
          RUNTIME_ERROR("Unterminated file name. (File name starts with a '""' and does not terminate.");
        }
        else
        {
          svList.remove_prefix(tokenEnd);
          if (!svList.empty())
          {
            if (svList[0] == ',' || svList[0] == ';')
            {
              svList.remove_prefix(1);
            }
          }
        }
      }
      else
      {
        tokenEnd = svList.find_first_of(",;");
        if (tokenEnd != std::string_view::npos)
        {
          szToken = svList.substr(0, tokenEnd);
          svList.remove_prefix(tokenEnd + 1);
        }
        else
        {
          szToken = svList;
          svList.remove_prefix(svList.size());
        };

        boost::trim(szToken);
        fileList.emplace_back(szToken);
      };
    };
  }

  /// @brief      Checks if the specified filename is readable. If the file does not exist, or there is any other error, the
  ///             function  false.
  /// @param[in]  p: The filename (or path) to check.
  /// @returns    true - if the filename or path is readable.
  /// @returns    false - if the filename or path is not readable, or does not exist.
  /// @throws     None.
  /// @version    2023-10-02/GGB - Converted to uese std::filesystem.
  /// @version    2018-05-20/GGB - Function created.

  bool file_readable(std::filesystem::path const &p)
  {
    bool returnValue = false;

    if (exists(p))
    {

      std::filesystem::file_status fileStatus = status(p);

      std::filesystem::perms permissions = fileStatus.permissions();

      returnValue = ((permissions & std::filesystem::perms::owner_read) |
                    (permissions & std::filesystem::perms::group_read) |
                    (permissions & std::filesystem::perms::others_read)) != std::filesystem::perms::none;
    };
    return returnValue;
  }

  std::filesystem::path temporaryFilename(std::uint8_t len)
  {
    std::filesystem::path rv = std::filesystem::temp_directory_path();
    std::random_device rd;
    std::mt19937 genT(rd());
    std::mt19937 gen1(rd());
    std::mt19937 genA(rd());
    std::mt19937 gena(rd());
    std::uniform_int_distribution<> type(1, 3);
    std::uniform_int_distribution<> UC('A', 'Z');
    std::uniform_int_distribution<> LC('a', 'z');
    std::uniform_int_distribution<> NUM('0', '9');

    std::string tempName;

    for (std::uint_fast8_t i = 0; i != len; i++)
    {
      switch (type(genT))
      {
        case 1:
        {
          tempName.push_back(UC(genA));
          break;
        }
        case 2:
        {
          tempName.push_back(LC(gena));
          break;
        }
        case 3:
        {
          tempName.push_back(NUM(gen1));
          break;
        }
        default:
        {
          CODE_ERROR();
          // Does not return.
        }
      }
    }
    rv /= tempName;

    return rv;
  }


}
