# CMakeLists.txt
cmake_minimum_required (VERSION 2.8.12.2)
project (cview)
project (simplex)
project (cmonitor)
project (unit_tests)

file (GLOB __cview_sources "cview/*.cpp")
file (GLOB __cmonitor_sources  "cmonitor/*.cpp")
file (GLOB __unit_tests_sources "tests/unit/*.cpp" "tests/unit/cconsole/*.cpp" "tests/unit/cmonitor/*.cpp")


set (CXX_STANDARD "-std=c++14")
#                         -fsanitize=address
set (CXX_SANITIZER_FLAGS "-fsanitize=thread -fno-omit-frame-pointer")
#set (CXX_SANITIZER_FLAGS "")
set (CXX_COMMON_FLAGS "${CXX_STANDARD} -Wall -Wextra -Wdeprecated -pedantic ${CXX_SANITIZER_FLAGS} -isystem ${BOOST_HOME}/include -isystem ${BOOGLE_TEST_HOME}/include")
set (CMAKE_CXX_FLAGS_DEBUG   "${CXX_COMMON_FLAGS} -O0 -g3 -pg")
set (CMAKE_CXX_FLAGS_RELEASE "${CXX_COMMON_FLAGS} -O3")


add_definitions (-DBOOST_ALL_DYN_LINK)

include_directories ("${GOOGLE_TEST_HOME}/include")
include_directories ("${PROJECT_SOURCE_DIR}")
include_directories ("${BOOST_HOME}/include")
add_subdirectory (cconsole)
add_subdirectory (cmonitor/detail)

link_directories ("${GOOGLE_TEST_HOME}/lib")
link_directories ("${BOOST_HOME}/lib")


add_executable (cview ${__cview_sources})
target_link_libraries (cview cconsole)

add_executable (cmonitor  ${__cmonitor_sources})
target_link_libraries (cmonitor ccmonitor cconsole boost_system boost_filesystem pthread)

add_executable (unit_tests ${__unit_tests_sources})
target_link_libraries (unit_tests ccmonitor cconsole gtest boost_system boost_filesystem pthread)

add_executable (simplex "tests/long_runners/simplex.cpp")
target_link_libraries (simplex m)

install (TARGETS unit_tests simplex DESTINATION "${PROJECT_BINARY_DIR}/bin/tests")
install (TARGETS cmonitor cview DESTINATION "${PROJECT_BINARY_DIR}/bin")

