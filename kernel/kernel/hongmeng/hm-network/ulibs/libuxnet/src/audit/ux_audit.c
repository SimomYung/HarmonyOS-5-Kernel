/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Netlink route operations
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 27 11:56:32 2021
 */
#include <libstrict/strict.h>
#include <libhwsecurec/securec.h>
#include <libhmaudit/audit.h>

#include "ux_netlog.h"
#include "ux_audit.h"

void ux_audit_fill_sock_addr(const char *addr, socklen_t addrlen)
{
	struct audit_ctx *ctx = NULL;

	if (addr == NULL || addrlen == 0) {
		return;
	}

	if (addrlen > AUDIT_SOCK_ADDR_LEN) {
		net_debug(HMNET_CORE, "audit: addrlen too large\n");
		return;
	}

	ctx = audit_ctx_get();
	if (!audit_ctx_is_valid()) {
		return;
	}
	if (ctx->sock.addr_len == 0) {
		if (memcpy_s(ctx->sock.addr, AUDIT_SOCK_ADDR_LEN, addr, addrlen) != 0) {
			net_error(HMNET_CORE, "audit: memcpy socket address failed\n");
			return;
		}
		ctx->sock.addr_len = addrlen;
	} else {
		net_error(HMNET_CORE, "context socket address is not NULL\n");
	}
}

void ux_audit_fill_fd(int fd1, int fd2)
{
	struct audit_ctx *ctx = NULL;

	ctx = audit_ctx_get();
	if (!audit_ctx_is_valid()) {
		return;
	}
	ctx->sock.fd[0] = fd1;
	ctx->sock.fd[1] = fd2;
}
