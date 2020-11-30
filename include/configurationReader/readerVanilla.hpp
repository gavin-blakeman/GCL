//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                configurationReader/readerVanilla.hpp
// SUBSYSTEM:           Configuration file reader
// LANGUAGE:						C++
// TARGET OS:						None.
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2020 Gavin Blakeman.
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
// OVERVIEW:            Implementation of a configuration file reader.
//
// CLASSES INCLUDED:
//
// HISTORY:             2020-04-27 GGB - File Created
//
//*********************************************************************************************************************************

#ifndef GCL_READERVANILLA_HPP
#define GCL_READERVANILLA_HPP

  // Standard C++ library header files

#include <cstdint>
#include <exception>
#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>

  // Miscellaneous library header files

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

  // GCL library header files.

#include "include/configurationReader/readerCore.hpp"
#include "include/logger/loggerCore.h"

namespace GCL
{
  /// @details  Implements standardised methods of reading configuration files. The CReaderVanilla reader is a standalone base class
  ///           that can be extended by descendents to implement further behavior.
  ///           The class implements a form of .conf file that uses a 'tag [seperator] value' approach. The seperators are defined
  ///           at runtime.
  ///           Configuration files are considered to be read-only. These are designed for stand-alone non-GUI application. These
  ///           applications generally startup and run automatically while serving requests. (Servers, daemons etc) While it is
  ///           possible to implement/add methods for updating these files, if frequent parameter changes are required an settings
  ///           file that is designed for frequent changes and updates may be a better option.
  ///           The approach used to reading values is that values are read from the file they are stored in a map for later
  ///           retrieval. As the type of the value is unknown when read, the values are initially stored as strings.
  ///           Comments - Must always appear on line by themselves and the first non-white space character on the line must be
  ///                      the comment character.

  class CReaderVanilla : CReaderCore
  {
  protected:
    using tagValueContainer_t = std::unordered_map<std::string, std::string>;

  private:
    tagValueContainer_t tagValueMap;

    CReaderVanilla() = delete;
    CReaderVanilla(CReaderVanilla const &) = delete;
    CReaderVanilla(CReaderVanilla &&) = delete;
    CReaderVanilla &operator =(CReaderVanilla const &) = delete;

    /// @brief Reads the configuration file until the tag value is found.
    /// @param[in] tagName: The tag to find
    /// @returns Standard optional with the value if found. If the tag is not found then the optional has no value.
    /// @throws
    /// @version 2020-04-27/GGB - Function created.

    virtual std::optional<std::string> readTag(std::string tagName)
    {
      std::optional<std::string> returnValue;
      std::ifstream ifs;
      std::size_t lineNumber = 0;
      std::size_t linesRead = 0;  // Used for readahead.
      std::string szLine ;
      std::size_t linesReadAhead = readAhead_;

        // See if we already have the tag in memory.

      auto iterator = tagValueMap.find(tagName);
      if (iterator != tagValueMap.end())
      {
          // Have already got the value in memory.

        returnValue = (iterator->second);
      }
      else
      {
        // Need to search the file.

        ifs.open(filename_);

        // Read through the already read lines quickly.

        while (lineNumber < lastLine_)
        {
          std::getline(ifs, szLine);
          lineNumber++;
        };

        // Now stat reading until we find the value we are looking for.

        while (!ifs.eof() && (linesReadAhead > 0))
        {

          bool tagFound = false;
          std::getline(ifs, szLine);
          linesRead++;
          lineNumber++;

          if (!szLine.empty())
          {
            // Check if a comment line

            if (szLine.substr(0, commentChar_.size()) != commentChar_)
            {
              std::size_t tokenEnd;
              std::string tag, value;

              tokenEnd = szLine.find(seperatorChar_);
              if (tokenEnd != std::string::npos)
              {
                tag = szLine.substr(0, tokenEnd);
                boost::trim(tag);

                value = szLine.substr(tokenEnd + seperatorChar_.size(), std::string::npos);
                boost::trim(value);

                if (tagFound)
                {
                  linesReadAhead--;
                }
                else if (tag == tagName)
                {
                  tagFound = true;
                  returnValue = value;
                };

                tagValueMap.emplace(std::move(tag), std::move(value));
              };
            };
          };
        };  // while loop

        if (ifs.eof())
        {
          readComplete_ = true;
        };

        ifs.close();
      };  // end if

      return returnValue;
    }

  protected:
    std::filesystem::path filename_;          ///< Filename and path of the configuration file
    std::string seperatorChar_;               ///< The seperator character to use between token and value.
    std::string commentChar_;                 ///< Character(s) used to start comment lines.
    std::size_t lastLine_ = 0;                ///< Last line read in the file.
    std::size_t const readAhead_ = 10;        ///< Number of tags to read ahead.
    bool readComplete_ = false;               ///< True if the entire file has been read.

  public:

    /// @brief Constructor for the class. A single constructor is provided and the default constructor is deleted.
    /// @param[in] filename: The filename and path of the configuration file.
    /// @param[in] seperatorChar: The character(s) used for seperating statements.
    /// @param[in] commentChar: The character(s) used for indicating comments.
    /// @throws std::bad_alloc
    /// @version 2020-11-30/GGB - Changed to std::filesystem.
    /// @version 2020-04-27/GGB - Function created.

    CReaderVanilla(std::filesystem::path const &filename, std::string seperatorChar = "=", std::string commentChar = "#")
      : CReaderCore(filename, seperatorChar, commentChar)
    {
    }

  }; // class

} // namespace GCL

#endif // GCL_READERVANILLA_HPP
