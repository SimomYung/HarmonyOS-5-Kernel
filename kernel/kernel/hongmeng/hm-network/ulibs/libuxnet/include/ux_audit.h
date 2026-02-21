/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description:  Header file for netlink audit
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 27 11:56:32 2021
 */

#ifndef __UX_AUDIT_H_
#define __UX_AUDIT_H_

#include <sys/socket.h>
#include <libstrict/strict.h>

#ifdef CONFIG_AUDIT
void ux_audit_fill_sock_addr(const char *addr, socklen_t addrlen);
void ux_audit_fill_fd(int fd1, int fd2);
#else
static inline void ux_audit_fill_sock_addr(const char *addr, socklen_t addrlen)
{
	UNUSED(addr);
	UNUSED(addrlen);
}
static inline void ux_audit_fill_fd(int fd1, int fd2)
{
	UNUSED(fd1);
	UNUSED(fd2);
}

#endif
#endif
