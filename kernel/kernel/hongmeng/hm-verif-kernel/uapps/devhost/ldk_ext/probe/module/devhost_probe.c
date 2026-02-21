/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sat Aug 19 17:07:14 2023
 */

#include <stddef.h>
#include <linux/ptrace.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/securec.h>
#include <linux/errno.h>
#include <linux/types.h>

#include "devhost_probe.h"
#include "probe_api.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("devhost probe kernel module");
MODULE_VERSION("0.01");

#define COPY_RT_REGS(to, from)	do {									\
	BUG_ON(sizeof((to)->regs) != sizeof((from)->regs));						\
	memcpy_s(&((to)->regs[0]), sizeof((to)->regs), &((from)->regs[0]), sizeof((from)->regs));	\
	(to)->pc = (from)->pc;										\
	(to)->sp = (from)->sp;										\
	(to)->pstate = (from)->pstate;									\
	(to)->orig_x0 = (from)->orig_x0;								\
} while (0)

static void copy_to_regs(const struct hmpt_regs *hm_regs, struct pt_regs *regs)
{
	COPY_RT_REGS(regs, hm_regs);
}

static void copy_to_hmpt_regs(const struct pt_regs *regs, struct hmpt_regs *hm_regs)
{
	COPY_RT_REGS(hm_regs, regs);
}

static int handle_brk(struct hmpt_regs *hm_regs)
{
	struct pt_regs regs = {0};
	int err = 0;

	copy_to_regs(hm_regs, &regs);
	err = kprobe_breakpoint_handler(&regs, 0);
	if (err == 0) {
		copy_to_hmpt_regs(&regs, hm_regs);
	}

	return err;
}

static int handle_ss_brk(struct hmpt_regs *hm_regs)
{
	struct pt_regs regs = {0};
	int err = 0;

	copy_to_regs(hm_regs, &regs);
	err = kprobe_breakpoint_ss_handler(&regs, 0);
	if (err == 0) {
		copy_to_hmpt_regs(&regs, hm_regs);
	}

	return err;
}

static struct devhost_plat_probe_ops g_probe_ops = {
	.handle_breakpoint = handle_brk,
	.handle_ss_breakpoint = handle_ss_brk,
};

static int __init devhost_probe_init(void)
{
	int ret;

	/* register ops */
	ret = devhost_probe_init_ops(&g_probe_ops);
	if (ret < 0) {
		pr_err("devhost_probe_ops init failed, err=%d\n", ret);
	}

	return ret;
}

module_init(devhost_probe_init);
