# GCL Library CMAKE file

CMAKE_MINIMUM_REQUIRED(VERSION 3.14 FATAL_ERROR)

PROJECT(GCL)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED True)

SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/GCL)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR})

ADD_LIBRARY(GCL ${PROJECT_SOURCE_DIR}/source/common.cpp
                ${PROJECT_SOURCE_DIR}/source/dateTime.cpp
                ${PROJECT_SOURCE_DIR}/source/error.cpp
                ${PROJECT_SOURCE_DIR}/source/filesystem.cpp
                ${PROJECT_SOURCE_DIR}/source/Functions.cpp
                ${PROJECT_SOURCE_DIR}/source/resource.cpp
                ${PROJECT_SOURCE_DIR}/source/SQLWriter.cpp
                ${PROJECT_SOURCE_DIR}/source/string.cpp
                ${PROJECT_SOURCE_DIR}/source/dataParser/dataDelimited.cpp
                ${PROJECT_SOURCE_DIR}/source/logger/fileSink.cpp
                ${PROJECT_SOURCE_DIR}/source/logger/loggerCore.cpp
                ${PROJECT_SOURCE_DIR}/source/logger/streamSink.cpp)
