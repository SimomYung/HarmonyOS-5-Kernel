/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Header file for tun
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 18 15:34:21 2021
 */

#ifndef _UX_IF_TUN_H
#define _UX_IF_TUN_H

#include <bits/ioctl.h>

#ifdef __cplusplus
extern "C" {
#endif

#define AF_TUN		45

#define TUN_TUN_DEV 	0x0001
#define TUN_TAP_DEV	0x0002

#define TUN_PERSIST     0x0800

#define TUNSETIFF       _IOW('T', 202, int)
#define TUNSETPERSIST   _IOW('T', 203, int)
#define TUNSETOWNER     _IOW('T', 204, int)

#define IFF_TUN		0x0001
#define IFF_TAP		0x0002

#ifdef __cplusplus
}
#endif

#endif
