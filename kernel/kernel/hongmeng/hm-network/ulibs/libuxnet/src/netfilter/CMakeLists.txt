# Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: CMakeLists.txt
# Author: Huawei OS Kernel Lab
# Create: Wed Jan 05 19:25:48 2022

if (NOT BUILD_USE_HMBUILD)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DEBUG_C_FLAGS}")

if (CONFIG_AUDIT)
    set(AUDIT_FILE ux_ipt_audit.c)
endif()

hm_add_library(objs_netfilter OBJECT
    SOURCES
    ux_netfilter.c
    ux_iptables.c
    ux_filter_table.c
    ux_ipt_tcpudp.c
    ux_ipt_icmp.c
    ux_ipt_conntrack.c
    ux_ipt_reject.c
    ux_conntrack_core.c
    ux_conntrack_impl.c
    ux_conntrack_proto_generic.c
    ux_conntrack_proto_tcp.c
    ux_conntrack_proto_udp.c
    ux_conntrack_proto_icmp.c
    ux_conntrack_timer.c
    ${AUDIT_FILE}

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
    ${CMAKE_SOURCE_DIR}/ulibs/libuxnet/include/netfilter
    ${CMAKE_SOURCE_DIR}/ulibs/libuxnet/include
    ${CMAKE_SOURCE_DIR}/ulibs/libevent/include
    ${CMAKE_SOURCE_DIR}/ulibs/libsocket/include

    INTERNAL_INCLUDES
    include
)
else()
include(HongmengCMakeNG/HMbuild)
endif()
