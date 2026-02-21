# Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: CMakeLists.txt
# Author: Huawei OS Kernel Lab
# Create: Wed Jan 05 19:25:10 2022

if (NOT BUILD_USE_HMBUILD)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DEBUG_C_FLAGS}")

hm_add_library(objs_lwip OBJECT
    SOURCES
    ux_liblwip.c
    ux_lwip_netdev.c
    ux_lwip_socket.c

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
    ${HMSDKINCLUDE}/hmnet/lwip/include
    ${HMSDKINCLUDE}/hmnet/lwip/config
    ${HMSDKINCLUDE}/hmnet/lwip/contrib/ports/hm/include
    ${CMAKE_SOURCE_DIR}/ulibs/libuxnet/include
    ${CMAKE_SOURCE_DIR}/ulibs/libevent/include
    ${CMAKE_SOURCE_DIR}/ulibs/libsocket/include

    INTERNAL_INCLUDES
    include
)
else()
include(HongmengCMakeNG/HMbuild)
endif()
