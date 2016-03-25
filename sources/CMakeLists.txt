# CMakeLists.txt
cmake_minimum_required (VERSION 2.8.12.2)
project (cview)
project (cmonitor)
# TODONE: add test runner project


file (GLOB __cview_sources "cview/*.cpp")
file (GLOB __cmonitor_sources  "cmonitor/*.cpp")


set (CXX_STANDARD "-std=c++11")
set (CXX_COMMON_FLAGS "${CXX_STANDARD} -Wall -Wextra -Wdeprecated -pedantic")
set (CMAKE_CXX_FLAGS_DEBUG   "${CXX_COMMON_FLAGS} -O0 -g3 -pg")
set (CMAKE_CXX_FLAGS_RELEASE "${CXX_COMMON_FLAGS} -O3")

include_directories ("${PROJECT_SOURCE_DIR}")
add_executable (cview "${__cview_sources}")
add_executable (cmonitor  "${__cmonitor_sources}")

