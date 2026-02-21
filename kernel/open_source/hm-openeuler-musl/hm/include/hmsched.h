/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 19 10:03:24 2020
 */
#ifndef HM_INCLUDE_HMSCHED_H
#define HM_INCLUDE_HMSCHED_H

#include <stdint.h>
#include <unistd.h>

#define SCHED_FLAG_UTIL_CLAMP_MIN			0x20
#define SCHED_FLAG_UTIL_CLAMP_MAX			0x40

#define SCHED_FLAG_UTIL_CLAMP (SCHED_FLAG_UTIL_CLAMP_MIN | \
			       SCHED_FLAG_UTIL_CLAMP_MAX)

struct sched_attr {
	uint32_t size;

	uint32_t sched_policy;
	uint64_t sched_flags;

	/* SCHED_OTHER */
	int32_t sched_nice;

	/* SCHED_FIFO, SCHED_RR */
	uint32_t sched_priority;

	/* SCHED_DEADLINE */
	uint64_t sched_runtime;
	uint64_t sched_deadline;
	uint64_t sched_period;

	/* UCLAMP */
	uint32_t sched_util_min;
	uint32_t sched_util_max;
};

int sched_get_available_priority_max(void);
int sched_setattr(pid_t pid, const struct sched_attr *attr,
		  unsigned int flags);
int sched_getattr(pid_t pid, const struct sched_attr *attr,
		  unsigned int size, unsigned int flags);
int getcpu(unsigned int *cpu, unsigned int *node);
#endif
