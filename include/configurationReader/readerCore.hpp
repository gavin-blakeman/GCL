//*********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                configurationReader/readerVanilla.hpp
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
// OVERVIEW:            Implementation of a configuration file reader.
//
// CLASSES INCLUDED:
//
// HISTORY:             2020-04-27 GGB - File Created
//
//*********************************************************************************************************************************

#ifndef GCL_READERCORE_HPP
#define GCL_READERCORE_HPP

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

#include "include/logger/defaultLogger.h"

namespace GCL
{
  template<class T>
  concept explicit_path =  std::same_as<T, std::filesystem::path>;

  using GCL::logger::DEBUGMESSAGE;

  class CReaderCore
  {
  private:
    CReaderCore() = delete;
    CReaderCore(CReaderCore const &) = delete;
    CReaderCore(CReaderCore &&) = delete;
    CReaderCore &operator =(CReaderCore const &) = delete;

  protected:
    std::filesystem::path filename_;          ///< Filename and path of the configuration file
    std::string seperatorChar_;               ///< The seperator character to use between token and value.
    std::string commentChar_;                 ///< Character(s) used to start comment lines.
    std::size_t lastLine_ = 0;                ///< Last line read in the file.
    std::size_t const readAhead_ = 10;        ///< Number of tags to read ahead.
    mutable bool readComplete_ = false;       ///< True if the entire file has been read.

    virtual std::optional<std::string> readTag(std::string const &) const = 0;

    /// @brief Returns a double tag value.
    /// @param[in] to s: The name of the tag to find.
    /// @returns The string converted to a double.
    /// @throws std::runtime_error - The value was not able to be converted.
    /// @version 2020-04-27/GGB - Function created.

    virtual double string2Double(std::string const &s)
    {
      double returnValue;

      try
      {
        returnValue = std::stod(s);  // According to standard if the stod throws, then the optional should be false.
      }
      catch(std::invalid_argument const &e)
      {
        // Could not convert the value.

        DEBUGMESSAGE(e.what());
        throw std::runtime_error("Unable to convert tag value to double");
      }
      catch(std::out_of_range const &e)
      {
        // Unusual to get an double that is out of range.

        DEBUGMESSAGE(e.what());
        throw std::runtime_error("Unable to convert tag value to double");
      };

      return returnValue;
    }

    /// @brief Returns an uint16 tag value.
    /// @param[in] s: The name of the tag to find.
    /// @returns The string converted to a uint16.
    /// @throws std::runtime_error - The value was not able to be converted.
    /// @throws std::out_of_range - The value was too large for the type.
    /// @version 2020-04-27/GGB - Function created.

    virtual std::uint16_t string2UInt16(std::string const &s) const
    {
      std::uint16_t returnValue;

      try
      {
        returnValue = boost::lexical_cast<std::uint16_t>(s);
      }
      catch(boost::bad_lexical_cast const &e)
      {
        // Could not convert the value.

        DEBUGMESSAGE(e.what());
        throw std::runtime_error("Unable to convert tag value to uint16.");
      };

      return returnValue;
    }

    /// @brief      Returns an uint32 tag value.
    /// @param[in]  s: The name of the tag to find.
    /// @returns    The string converted to a uint16.
    /// @throws     std::runtime_error - The value was not able to be converted.
    /// @throws     std::out_of_range - The value was too large for the type.
    /// @version    2022-12-06/GGB - Function created.

    virtual std::uint64_t string2UInt32(std::string const &s) const
    {
      std::uint64_t returnValue;

      try
      {
        returnValue = boost::lexical_cast<std::uint32_t>(s);
      }
      catch(boost::bad_lexical_cast const &e)
      {
        // Could not convert the value.

        DEBUGMESSAGE(e.what());
        throw std::runtime_error("Unable to convert tag value to Uint16");
      };

      return returnValue;
    }

    /// @brief      Returns an uint64 tag value.
    /// @param[in]  s: The name of the tag to find.
    /// @returns    The string converted to a uint16.
    /// @throws     std::runtime_error - The value was not able to be converted.
    /// @throws     std::out_of_range - The value was too large for the type.
    /// @version    2022-09-27/GGB - Function created.

