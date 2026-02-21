/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 *
 * Description: Debug improper memory alloc behaviours
 * Author: Li Xilun <lixilun1@huawei.com>
 * Create: 2023-03-23
 */

#ifndef _IMPROPER_ALLOC_CORE_H
#define _IMPROPER_ALLOC_CORE_H

/* Common used in improper alloc */
#include <linux/gfp.h>
#include <linux/atomic.h>
#include <linux/ratelimit_types.h>
#include <linux/sched.h>

extern unsigned int __read_mostly g_improper_alloc_enable;
extern struct improper_alloc_report *g_improper_alloc_report;

#define MAX_NAME_LEN 16

struct improper_alloc {
	const char name[MAX_NAME_LEN];
	bool (*const improper)(gfp_t gfp_mask, unsigned int order, size_t size);
	void (*const extra_info)(gfp_t gfp_mask, unsigned int order, size_t size);
	atomic_long_t sum;
	struct ratelimit_state rl;
};

extern struct improper_alloc g_improper_types[];

enum improper_type {
	IMPROPER_HIGH_ORDER = 0,
#ifndef CONFIG_LIBLINUX
	IMPROPER_ATOMIC,
	IMPROPER_LARGE_SLAB,
#endif
	IMPROPER_COUNT
};

#define for_each_improper_type(p) \
	for ((p) = g_improper_types; (p) <= &g_improper_types[IMPROPER_COUNT - 1]; ++(p))

#ifdef CONFIG_HISYSEVENT
#define MAX_REPORT_STACK_DEPTH 20
#define MAX_REPORT_MSG_LEN 1248
#define REPORT_ENTRIES_TO_SKIP 2
#define REPORT_ENTRY_BUF 256
struct improper_alloc_report {
	pid_t pid;
	char package_name[TASK_COMM_LEN];
	char msg[MAX_REPORT_MSG_LEN];
	int off;
	unsigned long entries[MAX_REPORT_STACK_DEPTH];
	unsigned int nr_entries;
	spinlock_t lock;
};
#endif

void improper_alloc_sysctl_init(void);
#endif
