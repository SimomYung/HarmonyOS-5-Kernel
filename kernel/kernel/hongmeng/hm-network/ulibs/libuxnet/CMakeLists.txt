# Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: CMakeLists.txt
# Author: Huawei OS Kernel Lab
# Create: Wed Jan 05 19:23:58 2022

if (NOT BUILD_USE_HMBUILD)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DEBUG_C_FLAGS}")

if (CONFIG_AUDIT)
    set(AUDIT_FILE src/audit/ux_audit.c)
endif()

if (CONFIG_UEV_TRACE)
    set(UEV_FILE src/uev/ux_uev.c)
endif()

if (CONFIG_UXNET_BPF)
    set(BPF_FILE
        src/bpf/ux_bpf_exec.c
        src/bpf/ux_bpf_filter.c
    )
endif()

if (CONFIG_UXNET_PACKET)
	set(PACKET_FILE src/packet/ux_packet.c)
endif()

hm_add_library(hmsrv_uxnet STATIC SSTACK_MEM
    SOURCES
    ${AUDIT_FILE}
    ${UEV_FILE}
    ${BPF_FILE}
    ${PACKET_FILE}
    src/core/ux_sockmgr.c
    src/core/ux_socket.c
    src/core/ux_sock.c
    src/core/ux_netdev.c
    src/core/ux_workqueue.c
    src/core/ux_domain.c
    src/core/ux_demux.c
    src/core/ux_ioctl.c
    src/core/ux_statistic.c
    src/core/ux_sockopt.c
    src/core/ux_eventpoll.c
    src/core/ux_phy_mem.c
    src/core/ux_pktbuf.c
    src/core/ux_netlog.c
    src/core/ux_compat.c
    src/core/ux_vlan.c
    src/core/ux_revoke_handler.c
    src/utils/ux_utils.c
    src/unix/ux_unix_util.c
    src/unix/ux_unix_stat.c
    src/unix/ux_unix_scm.c
    src/unix/ux_unix_socket.c

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
    ${CMAKE_SOURCE_DIR}/ulibs/libevent/include
    ${CMAKE_SOURCE_DIR}/ulibs/libsocket/include

    WHOLEARCHIVE
    ${DEBUG_EXT_LIBS}

    INTERNAL_INCLUDES
    include
)
hm_append_definitions(hmsrv_uxnet _GNU_SOURCE=1)

if (CONFIG_LWIP OR CONFIG_VISP)
add_subdirectory(src/${UXNET_OBJS})
target_link_libraries(hmsrv_uxnet objs_${UXNET_OBJS})
endif()

add_subdirectory(src/security)
target_link_libraries(hmsrv_uxnet objs_security)

if (CONFIG_UXNET_CAN)
add_subdirectory(src/can)
target_link_libraries(hmsrv_uxnet objs_can)
endif()

if (CONFIG_UXNET_VISP_PPP)
add_subdirectory(src/visp/visp_ppp)
target_link_libraries(hmsrv_uxnet objs_visp_ppp)
endif()

if (CONFIG_VIRTIO_VSOCK)
add_subdirectory(src/vsock)
target_link_libraries(hmsrv_uxnet objs_vsock)
endif()

if (CONFIG_VHOST)
add_subdirectory(src/vhost)
target_link_libraries(hmsrv_uxnet objs_vhost)
endif()

if (CONFIG_UXNET_NETFILTER)
add_subdirectory(src/netfilter)
target_link_libraries(hmsrv_uxnet objs_netfilter)
endif()

if (CONFIG_UXNET_NETLINK)
add_subdirectory(src/netlink)
target_link_libraries(hmsrv_uxnet objs_netlink)
endif()

if (CONFIG_UXNET_TUN)
add_subdirectory(src/tun)
target_link_libraries(hmsrv_uxnet objs_tun)
endif()

else()
include(HongmengCMakeNG/HMbuild)
endif()
