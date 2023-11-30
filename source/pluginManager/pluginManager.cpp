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
#include "fmt/format.h"
#include "fmt/std.h"

  // GCL library

#include "include/common.h"
#include "include/error.h"
#include "include/logger/loggerCore.h"
#include "include/GCLError.h"


namespace GCL::plugin
{

  std::atomic<pluginHandle_t> CPluginManager::lastPluginHandle;

  /// @brief    Class constructor.
  /// @version  2020-11-29/GGB - Function created.

  CPluginManager::CPluginManager()
  {
    unloadAll();
  }

  /// @brief    Class destructor. Need to ensure that all plugins are unloaded before exit.
  /// @version  2020-11-29/GGB - Function created.

  CPluginManager::~CPluginManager()
  {
    for(auto &plugin: pluginMap)
    {
      dlclose(plugin.second.systemHandle);
    }

      // All other data is destructed by the destructor calls on the maps and vectors.
  }

  /// @brief      Adds a search path to the list of search paths.
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
  /// @param[in]  alias: The alias name to use for the plugin. May be an empty string or not supplied.
  /// @note       1. The plugin name can include a full path, however this can create errors. Rather set the search paths for the
  ///               path and just pass the plugin name.
  /// @note       2. Unix and Linux will search /lib and /usr/lib. This appears to be unavoidable. Refer: dlopen(3)
  /// @note       3. Unix and Linux will load any libraries referenced by the plugin automatically. Refer: dlopen(3)
  /// @throws
  /// @version    2020-11-29/GGB - Function created.

  pluginHandle_t CPluginManager::loadPlugin(pluginName_t const &pluginName, int flags, std::string const &alias)
  {
    pluginHandle_t pluginHandle;
    pluginNameMap_t::iterator foundItem;

    if (pluginNameMap.contains(pluginName))
    {
        // plugin already loaded.

      pluginHandle = pluginNameMap.at(pluginName);   // This may throw.
      pluginMap[pluginHandle].pluginRefCount++;

        // Is the alias also defined and correct.

      if (!alias.empty() && aliasMap.contains(alias))
      {
        if (aliasMap[alias] != pluginHandle)
        {
            // Reusing the alias for a different plugin.

          RUNTIME_ERROR("Resusing plugin alias for a different plugin.");
        }
      }
      else
      {
          // Create an alternate alias for the plugin

        if (!alias.empty())
        {
          aliasMap[alias] = pluginHandle;
        };
      }
    }
    else
    {
        // Need to load the plugin.

#if defined(unix) || defined(__unix__) || defined(__unix) || defined (__linux__)

        // Search all the paths to see if the library can be found. On Unix/Linux, also search /lib and /usr/lib
        // The name to be loaded is 'pluginName.so' or 'plugin_pluginName.so'

      /// @todo The following line needs to be improved to select the .so only if required.

      //pluginName_t pluginToLoad = (appendPlugin ? "plugin_":"") / pluginName / ".so";
      pluginName_t pluginToLoad = pluginName;

      bool pluginFound = false;
      searchPath_t::iterator iter = searchPaths.begin();
      std::filesystem::path fileNameAndPath;

      pluginFound = std::filesystem::is_regular_file(pluginToLoad);

      if (pluginFound)
      {
        fileNameAndPath = pluginToLoad;
      }
      else
      {
        while (iter != searchPaths.end() && !pluginFound)
        {
          fileNameAndPath = *iter / pluginToLoad;
          pluginFound = std::filesystem::exists(fileNameAndPath);
          iter++;
        };
      }

      if (pluginFound)
      {
        pluginHandle = ++lastPluginHandle;
        pluginMap[pluginHandle] = {pluginName, 0, nullptr, symbolMap_t()};
        pluginNameMap[pluginName] = pluginHandle;

        dlerror();
        pluginMap[pluginHandle].systemHandle = dlopen(fileNameAndPath.c_str(), flags);

        if (pluginMap[pluginHandle].systemHandle != nullptr)
        {
          GCL::logger::INFOMESSAGE("Loaded plugin: " + fileNameAndPath.filename().native());
          pluginMap[pluginHandle].pluginRefCount++;
        }
        else
        {
          char *error = dlerror();
          RUNTIME_ERROR(std::string(error), E_PLUGINMANAGER_UNABLETOMAP, LIBRARYNAME);
          RUNTIME_ERROR(boost::locale::translate("PluginManager: Unable to load plugin: " + fileNameAndPath.native()));
        }

        if (!alias.empty())
        {
          aliasMap[alias] = pluginHandle;
        }
      }
      else
      {
        //RUNTIME_ERROR(fmt::format(fmt::runtime(boost::locale::gettext("Unable to open configuration file {:s}.")), filename_));
        RUNTIME_ERROR(fmt::format(fmt::runtime(boost::locale::gettext("PluginManager: Plugin {:s} not found.")), pluginToLoad));
      };

#endif // unix type platforms

    };

    return pluginHandle;
  }

  /// @brief      Maps a symbol in the specified plugin.
  /// @param[in]  pluginHandle: The plugin to query.
  /// @param[in]  symbol: The symbol to query.
  /// @param[in]  cacheSymbol: true if the symbol should be cached.
  /// @returns    pointer to the symbol (or nullptr)
  /// @throws     GCL::runtime_error
  /// @throws     std::out_of_range - If the pluginHandle is invalid.

  void *CPluginManager::mapSymbol(pluginHandle_t pluginHandle,  std::string const &symbol, bool cacheSymbol)
  {
    void *returnValue;
    symbolMap_t::iterator existingSymbol;

    if (pluginMap.contains(pluginHandle))
    {
      if (pluginMap[pluginHandle].symbolMap.contains(symbol))
      {
        returnValue = pluginMap[pluginHandle].symbolMap[symbol];
      }
      else
      {

#if defined(unix) || defined(__unix__) || defined(__unix) || defined (__linux__)


        dlerror();
        returnValue = dlsym(pluginMap[pluginHandle].systemHandle, symbol.c_str());

        if (returnValue == nullptr)
        {
          char *error = dlerror();
          RUNTIME_ERROR(std::string(error), E_PLUGINMANAGER_UNABLETOMAP, LIBRARYNAME);
        }
        else if (cacheSymbol)
        {
          pluginMap[pluginHandle].symbolMap[symbol] = returnValue;
        };

#endif // unix type platforms
      };
    }
    return returnValue;
  }

  void *CPluginManager::mapSymbol(std::string const &pluginAlias, std::string const &symbol, bool cacheSymbol)
  {
    return mapSymbol(aliasMap[pluginAlias], symbol, cacheSymbol);
  }

  /// @brief      Unload all the loaded plugins. (Called by the destructor, can also be called apart from the destructor.)
  /// @version    2023-03-28/GGB - Function created.

  void CPluginManager::unloadAll()
  {
    for (auto &plugin : pluginMap)
    {
      if (--plugin.second.pluginRefCount == 0)
      {
        logger::DEBUGMESSAGE(fmt::format("Closing plugin: {:s}", plugin.second.pluginName));
        dlerror();
        int error = dlclose(plugin.second.systemHandle);
        if (error == 0)
        {
          plugin.second.symbolMap.clear();
          logger::DEBUGMESSAGE(fmt::format("Plugin Closed: {:s}", plugin.second.pluginName));
        }
        else
        {
          char *error = dlerror();
          RUNTIME_ERROR(std::string(error), E_PLUGINMANAGER_UNABLETOUNLOAD, LIBRARYNAME);
        }
      };
    }
  }

}   // namespace GCL::plugin
