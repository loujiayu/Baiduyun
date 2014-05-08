project(baiduyun)

cmake_minimum_required(VERSION 2.8)

# Options. Turn on with 'cmake -Dmyvarname=ON'.
option(test "Build all tests." OFF)

find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(CURL REQUIRED)

set(CMAKE_CXX_FLAGS "-g -Wall -std=gnu++11")

add_library(baiduyun_lib
  src/Pcs.cc
  src/JsonEntry.cc
  src/Http.cc
  src/filetrans.cc
  src/filesystem.cc
  src/config.cc
)

target_link_libraries (baiduyun_lib
  ${Boost_LIBRARIES}
  ${CURL_LIBRARIES}
  crypto
  json-c
)

add_executable( baiduyun
	src/main.cc
)

target_link_libraries( baiduyun
  baiduyun_lib
)

if (test)
  add_subdirectory(tools/gtest-1.7.0)

  enable_testing()

  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

  add_executable(tests src/filetrans_test.cc)

  target_link_libraries(tests gtest gtest_main)

  target_link_libraries(tests baiduyun_lib)

  add_test(NAME unit_tset COMMAND tests)

endif()
