// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 *
 * Description: Debug improper memory alloc behaviours
 * Author: Li Xilun <lixilun1@huawei.com>
 * Create: 2023-03-23
 */

#define pr_fmt(fmt) "improper_alloc: " fmt

#include <platform/linux/improper_alloc.h>

#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/seq_file.h>

#include <log/log_usertype.h>

#include <improper_alloc_core.h>

#define SKIP_ON_START 2

/*
 * Hook for allocation. There are two cases:
 * 1. Hook in __alloc_pages_nodemask, large_slab_size = 0
 * 2. Hook in kmalloc_order, large_slab_size != 0
 */
void improper_alloc_hook(gfp_t gfp_mask, unsigned int order, size_t large_slab_size)
{
	struct improper_alloc *type = NULL;
	long sum = 0;

	if (!g_improper_alloc_enable)
		return;

	for_each_improper_type(type) {
		if (type->improper(gfp_mask, order, large_slab_size))
			sum = atomic_long_inc_return_relaxed(&type->sum);
		else
			continue;

		if (!__ratelimit(&type->rl))
			continue;

		pr_info("gfp_mask=%#x(%pGg) order=%u type=%s sum=%ld\n", gfp_mask, &gfp_mask,
			order, type->name, atomic_long_read(&type->sum));

		if (unlikely(sum < SKIP_ON_START))
			continue;

		type->extra_info(gfp_mask, order, large_slab_size);
	}
}

void improper_alloc_show(struct seq_file *m)
{
	struct improper_alloc *type = NULL;

	if (!g_improper_alloc_enable)
		return;

	for_each_improper_type(type)
		seq_printf(m, "%s_sum:%ld\n", type->name, atomic_long_read(&type->sum));
}

static int improper_alloc_init_handler(void *p)
{
	int i;
	unsigned int beta_flag;

	/* Try 60 times, wait 120s at most */
	for (i = 0; i < 60; i++) {
		beta_flag = get_logusertype_flag();
		/* Non-zero value means it is initialized */
		if (beta_flag != 0)
			break;
		/* Sleep 2 seconds */
		msleep(2000);
	}

	/* Enable in china and oversea beta version */
	if (beta_flag != BETA_USER && beta_flag != OVERSEA_USER) {
		pr_err("non-beta user, disable improper_alloc forever\n");
		g_improper_alloc_enable = false;
		goto improper_alloc_init_end;
	}

#ifdef CONFIG_HISYSEVENT
	/* Never upload any data in nolog mode, hisysevent is only initialized in beta version */
	g_improper_alloc_report = kzalloc(sizeof(struct improper_alloc_report), GFP_KERNEL);
	if (!g_improper_alloc_report)
		goto improper_alloc_init_end;

	spin_lock_init(&g_improper_alloc_report->lock);

	/* Make sure initialize is completed before g_improper_alloc_enable is set */
	barrier();
#endif
	g_improper_alloc_enable = IMPROPER_ALLOC_ENABLE_DEFAULT;

improper_alloc_init_end:
	if (g_improper_alloc_enable)
		improper_alloc_sysctl_init();

	pr_info("improper alloc debug is %s\n", !!g_improper_alloc_enable ? "enabled" : "disabled");
	return 0;
}

static int __init improper_alloc_init(void)
{
	struct task_struct *task = NULL;

	task = kthread_run(improper_alloc_init_handler, NULL, "improper_alloc_init");
	if (IS_ERR(task))
		pr_err("improper_alloc debug init failed\n");
	else
		pr_info("improper_alloc debug init successfully\n");

	return 0;
}

late_initcall(improper_alloc_init);
