/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2025. All rights reserved.
 * Description: Quantify the system's ability to allocate high-order memory
 * Author: OpenHarmony Dpt.1
 * Create: Wed Feb 22 16:26:24 2025
 */

#ifndef INTAPI_PROCESS_KILL_ACCT_H
#define INTAPI_PROCESS_KILL_ACCT_H

#include <libhmsync/atomic.h>

enum kill_acct_events_e {
	KILL_ACCT_KILL_START = 0,
	KILL_ACCT_DO_EXIT_END,
	KILL_ACCT_OOM_REAP_START,
	KILL_ACCT_OOM_REAP_END,
	KILL_ACCT_VSPACE_DESTROY_START,
	KILL_ACCT_VSPACE_DESTROY_END,
	KILL_ACCT_PROCESS_DELETE_START,
	KILL_ACCT_KILL_END,
	KILL_ACCT_EVENTS_NR,
};
struct process_s;
struct vspace_s;
#ifdef CONFIG_PROCESS_KILL_ACCT
enum kill_acct_mode_e {
	KILL_ACCT_MODE_OFF = 0,
	KILL_ACCT_MODE_TIME_ACCT,
	KILL_ACCT_MODE_STACK_TIME_ACCT,
	KILL_ACCT_MODE_END,
};

struct kill_stat_s {
	uint64_t kill_start;
	uint32_t oom_reap_start;
	uint32_t oom_reap_end;
	uint32_t vspace_destroy_start;
	uint32_t vspace_destroy_end;
	uint32_t anon_rss;
	uint32_t gpu_dma_rss;
	uint32_t pgtbl_rss;
	uint32_t swaprss;
};

void process_kill_acct_prepare(struct process_s *target, struct vspace_s *vs);
void process_kill_acct_end(struct process_s *target, pid_t pid);
void process_kill_acct_set_event(enum kill_acct_events_e event, struct process_s *target);
int process_kill_acct_read_time(char *data, size_t data_len);
void process_kill_acct_mode_write(enum kill_acct_mode_e mode);
void process_kill_acct_set_empty(struct kill_stat_s *kill_stat);
#else
static inline void process_kill_acct_prepare(__maybe_unused struct process_s *target,
	__maybe_unused struct vspace_s *vs) { }

static inline void process_kill_acct_end(__maybe_unused struct process_s *target,
	__maybe_unused pid_t pid) { }

static inline void process_kill_acct_set_event(__maybe_unused enum kill_acct_events_e event,
	__maybe_unused struct process_s *target) { }
#endif
#endif
