project(baiduyun)

cmake_minimum_required(VERSION 2.8)

find_package(Boost COMPONENTS system filesystem REQUIRED)

list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")

add_executable( baiduyun
	src/main.cc
	src/Pcs.cc
	src/JsonEntry.cc
	src/Http.cc
  src/filetrans.cc
  src/filesystem.cc
 )
target_link_libraries( baiduyun
  ${Boost_LIBRARIES}
	curl
	json-c)
