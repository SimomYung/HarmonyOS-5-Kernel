 /*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: The interface of guard actions
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 31 19:04:31 2019
 */
#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_GUARD_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_GUARD_H

#include <unistd.h> /* for pid_t */

/* Definitions in this file are used by hguard exclusively */

#define GUARD_THREAD_NAME_SIZE		16
#define GUARD_STATE_NAME_SIZE		6
#define GUARD_BLOCK_NAME_SIZE		6

#define GUARD_DUMP_MEM_FLAGS_SYS	0x1U
#define GUARD_DUMP_MEM_FLAGS_PROC	(0x1U << 1)

struct guard_dump_args {
	int type;
	pid_t tid;
	unsigned int flags;
};

struct guard_query_args {
	int type;   /* GUARD_TYPE_XXX */
	unsigned int ready_hungtask_timeout;
	unsigned int block_hungtask_timeout;
	unsigned int exit_hungtask_timeout;
};

struct guard_monitor_list_args {
	size_t len;
	const char *monitor_list;
};

struct guard_user_list_set_args {
	size_t len;
	const char *user_list;
	pid_t tid;
};

struct guard_user_list_get_args {
	size_t len;
	char *user_list;
};

union guard_config_args {
	struct guard_monitor_list_args monitor_list_args;
	struct guard_user_list_set_args user_list_set_args;
	struct guard_user_list_get_args user_list_get_args;
};

union guard_action_args {
	struct guard_dump_args dump_args;
	struct guard_query_args query_args;
};

struct guard_hangtask_stat {
	int type;  /* one of TYPE_READY or TYPE_LOCKED */
	pid_t tid;
	pid_t pid;
	pid_t ppid;
	unsigned int rq_cur;
	int prio_cur;
	unsigned long long cref;
	unsigned long long sched_cnt;
	unsigned long long prev_tcb_cref;
	unsigned long long preempt_tcb_cref;
	char state[GUARD_STATE_NAME_SIZE];
	char block[GUARD_BLOCK_NAME_SIZE];
	char name[GUARD_THREAD_NAME_SIZE];
	bool in_monitor_list;
};

struct guard_mem_stat {
	unsigned long total;
	unsigned long free;
};

struct guard_zombie_stat {
	unsigned int zombie_cnt;
};

struct guard_stat {
	struct guard_hangtask_stat hangtask_stat;
	struct guard_mem_stat mem_stat;
	struct guard_zombie_stat zombie_stat;
};

#endif
