# GCL Library CMAKE file

CMAKE_MINIMUM_REQUIRED(VERSION 3.14 FATAL_ERROR)

PROJECT(GCL DESCRIPTION "General Class Library")

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED True)

SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

set(SOURCES
  source/common.cpp
  source/dateTime.cpp
  source/error.cpp
  source/filesystem.cpp
  source/Functions.cpp
  source/resource.cpp
  source/SQLWriter.cpp
  source/string.cpp
  source/alarm/alarmCore.cpp
  source/alarm/alarmDaily.cpp
  source/alarm/alarmWeekly.cpp
  source/dataParser/dataDelimited.cpp
  source/dataParser/dataParserCore.cpp
  source/dataParser/dataParser_SAPBKG.cpp
  source/logger/fileSink.cpp
  source/logger/loggerCore.cpp
  source/logger/streamSink.cpp
  source/pluginManager/pluginManager.cpp)
set(HEADERS
  include/SQLWriter.h
  include/common.h
  include/config.h
  include/dateTime.h
  include/error.h
  include/filesystem.h
  include/resource.h)

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})


target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
