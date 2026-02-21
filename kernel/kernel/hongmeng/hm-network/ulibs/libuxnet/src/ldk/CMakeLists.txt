if (NOT BUILD_USE_HMBUILD)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DEBUG_C_FLAGS}")

hm_add_library(objs_ldk OBJECT
    SOURCES
    ux_libldk.c
    ux_ldk_socket.c

    COMPILER_FLAGS
    -g
    -Os
    -fPIC
    -std=gnu99
    -fno-asynchronous-unwind-tables
    -Wno-strict-prototypes
    -Wno-parentheses
    -Wno-unused-parameter
    ${CMAKE_STAGING_CFLAGS}

    PRIVATE_INCLUDES
    ${CMAKE_SOURCE_DIR}/ulibs/libuxnet/include
    ${CMAKE_SOURCE_DIR}/ulibs/libevent/include
    ${CMAKE_SOURCE_DIR}/ulibs/libsocket/include
)
else()
include(HongmengCMakeNG/HMbuild)
endif()
