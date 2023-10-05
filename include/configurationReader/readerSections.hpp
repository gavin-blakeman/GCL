//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                configurationReader/readerSections.hpp
// SUBSYSTEM:           Configuratoin file reader
// LANGUAGE:						C++
// TARGET OS:						None.
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2020-2023 Gavin Blakeman.
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
// OVERVIEW:            Implementation of a configuration file reader using supporting sections.
//
// CLASSES INCLUDED:
//
// HISTORY:             2020-04-28 GGB - File Created
//
//*********************************************************************************************************************************

#ifndef GCL_READERSECTIONS_HPP
#define GCL_READERSECTIONS_HPP

  // Standard C++ library header files

#include <concepts>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>

  // Miscellaneous library header files

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/locale.hpp>
#include <fmt/format.h>
#include <fmt/std.h>

  // GCL library header files.

#include "include/configurationReader/readerCore.hpp"
#include "include/error.h"

namespace GCL
{
  using GCL::RUNTIME_ERROR;

  /// @details  Implements standardised methods of reading configuration files. The CReaderSections reader is a derived from the
  ///           CVanillaReader class with additional behavior to manage sections.
  ///           Sections are tags that appear on a line with a tag opening and closing character. (Typically '[]').
  ///           Tag values are then
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

  class CReaderSections : public CReaderCore
  {
  public:
    using CReaderCore::tagValueDouble;
    using CReaderCore::tagValueInt32;
    using CReaderCore::tagValueString;
    using CReaderCore::tagValueUInt16;
    using CReaderCore::tagValueUInt32;
    using CReaderCore::tagValueUInt64;

    /// @brief      Constructor for the class.
    /// @param[in]  seperatorChar: The character(s) used for seperating statements.
    /// @param[in]  commentChar: The character(s) used for indicating comments.
    /// @throws     std::bad_alloc
    /// @version    2020-11-30/GGB - Changed to suse std::filesystem.
    /// @version    2020-04-27/GGB - Function created.

    CReaderSections(std::string seperatorChar = "=",
                    std::string commentChar = "#")
      :  CReaderCore(seperatorChar, commentChar), sectionOpenChar_("["), sectionCloseChar_("]"), namespaceChar_("/")
    {
    }

    /// @brief      Constructor for the class.
    /// @param[in]  filename: The filename and path of the configuration file.
    /// @param[in]  seperatorChar: The character(s) used for seperating statements.
    /// @param[in]  commentChar: The character(s) used for indicating comments.
    /// @throws     std::bad_alloc
    /// @version    2020-11-30/GGB - Changed to suse std::filesystem.
    /// @version    2020-04-27/GGB - Function created.


    CReaderSections(explicit_path auto const &filename,
                    std::string seperatorChar = "=",
                    std::string commentChar = "#")
      :  CReaderCore(filename, seperatorChar, commentChar), sectionOpenChar_("["), sectionCloseChar_("]"), namespaceChar_("/")
    {
    }

    virtual ~CReaderSections() {}

    /// @brief Returns a tag value.
    /// @param[in] section: The section to search for the tag.
    /// @param[in] tagName: The name of the tag to find.
    /// @returns A std::optional containing the data (if found)
    /// @throws
    /// @version 2020-04-27/GGB - Function created.

    virtual std::optional<std::string> tagValueString(std::string const &section, std::string const &tagName)
    {
      return std::move(readTag(section + namespaceChar_ + tagName));
    }

    /// @brief Returns a double tag value.
    /// @param[in] section: The section to search for the tag.
    /// @param[in] tagName: The name of the tag to find.
    /// @returns A std::optional containing the data (if found and converted) A false optional implies the tag could was no found.
    /// @throws std::runtime_error - The value was not able to be converted.
    /// @version 2020-04-27/GGB - Function created.

    virtual std::optional<double> tagValueDouble(std::string const &section, std::string const &tagName)
    {
      return std::move(tagValueDouble(section + namespaceChar_ + tagName));
    }

