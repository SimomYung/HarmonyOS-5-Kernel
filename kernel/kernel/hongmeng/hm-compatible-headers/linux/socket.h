/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Add linux/socket to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 24 15:13:01 2021
 */

#ifndef _LINUX_SOCKET_H
#define _LINUX_SOCKET_H

typedef unsigned short __kernel_sa_family_t;
typedef __kernel_sa_family_t    sa_family_t;

#endif
