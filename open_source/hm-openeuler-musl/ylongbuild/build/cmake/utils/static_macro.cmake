include(${CMAKE_SOURCE_DIR}/build/cmake/target/vars.cmake)

set(CMAKE_C_FLAGS ${TARGET_CFLAGS} "-DEXPORT='__attribute__((visibility(\"default\")))' -DEXPORT_SYMBOL\\(x\\)=''")
