/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Define structures for probe
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 14 09:40:18 2023
 */
#ifndef __DEVHOST_API_FILE_INFO_H__
#define __DEVHOST_API_FILE_INFO_H__

#if defined(MODULE)
#include <linux/types.h>
#else
#define u64 __u64
#include <stddef.h>
#endif

#if !defined(__aarch64__)
#error "only support aarch64"
#endif

struct hmpt_regs {
	u64 regs[31];
	u64 sp;
	u64 pc;
	u64 pstate;
	u64 orig_x0;
};

struct devhost_plat_probe_ops {
	int (*handle_breakpoint)(struct hmpt_regs *regs);
	int (*handle_ss_breakpoint)(struct hmpt_regs *regs);
};

int devhost_probe_init_ops(const struct devhost_plat_probe_ops *probe_ops);

#endif
