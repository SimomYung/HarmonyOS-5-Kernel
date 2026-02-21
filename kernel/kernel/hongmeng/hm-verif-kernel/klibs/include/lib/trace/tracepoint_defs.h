/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Definitions of tracepoint
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 25 11:03:11 2023
 */
#ifndef KLIBS_TRACEPOINT_DEFS_H
#define KLIBS_TRACEPOINT_DEFS_H

#ifdef __KERNEL__
#include <lib/atomic.h>
#else
#include <vsync/atomic.h>
#endif
#include <hmkernel/jump_label.h>

#define TRACE_EVENT_NAME_SIZE		64
#define TRACE_EVENT_GROUP_NAME_SIZE	32
#define TRACE_EVENT_CONFIG_MAX_NUM	1024
#define HMCAP_THREAD_NAME_SIZE 16
#define TRACE_EID_START 32768
#define INVALID_TP_TYPE 0U
#define MAX_TP_STRING_SIZE 64

#define HMPERF_TP_HOT_ETSIZE		256U
#define HMPERF_TP_HOT_ETMASK		(HMPERF_TP_HOT_ETSIZE - 1)

struct tracepoint_func {
	void *func;
	void *data;
	vatomic32_t refcnt;
};

#define TP_TRACE_ENABLED_OFFSET 0
#define TP_PROBE_ENABLED_OFFSET 1
#define TP_ENABLED_OFFSET ((1U << TP_TRACE_ENABLED_OFFSET) | (1U << TP_PROBE_ENABLED_OFFSET))

struct tracepoint {
	const char *group_name;
	const char *name;
	unsigned int type;
	unsigned short etable_size;
	unsigned short attached_events;
	/* bit0:trace enabled, bit1:hmprobe_enabled */
	struct static_key *key;
	struct tracepoint_func tp_func;
	struct dlist_node *etable;
};

static inline __s32 tracepoint_set_enabled(__uptr_t enabled_p,
					   unsigned int bitmask,
					   bool enable)
{
	BUG_ON((bitmask & ~TP_ENABLED_OFFSET) != 0);
	return static_key_set_enabled(enabled_p, bitmask, enable);
}

static inline int tracepoint_enable(struct tracepoint *tp,
				    unsigned int bitmask)
{
	BUG_ON((bitmask & ~TP_ENABLED_OFFSET) != 0);
	return static_key_enable(tp->key, bitmask);
}

static inline int tracepoint_disable(struct tracepoint *tp,
				     unsigned int bitmask)
{
	BUG_ON((bitmask & ~TP_ENABLED_OFFSET) != 0);
	return static_key_disable(tp->key, bitmask);
}

static inline __s32 tracepoint_enabled_of(struct tracepoint *tp)
{
	return (__s32)vatomic32_read_rlx((vatomic32_t *)(void *)&tp->key->enabled);
}

static inline bool tracepoint_is_trace_enabled(__s32 enabled)
{
	return ((unsigned int)enabled & (1U << TP_TRACE_ENABLED_OFFSET)) != 0;
}

static inline bool tracepoint_is_probe_enabled(__s32 enabled)
{
	return ((unsigned int)enabled & (1U << TP_PROBE_ENABLED_OFFSET)) != 0;
}

#define PARAMS(...)	__VA_ARGS__
#define TP_PROTO(...)	__VA_ARGS__
#define TP_ARGS(...)	__VA_ARGS__
#define TP_FIELD(...)	__VA_ARGS__
#define TP_ASSIGN(...)	__VA_ARGS__
#define TP_PRINT(...)	__VA_ARGS__

#define ____TP_ARG_NR_N(t1, a1, t2, a2, t3, a3, t4, a4, t5, a5,			\
			t6, a6, t7, a7, t8, a8, t9, a9, t10, a10, t11, a11, t12, a12,	\
			t13, a13, t14, a14, t15, a15, n, x, ...) n
#define __TP_ARG_NR(...)							\
	____TP_ARG_NR_N(__VA_ARGS__,						\
			15, x, 14, x, 13, x, 12, x, 11, x, 10, x, 9, x,	\
			8, x, 7, x, 6, x, 5, x, 4, x, 3, x, 2, x, 1, x, 0, x)

#define __TP_MAP0(n, m, ...)
#define __TP_MAP1(n, m, t, a, ...) m(n - 1, t, a)
#define __TP_MAP2(n, m, t, a, ...) m(n - 2, t, a), __TP_MAP1(n, m, __VA_ARGS__)
#define __TP_MAP3(n, m, t, a, ...) m(n - 3, t, a), __TP_MAP2(n, m, __VA_ARGS__)
#define __TP_MAP4(n, m, t, a, ...) m(n - 4, t, a), __TP_MAP3(n, m, __VA_ARGS__)
#define __TP_MAP5(n, m, t, a, ...) m(n - 5, t, a), __TP_MAP4(n, m, __VA_ARGS__)
#define __TP_MAP6(n, m, t, a, ...) m(n - 6, t, a), __TP_MAP5(n, m, __VA_ARGS__)
#define __TP_MAP7(n, m, t, a, ...) m(n - 7, t, a), __TP_MAP6(n, m, __VA_ARGS__)
#define __TP_MAP8(n, m, t, a, ...) m(n - 8, t, a), __TP_MAP7(n, m, __VA_ARGS__)
#define __TP_MAP9(n, m, t, a, ...) m(n - 9, t, a), __TP_MAP8(n, m, __VA_ARGS__)
#define __TP_MAP10(n, m, t, a, ...) m(n - 10, t, a), __TP_MAP9(n, m, __VA_ARGS__)
#define __TP_MAP11(n, m, t, a, ...) m(n - 11, t, a), __TP_MAP10(n, m, __VA_ARGS__)
#define __TP_MAP12(n, m, t, a, ...) m(n - 12, t, a), __TP_MAP11(n, m, __VA_ARGS__)
#define __TP_MAP13(n, m, t, a, ...) m(n - 13, t, a), __TP_MAP12(n, m, __VA_ARGS__)
#define __TP_MAP(n, ...) __TP_MAP##n(n, __VA_ARGS__)

#define __TP_SC_DEF(n, t, a)		t a
#define __TP_SC_CALL(n, t, a)		a

#define TRACE_FUNC_PROTO(n, ...)	__TP_MAP(n, __TP_SC_DEF, __VA_ARGS__)
#define TRACE_FUNC_ARGS(n, ...)		__TP_MAP(n, __TP_SC_CALL, __VA_ARGS__)
#define TP_FUNC_PROTO(...)		TRACE_FUNC_PROTO(__TP_ARG_NR(__VA_ARGS__), __VA_ARGS__)
#define TP_FUNC_ARGS(...)		TRACE_FUNC_ARGS(__TP_ARG_NR(__VA_ARGS__), __VA_ARGS__)

#define TRACEPOINT_SECTION "__tracepoints"
#define TRACEPOINT_STR_SECTION "__tracepoints_strings"

#define MAKE_TRACEPOINT_GROUP_STR(group_name)	\
	static const char __group_##group_name[] __attribute__((section(TRACEPOINT_STR_SECTION))) = #group_name;

#define for_each_tp(iter, start, end)	\
	for ((iter) = (struct tracepoint *)(start);(iter) < (struct tracepoint *)(end);(iter)++)
#endif /* KLIBS_TRACEPOINT_DEFS_H */
