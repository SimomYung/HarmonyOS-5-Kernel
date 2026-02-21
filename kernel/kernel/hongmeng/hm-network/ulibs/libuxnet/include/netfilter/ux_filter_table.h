/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: filter table head file
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 22 20:35:17 2021
 */

#ifndef _UX_FILTER_TABLE_H_
#define _UX_FILTER_TABLE_H_

#include "ux_pktbuf.h"
#include "ux_iptables.h"
#include "ux_netfilter.h"

struct ux_netdev;

int init_filter_table(void);

int ux_ipt_hook(unsigned int hook,
		struct ux_pktbuf *pkt,
		struct ux_netdev *indev,
		struct ux_netdev *outdev);
#endif
