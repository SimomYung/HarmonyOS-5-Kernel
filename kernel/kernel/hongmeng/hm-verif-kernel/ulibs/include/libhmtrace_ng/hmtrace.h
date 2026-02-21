/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Headers of hmtrace_ng
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 10 12:18:42 2023
 */
#ifndef ULIBS_LIBHMTRACE_NG_HMTRACE_H
#define ULIBS_LIBHMTRACE_NG_HMTRACE_H
#include <stdbool.h>
#include <stddef.h>
#include <hmkernel/types.h>
#include <libhmlog/hmlog.h>
#include <lib/trace/trace_misc.h>
#include <hmsysmgr/trace/trace_def.h>

void hm_trace_shm_set_addr(uintptr_t addr);
void hm_trace_shm_set_snapshot_addr(uintptr_t addr);
void hm_trace_shm_set_saved_taskinfo_addr(uintptr_t addr);
void *hm_trace_shm_addr_of(void);
void *hm_trace_shm_snapshot_addr_of(void);
void *hm_trace_shm_idle_addr_of(void);
uint64_t hm_trace_pmem_used(void);
int hm_trace_rmq_dequeue_wait(void);
void hm_trace_rmq_shrink_wait(bool snapshot);
const char *hm_trace_tcb_state2str(long state);
const char *hm_trace_tcb_unpack_schedinfo(unsigned char *sched_info, unsigned int len);
const char *hm_trace_el2_irq_type_str(unsigned int type);
void hm_trace_clean_savedcmd(void);
void hm_trace_tcb_name_of(char *buf);
int hm_trace_write_savedcmd(int tid, const char *name);
int hm_trace_write_savedtgid(int tid, int pid);

int hm_trace_register_events(enum trace_event_module module);
int hm_trace_register_ldk_events(struct tracepoint_config *tps, unsigned int *tp_type, unsigned int tp_num);
int hm_trace_add_ldk_printk_format(char *str);

void hm_trace_set_tracing_block_enabled(bool enable);
bool hm_trace_is_tracing_block_enabled(void);
#endif /* ULIBS_LIBHMTRACE_NG_HMTRACE_H */