    virtual std::uint64_t string2UInt64(std::string const &s)
    {
      std::uint64_t returnValue;

      try
      {
        returnValue = boost::lexical_cast<std::uint64_t>(s);
      }
      catch(boost::bad_lexical_cast const &e)
      {
        // Could not convert the value.

        DEBUGMESSAGE(e.what());
        throw std::runtime_error("Unable to convert tag value to Uint16");
      };

      return returnValue;
    }

    /// @brief      Returns an int16 tag value.
    /// @param[in]  s: The name of the tag to find.
    /// @returns    The string converted to a int16.
    /// @throws     std::runtime_error - The value was not able to be converted.
    /// @throws     std::out_of_range - The value was too large for the type.
    /// @version    2023-11-20/GGB - Function created.

    virtual std::int16_t string2Int16(std::string const &s) const
    {
      std::int16_t returnValue;

      try
      {
        returnValue = boost::lexical_cast<std::int16_t>(s);
      }
      catch(boost::bad_lexical_cast const &e)
      {
        // Could not convert the value.

        DEBUGMESSAGE(e.what());
        throw std::runtime_error("Unable to convert tag value to int16.");
      };

      return returnValue;
    }

    /// @brief Returns an int32 tag value.
    /// @param[in] s: The name of the tag to find.
    /// @returns A string conveted to an int32.
    /// @throws std::runtime_error - The value was not able to be converted.
    /// @version 2020-04-27/GGB - Function created.

    virtual std::int32_t string2Int32(std::string const &s)
    {
      std::int32_t returnValue;

      try
      {
        returnValue = std::stol(s, 0, 0);  // According to standard if the stod throws, then the optional should be false.
      }
      catch(std::invalid_argument const &e)
      {
        // Could not convert the value.

        DEBUGMESSAGE(e.what());
        throw std::runtime_error("Unable to convert tag value to std::int32_t");
      }
      catch(std::out_of_range const &e)
      {
        // Unusual to get an double that is out of range.

        DEBUGMESSAGE(e.what());
        throw std::runtime_error("Unable to convert tag value to std::int32_t");
      };

      return returnValue;
    }

  public:

    /// @brief Constructor for the class.
    ///     /// @param[in] seperatorChar: The character(s) used for seperating statements.
    /// @param[in] commentChar: The character(s) used for indicating comments.
    /// @throws std::bad_alloc
    /// @version 2023-02-02/GGB - Function created.

    CReaderCore(std::string seperatorChar = "=", std::string commentChar = "#")
      : filename_(), seperatorChar_(seperatorChar), commentChar_(commentChar)
    {

    }

    /// @brief Constructor for the class.
    /// @param[in] filename: The filename and path of the configuration file.
    /// @param[in] seperatorChar: The character(s) used for seperating statements.
    /// @param[in] commentChar: The character(s) used for indicating comments.
    /// @throws std::bad_alloc
    /// @version    2020-11-30/GGB - Changed to use std::filesystem
    /// @version 2020-04-27/GGB - Function created.

    CReaderCore(std::filesystem::path const &filename, std::string seperatorChar = "=", std::string commentChar = "#")
      : filename_(filename), seperatorChar_(seperatorChar), commentChar_(commentChar)
    {
    }

    virtual ~CReaderCore() {}

    /// @brief Returns a bool tag value.
    /// @param[in] tagname: The tag to get.
    /// @returns optional<bool>
    /// @throws
    /// @version 2022-12-03/GGB - Function created.

    virtual std::optional<bool> tagValueBool(std::string const &tagname)
    {
      std::optional<bool> rv;

      std::optional<std::string> value = readTag(tagname);

      if (value)
      {
        boost::to_upper(*value);
        rv = (*value == "TRUE");
      };

      return rv;
    }

    /// @brief Setter for the filename.
    /// @param[in] fn: The filename.
    /// @version 2023-02-02/GGB - Function created.

    void filename(std::filesystem::path &fn)
    {
      filename_ = fn;
    }

    /// @brief Returns a tag value.
    /// @param[in] tagName: The name of the tag to find.
    /// @returns A std::optional containing the data (if found)
    /// @throws
    /// @version 2020-04-27/GGB - Function created.

