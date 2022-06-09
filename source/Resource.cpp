//*********************************************************************************************************************************
//
// PROJECT:							General Class Library (GCL)
// FILE:								Resource Utilisations (CPU and Memory)
// SUBSYSTEM:						General Class Library Exception subsystem
// LANGUAGE:						C++
// TARGET OS:						All (Standard C++)
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2015-2022 Gavin Blakeman.
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
// OVERVIEW:            A class for determining resource utilisations.
//
// CLASSES INCLUDED:    CResourceUsage
//
// FUNCTIONS INCLUDED:  None
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2015-01-02/GGB - File created.
//
//*********************************************************************************************************************************

#include "include/Resource.h"

#include <cstdlib>
#include <cstdio>

#if defined(_WIN32)

#include "windows.h"
#include "TCHAR.h"
#include "pdh.h"

#elif defined(__linux__)

#include "sys/types.h"
#include "sys/sysinfo.h"

#elif defined(__MACH__)

#include "sys/types.h"
#include "sys/sysctl.h"
#include "mach/vm_statistics.h"
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>

#else
#endif

namespace GCL
{

  /// @brief Constructor. Must initialise certain values.
  /// @throws None.
  /// @version 2015-01-02/GGB - Function created.

  CResourceUsage::CResourceUsage()
  {
#if defined(_WIN32)

    PdhOpenQuery(NULL, NULL, &cpuQuery);
    PdhAddCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
    PdhCollectQueryData(cpuQuery);

#elif defined(__linux__)

    FILE* file = fopen("/proc/stat", "r");
    fscanf(file, "cpu %Ld %Ld %Ld %Ld", &lastTotalUser, &lastTotalUserLow, &lastTotalSys, &lastTotalIdle);
    fclose(file);

#elif defined(__MACH__)
#else

#endif
  }

  /// @brief Determine the CPU usage.
  /// @throws None.
  /// @version 2015-01-02/GGB - Function created.

  void CResourceUsage::determineCPU()
  {
#if defined(_WIN32)
    PDH_FMT_COUNTERVALUE counterVal;

    PdhCollectQueryData(cpuQuery);
    PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);

    percentCPU_ = counterVal.doubleValue;
#elif defined (__linux__)
    FILE *file;
    size_t totalUser;
    size_t totalUserLow;
    size_t totalSys;
    size_t totalIdle;
    size_t total;

    file = fopen("/proc/stat", "r");
    fscanf(file, "cpu %Lu %Lu %Lu %Lu", &totalUser, &totalUserLow, &totalSys, &totalIdle);
    fclose(file);

    if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow || totalSys < lastTotalSys || totalIdle < lastTotalIdle)
    {
      //Overflow detection. Just skip this value.

      percentCPU_ = -1.0;
     }
    else
    {
      total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) + (totalSys - lastTotalSys);
      percentCPU_ = total;
      total += (totalIdle - lastTotalIdle);
      percentCPU_ /= total;
      percentCPU_ *= 100;
    };

    lastTotalUser = totalUser;
    lastTotalUserLow = totalUserLow;
    lastTotalSys = totalSys;
    lastTotalIdle = totalIdle;

#elif defined(__MACH__)
#else
    percentCPU_ = 0;
#endif
  }

  /// @brief Determine the memory usage.
  /// @throws None.
  /// @version 2015-01-02/GGB - Function created.

  void CResourceUsage::determineMemory()
  {
#if defined(_WIN32)

    MEMORYSTATUSEX memInfo;

    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);

    totalRAM_ = memInfo.ullTotalPhys;
    usedRAM_ = memInfo.ullTotalPhys - memInfo.ullAvailPhys;

    totalSwap = memInfo.ullTotalPageFile - totalRAM_;
    usedSwap = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile - usedRAM_;

#elif defined (__linux__)

    struct sysinfo memInfo;

    sysinfo(&memInfo);

    totalRAM_ = memInfo.totalram;
    totalRAM_ *= memInfo.mem_unit;

    usedRAM_ = memInfo.totalram - memInfo.freeram;
    usedRAM_ *= memInfo.mem_unit;

    totalSwap_ = memInfo.totalswap;
    totalSwap_ *= memInfo.mem_unit;

    usedSwap_ = memInfo.totalswap - memInfo.freeswap;
    usedSwap_ *= memInfo.mem_unit;

#elif defined(__MACH__)

    int mib[2];
    int64_t physical_memory;
    mib[0] = CTL_HW;
    mib[1] = HW_MEMSIZE;
    length = sizeof(int64_t);
    sysctl(mib, &physical_memory, &length, NULL, 0);
    totalRAM_ =static_cast<size_t>(physical_memory);

    vm_size_t page_size;
    mach_port_t mach_port;
    mach_msg_type_number_t count;
    vm_statistics64_data_t vm_stats;

    mach_port = mach_host_self();
    count = sizeof(vm_stats) / sizeof(natural_t);
    if (KERN_SUCCESS == host_page_size(mach_port, &page_size) && KERN_SUCCESS == host_statistics64(mach_port, HOST_VM_INFO,
                                            (host_info64_t)&vm_stats, &count))
    {
      long long free_memory = (int64_t)vm_stats.free_count * (int64_t)page_size;

      long long used_memory = ((int64_t)vm_stats.active_count + (int64_t)vm_stats.inactive_count + (int64_t)vm_stats.wire_count) *  (int64_t)page_size;
      printf("free memory: %lld\nused memory: %lld\n", free_memory, used_memory);
    }

#else

#endif

  }
}
