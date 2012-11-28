cmake_minimum_required (VERSION 2.8)
project(LITE_EXTERN)

include_directories(
  ${LITE_EXTERN_SOURCE_DIR}/gtest-1.6.0/include
  ${LITE_EXTERN_SOURCE_DIR}/gtest-1.6.0
  )

add_library(gtest ${LITE_EXTERN_SOURCE_DIR}/gtest-1.6.0/src/gtest-all.cc)
add_library(gtest_main ${LITE_EXTERN_SOURCE_DIR}/gtest-1.6.0/src/gtest_main.cc)

