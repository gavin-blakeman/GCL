# GCL Library CMAKE file

CMAKE_MINIMUM_REQUIRED(VERSION 3.14 FATAL_ERROR)

PROJECT(GCL DESCRIPTION "General Class Library")

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT DEFINED SHARED_LIBRARY_DIR)
set(SHARED_LIBRARY_DIR  ${CMAKE_BINARY_DIR})
endif()

SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

set(SOURCES
  source/common.cpp
  source/dateTime.cpp
  source/error.cpp
  source/filesystem.cpp
  source/functions.cpp
  source/functionTrace.cpp
  source/resource.cpp
  source/SQLWriter.cpp
  source/stringFunctions.cpp
  source/tempFileManager.cpp
  source/alarm/alarmCore.cpp
  source/alarm/alarmDaily.cpp
  source/alarm/alarmWeekly.cpp
  source/dataParser/dataDelimited.cpp
  source/dataParser/dataLexerXLSX.cpp
  source/dataParser/dataParserCore.cpp
  source/dataParser/dataParserHTML.cpp
  source/dataParser/dataParser_SAPBKG.cpp
  source/dataParser/dataTables.cpp
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
  source/parsers/DOM/DOMNodeAttribute.cpp
  source/parsers/DOM/DOMNodeDocument.cpp
  source/parsers/DOM/DOMNodeBase.cpp
  source/parsers/DOM/DOMNodeElement.cpp
#  source/parsers/html/htmlParser.cpp
  source/parsers/html/htmlTokeniser.cpp
  source/parsers/html/htmlTokens.cpp
  source/parsers/token.cpp
  source/pluginManager/pluginManager.cpp
  source/utf.cpp
  )

set(HEADERS
  GCL
  include/functions.h
  include/any.hpp
  include/common.h
  include/functionTrace.h
  include/SQLWriter.h
  include/dataParser/dataDelimited.h
  include/dataParser/dataLexerXLSX.h
  include/dataParser/dataParserCore.h
  include/dataParser/dataParserHTML.h
  include/dataParser/dataParser_SAPBKG.h
  include/dataParser/dataTables.h
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
  include/parsers/DOM/DOMNodeAttribute.h
  include/parsers/DOM/DOMNodeBase.h
  include/parsers/DOM/DOMNodeComment.h
  include/parsers/DOM/DOMNodeElement.h
  include/parsers/DOM/DOMNodeText.h
  include/parsers/html/htmlTokeniser.h
  include/parsers/html/htmlParser.h
  include/parsers/html/htmlTokens.h
  include/parsers/lexer.hpp
  include/parsers/token.h
  include/parsers/languageTokens.h
  include/pluginManager/pluginManager.h
  include/resource.h
  include/traits.h
  include/tempFileManager.h
  include/sqlWriter_typedef.inc
  include/sqlWriter_variables.inc
  include/utf/utf.h
  )

set(DIRECTORIES
  ${CMAKE_SOURCE_DIR}/MCL
  ${CMAKE_SOURCE_DIR}/SCL
  ${CMAKE_SOURCE_DIR}/fmt/include
  ${CMAKE_SOURCE_DIR}/xlnt/include
  ${Boost_INCLUDE_DIRS}
)

set(LIBRARIES
  Boost::locale
  fmt
  xlnt
)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} PRIVATE ${DIRECTORIES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

add_subdirectory(test)
