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
//                      Copyright 2020-2022 Gavin Blakeman.
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

#include "include/pluginManager/pluginManager.h"

  // Standard C++ library header files

  // Miscellaneous library header files

#include "boost/locale.hpp"

  // GCL library

#include "include/common.h"
#include "include/error.h"
#include "include/GCLError.h"

#if defined(unix) || defined(__unix__) || defined(__unix) || defined (__linux__)

#include <dlfcn.h>

#endif // unix type platforms


namespace GCL::plugin
{
  /// @brief    Class constructor.
  /// @version  2020-11-29/GGB - Function created.

  CPluginManager::CPluginManager()
  {

  }

  /// @brief    Class destructor. Need to ensure that all plugins are unloaded before exit.
  /// #version  2020-11-29/GGB - Function created.

  CPluginManager::~CPluginManager()
  {

  }

  /// @brief      Adds a search path to the list of search paths. Conver
  /// @param[in]  path: The path to add to the exising list.
  /// @returns    true - the path was added. false - the path was not added (due to already having an equivilent.)
  /// @throws     GCL::
  /// @note       1. Checks are made to see if the path actually exists in the filesystem and an exception is thrown if the path
  ///               does not exist.
  /// @note       2. Checks are made to see if an equivilent path is already existing in the list of search paths. (This in O(n)))
  /// @todo       Should this be changed to use move syntax?
  /// @version    2020-11-29/GGB - Function created.

  bool CPluginManager::addSearchPath(pluginPath_t const &path)
  {
    bool returnValue = false;

    if (std::filesystem::exists(path))
    {
      bool exists = false;
      auto iterator = searchPaths.begin();

      while ((iterator != searchPaths.end()) && !exists)
      {
        exists = std::filesystem::equivalent(path, *iterator++);
      };

      if (!exists)
      {
        searchPaths.push_back(path);
        returnValue = true;
      };
    }
    else
    {
      RUNTIME_ERROR(boost::locale::translate("PluginManager: Directory does not exist."), E_PLUGINMANAGER_NODIRECTORY, LIBRARYNAME);
    };

    return returnValue;
  }

  /// @brief      Loads the specified plugin. First checks if it is already loaded and if so returns the existing handle. If not
  ///             loaded, loads the plugin and returns the handle.
  /// @param[in]  pluginName: The name of the plugin to load.
  /// @note       1. The plugin name can include a full path, however this can create errors. Rather set the search paths for the
  ///               path and just pass the plugin name.
  /// @note       2. Unix and Linux will search /lib and /usr/lib. This appears to be unavoidable. Refer: dlopen(3)
  /// @note       3. Unix and Linux will load any libraries referenced by the plugin automatically. Refer: dlopen(3)
  /// @throws
  /// @version    2020-11-29/GGB - Function created.

  pluginHandle_t CPluginManager::loadPlugin(pluginName_t const &pluginName)
  {
    pluginHandle_t pluginHandle;
    pluginNameMap_t::iterator foundItem;

    if ((foundItem = pluginNameMap.find(pluginName)) != pluginNameMap.end())
    {
        // plugin already loaded.

      pluginHandle = pluginVector.at(foundItem->second).pluginHandle;   // This may throw.
      pluginVector.at(foundItem->second).pluginRefCount++;
    }
    else
    {
        // Need to load the plugin.

#if defined(unix) || defined(__unix__) || defined(__unix) || defined (__linux__)

        // Search all the paths to see if the library can be found. On Unix/Linux, also search /lib and /usr/lib
        // The name to be loaded is 'pluginName.so' or 'plugin_pluginName.so'

      pluginName_t pluginToLoad = (appendPlugin ? "plugin_":"") + pluginName + ".so";

      bool pluginFound = false;
      searchPath_t::iterator iter = searchPaths.begin();
      std::filesystem::path fileNameAndPath;

      while (iter != searchPaths.end() && !pluginFound)
      {
        fileNameAndPath = *iter / pluginToLoad;
        pluginFound = std::filesystem::exists(fileNameAndPath);
        iter++;
      };

      if (pluginFound)
      {
        pluginVector.push_back({++lastPluginHandle, pluginName, 0, nullptr, symbolMap_t()});
        pluginVector.back().systemHandle = dlopen(fileNameAndPath.c_str(), RTLD_LAZY);

        if (pluginVector.back().systemHandle != nullptr)
        {
          pluginVector.back().pluginRefCount++;
        }
        else
        {
          RUNTIME_ERROR(boost::locale::translate("PluginManager: Directory does not exist."), E_PLUGINMANAGER_UNABLETOLOAD,
                        LIBRARYNAME);
        };

      };

#endif // unix type platforms

    };

    return pluginHandle;
  }

  /// @brief      Maps a symbol in the specified plugin.
  /// @param[in]  pluginHandle: The plugin to query.
  /// @param[in]  symbol: The symbol to query.
  /// @returns    pointer to the symbol (or nullptr)
  /// @throws     GCL::runtime_error
  /// @throws     std::out_of_range - If the pluginHandle is invalid.

  void *CPluginManager::mapSymbol(pluginHandle_t pluginHandle,  symbol_t const &symbol)
  {
    void *returnValue;
    pluginVector_t::size_type handleIndex;
    symbolMap_t::iterator existingSymbol;

    handleIndex = pluginHandleMap.at(pluginHandle); // This may throw if the item is not found.

    if ((existingSymbol = pluginVector[handleIndex].symbolMap.find(symbol)) != pluginVector[handleIndex].symbolMap.end())
    {
      returnValue = existingSymbol->second;
    }
    else
    {


#if defined(unix) || defined(__unix__) || defined(__unix) || defined (__linux__)
      char *error;

      dlerror();
      returnValue = dlsym(pluginVector[handleIndex].systemHandle, symbol.c_str());

      if ((error = dlerror()) != nullptr)
      {
        RUNTIME_ERROR(std::string(error), E_PLUGINMANAGER_UNABLETOMAP, LIBRARYNAME);
      }
      else
      {
        pluginVector[handleIndex].symbolMap.emplace(symbol, returnValue);
      };

#endif // unix type platforms


    };
    return returnValue;
  }

}   // namespace GCL::plugin
