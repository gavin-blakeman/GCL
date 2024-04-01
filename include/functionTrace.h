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

#ifndef INCLUDE_FUNCTIONTRACE_H
#define INCLUDE_FUNCTIONTRACE_H

// Standard C++ library header files.
#include <filesystem>
#include <fstream>
#include <map>
#include <source_location>

#define INSTRUMENT_CODE


namespace GCL
{
  class CFunctionTrace
  {
  public:
    static void setOutputDirectory(std::filesystem::path const &op) { outputPath = op; }

    static void registerHit(std::string const &, std::string const &);

    static void writeData();

  protected:
  private:
    using functionMap_t = std::map<std::string, std::size_t>;
    using fileMap_t = std::map<std::string, functionMap_t>;

    static std::filesystem::path outputPath;
    static fileMap_t fileMap;
  };


#ifdef INSTRUMENT_CODE
  inline void FUNCTION_TRACE(std::source_location const location = std::source_location::current())
  {
    CFunctionTrace::registerHit(std::string(location.file_name()), std::string(location.function_name()));
  }
#else
  // If not using instrumentation change to a empty macro.
#define FUNCTION_TRACE
#endif

}



#endif /* INCLUDE_FUNCTIONTRACE_H_ */
