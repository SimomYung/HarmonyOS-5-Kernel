/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Ctrlmem rwlock related defs
 * Author: Huawei OS Kernel Lab
 * Created: Tue Jun 27 2023
 */

#ifndef MAPI_HMKERNEL_CTRLMEM_KOBJS_RWLOCK_H
#define MAPI_HMKERNEL_CTRLMEM_KOBJS_RWLOCK_H

#include <uapi/hmkernel/ctrlmem/kobjs/rwlock.h>
#include <hmkernel/ctrlmem/syspg/syspg.h>
#include <hmkernel/atomic.h>
#include <hmkernel/errno.h>
#include <hmkernel/types.h>
#include <hmkernel/panic.h>
#include <hmasm/memory.h>

#define CTRLMEM_RWLOCK_RDSHIFT		__CTRLMEM_RWLOCK_RDSHIFT
#define CTRLMEM_RWLOCK_WRSHIFT		__CTRLMEM_RWLOCK_WRSHIFT
#define CTRLMEM_RWLOCK_RDMASK		__CTRLMEM_RWLOCK_RDMASK
#define CTRLMEM_RWLOCK_WRMASK		__CTRLMEM_RWLOCK_WRMASK
#define CTRLMEM_RWLOCK_RD_MAXCNT	__CTRLMEM_RWLOCK_RD_MAXCNT
#define CTRLMEM_RWLOCK_WR_MAXCNT	__CTRLMEM_RWLOCK_WR_MAXCNT

struct ctrlmem_kobj_rwlock_s {
	vatomic32_t counter;
};

static inline __always_inline int ctrlmem_kobj_tryrdlock(struct ctrlmem_kobj_rwlock_s *lock)
{
	int err = E_HM_OK;
	unsigned int iter_num = 0;
	u32 reader_count;
	u32 newval;
	u32 old_post;

	old_post = vatomic32_read_rlx(&lock->counter);
	do {
		reader_count = (old_post & CTRLMEM_RWLOCK_RDMASK);
		if (reader_count == CTRLMEM_RWLOCK_RD_MAXCNT) {
			err = E_HM_OVERFLOW;
			break;
		}
		newval = reader_count + 1;
		old_post = vatomic32_cmpxchg(&lock->counter, reader_count, newval);
		if (old_post == reader_count) {
			err = E_HM_OK;
			break;
		}
		if ((old_post >> CTRLMEM_RWLOCK_WRSHIFT) != 0) {
			err = E_HM_NOENT;
			break;
		} else {
			err = E_HM_AGAIN;
		}
		iter_num++;
#define MAX_ITER_NUM		16
		if (iter_num > MAX_ITER_NUM) {
			err = E_HM_OVERFLOW;
			break;
		}
	} while (err == E_HM_AGAIN);

	return err;
}

static inline __always_inline void ctrlmem_kobj_rdunlock(struct ctrlmem_kobj_rwlock_s *lock)
{
	int err = E_HM_OK;
	u32 old_pre;
	u32 reader_count;
	u32 newval;
	u32 old_post;

	old_pre = vatomic32_read_rlx(&lock->counter);
	do {
		reader_count = (old_pre & CTRLMEM_RWLOCK_RDMASK);
		if (reader_count == 0) {
			panic("ctrlmem_kobj_rdunlock failed.\n");
		}
		newval = (reader_count - 1) | (old_pre & CTRLMEM_RWLOCK_WRMASK);
		old_post = vatomic32_cmpxchg(&lock->counter, old_pre, newval);
		if (old_post == old_pre) {
			break;
		} else {
			err = E_HM_AGAIN;
			old_pre = old_post;
		}
	} while (err == E_HM_AGAIN);
}

static inline __always_inline int ctrlmem_kobj_trywrlock(struct ctrlmem_kobj_rwlock_s *lock)
{
	int err = E_HM_OK;
	if (vatomic32_cmpxchg(&lock->counter, 0U, (1U <<  CTRLMEM_RWLOCK_WRSHIFT)) != 0U) {
		err = E_HM_NOENT;
	}
	return err;
}

