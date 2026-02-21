/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Kernel lock tracing mechanism
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 18 11:12:21 2020
 */

#ifndef MAPI_HMKERNEL_LOCK_TRACE_H
#define MAPI_HMKERNEL_LOCK_TRACE_H

#include <hmasm/memory.h>
#include <hmkernel/strict.h>

#ifdef CONFIG_KLOCK_TRACE
#define KLOCK_INIT_LOCK_TYPE(type)	\
	.lock_type = type,
#else
#define KLOCK_INIT_LOCK_TYPE(type)
#endif

#if defined(CONFIG_KLOCK_TRACE) || defined(__HOST_LLT__)

#ifdef __HOST_LLT__
#define MAX_TRACE_NR ((size_t)(128UL))
#else
#define MAX_TRACE_NR ((((size_t)CPU_LOCAL_SIZE - sizeof(struct arch_cpu_local_s))) / sizeof(struct lock_info_s))
#endif

enum lock_status_e {
	LOCK_STATUS_INVAL,
	LOCK_STATUS_WAIT,
	LOCK_STATUS_HOLD,
};

struct lock_info_s {
	vaddr_t lock;
	vaddr_t pc;
	unsigned int lock_type;
	unsigned int omit;
	enum lock_status_e status;
};

struct lock_buf_s {
	unsigned int cur_idx;
	struct lock_info_s lock_info[];
};

void lock_trace_init(unsigned int cpuid);
void lock_trace_assert(unsigned int lock_type);
void lock_trace_push(vaddr_t lock, unsigned int lock_type, bool has_hold,
		     unsigned int cpuid, vaddr_t pc);
void lock_trace_pop(vaddr_t lock,
		    unsigned int cpuid);
void lock_trace_dump(bool nolock);
bool lock_trace_check_hold_lock(unsigned int curr_cpu);
bool lock_trace_check_curr_lock(unsigned int curr_cpu, unsigned int lock_type);
void lock_trace_check(unsigned int curr_cpu);
void lock_trace_disable(void);
JAM_INLINE void lock_trace_update_status(unsigned int cpuid);

#else

static inline void lock_trace_init(unsigned int cpuid)
{
	UNUSED(cpuid);
}

static inline void lock_trace_assert(unsigned int lock_type)
{
	UNUSED(lock_type);
}

static inline void lock_trace_push(vaddr_t lock, unsigned int lock_type, bool has_hold,
				   unsigned int cpuid, vaddr_t pc)
{
	UNUSED(lock, lock_type, has_hold, cpuid, pc);
}

static inline void lock_trace_pop(vaddr_t lock,
				  unsigned int cpuid)
{
	UNUSED(lock, cpuid);
}

static inline void lock_trace_update_status(unsigned int cpuid)
{
	UNUSED(cpuid);
}

static inline void lock_trace_dump(bool nolock)
{
	UNUSED(nolock);
}
static inline bool lock_trace_check_hold_lock(unsigned int curr_cpu) { return false; }
static inline bool lock_trace_check_curr_lock(unsigned int curr_cpu,
						unsigned int lock_type) {return false; }
static inline void lock_trace_check(unsigned int curr_cpu) { }
static inline void lock_trace_disable(void) { }

#endif /* defined(CONFIG_KLOCK_TRACE) || defined(__HOST_LLT__) */

#endif /* MAPI_HMKERNEL_LOCK_TRACE_H */
