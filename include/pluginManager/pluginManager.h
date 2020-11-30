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

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <vector>

  // Miscellaneous library header files.

namespace GCL::plugin
{
  using pluginHandle_t = std::uint32_t;
  using pluginName_t = std::string;
  using pluginRefCount_t = std::uint16_t;
  using pluginPath_t = std::filesystem::path;
  using symbol_t = std::string;

  class CPluginManager
  {
    using searchPath_t = std::vector<pluginPath_t>;
    using symbolMap_t = std::map<symbol_t, void *>;

    struct plugin_t
    {
      pluginHandle_t pluginHandle;
      pluginName_t pluginName;
      pluginRefCount_t pluginRefCount;
      void *systemHandle;
      symbolMap_t symbolMap;
    };

    using pluginVector_t = std::vector<plugin_t>;
    using pluginNameMap_t = std::map<std::string, pluginVector_t::size_type>;
    using pluginHandleMap_t = std::map<pluginHandle_t, pluginVector_t::size_type>;

  private:
    static pluginHandle_t lastPluginHandle;

    pluginVector_t pluginVector;
    searchPath_t searchPaths;
    bool appendPlugin;            ///< True = append "plugin_" to the front of the passed name.
    pluginNameMap_t pluginNameMap;
    pluginHandleMap_t pluginHandleMap;


    CPluginManager(CPluginManager const &) = delete;
    CPluginManager(CPluginManager &&) = delete;
    CPluginManager operator =(CPluginManager const &) = delete;

    void unload();

  protected:
  public:
    CPluginManager();
    ~CPluginManager();

    bool addSearchPath(pluginPath_t const &);
    pluginHandle_t loadPlugin(pluginName_t const &);
    void *mapSymbol(pluginHandle_t,  symbol_t const &);
  };
}

#endif // PLUGINMANAGER_H
