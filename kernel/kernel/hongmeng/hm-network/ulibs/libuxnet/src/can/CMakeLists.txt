if (NOT BUILD_USE_HMBUILD)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DEBUG_C_FLAGS}")

hm_add_library(objs_can OBJECT
    SOURCES
    ux_can_core.c
    ux_can_proto.c
    ux_can_netdev.c
    ux_can_filter.c
    ux_can_raw.c
    ux_can_vcan.c

    COMPILER_FLAGS
    -fPIC
    -g
    -O0
    -std=gnu99
    -fno-asynchronous-unwind-tables
    -Wno-strict-prototypes
    -Wno-parentheses
    -Wno-unused-parameter

    PRIVATE_INCLUDES
    ${CMAKE_SOURCE_DIR}/ulibs/libsocket/include
    ${CMAKE_SOURCE_DIR}/ulibs/libuxnet/include
    ${CMAKE_SOURCE_DIR}/ulibs/libevent/include
)
else()
include(HongmengCMakeNG/HMbuild)
endif()
