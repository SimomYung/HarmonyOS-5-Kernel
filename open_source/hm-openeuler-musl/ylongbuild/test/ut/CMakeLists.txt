include(${CMAKE_SOURCE_DIR}/build/cmake/utils/executable_macro.cmake)

SET (TEST_FRAME_DIR ${CMAKE_SOURCE_DIR}/../../../test_frame)
SET (LIBRARY_OUTPUT_PATH ${TEST_FRAME_DIR}/gtest/lib)

INCLUDE_DIRECTORIES(
    ${TEST_FRAME_DIR}/gtest/include
    ${TEST_FRAME_DIR}/gtest/include/stub
    ${CMAKE_SOURCE_DIR}/src/hlog/include
)

LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

file(GLOB_RECURSE base_src CONFIGURE_DEPENDS "base/*.cc")

set(CMAKE_CXX_FLAGS "-g -O2")
set(CMAKE_CXX_FLAGS "-Wno-error=deprecated-declarations -Wno-deprecated-declarations ")
set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_C_COMPILER> -o <TARGET> ${CMAKE_CXX_FLAGS} <LINK_FLAGS> <OBJECTS> <LINK_LIBRARIES> -Wl,--start-group $ENV{STDCXXLIB_PATH}/libstdc++.a -Wl,--end-group")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} $ENV{PNF_LD_FLAGS} -rdynamic")

add_executable(ut_hlibc ${base_src} ${extend_src})

TARGET_LINK_LIBRARIES(ut_hlibc     libstub.a libgtest.a libgtest_main.a libgmock.a)

set(TEST_TARGET_NAME ut_hlibc)
install(TARGETS ${TEST_TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
