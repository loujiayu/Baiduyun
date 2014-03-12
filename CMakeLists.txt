project(baiduyun)
cmake_minimum_required(VERSION 2.8)

add_executable( baiduyun 
	src/main.cc
	src/Pcs.cc
	src/JsonEntry.cc
	src/Http.cc
 )
target_link_libraries( baiduyun 
	curl
	json-c) 