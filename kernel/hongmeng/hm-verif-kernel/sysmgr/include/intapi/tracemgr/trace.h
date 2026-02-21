/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Interface of trace
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 22 17:39:58 2023
 */
#ifndef SYSMGR_TRACEMGR_TRACE_H
#define SYSMGR_TRACEMGR_TRACE_H
#include <hmasm/types.h>
#include <libstrict/strict.h>
#include <hmsysmgr/trace/trace_def.h>

#define TRACE_ITER_BUF_SIZE 4096
struct tp_section {
	unsigned long start_tp;         /* start va of __tracepoints section */
	size_t tp_size;                 /* size of __tracepoints section */
	unsigned long start_tp_str;     /* start va of __tracepoints_strings section */
	size_t tp_str_size;             /* size of __tracepoints_strings section */
	unsigned long start_jump_table; /* start va of __jump_table section */
	size_t jump_table_size;         /* size of __jump_table section */
};

struct process_s;

#ifdef CONFIG_TRACEMGR
bool check_tp_section(size_t tp_size, size_t tp_str_size, size_t jump_table_size);
void tracemgr_set_rmq_dequeue_futex_addr(uintptr_t addr);
struct tracepoint;
int tracemgr_tp_enable_disable(struct tracepoint *tp, bool enable);
int tracemgr_tp_register_handler(struct tracepoint *tp, void *func, void *data);
int tracemgr_tp_unregister_handler(struct tracepoint *tp, void *func);
bool tracemgr_tp_check(unsigned int tp_type);
void tracemgr_unregister_events(struct process_s *process);
int tracefs_init(void);
void tracefs_uninit(void);
int tracemgr_futex_wait(unsigned int futex_type, __u64 *ptimeout);
#else
static inline bool check_tp_section(size_t tp_size, size_t tp_str_size, size_t jump_table_size)
{
	UNUSED(tp_size, tp_str_size, jump_table_size);

	return false;
}
static inline void tracemgr_set_rmq_dequeue_futex_addr(uintptr_t addr)
{
}
static inline bool tracemgr_tp_check(unsigned int tp_type)
{
	UNUSED(tp_type);

	return false;
}
static inline void tracemgr_unregister_events(struct process_s *process)
{
	UNUSED(process);
}
static inline int tracefs_init(void)
{
	return E_HM_NOSYS;
}
static inline void tracefs_uninit(void)
{
}
int tracemgr_futex_wait(unsigned int futex_type, __u64 *ptimeout)
{
	UNUSED(futex_type, ptimeout);
	return E_HM_NOSYS;
}
#endif /* CONFIG_TRACEMGR */
#endif /* SYSMGR_TRACEMGR_TRACE_H */