    virtual std::optional<std::string> tagValueString(std::string const &tagName)
    {
      return std::move(readTag(tagName));
    }

    /// @brief Returns a double tag value.
    /// @param[in] tagName: The name of the tag to find.
    /// @returns A std::optional containing the data (if found and converted) A false optional implies the tag could was no found.
    /// @throws std::runtime_error - The value was not able to be converted.
    /// @version 2020-04-27/GGB - Function created.

    virtual std::optional<double> tagValueDouble(std::string const &tagName)
    {
      std::optional<double> returnValue;

      std::optional<std::string> value = readTag(tagName);

      if (value)
      {
        returnValue = string2Double(*value);
      };

      return returnValue;
    }

    /// @brief Returns an uint16 tag value.
    /// @param[in] tagName: The name of the tag to find.
    /// @returns A std::optional containing the data (if found and converted) A false optional implies the tag could was no found.
    /// @throws std::runtime_error - The value was not able to be converted.
    /// @throws std::out_of_range - The value was too large for the type.
    /// @version 2020-04-27/GGB - Function created.

    virtual std::optional<std::uint16_t> tagValueUInt16(std::string const &tagName) const
    {
      std::optional<std::uint16_t> returnValue;

      std::optional<std::string> value = readTag(tagName);

      if (value)
      {
        returnValue = string2UInt16(*value);
      };

      return returnValue;
    }

    /// @brief      Returns an uint32 tag value.
    /// @param[in]  tagName: The name of the tag to find.
    /// @returns    A std::optional containing the data (if found and converted) A false optional implies the tag could was no found.
    /// @throws     std::runtime_error - The value was not able to be converted.
    /// @throws     std::out_of_range - The value was too large for the type.
    /// @version    2022-12-06/GGB - Function created.

    virtual std::optional<std::uint32_t> tagValueUInt32(std::string const &tagName)
    {
      std::optional<std::uint32_t> returnValue;

      std::optional<std::string> value = readTag(tagName);

      if (value)
      {
        returnValue = string2UInt32(*value);
      };

      return returnValue;
    }

    /// @brief      Returns an uint64 tag value.
    /// @param[in]  tagName: The name of the tag to find.
    /// @returns    A std::optional containing the data (if found and converted) A false optional implies the tag could was no found.
    /// @throws     std::runtime_error - The value was not able to be converted.
    /// @throws     std::out_of_range - The value was too large for the type.
    /// @version    2022-09-27/GGB - Function created.

    virtual std::optional<std::uint64_t> tagValueUInt64(std::string const &tagName)
    {
      std::optional<std::uint64_t> returnValue;

      std::optional<std::string> value = readTag(tagName);

      if (value)
      {
        returnValue = string2UInt64(*value);
      };

      return returnValue;
    }

    /// @brief Returns an int16 tag value.
    /// @param[in] tagName: The name of the tag to find.
    /// @returns A std::optional containing the data (if found and converted) A false optional implies the tag could was no found.
    /// @throws std::runtime_error - The value was not able to be converted.
    /// @throws std::out_of_range - The value was too large for the type.
    /// @version 2020-04-27/GGB - Function created.

    virtual std::optional<std::int16_t> tagValueInt16(std::string const &tagName) const
    {
      std::optional<std::int16_t> returnValue;

      std::optional<std::string> value = readTag(tagName);

      if (value)
      {
        returnValue = string2Int16(*value);
      };

      return returnValue;
    }

    /// @brief Returns an int32 tag value.
    /// @param[in] tagName: The name of the tag to find.
    /// @returns A std::optional containing the data (if found and converted) A false optional implies the tag could was no found.
    /// @throws std::runtime_error - The value was not able to be converted.
    /// @throws std::out_of_range - The value was too large for the type.
    /// @version 2020-04-27/GGB - Function created.

    virtual std::optional<std::int32_t> tagValueInt32(std::string const &tagName)
    {
      std::optional<std::int32_t> returnValue;

      std::optional<std::string> value = readTag(tagName);

      if (value)
      {
        returnValue = string2Int32(*value);
      };

      return returnValue;
    }

  };  // class CReaderCore

} // namespace GCL

#endif // GCL_READERCORE_HPP
