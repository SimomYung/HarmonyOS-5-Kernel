/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Add linux/in_route.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jun 24 09:38:03 2022
 */

#ifndef _LINUX_IN_ROUTE_H
#define _LINUX_IN_ROUTE_H

#define RTCF_NOTIFY	0x00010000
#define RTCF_DIRECTDST	0x00020000
#define RTCF_REDIRECTED	0x00040000

#define RTCF_DOREDIRECT 0x01000000
#define RTCF_DIRECTSRC	0x04000000
#define RTCF_DNAT	0x08000000
#define RTCF_BROADCAST	0x10000000
#define RTCF_MULTICAST	0x20000000
#define RTCF_LOCAL	0x80000000

#endif
