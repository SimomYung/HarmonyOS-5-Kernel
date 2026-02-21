# Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: CMakeLists.txt
# Author: Huawei OS Kernel Lab
# Create: Wed Jan 05 19:28:30 2022

if (NOT BUILD_USE_HMBUILD)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DEBUG_C_FLAGS}")

set(VHOST_SRC
    ux_vhost_genl.c
)

if (CONFIG_VIRTIO_VSOCK)
    list(APPEND VHOST_SRC ux_vhost_vsock.c)
endif()

if (CONFIG_VHOST_VNET)
    list(APPEND VHOST_SRC ux_vhost_vnet.c)
endif()

hm_add_library(objs_vhost OBJECT
    SOURCES
    ${VHOST_SRC}

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
