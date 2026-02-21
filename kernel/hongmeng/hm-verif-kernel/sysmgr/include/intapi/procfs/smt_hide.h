/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Declaration of SMT hide interfaces.
 * Author: Huawei OS Kernel Lab
 * Create: Tue 4 June 2024 16:00
 */
#ifndef INTAPI_PROCFS_SMT_HIDE_H
#define INTAPI_PROCFS_SMT_HIDE_H

#include <hongmeng/types.h>
#include <hongmeng/errno.h>
#include <libstrict/strict.h>

enum smt_hide_flag {
	SMT_HIDE_FILE_NODE,
	SMT_HIDE_SYSCALL,
	SMT_HIDE_MAX
};

#ifdef CONFIG_SCHED_SMT_HIDE
bool smt_privileged_user(int tid, enum smt_hide_flag flag);
unsigned int hm_physic_cpu_num(void);
void physic_core_to_cluster_id(unsigned int cpu_id, unsigned int *cluster_id);
void core_to_cpu_mask(__cpuset_t *orig_mask, int tid);
#else /* !CONFIG_SCHED_SMT_HIDE */
static inline bool smt_privileged_user(int tid, enum smt_hide_flag flag)
{
	return true;
}
static inline void physic_core_to_cluster_id(unsigned int cpu_id, unsigned int *cluster_id)
{
    return;
}
static inline void core_to_cpu_mask(__cpuset_t *orig_mask, int tid)
{
	return;
}
#endif /* CONFIG_SCHED_SMT_HIDE */

#endif
