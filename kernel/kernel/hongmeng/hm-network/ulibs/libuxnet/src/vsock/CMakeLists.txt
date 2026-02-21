# Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: CMakeLists.txt
# Author: Huawei OS Kernel Lab
# Create: Wed Jan 05 19:30:23 2022

if (NOT BUILD_USE_HMBUILD)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DEBUG_C_FLAGS}")

hm_add_library(objs_vsock OBJECT
    SOURCES
    ux_vsock_trans.c
    ux_vsock_stream.c
    ux_vsock_socket.c
    ux_vsock_netdev.c
    ux_vsock_timer.c
    ux_vsock_actv_netdev.c
    ux_vsock_host.c

    COMPILER_FLAGS
    -fPIC
    -g
    -Os
    -std=gnu99
    -fno-asynchronous-unwind-tables
    -Wno-strict-prototypes
    -Wno-parentheses
    -Wno-unused-parameter

    PRIVATE_INCLUDES
    ${CMAKE_SOURCE_DIR}/ulibs/libsocket/include
    ${CMAKE_SOURCE_DIR}/ulibs/libuxnet/include
    ${CMAKE_SOURCE_DIR}/ulibs/libevent/include
    ${HMSDKINCLUDE}/generated/sysif_client/uvmm
)
else()
include(HongmengCMakeNG/HMbuild)
endif()
