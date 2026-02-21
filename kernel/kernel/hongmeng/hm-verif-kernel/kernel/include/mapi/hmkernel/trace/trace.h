/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 23 10:30:58 2023
 */
#ifndef MAPI_HMKERNEL_TRACE_H
#define MAPI_HMKERNEL_TRACE_H

#include <hmkernel/types.h>
#include <hmkernel/errno.h>
#include <lib/trace/tracepoint_defs.h>

#ifdef CONFIG_TRACEMGR
int tracepoint_of(int eid, struct tracepoint **ret);
int trace_find_tracepoint(unsigned int tp_type, struct tracepoint **ret);
u32 trace_sequence_inc(void);
struct tracepoint;
int tracepoint_register_handler(struct tracepoint *tp, void *func, void *data);
int tracepoint_unregister_handler(struct tracepoint *tp, void *func);
int tracepoint_enable_probe(struct tracepoint *tp);
int tracepoint_disable_probe(struct tracepoint *tp);
int trace_init_event(const char *name, unsigned int tp_type, bool ishmvirt, unsigned int *tp_index);
int trace_enable_disable_event(unsigned int tp_index, bool enable, bool ishmvirt);
#ifdef __HOST_LLT__
void llt_trace_init(void);
#endif
#else
static inline int tracepoint_of(int eid, struct tracepoint **ret)
{
	UNUSED(eid, ret);

	return E_HM_NOSYS;
}
static inline int trace_find_tracepoint(unsigned int tp_type, struct tracepoint **ret)
{
	UNUSED(tp_type, ret);

	return E_HM_NOSYS;
}
static inline int trace_init_event(const char *name, unsigned int tp_type,
				   bool ishmvirt, unsigned int *tp_index)
{
	UNUSED(name, tp_type, ishmvirt, tp_index);

	return E_HM_NOSYS;
}
static inline int trace_enable_disable_event(unsigned int tp_index, bool enable,
					     bool ishmvirt)
{
	UNUSED(tp_index, ishmvirt, enable);

	return E_HM_NOSYS;
}
#endif /* CONFIG_TRACEMGR */

#ifdef CONFIG_HMVIRT_TRACE
void hyp_el2_trace_init(void);
#else
static inline void hyp_el2_trace_init(void) {}
#endif

#endif /* MAPI_HMKERNEL_TRACE_H */
