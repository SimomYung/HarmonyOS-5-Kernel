# Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: CMakeLists.txt
# Author: Huawei OS Kernel Lab
# Create: Wed Jan 05 19:26:25 2022

if (NOT BUILD_USE_HMBUILD)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DEBUG_C_FLAGS}")

if (CONFIG_AUDIT)
    set(AUDIT_FILE ux_netlink_audit.c)
endif()

hm_add_library(objs_netlink OBJECT
    SOURCES
    ${AUDIT_FILE}
    ux_netlink_core.c
    ux_netlink_proto.c
    ux_netlink_table.c
    ux_netlink_route.c
    ux_netlink_generic.c
    modules/ux_netlink_uevent.c
    modules/ux_netlink_modules.c
    modules/ux_netlink_taskstats.c

    COMPILER_FLAGS
    -fPIC
    -g
    -Os
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
    ./

    INTERNAL_INCLUDES
    include
)
else()
include(HongmengCMakeNG/HMbuild)
endif()
