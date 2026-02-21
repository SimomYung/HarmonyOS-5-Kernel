/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Process rlimit function declaration
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar 18 20:03:06 2019
 */
#ifndef SYSMGR_INTAPI_PROCESS_RLIMIT_H
#define SYSMGR_INTAPI_PROCESS_RLIMIT_H

#include <libhmsync/raw_mutex.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>

#include <sys/resource.h>

struct rlimit_ctrl {
	/*
	 * We add rwlock here for rlim, because we just read the value
	 * of rlim in most cases.
	 * If you want to modify consumed, we don't recommend to use
	 * this lock because this value is frequently rw.
	 */
	struct raw_mutex lock;
	struct rlimit rlim[RLIM_NLIMITS];
};

#define STACK_RLIMIT_DEFAULT (8 * 1024 * 1024)

struct process_s;
#ifdef CONFIG_RLIMIT
int procmgr_prlimit(struct process_s *process, unsigned int type,
		    const struct rlimit *new_rlim,
		    struct rlimit *old_rlim);
bool procmgr_rlimit_check(struct process_s *process, unsigned int type,
			  uint64_t required);
uint64_t procmgr_rlimit_get_cur(struct process_s *process, unsigned int type);
struct rlimit procmgr_rlimit_of(struct process_s *process, unsigned int type);
#else
static inline int procmgr_prlimit(struct process_s *process, unsigned int type,
				  const struct rlimit *new_rlim,
				  struct rlimit *old_rlim)
{
	old_rlim->rlim_cur = RLIM_INFINITY;
	old_rlim->rlim_max = RLIM_INFINITY;
	return E_HM_OK;
}
static inline bool procmgr_rlimit_check(struct process_s *process,
					unsigned int type,
					uint64_t required)
{
	return true;
}
#endif

#endif