    /// @brief Returns an uint16 tag value.
    /// @param[in] section: The section to search for the tag.
    /// @param[in] tagName: The name of the tag to find.
    /// @returns A std::optional containing the data (if found and converted) A false optional implies the tag could was no found.
    /// @throws std::runtime_error - The value was not able to be converted.
    /// @throws std::out_of_range - The value was too large for the type.
    /// @version 2020-04-27/GGB - Function created.

    virtual std::optional<std::uint16_t> tagValueUInt16(std::string const &section, std::string const &tagName)
    {
      return std::move(tagValueUInt16(section + namespaceChar_ + tagName));
    }

    /// @brief      Returns an uint32 tag value.
    /// @param[in]  section: The section to search for the tag.
    /// @param[in]  tagName: The name of the tag to find.
    /// @returns    A std::optional containing the data (if found and converted) A false optional implies the tag could was no found.
    /// @throws     std::runtime_error - The value was not able to be converted.
    /// @throws     std::out_of_range - The value was too large for the type.
    /// @version    2022-12-06/GGB - Function created.

    virtual std::optional<std::uint32_t> tagValueUInt32(std::string const &section, std::string const &tagName)
    {
      return std::move(CReaderCore::tagValueUInt32(section + namespaceChar_ + tagName));
    }

    /// @brief      Returns an uint64 tag value.
    /// @param[in]  section: The section to search for the tag.
    /// @param[in]  tagName: The name of the tag to find.
    /// @returns    A std::optional containing the data (if found and converted) A false optional implies the tag could was no found.
    /// @throws     std::runtime_error - The value was not able to be converted.
    /// @throws     std::out_of_range - The value was too large for the type.
    /// @version    2022-09-27/GGB - Function created.

    virtual std::optional<std::uint64_t> tagValueUInt64(std::string const &section, std::string const &tagName)
    {
      return std::move(tagValueUInt64(section + namespaceChar_ + tagName));
    }

    /// @brief Returns an int32 tag value.
    /// @param[in] section: The section to search for the tag.
    /// @param[in] tagName: The name of the tag to find.
    /// @returns A std::optional containing the data (if found and converted) A false optional implies the tag could was no found.
    /// @throws std::runtime_error - The value was not able to be converted.
    /// @throws std::out_of_range - The value was too large for the type.
    /// @version 2020-04-27/GGB - Function created.

    virtual std::optional<std::int32_t> tagValueInt32(std::string const &section, std::string const &tagName)
    {
      return std::move(tagValueInt32(section + namespaceChar_ + tagName));
    }

  protected:

    /// @brief      Reads the configuration file until the tag value is found.
    /// @param[in]  sectionTagName: The section and tag seperated by a namespace seperator.
    /// @returns    Standard optional with the value if found. If the tag is not found then the optional has no value.
    /// @throws
    /// @version    2020-11-30/GGB - Debugging function.
    /// @version    2020-04-27/GGB - Function created.

