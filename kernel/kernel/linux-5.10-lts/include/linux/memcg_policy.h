/* SPDX-License-Identifier: GPL-2.0 */
/*
 * include/linux/memcg_policy.h
 *
 * Copyright (c) 2020-2022 Huawei Technologies Co., Ltd.
 *
 */
#ifndef _MEMCG_POLICY_H
#define _MEMCG_POLICY_H

struct mem_cgroup;
struct pglist_data;
struct scan_control;


extern struct list_head score_head;
extern bool score_head_inited;
extern rwlock_t score_list_lock;
extern struct cgroup_subsys memory_cgrp_subsys;
#ifdef CONFIG_HYPERHOLD_FILE_LRU
void shrink_anon_memcg(struct pglist_data *pgdat,
		struct mem_cgroup *memcg, struct scan_control *sc,
		unsigned long *nr);
bool shrink_node_hyperhold(struct pglist_data *pgdat, struct scan_control *sc);
#endif /* CONFIG_HYPERHOLD_FILE_LRU */

#ifdef CONFIG_PARA_SWAPD
extern void parak_set_kswapd_pid(pid_t pid);
extern int parak_task_create(void);
extern int parad_task_create(void);
#endif

#ifdef CONFIG_HYPERHOLD_MEMCG
struct mem_cgroup *get_next_memcg(struct mem_cgroup *prev);
struct mem_cgroup *get_prev_memcg(struct mem_cgroup *next);
void get_next_memcg_break(struct mem_cgroup *memcg);
void get_prev_memcg_break(struct mem_cgroup *memcg);
void memcg_app_score_update(struct mem_cgroup *target);

struct memcg_reclaim {
	atomic64_t app_score;
	atomic64_t ub_ufs2zram_ratio;
#ifdef CONFIG_HYPERHOLD_ZSWAPD
	atomic_t ub_zram2ufs_ratio;
	atomic_t ub_mem2zram_ratio;
	atomic_t refault_threshold;
	/* anon refault */
	unsigned long long reclaimed_pagefault;
#endif
};
#define MAX_APP_SCORE 1000
#endif

#ifdef CONFIG_HYPERHOLD_ZSWAPD
extern void zswapd_pressure_info(struct seq_file *m);
#else
static inline void zswapd_pressure_info(struct seq_file *m)
{
}
#endif

#ifdef CONFIG_KSWAPD_SOFT_LIMIT
inline unsigned int get_start_priority(void);
inline unsigned int get_end_priority(void);
inline unsigned int get_soft_limit_costly_order(void);
inline unsigned int get_kswapd_high_load_threshold(void);
#endif

#endif /* _LINUX_MEMCG_POLICY_H */
