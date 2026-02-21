/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Define getrusage interface
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 17 09:35:18 2020
 */
#ifndef SYSMGR_INTAPI_PROCESS_RUSAGE_H
#define SYSMGR_INTAPI_PROCESS_RUSAGE_H

#include <stdint.h>
#include <sys/resource.h>
#include <intapi/thread/thread_ops.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsrv_sys/hm_procmgr.h>
#include <intapi/mem/mcache.h>

struct process_s;
struct thread_s;

#if defined(CONFIG_COMPAT) || defined(CONFIG_ILP32)
struct rusage_compat32 {
	/* user CPU time used */
	uint32_t hm_ru_utime_tv_sec;
	uint32_t hm_ru_utime_tv_usec;
	/* system CPU time used */
	uint32_t hm_ru_stime_tv_sec;
	uint32_t hm_ru_stime_tv_usec;

	uint32_t hm_ru_maxrss;
	uint32_t hm_ru_ixrss;
	uint32_t hm_ru_idrss;
	uint32_t hm_ru_isrss;
	uint32_t hm_ru_minflt;
	uint32_t hm_ru_majflt;
	uint32_t hm_ru_nswap;
	uint32_t hm_ru_inblock;
	uint32_t hm_ru_oublock;
	uint32_t hm_ru_msgsnd;
	uint32_t hm_ru_msgrcv;
	uint32_t hm_ru_nsignals;
	uint32_t hm_ru_nvcsw;
	uint32_t hm_ru_nivcsw;
};
#endif

struct procmgr_rusage_ops_s {
	int (*stat_to_rusage)(struct thread_s *thread, struct rusage_compat *usage);
	void (*add_rusage_to_process)(struct thread_s *thread, struct process_s *process);
};

int process_getrusage(struct process_s *process, int who, struct rusage_compat *usage);
void process_rusage_put(struct process_s *process);
void process_rusage_to_parent(struct process_s *process, struct process_s *parent);
void process_rusage_update(struct process_s *process);
void add_thread_rusage_to_process(struct thread_s *thread, struct process_s *process);
void process_rusage_copy(struct rusage_compat *rusage, struct process_s *process);

#endif
