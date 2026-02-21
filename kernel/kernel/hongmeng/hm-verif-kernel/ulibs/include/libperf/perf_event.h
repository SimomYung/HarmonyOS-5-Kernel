/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Header file of sysmgr and perf interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 30 10:05:16 2020
 */

#ifndef ULIBS_LIBPERF_PERF_EVENT_H
#define ULIBS_LIBPERF_PERF_EVENT_H

#include <stdint.h>
#include <sys/types.h>
#include <hmkernel/perf_event.h>

#define PERF_BUFFER_PAGE_SIZE      4096u
#define PERF_BUFFER_PAGE_MAX_NR    32u
#define PERF_BUFFER_PAGE_MIN_NR    2u

enum _perf_event_type {
	PERF_EVENT_INVAL = -1,
	PERF_EVENT_COUNTING,
	PERF_EVENT_SAMPLING,
	PERF_EVENT_TYPE_NR,
};

enum perf_event_mode {
	PERF_MODE_INVAL = -1,
	PERF_MODE_PROCESS = 0, /* Ensure that valid mode must start from 0. */
	PERF_MODE_VM,
	PERF_MODE_THREAD,
	PERF_MODE_CPU,
	PERF_MODE_CLUSTER,
	PERF_MODE_NR,
};

struct perf_event_attr_s {
	enum _perf_event_type type;
	enum perf_event_mode mode;
	bool inherit;
	pid_t target;
	unsigned int cpu;
	unsigned int cluster;
	struct __perf_event_config_s config;
};

struct perf_event_counting_s {
	pid_t ppid;
	pid_t pid;
	pid_t tid;
	unsigned int cpu;
	unsigned int cluster;
	pid_t vmmpid;
	unsigned int vcpu;
	struct __perf_event_ret_s ret;
};

struct perf_read_data_ret_s {
	uint32_t size;
	bool last;
	uint64_t total_events;
	uint64_t lost_events;
};

#endif
