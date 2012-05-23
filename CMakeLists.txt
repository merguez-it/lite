cmake_minimum_required(VERSION 2.8)
project(lite)

add_definitions(-Wall)

if(DEBUG)
  set(CMAKE_C_FLAGS "-g -O0")
  set(CMAKE_CXX_FLAGS "-g -O0")
  set(CMAKE_VERBOSE_MAKEFILE ON)
endif()

enable_testing()
add_subdirectory(src)
add_subdirectory(test)
