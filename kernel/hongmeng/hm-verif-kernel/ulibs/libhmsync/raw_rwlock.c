/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Interfaces of raw_rwlock
 * Author: Huawei OS Kernel Lab
 * Create: Sun May 05 11:38:02 2019
 */
#include <hongmeng/types.h>
#include <hongmeng/errno.h>
#include <hmkernel/activation/local_area.h>
#include <libhwsecurec/securec.h>
#include <libhmactv/actv.h>
#include <libstrict/strict.h>
#include <stdbool.h>
#include "raw_thread_impl.h"
#include "uev_helper.h"

#include <vsync/atomic.h>
#include <libhmsync/raw_rwlock.h>
#include <hmkernel/attributes/hmattributes.h>

/*
 * The rwlock operation and memory ordering.
 * =========================================
 * Note that smp_mb() is used to show where the ordering must be guaranteed, but the
 * raw_rwlock is implemented by load/store/cas function with acquire/release semantic.
 *
 * raw_rwlock_rdlock                                       raw_rwlock_unlock
 *
 * loop:                                                       val=rwlock->val
 *
 *     val=rwlock->val                                         new=val-1
 *
 *     if(val!=WRLOCK)                   +-paired(A) with----->smp_mb()
 *                                       |
 *          cas(&rwlock->val,val,val+1)  |                     cas(&rwlock->val,val,new)
 *                                       |
 *          smp_mb()<--------------------+     +-------------->smp_mb()
 *                                             |
 *     else                                    |               if(!new && (val&WAIT_BIT || waiters))
 *                                             |
 *          waiters++                          |                    futex_wake(&rwlock)
 *                                             |
 *          cas(&rwlock->val,val,val|WAIT_BIT) |
 *                                             |
 *          smp_mb()<--------paired(B) with----+
 *
 *          futex_wait(&rwlock)
 *
 *          waiters--
 *
 *          goto loop
 * +-----------------------------------------------------------------------------------------------+
 *
 *     raw_rwlock_wrlock                                       raw_rwlock_unlock
 *
 * loop:                                                       val=rwlock->val
 *
 *     val=rwlock->val
 *
 *     if(val==0)                        +-paired(A) with----->smp_mb()
 *                                       |
 *          cas(&rwlock->val,0,WRLOCK)   |                     cas(&rwlock->val,val,0)
 *                                       |
 *          smp_mb()<--------------------+     +-------------->smp_mb()
 *                                             |
 *     else                                    |               if(val&WAIT_BIT || waiters)
 *                                             |
 *          waiters++                          |                    futex_wake(&rwlock)
 *                                             |
 *          cas(&rwlock->val,val,val|WAIT_BIT) |
 *                                             |
 *          smp_mb()<--------paired(B) with----+
 *
 *          futex_wait(&rwlock)
 *
 *          waiters--
 *
 *          goto loop
 *
 *
 * The pair(A) can guarantee that the code in critical zone will not
 * be executed outside the critical zone.
 *
 * The pair(B) can avoid the following scenarios(waiters=0, val=0):
 *           rwlock_rd/wrlock:     rwlock_unlock:
 *
 *           if (rwlock->val!=0)
 *             futex_wait          if(waiters) futex_wake
 *             waiters++           cas(&rwlock->val, val, 0)
 * Under this case, the waiter will not be woken up.
 */

/*
 * RWLOCK_WAITER_BIT means write lock caller exits,
 * RWLOCK_LOCK_MASK marks the maximum read lock callers.
 */
#define RWLOCK_WAITER_BIT 0x80000000U
#define RWLOCK_LOCK_MASK 0x3fffffffUL
#define WRLOCK_VALUE 0x3fffffffUL
#define RWLOCK_WRITER_BIT 0x40000000U
#define SPIN_TIMES 100

/*
 * Disable rwlock in special cases such as dump all process info
 * when trigger fiq or system hangup. It's safe to disable locks
 * as the system is already abnormal and will reboot.
 */
static int disable_rwlock_flag = 0;
#define DISABLE_MAGIC_NUM 0x55AA
#define raw_rwlock_disabled() (disable_rwlock_flag == DISABLE_MAGIC_NUM)

