/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Header file for refcnt
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 09 11:20:01 2023
 */

#ifndef KLIBS_REFCNT_H
#define KLIBS_REFCNT_H

#include <hmkernel/errno.h>
#include <hmkernel/types.h>
#include <lib/atomic.h>

#define __REFCOUNT_RELEASE_BIT  30
#define __REFCOUNT_COUNTER_MAX  0x3fffffff
#define __REFCOUNT_COUNTER_MASK 0x3fffffffU

static inline bool __refcnt_is_released(u32 val)
{
	return ((val & (1U << __REFCOUNT_RELEASE_BIT)) == 0U) ? false : true;
}

static inline int __refcnt_of(vatomic32_t *refcnt)
{
	return (int)(vatomic32_read(refcnt) & __REFCOUNT_COUNTER_MASK);
}

static inline int __refcnt_inc(vatomic32_t *refcnt)
{
	while (1) {
		u32 old_val = vatomic32_read_rlx(refcnt);
		if (__refcnt_is_released(old_val)) { /* object is marked for release */
			return E_HM_NOENT;
		}
		BUG_ON(old_val >= __REFCOUNT_COUNTER_MAX);
		u32 new_val = old_val + 1;
		if (vatomic32_cmpxchg(refcnt, old_val, new_val) == old_val) {
			return (int)new_val;
		}
	}
	__builtin_unreachable();
}

/*
 * we use bit:30 in refcnt as release flag, when refcnt decreased to 1, we need set it.
 * but, the value of refcnt may be modified by someone else before we set it,
 * in that case, we need try it again.
 */
static inline int __refcnt_dec(vatomic32_t *refcnt)
{
	u32 new_val, old_val, cnt;
	while (1) {
		old_val = vatomic32_read_rlx(refcnt);
		BUG_ON(old_val == 0U);
		if (__refcnt_is_released(old_val)) { /* object is marked for release */
			return E_HM_NOENT;
		}
		cnt = (u32)((unsigned int)old_val & __REFCOUNT_COUNTER_MASK);
		new_val = (cnt == 1U) ? (1U << __REFCOUNT_RELEASE_BIT) : (old_val - 1);
		if (vatomic32_cmpxchg(refcnt, old_val, new_val) == old_val) {
			return (int)(cnt - 1);
		}
	}
	__builtin_unreachable();
}

static inline void __refcnt_set(vatomic32_t *refcnt, u32 count)
{
	vatomic32_write_rlx(refcnt, (count & __REFCOUNT_COUNTER_MASK));
}

#endif
