include(${CMAKE_SOURCE_DIR}/build/cmake/target/vars.cmake)

set(COMMON_CFLAGS "${COMMON_CFLAGS} -fpic")
set(COMMON_SFLAGS "${COMMON_SFLAGS} -fpic")

set(CMAKE_C_FLAGS "${TARGET_CFLAGS}")
message("CMAKE_C_FLAGS ${CMAKE_C_FLAGS}")