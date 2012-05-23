include(FindGTest)
include(CheckFunctionExists)

CHECK_FUNCTION_EXISTS(pthread_create HAVE_PTHREAD)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  # TODO : detect -pthread compiler option, and use it if available
  if(HAVE_PTHREAD)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
  endif()
endif()

include_directories(${PROJECT_SOURCE_DIR}/include ${GTEST_INCLUDE_DIRS})

add_executable(util_unittest "util_unittest.cc")
target_link_libraries(util_unittest lite-static ${GTEST_BOTH_LIBRARIES}) 
add_test(UTIL_UNITTEST util_unittest)

add_executable(data_unittest "data_unittest.cc")
target_link_libraries(data_unittest lite-static ${GTEST_BOTH_LIBRARIES}) 
add_test(DATA_UNITTEST data_unittest)

add_executable(token_unittest "token_unittest.cc")
target_link_libraries(token_unittest lite-static ${GTEST_BOTH_LIBRARIES}) 
add_test(TOKEN_UNITTEST token_unittest)

add_executable(lite_unittest "lite_unittest.cc")
target_link_libraries(lite_unittest lite-static ${GTEST_BOTH_LIBRARIES}) 
add_test(LITE_UNITTEST lite_unittest)

