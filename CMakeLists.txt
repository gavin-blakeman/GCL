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
  source/dataParser/dataLexerXLSX
  source/dataParser/dataParserCore.cpp
  source/dataParser/dataParser_SAPBKG.cpp  
  source/logger/loggerManager.cpp
  source/logger/loggerCore.cpp
  source/logger/filters/baseFilter.cpp
  source/logger/filters/debugFilter.cpp
  source/logger/queues/baseQueue.cpp
  source/logger/queues/queueQueue.cpp
  source/logger/records/baseRecord.cpp
  source/logger/records/debugRecord.cpp
  source/logger/sinks/baseSink.cpp
  source/logger/sinks/fileSink.cpp
  source/logger/sinks/streamSink.cpp
  source/pluginManager/pluginManager.cpp
  )
set(HEADERS
  GCL
  include/Functions.h
  include/any.hpp
  include/common.h
  include/SQLWriter.h
  include/dataParser/dataDelimited.h
  include/dataParser/dataLexerXLSX.h
  include/dataParser/dataParserCore.h
  include/dataParser/dataParser_SAPBKG.h
  include/dateTime.h
  include/config.h
  include/filesystem.h
  include/alarm/alarmCore.h
  include/alarm/alarmDaily.h
  include/alarm/alarmWeekly.h
  include/error.h
  include/logger/loggerManager.h
  include/logger/loggerCore.h
  include/logger/filters/baseFilter.h
  include/logger/filters/debugFilter.h
  include/logger/queues/baseQueue.h
  include/logger/queues/queueQueue.h
  include/logger/records/baseRecord.h
  include/logger/records/debugRecord.h
  include/logger/sinks/baseSink.h
  include/logger/sinks/fileSink.h
  include/logger/sinks/streamSink.h
  include/configurationReader/readerSections.hpp
  include/configurationReader/readerVanilla.hpp
  include/configurationReader/readerCore.hpp
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
set(LIBRARIES
	Boost::locale
	)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME}
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                           PRIVATE ${DIRECTORIES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

#ADD_SUBDIRECTORY(test)