void disable_raw_rwlock(void)
{
	disable_rwlock_flag = DISABLE_MAGIC_NUM;
}

void enable_raw_rwlock(void)
{
	disable_rwlock_flag = 0;
}

_Bool is_raw_rwlock_disabled(void)
{
	return raw_rwlock_disabled();
}

static int shared_flag_of(const struct raw_rwlock *rwlock)
{
	return (int)(rwlock->attr.shared);
}

#ifdef CONFIG_WRHOLD_RAW_RWLOCK
static int wrhold_flag_of(const struct raw_rwlock *rwlock)
{
	return (int)(rwlock->attr.wrhold);
}
#endif
/*
 * Initialize rwlock.
 */
void raw_rwlock_init(struct raw_rwlock *rwlock, struct raw_rwlock_attr *attr)
{
	if (rwlock != NULL) {
		mem_zero_s(*rwlock);
		vatomic32_write_rlx(&rwlock->lock, 0);
		vatomic32_write_rlx(&rwlock->waiters, 0);
		lock_trace_init_lock_id(rwlock);
		lock_trace_log(rwlock, LOCKTRACE_EVENT_TYPE_RWLOCK_INIT);
		if (attr != NULL) {
			rwlock->attr.shared = attr->shared;
			rwlock->attr.wrhold = attr->wrhold;
			rwlock->attr.pip = attr->pip;
		}
	}
}

void raw_rwlock_attr_setshared(struct raw_rwlock_attr *attr, int shared)
{
	if (attr != NULL) {
		attr->shared = 0u;
		if (shared != 0) {
			attr->shared = 1u;
		}
	}
}

void raw_rwlock_attr_set_pip(struct raw_rwlock_attr *attr, int pip)
{
	if (attr != NULL) {
		attr->pip = 0U;
		if (pip != 0) {
			attr->pip = 1U;
		}
	}
}

void raw_rwlock_attr_set_wrhold(struct raw_rwlock_attr *attr, int wrhold)
{
	if (attr != NULL) {
		attr->wrhold = 0U;
		if (wrhold != 0) {
			attr->wrhold = 1U;
		}
	}
}

void raw_rwlock_init_pip(struct raw_rwlock_pip *rwlock_pip)
{
	struct raw_rwlock_attr attr;

	mem_zero_s(attr);
	mem_zero_a(rwlock_pip->holders);
	mem_zero_a(rwlock_pip->recursive_locks);
	vatomic64_init(&rwlock_pip->holders_bm, 0);
	raw_rwlock_attr_set_pip(&attr, 1);
	raw_rwlock_init(raw_rwlockpip_castto_rwlock(rwlock_pip), &attr);
}

/* Initialize rwlock with shared attribute */
void raw_rwlock_init_shared(struct raw_rwlock *rwlock)
{
	struct raw_rwlock_attr attr;

	mem_zero_s(attr);
	raw_rwlock_attr_setshared(&attr, 1);
	raw_rwlock_init(rwlock, &attr);
}

static void rwlock_holder_add_list(struct raw_rwlock_pip *rwlock, cref_t caller_cref)
{
	__u64 index = RAW_RWLOCK_MAX_HOLDER_CREFS;
	__u64 index_mask;
	__u64 prev_bm;

	if (caller_cref == 0ULL) {
		return;
	}

	index = caller_cref & RAW_RWLOCK_HOLDER_HASH_MASK;
	index_mask = 1ULL << index;

	/*
	   We don't allow new coming TCBs can be boosted when a bunch of TCBs
	   have been boosted, so a barrier is needed. Set the barrier first and
	   unconditionally. Then, if the condition is not satisfied, we will
	   unset the barrier.
	*/
	prev_bm = vatomic64_get_or(&rwlock->holders_bm, index_mask);
	if (!(prev_bm & RAW_RWLOCK_HOLDER_BOOSTING) && /* check if rwlock has boosted */
	    (((prev_bm & index_mask) != index_mask) || /* check for colissions */
	     (((prev_bm & index_mask) == index_mask) && (rwlock->holders[index] == caller_cref)))) {
		rwlock->holders[index] = caller_cref;
		++rwlock->recursive_locks[index];
	} else if ((prev_bm & index_mask) != index_mask) {
		/* if the holder is empty, need to unset the barrier for the previous unconditionally setting */
		vatomic64_and(&rwlock->holders_bm, ~index_mask);
	}
}

