project(LITE_LIB)
include_directories(${LITE_SOURCE_DIR}/include)

add_library(lite SHARED lite.cc)
add_library(lite-static STATIC lite.cc)

set(HS ${LITE_SOURCE_DIR}/include/lite.h)
install(FILES ${HS} DESTINATION include)
install(TARGETS lite lite-static
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static)
