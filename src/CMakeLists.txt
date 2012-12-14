project(LITE_LIB)
include_directories(${LITE_SOURCE_DIR}/include)

set(LITE_SOURCES
  data.cc
  token.cc
  lite.cc
  )
add_library(lite SHARED ${LITE_SOURCES})
add_library(lite-static STATIC ${LITE_SOURCES})

set(HS ${LITE_SOURCE_DIR}/include/lite.h)
install(FILES ${HS} DESTINATION include)
install(TARGETS lite lite-static
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static)