/* return true for need to unboost */
static bool rwlock_holder_remove_list(struct raw_rwlock_pip *rwlock, cref_t caller_cref)
{
	__u64 index;
	__u64 cur_bm;
	bool rst = false;

	if (caller_cref == 0ULL) {
		return rst;
	}

	index = caller_cref & RAW_RWLOCK_HOLDER_HASH_MASK;
	/* Verify the cref is the one that can be removed. Do not allow a colliding cref may try remove itself */
	if (rwlock->holders[index] == caller_cref) {
		/* Do not merge these ifs. Notice pre-decrement in if statement */
		if (--rwlock->recursive_locks[index] == 0) {
			rwlock->holders[index] = 0x00ULL;
			/* Prevents compiler from reordering bitmap update with array write */
			asm volatile("": : :"memory");
			cur_bm = vatomic64_and_get(&rwlock->holders_bm, ~(1ULL << index));
			if (cur_bm & RAW_RWLOCK_HOLDER_BOOSTING) {
				rst = true; /* unboost needed for this TCB */
			}
		}
	}

	return rst;
}

static int __raw_rwlock_tryrdlock(struct raw_rwlock *rwlock)
{
	uint32_t val, cnt;
	int rc = E_HM_OK;

	do {
		val = vatomic32_read_rlx(&rwlock->lock);
		cnt = (uint32_t)(val & RWLOCK_LOCK_MASK);
		/* write lock is occupied */
		if (cnt == WRLOCK_VALUE) {
			rc = E_HM_BUSY;
			break;
		}
#ifdef CONFIG_WRHOLD_RAW_RWLOCK
		/* RWLOCK_WRITER_BIT bit means a writing request is waiting for the lock. */
		if ((wrhold_flag_of(rwlock) != 0) && ((val & RWLOCK_WRITER_BIT) != 0)) {
			rc = E_HM_BUSY;
			break;
		}
#endif
		/* nonblock when rwlock overflow */
		if (cnt == RWLOCK_LOCK_MASK - 1U) {
			rc = E_HM_AGAIN;
			break;
		}
	} while (vatomic32_cmpxchg_acq(&rwlock->lock, val, val + 1U) != val);

	if (rc == E_HM_OK && rwlock->attr.pip) {
		rwlock_holder_add_list(raw_rwlock_castto_rwlockpip(rwlock), hm_actv_local_sched_thread_cref_of());
	}

	return rc;
}

