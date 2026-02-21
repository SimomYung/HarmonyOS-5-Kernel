/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: mpam sched
 * Create: Sat Aug 24 10:20:00 CST 2024
 */
#ifndef __MPAM_SCHED_H
#define __MPAM_SCHED_H

#include <hmkernel/tcb.h>

enum user_type {
	BASE,
	VIP,
	AI,
};

enum mpam_policy_type {
	MPAM_POLICY_BASE,
	MPAM_POLICY_VIPTASK,
	MPAM_POLICY_VIPTASK0,
	MPAM_POLICY_AIGAME,
	MPAM_POLICY_MAX
};

enum mpam_partid_type {
	MPAM_PARTID_BASE,
	MPAM_PARTID_VIPTASK,
	MPAM_PARTID_AIGAME,
	MPAM_PARTID_MAX
};

#ifdef CONFIG_MPAM_SCHED
 
void mpam_sched_in(void);
void set_mpam_policy(u32 user, u32 policy_type);
int mpam_cpus_write(u32 cpus_mask, u32 partid);
void mpam_partid_set(struct tcb_s *tcb, u32 policy_type);
void tcb_partition_ctrl_policy(struct tcb_s *tcb, u32 policy_type);
void task_partition_context_switch(struct tcb_s *prev_tcb,struct tcb_s *next_tcb, int cpu);
void tcb_partition_ctrl_init(struct tcb_s *tcb);
void tcb_partition_ctrl_exit(struct tcb_s *tcb);
void tcb_partition_ctrl_out(struct tcb_s *tcb);
 
#else
 
static inline void mpam_sched_in(void) {}
static inline void set_mpam_policy(u32 user, u32 policy_type) {
	UNUSED(user, policy_type);
}

static inline int mpam_cpus_write(u32 cpus_mask, u32 partid) {
	UNUSED(cpus_mask, partid);
	return 0;
}

static inline void mpam_partid_set(struct tcb_s *tcb, u32 policy_type) {
	UNUSED(tcb, policy_type);
}

static inline void tcb_partition_ctrl_policy(struct tcb_s *tcb, u32 policy_type)
{
	UNUSED(tcb, policy_type);
}

static inline void task_partition_context_switch(struct tcb_s *prev_tcb,struct tcb_s *next_tcb, int cpu)
{
	UNUSED(prev_tcb, next_tcb, cpu);
}
 
static inline void tcb_partition_ctrl_init(struct tcb_s *tcb)
{
	UNUSED(tcb);
}

static inline void tcb_partition_ctrl_exit(struct tcb_s *tcb)
{
	UNUSED(tcb);
}

static inline void tcb_partition_ctrl_out(struct tcb_s *tcb)
{
	UNUSED(tcb);
}

#endif
 
#endif /* __MPAM_SCHED_H */
