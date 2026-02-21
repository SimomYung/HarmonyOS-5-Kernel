/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Wakelock stat api
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 25 04:26:50 2022
 */

#ifndef ULIBS_LIBHMPM_WL_STAT_H
#define ULIBS_LIBHMPM_WL_STAT_H

/* this head file is used in user mode and ldk, ldk do not need include stdint.h */
#ifndef CONFIG_LIBLINUX
#include <stdint.h>
#endif

#define WAKELOCK_NAME_MAX	64

struct wl_stat {
	char		name[WAKELOCK_NAME_MAX];
	uint64_t	acquired_count;
	uint64_t	total_count;
	uint64_t	wakeup_count;
	uint64_t	timeout_count;
	uint64_t	this_acquired_time;
	uint64_t	total_acquired_time;
	uint64_t	max_acquired_time;
	uint64_t	last_change_time;
	uint64_t	prevent_sleep_time;
};

struct wl_stat_queue {
	size_t total_cnt;
	size_t size;
	struct sring_buf *sring;
	void *base_addr;
};

struct wl_stat_queue_info {
	unsigned int cnode_idx;
	unsigned long long ring_key;
	unsigned long long data_key;
	size_t size;
};

#endif
