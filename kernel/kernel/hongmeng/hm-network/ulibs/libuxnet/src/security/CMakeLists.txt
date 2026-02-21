# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: CMakeLists.txt
# Author: Huawei OS Kernel Lab
# Create: Wed Nov 03 14:10:48 2021

if (NOT BUILD_USE_HMBUILD)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DEBUG_C_FLAGS}")

hm_add_library(objs_security OBJECT
    SOURCES
    sec_chk_def.c
    security.c
    ability_reserved_port_range.c
    ability_raw_socket.c
    ability_net_admin.c
    ability_net_bind.c
    ability_net_dev.c
    ability_net_fs.c
    ability_vsock.c
    ability_net_stack_init.c
    ability_as_devhost.c
    ability_as_netd.c
    sec_reserved_ports.c
    net_posix_cap.c
    ux_rsvport.c

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
    ${CMAKE_SOURCE_DIR}/ulibs/libsocket/include
    ${CMAKE_SOURCE_DIR}/ulibs/libevent/include

    INTERNAL_INCLUDES
    include
)
else()
include(HongmengCMakeNG/HMbuild)
endif()
