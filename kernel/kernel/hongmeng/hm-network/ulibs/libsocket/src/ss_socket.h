/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Header file of socket
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 24 11:22:46 2019
 */

#ifndef __LIBSOCKET_SS_SOCKET_H_
#define __LIBSOCKET_SS_SOCKET_H_

#define MAX_ARG_SIZE	0x1000

#ifndef access_ok
static inline unsigned int __range_ok(unsigned long addr, unsigned long size)
{
	(void)size;
	if ((addr == 0) || (addr == (unsigned long)-1)) {
		return 0;
	}
	return 1;
}

#define VERIFY_READ	0
#define VERIFY_WRITE	1

#define access_ok(type, addr, size) (__range_ok((unsigned long)(addr), (unsigned long)(size)))
#define access_bad(type, addr, size) (__range_ok((uintptr_t)(addr), (unsigned long)(size))==0)

#endif

#define SOCK_TYPE_MASK 0xf

#endif