    virtual std::optional<std::string> readTag(std::string const &sectionTagName) const
    {
      std::optional<std::string> returnValue;
      std::size_t lineNumber = 0;
      std::size_t linesRead = 0;  // Used for readahead.
      std::string szLine, szSection;
      std::size_t linesReadAhead = readAhead_;
      bool readAheadComplete = false;
      bool sectionFound = false, tagFound = false;
      sectionContainer_t::iterator sectionIterator;
      tagValueContainer_t::iterator tagIterator;
      std::string section, tagName;

      section = sectionTagName.substr(0, sectionTagName.find(namespaceChar_, 0));
      tagName = sectionTagName.substr(sectionTagName.find(namespaceChar_, 0) + 1, std::string::npos);

      boost::trim(section);
      boost::trim(tagName);

      logger::TRACEMESSAGE("READERSECTIONS: Searching for section: " + section + " tag: " + tagName);

        // Has the section been loaded?

      sectionIterator = sectionContainer.find(section);
      if (sectionIterator != sectionContainer.end())
      {
          // Have already got the section in memory.

        tagIterator = sectionIterator->second.find(tagName);
        if (tagIterator != sectionIterator->second.end())
        {
            // Have already got the value in memory. If the tag has not been read then it does not exist in the file.

          returnValue = (tagIterator->second);
        };
      }
      else
      {

          // Section::Tag pair not found yet, read from the file.

        std::ifstream ifs(filename_);

        if (!ifs.is_open())
        {
          RUNTIME_ERROR(fmt::format(fmt::runtime(boost::locale::gettext("Unable to open configuration file {:s}.")), filename_));
        };

          // Read through the already read lines quickly without any translation.

        while (lineNumber < lastLine_)
        {
          std::getline(ifs, szLine);
          lineNumber++;
        };

          // Now start reading sections and tags until we find the value we are looking for.

        while (!ifs.eof() && !readAheadComplete)
        {
          std::getline(ifs, szLine);

          boost::trim(szLine);  // Drop of all whitespace

          linesRead++;
          lineNumber++;

          if (!szLine.empty())
          {
            std::size_t indexStart, indexEnd;

              // Check if a comment, section or tag line

            if ((indexStart = szLine.find(commentChar_)) != std::string::npos)
            {
                // Comment in line, discard everything after the comment.

              szLine = szLine.substr(0, indexStart);

              logger::TRACEMESSAGE("Line after removing comments: " + szLine);
            };

            if ((indexStart = szLine.find(sectionOpenChar_)) != std::string::npos)
            {
                // Section line.

              if (linesReadAhead == 0)
              {
                readAheadComplete = true;
              }
              else
              {
                indexEnd = szLine.find(sectionCloseChar_);

                szSection = szLine.substr(indexStart + sectionOpenChar_.size(), indexEnd -
                                          (indexStart + sectionOpenChar_.size()));

                logger::TRACEMESSAGE("Section Found: " + szSection);

                sectionIterator = std::get<0>(sectionContainer.emplace(szSection, tagValueContainer_t()));

                if (szSection == section)
                {
                  sectionFound = true;
                }
                else if (sectionFound)
                {
                  linesReadAhead --;
                }
              };
            }
            else
            {
                // tag line

              std::size_t tokenEnd;
              std::string tag, value;

              tokenEnd = szLine.find(seperatorChar_);
              if (tokenEnd != std::string::npos)
              {
                tag = szLine.substr(0, tokenEnd);
                boost::trim(tag);

                value = szLine.substr(tokenEnd + seperatorChar_.size(), std::string::npos);
                boost::trim(value);

                logger::TRACEMESSAGE("Tag identified: " + tag  + " value: " + value);

                if (sectionFound)
                {
                  if (tagFound)
                  {
                    if (linesReadAhead > 0)
                    {
                      linesReadAhead--;
                    };
                  }
                  else if (tag == tagName)
                  {
                    logger::TRACEMESSAGE("Tag Found");

                    tagFound = true;
                    returnValue = value;
                  };
                };

                sectionIterator->second.emplace(std::move(tag), std::move(value));
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

  private:
    using tagValueContainer_t = std::unordered_map<std::string, std::string>;
    using sectionContainer_t = std::unordered_map<std::string, tagValueContainer_t>;

    std::string sectionOpenChar_;                 ///< The seperator character to use to open section declarations.
    std::string sectionCloseChar_;                ///< The seperator character to use to close section declarations.
    std::string namespaceChar_;                   ///< The seperator character to use to seperate namespace tag combinations.
    mutable sectionContainer_t sectionContainer;  ///< The container storing the section values and data.

    CReaderSections() = delete;
    CReaderSections(CReaderSections const &) = delete;
    CReaderSections(CReaderSections &&) = delete;
    CReaderSections &operator =(CReaderSections const &) = delete;


  };

} // namespace GCL

#endif // GCL_READERSECTIONS_HPP
