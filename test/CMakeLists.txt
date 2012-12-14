include(CheckFunctionExists)

set(LITE_TEST_PATH ${CMAKE_CURRENT_SOURCE_DIR})
configure_file(
  ${LITE_SOURCE_DIR}/config-test.h.in
  ${LITE_OUTPUT_TEMP_DIR}/config-test.h
  )

include_directories(
  ${LITE_OUTPUT_TEMP_DIR}
  ${LITE_EXTERN_SOURCE_DIR}/gtest-1.6.0/include
  ${LITE_SOURCE_DIR}/include
  )

CHECK_FUNCTION_EXISTS(pthread_create HAVE_PTHREAD)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  # TODO : detect -pthread compiler option, and use it if available
  if(HAVE_PTHREAD)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
  endif()
endif()

set(TEST_LIBS
  lite-static
  gtest
  gtest_main
  )

include_directories(${PROJECT_SOURCE_DIR}/include ${GTEST_INCLUDE_DIRS})

add_executable(data_unittest "data_unittest.cc")
target_link_libraries(data_unittest ${TEST_LIBS}) 
add_test(DATA_UNITTEST data_unittest)

add_executable(token_unittest "token_unittest.cc")
target_link_libraries(token_unittest ${TEST_LIBS}) 
add_test(TOKEN_UNITTEST token_unittest)

add_executable(lite_unittest "lite_unittest.cc")
target_link_libraries(lite_unittest ${TEST_LIBS}) 
add_test(LITE_UNITTEST lite_unittest)