static int raw_rwlock_timedrdlock(struct raw_rwlock *rwlock, const struct timespec *at,
				  unsigned long pc)
{
	uint32_t r, v;
	int ret = E_HM_OK;
	unsigned int flags = FUTEX_TYPE_PRIVATE;
	int spins = SPIN_TIMES;
	unsigned long bitset2_pip = 0x00UL;
	volatile cref_t *cref_array_p = NULL;
	struct __block_param_s block_param = {0};

	UNUSED(at);

	if (rwlock->attr.pip) {
		cref_array_p = (raw_rwlock_castto_rwlockpip(rwlock))->holders;
		bitset2_pip = FUTEX_BITSET2_RWLOCK_PIP;
	}

	if (shared_flag_of(rwlock) != 0) {
		flags = FUTEX_TYPE_SHARED;
	}

	/* do not take the lock when the system is abnormal */
	if (likely(!raw_rwlock_disabled()) &&
	    /* fastpath for apply a rlock */
	    (__raw_rwlock_tryrdlock(rwlock) != E_HM_OK)) {
		/* optimize spins */
		while ((spins-- != 0) &&
		       vatomic32_read_rlx(&rwlock->lock) != 0 &&
		       vatomic32_read_rlx(&rwlock->waiters) == 0) {
			raw_spin();
		}

		flags |= FUTEX_BLOCKED_TIMEDOUT_DETECT;
		while ((ret = __raw_rwlock_tryrdlock(rwlock)) == E_HM_BUSY) {
			/* if unlocked by others, continue to try-lock the rwlock */
			r = vatomic32_read_rlx(&rwlock->lock);
			if (((r & RWLOCK_LOCK_MASK) != WRLOCK_VALUE) && ((r & RWLOCK_WRITER_BIT) == 0)) {
				continue;
			}

			/* increse waiters */
			vatomic32_inc_rlx(&rwlock->waiters);

			/* set wait bit, will wakeup me when unlock the rwlock */
			v = r | RWLOCK_WAITER_BIT;
			(void)vatomic32_cmpxchg(&rwlock->lock, r, v);

			/* futex wait until lock the rwlock */
			hm_set_block_param(&block_param, pc);
			if (rwlock->attr.pip) {
				vatomic64_or(&(raw_rwlock_castto_rwlockpip(rwlock))->holders_bm, RAW_RWLOCK_HOLDER_BOOSTING);
			}
			ret = raw_futex_wait_ex(ptr_to_type(&rwlock->lock, int *), (int)v, flags,
						FUTEX_BITSET2_RD, bitset2_pip, (unsigned long)(uintptr_t)cref_array_p,
						&block_param);
			vatomic32_dec_rlx(&rwlock->waiters);

			/* futex wait error, and not E_HM_INTR will break */
			if (ret != E_HM_OK && ret != E_HM_INTR) {
				break;
			}
		}
	}
	return ret;
}

/*
 * Try to apply a read lock in non-block way,
 * if any writer exits, tryrlock failed and return immediately.
 */
int raw_rwlock_tryrdlock(struct raw_rwlock *rwlock)
{
	int rc = E_HM_OK;

	if (unlikely(raw_rwlock_disabled())) {
		return E_HM_OK;
	}

	if (unlikely(rwlock == NULL)) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		rc = __raw_rwlock_tryrdlock(rwlock);
		if (rc == E_HM_OK) {
			lock_trace_log(rwlock, LOCKTRACE_EVENT_TYPE_RWLOCK_READ_TRY_ACQUIRE_OK);
			/*
			 * try to save last owner of this rdlock, but current owner
			 * will not block other thread on the rdlock, so they maybe
			 * overwrite the owner of the rdlock, but it can be ignore
			 * due to rdlock owner is no guarantee of correctness.
			 */
		} else {
			lock_trace_log(rwlock, LOCKTRACE_EVENT_TYPE_RWLOCK_READ_TRY_ACQUIRE_FAIL);
		}
	}

	return rc;
}

static inline __always_inline int raw_rwlock_rdlock_fast(struct raw_rwlock *rwlock)
{
	uint32_t val, cnt;
	int rc = E_HM_OK;

	if (unlikely(rwlock == NULL)) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK && rwlock->attr.pip != 0) {
		rc = E_HM_BUSY;
	}
	if (rc == E_HM_OK) {
		val = vatomic32_read_rlx(&rwlock->lock);
	}
#ifdef CONFIG_WRHOLD_RAW_RWLOCK
	if (rc == E_HM_OK && wrhold_flag_of(rwlock) != 0 && (val & RWLOCK_WRITER_BIT) != 0) {
		rc = E_HM_BUSY;
	}
#endif
	if (rc == E_HM_OK) {
		cnt = (uint32_t)(val & RWLOCK_LOCK_MASK);
		if (cnt == WRLOCK_VALUE || cnt == RWLOCK_LOCK_MASK - 1U) {
			rc = E_HM_BUSY;
		}
	}
	if (rc == E_HM_OK && vatomic32_cmpxchg_acq(&rwlock->lock, val, val + 1U) != val) {
		rc = E_HM_BUSY;
	}

	return rc;
}

/*
 * Apply for a read lock.
 */
