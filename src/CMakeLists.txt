include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(lite SHARED lite.cc)
add_library(lite-static STATIC lite.cc)
