/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description:
 * Author:
 * Create: 2021-07-16
 */
#ifndef __NPU_SYSCACHE_H
#define __NPU_SYSCACHE_H

#include <linux/types.h>
#include <linux/timer.h>
#include <linux/mm_lb/mm_lb.h>
#include "npu_proc_ctx.h"
#include "npu_common.h"

#define NPU_PGID_NUM_MAX 3

/* map runtime pgid idx to real pgid type */
#ifdef CONFIG_MM_LB_V550
static uint32_t g_syscahce_pgid[NPU_PGID_NUM_MAX] = {
	PID_AIRAW, PID_NPU, PID_NPU_LOWPRIO,
};
#endif

int npu_ioctl_attach_syscache(struct npu_proc_ctx *proc_ctx,
	unsigned long arg);
int npu_ioctl_detach_syscache(struct npu_proc_ctx *proc_ctx,
	unsigned long arg);

void npu_sc_timeout(struct timer_list *syscache_timer);
void npu_sc_enable(struct npu_dev_ctx *dev_ctx);
void npu_sc_enable_and_high(struct npu_dev_ctx *dev_ctx);
void npu_sc_low(struct npu_dev_ctx *dev_ctx);
void npu_sc_disable(struct npu_dev_ctx *dev_ctx);

#endif
