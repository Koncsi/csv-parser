cmake_minimum_required(VERSION 2.6)
project(CSV-PARSER)
set(CMAKE_CXX_STANDARD 11)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(runTests tests.cpp CSVParser.cpp)
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)