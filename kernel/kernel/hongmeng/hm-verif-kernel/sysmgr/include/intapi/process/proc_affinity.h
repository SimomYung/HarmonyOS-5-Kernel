/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Interfaces for proc_affinity
 * Author: Huawei OS Kernel Lab
 * Create: Sun Nov 28 11:47:33 2021
 */

#ifndef SYSMGR_INTAPI_PROCESS_PROC_AFFINITY_H
#define SYSMGR_INTAPI_PROCESS_PROC_AFFINITY_H

#include <intapi/thread/thread.h>
#include <libhmsrv_sys/hm_proc_affinity.h>

int process_proc_affinity_get(struct process_s *process, struct proc_affinity_info_s *proc_affinity_info);

void process_proc_affinity_activate(struct process_s *process, struct thread_s *thread);

#ifdef __HOST_LLT__
void set_cached_false(void);
#endif

#endif
