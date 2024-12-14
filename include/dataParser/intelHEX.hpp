//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                dataParser/intelHex.h
// SUBSYSTEM:           Data File Parser
// LANGUAGE:            C++
// TARGET OS:           None.
// NAMESPACE:           GCL
// AUTHOR:              Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2021, 2024 Gavin Blakeman.
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
// HISTORY:             2021-07-08 GGB - File Created
//
//**********************************************************************************************************************************


#ifndef INTELHEX_H
#define INTELHEX_H

// Standard C++ library header files

#include <cctype>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>

// Miscellaneous library header files

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include "fmt/format.h"
#include "include/concepts.hpp"
#include "include/error.h"
#include "include/logger/loggerManager.h"

namespace GCL
{
  class CIntelHex
  {
  public:
    CIntelHex(std::filesystem::path const &ifn) : inputFile(ifn) {}

    void readFile(std::uint8_t *memory, std::uint64_t memorySize)
    {
      std::uint16_t LBA = 0;
      std::ifstream ifs;                  // Input file stream
      std::uint64_t lineNumber = 0;
      std::uint64_t address = 0;

      ifs.open(inputFile);

      while (!ifs.eof())
      {
        std::uint8_t index = 0;
        std::string digits;
        std::string szLine;
        std::uint8_t recordLength;
        std::uint16_t loadOffset;
        std::uint8_t recordType;
        std::string szChecksum;
        std::uint8_t checksum = 0;

        std::getline(ifs, szLine);          // Get a single line
        boost::to_upper(szLine);            // Convert to upper case.

        if (szLine[index++] == ':')
        {
          // Next two digits are the length.

          digits = szLine.substr(index, 2);
          index += 2;
          recordLength = static_cast<uint8_t>(stoul(digits, nullptr, 16));
          checksum += recordLength;

          digits = szLine.substr(index, 4);
          index += 4;
          loadOffset = static_cast<uint16_t>(stoul(digits, nullptr, 16));
          checksum += loadOffset;

          digits = szLine.substr(index, 2);
          index += 2;
          recordType = static_cast<uint8_t>(stoul(digits, nullptr, 16));
          checksum += recordType;

          switch(recordType)
          {
            case 0:
            {
              while (recordLength--)
              {
                digits = szLine.substr(index, 2);
                index += 2;
                RUNTIME_ASSERT(address <= memorySize, "Address greater than memory size.");
                memory[address] = static_cast<std::uint8_t>(std::stoul(digits, nullptr, 16));
                checksum += memory[address++];
              }

//              checksum = ~checksum;
//              checksum += 1;

              szChecksum = szLine.substr(index, 2);
              checksum += static_cast<uint8_t>(stoul(szChecksum, nullptr, 16));

              if (checksum != 0)
              {
                logger::ERRORMESSAGE(fmt::format("Checksum failure on line: {}", lineNumber));
                /* Throw error and exit. Checksum failure. */
              }

              break;
            }
            case 1:
            {
              recordType = EOFRecord;
              break;
            }
            case 2:
            {
              recordType = ESARecord;
              break;
            }
            case 3:
            {
              recordType = SSARecord;
              break;
            }
            case 4:
            {
              /* Extended linear address record. This is used to specify bits 16-31 of the linear base address (LBA) where
               * bits 0-15 of the LBA are zero. */

              recordType = ELARecord;
              digits = szLine.substr(index, 4);
              index += 4;
              LBA = static_cast<uint16_t>(stoul(digits, nullptr, 16));
              checksum += LBA;

              szChecksum = szLine.substr(index, 2);
              checksum += static_cast<uint8_t>(stoul(szChecksum, nullptr, 16));

              if (checksum != 0)
              {
                //std::cout << "Checksum failure on line: " << lineNumber << std::endl;
                /* Throw error and exit. Checksum failure. */
              }
              break;
            }
            case 5:
            {
              recordType = SLARecord;
              break;
            }
            default:
            {
              /* Error */
              break;
            }
          }
        }
        else
        {
          /* Error, invalid character. */
        }
        lineNumber++;
      }
    }

  private:
    CIntelHex() = delete;
    CIntelHex(CIntelHex const &) = delete;
    CIntelHex(CIntelHex &&) = delete;
    CIntelHex &operator=(CIntelHex const &) = delete;
    CIntelHex &operator=(CIntelHex &&) = delete;

    enum recordType_e
    {
      DataRecord = 0x00,
      EOFRecord = 0x01,
      ESARecord = 0x02,
      SSARecord = 0x03,
      ELARecord = 0x04,
      SLARecord = 0x05,
    };
    std::filesystem::path inputFile;

  };
}

#endif // INTELHEX_H
