/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Provide interfaces for traces in kernel
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 14 10:39:52 2023
 */
#ifndef MAPI_HMKERNEL_TRACE_MISC_H
#define MAPI_HMKERNEL_TRACE_MISC_H
#include <lib/trace/trace_misc.h>
#include <hmkernel/tcb/struct.h>
#include <hmkernel/types.h>
#include <lib/klog.h>

void trace_shm_set_kernel_addr(uptr_t addr);
void trace_shm_set_futex_addr(uptr_t addr);
uptr_t trace_shm_kernel_addr_of(void);
void trace_notify_event(void);
void trace_shm_set_kernel_snapshot_addr(uptr_t addr);
void trace_shm_set_kernel_saved_taskinfo_addr(uptr_t addr);
bool hm_trace_is_saved_taskinfo_shm_addr_valid(void);
bool hm_trace_is_saved_taskinfo_enable(unsigned int cpu);
bool hm_trace_is_tracing_block_enabled(void);
void trace_set_block_info(struct tcb_s *tcb,
			  enum tcb_block_reason reason,
			  char *mod_name, char *func_name,
			  unsigned long *offset, unsigned long *size,
			  bool *iowait);
#endif
