# Tests for GCL

CMAKE_MINIMUM_REQUIRED(VERSION 3.16 FATAL_ERROR)

PROJECT(GCL_tests)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(EXECUTABLE_OUTPUT_PATH ${LIBRARY_OUTPUT_PATH}/test)

SET(Boost_USE_STATIC_LIBS ON)
SET(Boost_USE_MULTITHREADED ON)

find_package(Boost CONFIG)
find_package(Boost REQUIRED COMPONENTS locale thread unit_test_framework)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}
	../
	${CMAKE_SOURCE_DIR}/SCL
	)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})


set(SOURCES
  main.cpp
  parsers/test_htmlLexer.cpp
  parsers/test_lexer.cpp  
  test_dateTime.cpp
  test_error.cpp
  test_filesystem.cpp
  test_sqlWriter.cpp
  test_string.cpp
  test_utf.cpp
  )

set(LIBRARIES
  GCL
  fmt
  mpfr
  ${Boost_LIBRARIES}
  )

LINK_DIRECTORIES(${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME} ${SOURCES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


ENABLE_TESTING()
ADD_TEST(${PROJECT_NAME} ${PROJECT_NAME})

ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} COMMENT "Run tests" POST_BUILD  COMMAND ${PROJECT_NAME})
