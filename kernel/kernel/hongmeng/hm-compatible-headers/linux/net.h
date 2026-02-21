/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add linux/net.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 05 17:19:35 2021
 */

#ifndef _UAPI_LINUX_NET_H
#define _UAPI_LINUX_NET_H

#define SYS_SOCKET	1
#define SYS_BIND	2
#define SYS_CONNECT	3
#define SYS_LISTEN	4
#define SYS_ACCEPT	5
#define SYS_GETSOCKNAME	6
#define SYS_GETPEERNAME	7
#define SYS_SOCKETPAIR	8
#define SYS_SEND	9
#define SYS_RECV	10
#define SYS_SENDTO	11
#define SYS_RECVFROM	12
#define SYS_SHUTDOWN	13
#define SYS_SETSOCKOPT	14
#define SYS_GETSOCKOPT	15
#define SYS_SENDMSG	16
#define SYS_RECVMSG	17
#define SYS_ACCEPT4	18

#endif