static int __noinline raw_rwlock_rdlock_slow(struct raw_rwlock *rwlock)
{
	int rc = E_HM_OK;

	if (rwlock == NULL) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		rc = raw_rwlock_timedrdlock(rwlock, NULL, ptr_to_ulong(__builtin_return_address(0U)));
	}

	lock_trace_log(rwlock, LOCKTRACE_EVENT_TYPE_RWLOCK_READ_ACQUIRE);
	return rc;
}

int raw_rwlock_rdlock(struct raw_rwlock *rwlock)
{
	int rc = E_HM_BUSY;

	if (likely(!raw_rwlock_disabled())) {
		rc = raw_rwlock_rdlock_fast(rwlock);
	}
	if (rc == E_HM_BUSY) {
		rc = raw_rwlock_rdlock_slow(rwlock);
	}
	return rc;
}

static int __raw_rwlock_trywrlock(struct raw_rwlock *rwlock)
{
	int rc = E_HM_OK;

	rc = (vatomic32_cmpxchg_acq(&rwlock->lock, 0, (unsigned int)WRLOCK_VALUE) != 0U) ? E_HM_BUSY : E_HM_OK;
	if (rc == E_HM_OK && rwlock->attr.pip) {
		rwlock_holder_add_list(raw_rwlock_castto_rwlockpip(rwlock), hm_actv_local_sched_thread_cref_of());
	}

	return rc;
}

static inline __always_inline int raw_rwlock_wrlock_fast(struct raw_rwlock *rwlock)
{
	int rc = E_HM_OK;

	if (unlikely(rwlock == NULL)) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK && rwlock->attr.pip != 0) {
		rc = E_HM_BUSY;
	}

	if (rc == E_HM_OK && vatomic32_cmpxchg_acq(&rwlock->lock, 0, (unsigned int)WRLOCK_VALUE) != 0U) {
		rc = E_HM_BUSY;
	}

	return rc;
}

static int raw_rwlock_timedwrlock(struct raw_rwlock *rwlock, const struct timespec *at,
				  unsigned long pc)
{
	uint32_t r, v;
	int ret = E_HM_OK;
	unsigned int flags = ((shared_flag_of(rwlock) != 0) ? FUTEX_TYPE_SHARED : FUTEX_TYPE_PRIVATE);
	int spins = SPIN_TIMES;
	unsigned long bitset2_pip = 0x00UL;
	volatile cref_t *cref_array_p = NULL;
	struct __block_param_s block_param = {0};

	UNUSED(at);

	if (rwlock->attr.pip) {
		cref_array_p = (raw_rwlock_castto_rwlockpip(rwlock))->holders;
		bitset2_pip = FUTEX_BITSET2_RWLOCK_PIP;
	}

	/* do not take the lock when the system is abnormal */
	if (likely(!raw_rwlock_disabled()) &&
	    /* fastpath for apply a rlock */
	    (__raw_rwlock_trywrlock(rwlock) != E_HM_OK)) {
		/* optimize spins */
		while (spins-- != 0 &&
		       vatomic32_read_rlx(&rwlock->lock) != 0 &&
		       vatomic32_read_rlx(&rwlock->waiters) == 0) {
			raw_spin();
		}

		flags |= FUTEX_BLOCKED_TIMEDOUT_DETECT;
		while ((ret = __raw_rwlock_trywrlock(rwlock)) == E_HM_BUSY) {
			r = vatomic32_read_rlx(&rwlock->lock);
			if ((r & RWLOCK_LOCK_MASK) == 0) {
				continue;
			}

			/* increse waiters */
			vatomic32_inc_rlx(&rwlock->waiters);

			/* writer before waiting on futex, set the writer bits. */
			v = r | RWLOCK_WAITER_BIT;
#ifdef CONFIG_WRHOLD_RAW_RWLOCK
			if (wrhold_flag_of(rwlock) != 0) {
				v |= RWLOCK_WRITER_BIT;
			}
#endif
			(void)vatomic32_cmpxchg(&rwlock->lock, r, v);

			/* futex wait until lock the rwlock */
			hm_set_block_param(&block_param, pc);
			if (rwlock->attr.pip) {
				vatomic64_or(&(raw_rwlock_castto_rwlockpip(rwlock))->holders_bm, RAW_RWLOCK_HOLDER_BOOSTING);
			}
			ret = raw_futex_wait_ex((int *)(uintptr_t)&rwlock->lock, (int)v, flags,
						FUTEX_BITSET2_WR, bitset2_pip, (unsigned long)(uintptr_t)cref_array_p,
						&block_param);
			vatomic32_dec_rlx(&rwlock->waiters);

			/* futex wait error, and not E_HM_INTR will break */
			if (ret != E_HM_OK && ret != E_HM_INTR) {
				break;
			}
		}
	}
	return ret;
}

