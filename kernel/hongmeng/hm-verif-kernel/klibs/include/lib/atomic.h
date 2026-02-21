/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Atomic operations
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 14 11:58:01 CST 2020
 */
#ifndef KLIBS_ATOMIC_H
#define KLIBS_ATOMIC_H

#include <hmkernel/compiler.h>
#include <hmkernel/types.h>
#include <lib/vsync_atomic.h>

JAM_EXTERN_INLINE int
atomic64_add_with_bound_smp(vatomic64_t *a, u64 val, u64 bound);

JAM_EXTERN_INLINE int
atomic64_minus_with_bound_smp(vatomic64_t *a, u64 val, u64 bound);

/* Kernel doesn't have preemption, so non SMP ops fallback to normal
 * memory read/write. */
static inline
int atomic64_add_with_bound_non_smp(vatomic64_t *a, u64 val, u64 bound)
{
	int ret = 0;
	u64 oldv = vatomic64_read_rlx(a);
	if (oldv + val > bound || oldv + val < oldv) {
		ret = -1;
	} else {
		vatomic64_write_rlx(a, oldv + val);
	}
	return ret;
}

static inline
int atomic64_minus_with_bound_non_smp(vatomic64_t *a, u64 val, u64 bound)
{
	int ret = 0;
	u64 oldv = vatomic64_read_rlx(a);
	if (oldv - val < bound || oldv - val > oldv) {
		ret = -1;
	} else {
		vatomic64_write_rlx(a, oldv - val);
	}
	return ret;
}

JAM_EXTERN_INLINE int
atomic_add_with_bound_smp(vatomic32_t *a, u32 val, u32 bound);

JAM_EXTERN_INLINE int
atomic_minus_with_bound_smp(vatomic32_t *a, u32 val, u32 bound);

/* Kernel doesn't have preemption, so non SMP ops fallback to normal
 * memory read/write. */
static inline
int atomic_add_with_bound_non_smp(vatomic32_t *a, u32 val, u32 bound)
{
	int ret = 0;
	u32 oldv = vatomic32_read_rlx(a);
	if (oldv + val > bound || oldv + val < oldv) {
		ret = -1;
	} else {
		vatomic32_write_rlx(a, oldv + val);
	}
	return ret;
}

static inline
int atomic_minus_with_bound_non_smp(vatomic32_t *a, u32 val, u32 bound)
{
	int ret = 0;
	u32 oldv = vatomic32_read_rlx(a);
	if (oldv - val < bound || oldv - val > oldv) {
		ret = -1;
	} else {
		vatomic32_write_rlx(a, oldv - val);
	}
	return ret;
}
#endif
