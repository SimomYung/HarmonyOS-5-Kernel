/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: The security interface of reserved port
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 23 14:51:31 2019
 */

#include <arpa/inet.h>
#include <sys/socket.h>

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_mem.h>

#include "security/ux_security.h"
#include "security/security.h"
#include <libsecplc/policy_fmt.h>
#include "security/reserved_port_range.h"
#include "security/net_bind_service.h"
#include "vsock/ux_vsock_common.h"
#include <generated/hmsd/hook/net/reserved_port_range.h>
#include <generated/hmsd/hook/net/net_bind_service.h>

#define LOCAL_PORT_RANGE_START_PORT	1024

static int check_inet_port_policy(uint16_t port)
{
	sec_plc_port_range_t port_range = {0};
	int ret;

	if (port != 0 && port < LOCAL_PORT_RANGE_START_PORT) {
		ret = sec_chk_net_bind_service();
		if (ret != E_HM_OK) {
			return ret;
		}
	}

	if (port == 0 || !ux_sec_rsvport_check(port)) {
		return 0;
	}

	port_range.start = port;
	port_range.end = port;

	return sec_chk_reserved_port_range(&port_range);
}

static int check_vsock_port_policy(unsigned int port)
{
	if (port <= LAST_RESERVED_PORT) {
		return sec_chk_net_bind_service();
	} else {
		return 0;
	}
}

int check_port_policy(const struct sockaddr_storage *sockaddr)
{
	if (sockaddr->ss_family == AF_INET6) {
		struct sockaddr_in6 *socketaddr = (struct sockaddr_in6 *)(void *)sockaddr;

		return check_inet_port_policy(ntohs(socketaddr->sin6_port));
	} else if (sockaddr->ss_family == AF_INET) {
		struct sockaddr_in *socketaddr = (struct sockaddr_in *)(void *)sockaddr;

		return check_inet_port_policy(ntohs(socketaddr->sin_port));
	} else if (sockaddr->ss_family == AF_VSOCK) {
		struct sockaddr_vm *vm_addr = (struct sockaddr_vm*)sockaddr;

		return check_vsock_port_policy(vm_addr->svm_port);
	} else {
		return 0;
	}
}
