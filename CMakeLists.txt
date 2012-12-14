cmake_minimum_required(VERSION 2.8)
project(LITE)

include(FindDoxygen)
include(CheckFunctionExists)

set(LITE_OUTPUT_TEMP_DIR ${LITE_BINARY_DIR}/.build)

add_definitions(-Wall)
if(DEBUG)
  set(CMAKE_C_FLAGS "-g -O0")
  set(CMAKE_CXX_FLAGS "-g -O0")
  set(CMAKE_VERBOSE_MAKEFILE ON)
endif()

CHECK_FUNCTION_EXISTS(pthread_create HAVE_PTHREAD)

# Documentation
if(DOXYGEN_FOUND)
  find_program(GRAPHVIZ
    dot
    )
  if("${GRAPHVIZ}" STREQUAL "GRAPHVIZ-NOTFOUND")
    message(STATUS "GraphViz not found!")
    set(HAVE_DOT NO)
  else()
    message(STATUS "Found GraphViz: ${GRAPHVIZ}")
    set(HAVE_DOT YES)
  endif()
  configure_file(
    ${LITE_SOURCE_DIR}/Doxyfile.in 
    ${LITE_OUTPUT_TEMP_DIR}/Doxyfile
    )

  set(DOXYGEN_INPUT ${LITE_OUTPUT_TEMP_DIR}/Doxyfile)
  add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT})
endif()

enable_testing()
add_subdirectory(src)
add_subdirectory(ext)
add_subdirectory(test)
