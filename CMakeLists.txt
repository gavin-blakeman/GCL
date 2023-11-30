# GCL Library CMAKE file

CMAKE_MINIMUM_REQUIRED(VERSION 3.14 FATAL_ERROR)

PROJECT(GCL DESCRIPTION "General Class Library")

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-fPIC")

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
  source/pluginManager/pluginManager.cpp
  )
set(HEADERS
  GCL
  include/Functions.h
  include/any.hpp
  include/common.h
  include/SQLWriter.h
  include/dataParser/dataDelimited.h
  include/dataParser/dataParserCore.h
  include/dataParser/dataParser_SAPBKG.h
  include/dateTime.h
  include/config.h
  include/filesystem.h
  include/alarm/alarmCore.h
  include/alarm/alarmDaily.h
  include/alarm/alarmWeekly.h
  include/error.h
  include/logger/loggerCore.h
  include/logger/fileSink.h
  include/logger/streamSink.h
  include/configurationReader/readerSections.hpp
  include/configurationReader/readerVanilla.hpp
  include/configurationReader/readerCore.hpp
  include/GCLError.h
  include/pluginManager/pluginManager.h
  include/resource.h
  include/traits.h
  include/sqlWriter_typedef.inc
  include/sqlWriter_variables.inc
  )

set(DIRECTORIES
  ../MCL
  ../SCL
  ${fmt_DIR}
  ${Boost_INCLUDE_DIRS}
  )

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME}
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                           PRIVATE ${DIRECTORIES})
