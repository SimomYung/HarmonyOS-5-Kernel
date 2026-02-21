/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Out of memory management
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 30 09:01:57 2019
 */

#ifndef SYSMGR_MEMMGR_MEM_OOM_H
#define SYSMGR_MEMMGR_MEM_OOM_H

#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_mutex_guard.h>
#include <intapi/process/process.h>
#include <intapi/process/service.h>
#include <intapi/process/procpool.h>

/* oom_score_adj range [-1000, 1000] */
#define OOM_SCORE_ADJ_MIN (-1000)
#define OOM_SCORE_ADJ_MAX 1000
/* oom_adj range [-17, 15] */
#define OOM_ADJUST_MAX 15
#define OOM_ADJUST_MIN (-16)
#define OOM_DISABLE (-17)
#define OOM_SCORE_ADJ_PROCFS_MAX_SIZE 6
#define OOM_ADJ_PROCFS_MAX_SIZE 4

#define __OOM_PANIC CONFIG_OOM_PANIC
#define OOM_PANIC_MAX_SIZE 2
#define OOM_PANIC_MAX 2U

#define OOM_KILL_MAX_SIZE 2
#define OOM_CONTROL_MAX_SIZE 2

struct victim_selector_s {
	struct process_s *chosen;
	struct process_s *invoker;
	bool exceed_actv_depth;
	int adj;
	uint64_t rss;
	long points;
	struct dlist_node select_proc_head;
};

enum oom_policy_e {
	__OOM_KILL_ALWAYS,
	__OOM_KILL_MEMCG,
	__OOM_PANIC_ALWAYS
};

void oom_init(void);
void oom_kill_policy_set(uint32_t policy);
uint32_t oom_kill_policy_get(void);
__noinline int oom_select_process(struct victim_selector_s *selector);
void oom_panic_set(uint32_t policy);
uint32_t oom_panic_get(void);
int oom_show_score(struct process_s *process);
int oom_kill(bool *kill_self);
int oom_after_memcg_charge(bool *need_retry);
void unmask_oom(void);
void mask_oom(void);
void oom_set_no_need(void);
bool oom_is_no_need(void);
void oom_clear_no_need(void);
uint64_t oom_kill_event_read(void);
__noinline int do_cpa_kill(uint64_t tokill_rss, int cpa_process_adj);

#ifdef CONFIG_HARMONYOS
void boost_sigkill_free_set(bool value);
bool boost_sigkill_free_get(void);
#endif

void oom_adj_lock(void);
void oom_adj_unlock(void);

#endif
