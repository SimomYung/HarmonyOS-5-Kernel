# Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: CMakeLists.txt
# Author: Huawei OS Kernel Lab
# Create: Wed Jan 05 19:29:45 2022

if (NOT BUILD_USE_HMBUILD)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DEBUG_C_FLAGS}")

hm_add_library(objs_visp_ppp OBJECT
    SOURCES
    ux_visp_ppp_netdev.c
    ux_visp_ppp_core.c

    COMPILER_FLAGS
    -fPIC
    -g
    -O0
    -std=gnu99
    -fno-asynchronous-unwind-tables
    -Wno-strict-prototypes
    -Wno-parentheses
    -Wno-unused-parameter
    ${CMAKE_STAGING_CFLAGS}

    PRIVATE_INCLUDES
    ${CMAKE_SOURCE_DIR}/ulibs/libsocket/include
    ${CMAKE_SOURCE_DIR}/ulibs/libuxnet/include
    ${CMAKE_SOURCE_DIR}/ulibs/libevent/include
)
else()
include(HongmengCMakeNG/HMbuild)
endif()
