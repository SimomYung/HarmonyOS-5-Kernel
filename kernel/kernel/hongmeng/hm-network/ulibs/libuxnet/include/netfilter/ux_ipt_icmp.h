/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: ICMP filter rule head file
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov  1 15:14:33 2021
 */

#ifndef _UX_IPT_ICMP_H_
#define _UX_IPT_ICMP_H_
#include <sys/types.h>

struct ux_ipt_icmp {
	u_int8_t type;
	u_int8_t code[2];
	u_int8_t invflags;
};

void init_icmp_mt(void);

#endif