/*
 * Try to apply a write lock in non-block way,
 * if any reader or writer exits, trywlock failed and return immediately.
 */
int raw_rwlock_trywrlock(struct raw_rwlock *rwlock)
{
	int rc = E_HM_OK;

	if (unlikely(raw_rwlock_disabled())) {
		return rc;
	}

	if (unlikely(rwlock == NULL)) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		rc = __raw_rwlock_trywrlock(rwlock);
		if (likely(rc == E_HM_OK)) {
			lock_trace_log(rwlock, LOCKTRACE_EVENT_TYPE_RWLOCK_WRITE_TRY_ACQUIRE_OK);
		} else {
			lock_trace_log(rwlock, LOCKTRACE_EVENT_TYPE_RWLOCK_WRITE_TRY_ACQUIRE_FAIL);
		}
	}

	return rc;
}

/*
 * Apply for a write lock.
 */
static int __noinline raw_rwlock_wrlock_slow(struct raw_rwlock *rwlock)
{
	int rc = E_HM_OK;

	if (rwlock == NULL) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		rc = raw_rwlock_timedwrlock(rwlock, NULL, ptr_to_ulong(__builtin_return_address(0U)));
	}

	lock_trace_log(rwlock, LOCKTRACE_EVENT_TYPE_RWLOCK_WRITE_ACQUIRE);
	return rc;
}

int raw_rwlock_wrlock(struct raw_rwlock *rwlock)
{
	int rc = E_HM_BUSY;

	if (likely(!raw_rwlock_disabled())) {
		rc = raw_rwlock_wrlock_fast(rwlock);
	}
	if (rc == E_HM_BUSY) {
		rc = raw_rwlock_wrlock_slow(rwlock);
	}
	return rc;
}

static inline __always_inline int raw_rwlock_unlock_fast(struct raw_rwlock *rwlock)
{
	uint32_t val, cnt, waiters, new;
	int rc = E_HM_OK;

	if (unlikely(rwlock == NULL)) {
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK && rwlock->attr.pip != 0) {
		rc = E_HM_BUSY;
	}
	/* check if the rwlock is locked */
	if (rc == E_HM_OK && (vatomic32_read_rlx(&rwlock->lock) & RWLOCK_LOCK_MASK) == 0U) {
		hm_warn("try to unlock a non-locked rwlock: rwlock=0x%lx, lock=0x%x, waiters=%u.\n",
			vaddr_of(rwlock), vatomic32_read_rlx(&rwlock->lock),
			vatomic32_read_rlx(&rwlock->waiters));
		__hmattr_sideff_resolved("no relative side effect");
		return E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		val = vatomic32_read_rlx(&rwlock->lock);
		waiters = vatomic32_read_rlx(&rwlock->waiters);
		if (waiters != 0 || (val & RWLOCK_WAITER_BIT) != 0) {
			rc = E_HM_BUSY;
		}
	}
	if (rc == E_HM_OK) {
		cnt = (unsigned int)(val & RWLOCK_LOCK_MASK);
		new = (cnt == WRLOCK_VALUE || cnt == 1U) ? (uint32_t)0 : val - 1U;
		if (vatomic32_cmpxchg_rel(&rwlock->lock, val, new) != val) {
			rc = E_HM_BUSY;
		}
	}

	return rc;
}

/*
 * Readlock minus 1 in each unlock.
 */
