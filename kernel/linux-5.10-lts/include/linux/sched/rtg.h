/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __SCHED_RTG_H
#define __SCHED_RTG_H

#include <linux/cpumask.h>

#ifdef CONFIG_SCHED_RTG

#define MULTI_FRAME_NUM 40
#define FFRT_RTG_NUM 10
enum RTG_GRP_ID {
	DEFAULT_RTG_GRP_ID,
	DEFAULT_CGROUP_COLOC_ID = 1,
	DEFAULT_AI_ID = 2,
	DEFAULT_AI_RENDER_THREAD_ID = 3,
	DEFAULT_AI_OTHER_THREAD_ID = 4,
	DEFAULT_RT_FRAME_ID = 8,
	DEFAULT_AUX_ID = 9,
	MULTI_FRAME_ID = 10,
	FFRT_RTG_ID = MULTI_FRAME_ID + MULTI_FRAME_NUM,
	MAX_NUM_CGROUP_COLOC_ID = FFRT_RTG_ID + FFRT_RTG_NUM,
};

struct group_cpu_time {
	u64	window_start;
	u64	curr_runnable_sum;
	u64	prev_runnable_sum;
	u64	nt_curr_runnable_sum;
	u64	nt_prev_runnable_sum;
};

struct group_time {
	unsigned long curr_window_load;
	unsigned long curr_window_exec;
	unsigned long prev_window_load;
	unsigned long prev_window_exec;
	unsigned long normalized_util;
};

#ifdef CONFIG_HISI_EAS_SCHED
struct grp_load_mode {
	bool freq_enabled;
	bool util_enabled;
};
#endif

struct rtg_class;

struct related_thread_group {
	int id;
	raw_spinlock_t lock;
	struct list_head tasks;
	struct list_head list;

	unsigned int nr_running;
	cpumask_t distribution_mask;
	struct group_time time;
	u64 window_start;
	u64 mark_start;
	u64 prev_window_time;
	/* rtg window information for WALT */
	unsigned int window_size;
	const struct rtg_class *rtg_class;
	struct sched_cluster *preferred_cluster;
	int max_boost;
	unsigned long util_invalid_interval; /* in nanoseconds */
	unsigned long util_update_timeout; /* in nanoseconds */
	unsigned long freq_update_interval; /* in nanoseconds */
	u64 last_util_update_time;
	u64 last_freq_update_time;

#ifdef CONFIG_HISI_EAS_SCHED
	/* the min freq set by userspace */
	unsigned int us_set_min_freq;
	struct grp_load_mode mode;
	unsigned int capacity_margin;
#endif

#ifdef CONFIG_ED_TASK
	bool ed_enabled;
	unsigned int ed_task_running_duration;
	unsigned int ed_task_waiting_duration;
	unsigned int ed_new_task_running_duration;
#endif /* CONFIG_ED_TASK */

	void *private_data;
};

struct rtg_class {
	void (*sched_update_rtg_tick)(struct related_thread_group *grp, void *cluster);
};

/* FORCE means update could skip grp->freq_update_interval */
enum rtg_freq_update_flag {
	RTG_FREQ_FORCE_UPDATE = (1 << 0),
	RTG_FREQ_NORMAL_UPDATE = (1 << 1),
};

int sched_set_group_id(struct task_struct *p, unsigned int group_id);
unsigned int sched_get_group_id(struct task_struct *p);
#endif /* CONFIG_SCHED_RTG */
#endif
