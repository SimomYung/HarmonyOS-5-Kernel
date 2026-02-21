/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add linux/gsmmux.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 17 15:03:34 2021
 */

#ifndef LINUX_GSMMUX_H
#define LINUX_GSMMUX_H

struct gsm_config {
	unsigned int adaption;
	unsigned int encapsulation;
	unsigned int initiator;
	unsigned int t1;
	unsigned int t2;
	unsigned int t3;
	unsigned int n2;
	unsigned int mru;
	unsigned int mtu;
	unsigned int k;
	unsigned int i;
	unsigned int unused[8];
};

#define GSMIOC_GETCONF		_IOR('G', 0, struct gsm_config)
#define GSMIOC_SETCONF		_IOW('G', 1, struct gsm_config)

#endif
