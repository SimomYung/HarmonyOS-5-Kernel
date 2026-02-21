include(${CMAKE_SOURCE_DIR}/build/cmake/machine/armv8a/vars.cmake)
include(${CMAKE_SOURCE_DIR}/build/cmake/gcc/vars.cmake)

add_compile_definitions(HLIBC_COMPILE_COMMIT_ID="$ENV{HLIBC_COMPILE_COMMIT_ID}")
message("HLIBC_COMPILE_COMMIT_ID $ENV{HLIBC_COMPILE_COMMIT_ID}")

set(TARGET_CFLAGS "${COMMON_CFLAGS} ${MACHINE_CFLAGS} -DEXPORT='__attribute__((visibility(\"default\")))' -DEXPORT_SYMBOL\\(x\\)=''")