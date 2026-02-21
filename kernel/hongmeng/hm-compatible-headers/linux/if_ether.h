/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Add linux/if_ether.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 10 21:34:45 2022
 */
#ifndef _LINUX_IF_ETHER_H
#define _LINUX_IF_ETHER_H

#include <linux/types.h>
#include <netinet/if_ether.h>

#define ETH_P_ALL 0x0003 /* All packets */

#endif
