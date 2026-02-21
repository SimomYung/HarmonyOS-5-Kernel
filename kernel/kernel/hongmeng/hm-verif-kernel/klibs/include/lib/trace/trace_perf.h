/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: header for trace perf
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 8 15:12:10 2023
 */

#ifndef KLIBS_TRACE_PERF_H
#define KLIBS_TRACE_PERF_H
#include <hmkernel/types.h>
#include <lib/dlist.h>
#include <hmasm/registers.h>

#if defined(CONFIG_HMPERF_NG) && defined(__KERNEL__)
#include <hmasm/perf_ctx.h>
extern void perf_trace_buf_submit(__u64 raw_data, int size, struct arch_regs *regs,
				  struct dlist_node *etable, unsigned int etsize);
extern __u64 perf_trace_alloc_entry(__u32 size);
extern void perf_trace_free_entry(__u64 entry);
extern struct arch_regs *perf_trace_alloc_regs(void);
extern void perf_trace_free_regs(struct arch_regs *regs);

static inline __always_inline
void perf_fetch_caller_regs(struct arch_regs *regs)
{
	perf_arch_fetch_caller_regs(regs);
}

#else
static inline void perf_trace_buf_submit(__maybe_unused __u64 raw_data,
					 __maybe_unused int size,
					 __maybe_unused struct arch_regs *regs,
					 __maybe_unused struct dlist_node *etable,
					 __maybe_unused unsigned int etsize)
{
	UNUSED(raw_data, size, etable, etsize);
}

static inline __u64 perf_trace_alloc_entry(__u32 size)
{
	UNUSED(size);
	return 0ULL;
}

static inline void perf_trace_free_entry(__u64 entry)
{
	UNUSED(entry);
}

static inline struct arch_regs *perf_trace_alloc_regs(void)
{
	return NULL;
}

static inline void perf_trace_free_regs(struct arch_regs *regs)
{
	UNUSED(regs);
}

static inline void perf_fetch_caller_regs(struct arch_regs *regs)
{
	UNUSED(regs);
}

#endif

#endif
