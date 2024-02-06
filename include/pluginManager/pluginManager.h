//*********************************************************************************************************************************
//
// PROJECT:							General Class Library
// FILE:								pluginManager/pluginManager.h
// SUBSYSTEM:						plugin Manager
// LANGUAGE:						C++
// TARGET OS:						None.
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2020-2024 Gavin Blakeman.
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
// OVERVIEW:						Manager to load/unload plugins.
//
//
// CLASSES INCLUDED:		None
//
// CLASS HIERARCHY:     None.
//
//
// HISTORY:             2020-11-28 GGB - File Created
//
//*********************************************************************************************************************************


#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

  // Standard C++ library header files

#include <atomic>
#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <vector>

#if defined(unix) || defined(__unix__) || defined(__unix) || defined (__linux__)
#include <dlfcn.h>
#endif // unix type platforms

  // Miscellaneous library header files.

namespace GCL::plugin
{
  using pluginHandle_t = std::uint32_t;
  using pluginName_t = std::filesystem::path;
  using pluginRefCount_t = std::uint16_t;
  using pluginPath_t = std::filesystem::path;

  class CPluginManager
  {
  public:
    using searchPath_t = std::vector<pluginPath_t>;
    using symbolMap_t = std::map<std::string, void *>;

    struct plugin_t
    {
      pluginName_t pluginName;
      pluginRefCount_t pluginRefCount;
      void *systemHandle;
      symbolMap_t symbolMap;
    };

    using pluginMap_t = std::map<pluginHandle_t, plugin_t>;
    using pluginNameMap_t = std::map<pluginName_t, pluginHandle_t>;

  private:
    static std::atomic<pluginHandle_t> lastPluginHandle;

    searchPath_t searchPaths;
    bool appendPlugin = false;            ///< True = append "plugin_" to the front of the passed name.
    pluginNameMap_t pluginNameMap;
    pluginMap_t pluginMap;
    std::map<std::string, pluginHandle_t> aliasMap;


    CPluginManager(CPluginManager const &) = delete;
    CPluginManager(CPluginManager &&) = delete;
    CPluginManager operator =(CPluginManager const &) = delete;

  protected:
  public:
    CPluginManager();
    ~CPluginManager();

    bool addSearchPath(pluginPath_t const &);
    pluginHandle_t loadPlugin(pluginName_t const &, int = RTLD_NOW, std::string const & = std::string());
    void *mapSymbol(pluginHandle_t,  std::string const &, bool = false);
    void *mapSymbol(std::string const &, std::string const &, bool = false);

    void unload(pluginHandle_t);
    void unloadAll();
  };
}

#endif // PLUGINMANAGER_H
