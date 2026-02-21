# Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: CMakeLists.txt
# Author: Huawei OS Kernel Lab
# Create: Wed Jan 05 19:29:04 2022

if (NOT BUILD_USE_HMBUILD)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DEBUG_C_FLAGS}")

hm_add_library(objs_visp OBJECT
    SOURCES
    ux_libvisp.c
    ux_visp_netdev.c
    ux_visp_socket.c
    ux_visp_arp.c
    ux_visp_netstat.c
    ux_visp_route.c

    COMPILER_FLAGS
    -g
    -Os
    -fPIC
    -std=gnu99
    -fno-asynchronous-unwind-tables
    -Wno-strict-prototypes
    -Wno-parentheses
    -Wno-unused-parameter
    -Wno-missing-prototypes
    -Wno-implicit-function-declaration
    -Wno-missing-declarations
    -Wno-old-style-definition
    -Wno-strict-aliasing

    PRIVATE_INCLUDES
    ${CMAKE_SOURCE_DIR}/ulibs/libuxnet/include
    ${CMAKE_SOURCE_DIR}/ulibs/libevent/include
    ${CMAKE_SOURCE_DIR}/ulibs/libsocket/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include

    INTERNAL_INCLUDES
    include
)
else()
include(HongmengCMakeNG/HMbuild)
endif()
