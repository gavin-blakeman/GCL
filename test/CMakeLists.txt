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

function(make_includable input_file output_file)
    file(READ ${input_file} content)
    set(delim "for_c++_include")
    set(content "R\"${delim}(\n${content})${delim}\"")
    file(WRITE ${output_file} "${content}")
endfunction(make_includable)

MAKE_INCLUDABLE(${PROJECT_SOURCE_DIR}/testFiles/testPage.html ${PROJECT_SOURCE_DIR}/generated/testPage.cpp)

set(DIRECTORIES
  ${CMAKE_SOURCE_DIR}/SCL
  ${Boost_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
)

set(SOURCES
  main.cpp
#  parsers/html/test_htmlAttributes.cpp
#  parsers/html/test_htmlLexer.cpp
#  parsers/html/test_htmlParser.cpp
#  parsers/test_lexer.cpp
  test_dateTime.cpp
  test_error.cpp
  test_filesystem.cpp
  test_functions.cpp
  test_sqlWriter.cpp
  test_stringFunctions.cpp
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
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} PRIVATE ${DIRECTORIES})

execute_process(COMMAND ${CMAKE_COMMAND} -E
  copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/testFiles/testPage.html ${CMAKE_BINARY_DIR}/testFiles)

ENABLE_TESTING()
add_test(${PROJECT_NAME} ${PROJECT_NAME})

add_custom_command(TARGET ${PROJECT_NAME} COMMENT "Run tests" POST_BUILD  COMMAND ${PROJECT_NAME})