static inline __always_inline void ctrlmem_kobj_wrunlock(struct ctrlmem_kobj_rwlock_s *lock)
{
	vatomic32_write(&lock->counter, 0U);
}

#ifdef CONFIG_CTRLMEM_PAGEFAULT_PAGE_META_MERGE
static inline __always_inline int ctrlmem_syspg_trylock(struct syspg_knode_s *pg)
{
	int err = E_HM_OK;
#if defined(CONFIG_CTRLMEM_PAGEFAULT_COCALL) || defined(__HOST_LLT__)
	__u64 newv;
	__u64 oldv;
	__u64 curr;

	curr = vatomic64_read((vatomic64_t *)(uptr_t)&pg->flags);
	newv = curr | ~SYSMGR_PAGE_CTRLMEM_BITLOCK_MASK;
	oldv = curr & SYSMGR_PAGE_CTRLMEM_BITLOCK_MASK;
	if (oldv != vatomic64_cmpxchg((vatomic64_t *)(uptr_t)&pg->flags, oldv, newv)) {
		err = E_HM_BUSY;
	}
#else
	UNUSED(pg);
#endif
	return err;
}

static inline __always_inline void ctrlmem_syspg_unlock(struct syspg_knode_s *pg)
{
#if defined(CONFIG_CTRLMEM_PAGEFAULT_COCALL) || defined(__HOST_LLT__)
	__u64 newv;
	__u64 oldv;
	__u64 curr;

	curr = vatomic64_read((vatomic64_t *)(uptr_t)&pg->flags);
	do {
		if ((curr & ~SYSMGR_PAGE_CTRLMEM_BITLOCK_MASK) == 0) {
			break;
		}
		newv = curr & SYSMGR_PAGE_CTRLMEM_BITLOCK_MASK;
		oldv = curr | ~SYSMGR_PAGE_CTRLMEM_BITLOCK_MASK;
		curr = vatomic64_cmpxchg((vatomic64_t *)(uptr_t)&pg->flags, oldv, newv);
	} while (oldv != curr);
#else
	UNUSED(pg);
#endif
}
#else /* !CONFIG_CTRLMEM_PAGEFAULT_PAGE_META_MERGE */
static inline __always_inline int ctrlmem_syspg_trylock(struct syspg_knode_s *pg)
{
	int err = E_HM_OK;
#if defined(CONFIG_CTRLMEM_PAGEFAULT_COCALL) || defined(__HOST_LLT__)
	__u32 newv;
	__u32 oldv;
	__u32 curr;

	curr = vatomic32_read((vatomic32_t *)(uptr_t)&pg->lock);
	newv = curr | ~__SYSPG_L_CTRLMEM_MASK;
	oldv = curr & __SYSPG_L_CTRLMEM_MASK;
	if (oldv != vatomic32_cmpxchg((vatomic32_t *)(uptr_t)&pg->lock, oldv, newv)) {
		err = E_HM_BUSY;
	}
#else
	UNUSED(pg);
#endif
	return err;
}

static inline __always_inline void ctrlmem_syspg_unlock(struct syspg_knode_s *pg)
{
#if defined(CONFIG_CTRLMEM_PAGEFAULT_COCALL) || defined(__HOST_LLT__)
	__u32 newv;
	__u32 oldv;
	__u32 curr;

	curr = vatomic32_read((vatomic32_t *)(uptr_t)&pg->lock);
	do {
		if ((curr & ~__SYSPG_L_CTRLMEM_MASK) == 0) {
			break;
		}
		newv = curr & __SYSPG_L_CTRLMEM_MASK;
		oldv = curr | ~__SYSPG_L_CTRLMEM_MASK;
		curr = vatomic32_cmpxchg((vatomic32_t *)(uptr_t)&pg->lock, oldv, newv);
	} while (oldv != curr);
#else
	UNUSED(pg);
#endif
}
#endif

#endif
