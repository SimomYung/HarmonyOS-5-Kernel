/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: timer for conntrack
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 29 15:39:58 2021
 */
#ifndef __HM_NET_UX_TIMER_H
#define __HM_NET_UX_TIMER_H

#include <lib/dlist.h>

typedef struct ux_ct_timer {
	unsigned long expire;
} ux_ct_timer_t;

void ux_timer_init(ux_ct_timer_t *timer,
		   unsigned long time, unsigned long interval);
int ux_ct_timer_create_init(void);

#endif