static int __noinline raw_rwlock_unlock_slow(struct raw_rwlock *rwlock)
{
	uint32_t val, cnt, waiters, new, rw_bitset, rw_bitset2, attr_pip;
	uint32_t bitset2_pip = 0x00U;
	int priv;

#ifdef CONFIG_RWLOCK_REDUCE_USELESS_WAKE
	rw_bitset = FUTEX_BITSET2_RD;
	rw_bitset2 = FUTEX_BITSET2_WR;
#else
	rw_bitset = FUTEX_BITSET_MATCH_ANY;
	rw_bitset2 = 0UL;
#endif

	if (rwlock == NULL) {
		__hmattr_sideff_resolved("no relative side effect");
		return E_HM_INVAL;
	}

	priv = (int)(shared_flag_of(rwlock) == 0);
	/* skip unlock in special cases */
	if (unlikely(raw_rwlock_disabled())) {
		__hmattr_sideff_resolved("no relative side effect");
		return 0;
	}

	lock_trace_log(rwlock, LOCKTRACE_EVENT_TYPE_RWLOCK_UNLOCK);
	/* check if the rwlock is locked */
	if ((vatomic32_read_rlx(&rwlock->lock) & RWLOCK_LOCK_MASK) == 0U) {
		hm_warn("try to unlock a non-locked rwlock: rwlock=0x%lx, lock=0x%x, waiters=%u.\n",
			vaddr_of(rwlock), vatomic32_read_rlx(&rwlock->lock),
			vatomic32_read_rlx(&rwlock->waiters));
		__hmattr_sideff_resolved("no relative side effect");
		return E_HM_INVAL;
	}

	attr_pip = rwlock->attr.pip;
	/* unlock a locked rwlock */
	do {
		val = vatomic32_read_rlx(&rwlock->lock);
		cnt = (unsigned int)(val & RWLOCK_LOCK_MASK);
		waiters = vatomic32_read_rlx(&rwlock->waiters);
		new = (cnt == WRLOCK_VALUE || cnt == 1U) ? (uint32_t)0 : val - 1U;
	} while (vatomic32_cmpxchg_rel(&rwlock->lock, val, new) != val);

	bool unboost_needed = false;
	if (attr_pip != 0) {
		unboost_needed = rwlock_holder_remove_list(raw_rwlock_castto_rwlockpip(rwlock), hm_actv_local_sched_thread_cref_of());
		bitset2_pip = FUTEX_BITSET2_RWLOCK_PIP;
	}

	/* wakeup unbloked waiters */
	if (((new & RWLOCK_LOCK_MASK) == 0U) && ((waiters != 0U) || ((val & RWLOCK_WAITER_BIT) != 0U))) {
		if (attr_pip != 0) { /* clear the boosting bit */
			vatomic64_and(&((raw_rwlock_castto_rwlockpip(rwlock))->holders_bm), ~(RAW_RWLOCK_HOLDER_BOOSTING));
		}
		(void)raw_futex_wake_ex(ptr_to_type(&rwlock->lock, int *), (int)cnt, priv,
					rw_bitset, bitset2_pip | rw_bitset2);
	} else if (unboost_needed) {
		(void)raw_futex_wake_ex(ptr_to_type(&rwlock->lock, int *), (int)cnt, priv,
					rw_bitset, FUTEX_BITSET2_RWLOCK_UNBOOST_ONLY | rw_bitset2);
	}

	return 0;
}

int raw_rwlock_unlock(struct raw_rwlock *rwlock)
{
	int rc = E_HM_BUSY;

	if (likely(!raw_rwlock_disabled())) {
		rc = raw_rwlock_unlock_fast(rwlock);
	}
	if (rc == E_HM_BUSY) {
		rc = raw_rwlock_unlock_slow(rwlock);
	}
	return rc;
}

void raw_rwlock_destroy(struct raw_rwlock *rwlock)
{
	UNUSED(rwlock);
}

int raw_rwlock_getwaiter(struct raw_rwlock *rwlock)
{
	return vatomic32_read_rlx(&rwlock->waiters);
}
