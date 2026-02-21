/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Structures of scheduling for kobj
 * Author: Huawei OS Kernel Lab
 * Created: Wen Apr 10 19:51:50 2024
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_SCHED_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_SCHED_H

#include <hmkernel/types.h>

#define CTRLMEM_MAX_NR_CPU 20
typedef struct __migration_hmlt_load_s {
	/* task running load in every cpu */
	__u64 last_run_time;
	__u64 last_run_window_start_time;
	__u16 curr_run_load;
	__u16 prev_run_load;
	__u16 curr_load_in_cpu[CTRLMEM_MAX_NR_CPU];
	__u16 prev_load_in_cpu[CTRLMEM_MAX_NR_CPU];
} __migration_hmlt_load_t;
typedef __migration_hmlt_load_t migration_hmlt_load_t;

#define HMLT_HISTORY_SIZE   5U
struct __hmlt_load_s {
	/* last update time  */
	__u64 start_time;
	/* time of window recording before start_time */
	__u64 window_start_time;
	/* latest update time  */
	__u64 curr_runtime;
	__u64 curr_origin_load;
	__u64 curr_running_load;
	__u64 load_sum;
	__u64 padding[3];
	__u32 window_sz;

	__u16 prev_load;
	__u16 curr_load;
	__u16 pred_load;
	__u16 trans_load;
	__u16 last_cpu;
	__u16 last_cluster_id;
	__u16 prev_top_load;
	__u16 pred_top_load;
	__u16 pred_top_load_last_cpu;
	__u16 history_top_load[HMLT_HISTORY_SIZE];
	__u16 history_load[HMLT_HISTORY_SIZE];
	__u8 curr_hist_idx;
	/* running cluster may be different with enqueue */
	__u8 queued_cpu;
	__u8 curr_cpu;
	__u8 curr_top_load_hist_idx;
	__bool inited;
	__bool is_on_cluster_top_load;
	__bool is_on_cluster_sum_load;
	__migration_hmlt_load_t mig_hmlt_load;
};

struct lt_group_config_s;
typedef struct __lt_s {
	__u64 sum_running_load;
	__u16 min_load;
	__u16 max_load;
	__u16 user_min_load;
	__u16 user_max_load;
	__u8 user_group;
	__u8 group; /* load tracking group, to indicate different demand */
	__u8 smartio_group;
	__s8 task_boost;
	__bool load_trans;
	struct lt_group_config_s *configs;
} __lt_t;
typedef __lt_t lt_t;

#endif
