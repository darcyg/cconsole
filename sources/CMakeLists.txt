# CMakeLists.txt
cmake_minimum_required (VERSION 2.8.12.2)
project (cview)
project (cmonitor)
project (unit_tests)

file (GLOB __cview_sources "cview/*.cpp")
file (GLOB __cmonitor_sources  "cmonitor/*.cpp")
file (GLOB __unit_tests_sources "tests/unit/*.cpp")


set (CXX_STANDARD "-std=c++14")
set (CXX_COMMON_FLAGS "${CXX_STANDARD} -Wall -Wextra -Wdeprecated -pedantic -isystem ${BOOST_HOME}/include -isystem ${BOOGLE_TEST_HOME}/include")
set (CMAKE_CXX_FLAGS_DEBUG   "${CXX_COMMON_FLAGS} -O0 -g3 -pg")
set (CMAKE_CXX_FLAGS_RELEASE "${CXX_COMMON_FLAGS} -O3")


add_definitions (-DBOOST_ALL_DYN_LINK)

include_directories ("${PROJECT_SOURCE_DIR}/cconsole")
include_directories ("${GOOGLE_TEST_HOME}/include")
include_directories ("${PROJECT_SOURCE_DIR}")
include_directories ("${BOOST_HOME}/include")
#add_subdirectory (cconsole)

link_directories ("${GOOGLE_TEST_HOME}/lib")
link_directories ("${BOOST_HOME}/lib")


add_executable (cview ${__cview_sources})
#target_link_libraries (cview cconsole)

add_executable (cmonitor  ${__cmonitor_sources})
target_link_libraries (cmonitor boost_system pthread)

add_executable (unit_tests ${__unit_tests_sources})
target_link_libraries (unit_tests gtest pthread)

