include(${CMAKE_SOURCE_DIR}/build/cmake/target/vars.cmake)

set(COMMON_CFLAGS "${COMMON_CFLAGS} -fpie")
set(COMMON_SFLAGS "${COMMON_SFLAGS} -fpie")

set(BUILD_C_CFLAGS ${COMMON_CFLAGS}  ${TARGET_CFLAGS})
set(BUILD_S_CFLAGS ${COMMON_SFLAGS})
set(CMAKE_EXE_LINKER_FLAGS "-rdynamic $ENV{PNF_LD_FLAGS}")

add_definitions(-DAARCH64)
add_definitions($ENV{PNF_CFLAGS})

set(CMAKE_C_FLAGS "-O2 -g")
set(CMAKE_CXX_FLAGS "-O2 -g")