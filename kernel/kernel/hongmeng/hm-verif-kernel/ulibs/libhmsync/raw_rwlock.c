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

#define raw_rwlock_xdep_cast_of(rwlock)	ptr_to_type(rwlock, struct raw_rwlock_xdep *)
#define raw_rwlock_cast_of(rwlock_xdep)	ptr_to_type(rwlock_xdep, struct raw_rwlock *)

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

#ifdef CONFIG_XDEP_RAW_RWLOCK_MULTI
static inline int xdep_flag_of(const struct raw_rwlock *rwlock)
{
	return (int)(rwlock->attr.xdep);
}

static inline int xdep_trace_flag_of(const struct raw_rwlock *rwlock)
{
	return (int)(rwlock->attr.xdep_trace);
}

static inline void xdep_trace_set_flag(struct raw_rwlock *rwlock)
{
	rwlock->attr.xdep_trace = 1;
}

static inline void raw_rwlock_attr_set_xdep(struct raw_rwlock_attr *attr, int xdep)
{
	if (attr != NULL) {
		attr->xdep = 0U;
		if (xdep != 0) {
			attr->xdep = 1U;
		}
	}
}

void raw_rwlock_init_xdep(struct raw_rwlock_xdep *rwlock_xdep, struct raw_rwlock_attr *attr)
{
	if (rwlock_xdep != NULL) {
		struct raw_rwlock_attr _attr, *pattr;

		if (attr == NULL) {
			mem_zero_s(_attr);
			pattr = &_attr;
		} else {
			pattr = attr;
		}

		mem_zero_a(rwlock_xdep->holders);
		mem_zero_a(rwlock_xdep->recursive_locks);
		vatomic64_init(&rwlock_xdep->holders_bm, 0);

		raw_rwlock_attr_set_xdep(pattr, 1);
		raw_rwlock_init(raw_rwlock_cast_of(rwlock_xdep), pattr);
	}
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
			rwlock->attr.xdep = attr->xdep;
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

void raw_rwlock_attr_set_wrhold(struct raw_rwlock_attr *attr, int wrhold)
{
	if (attr != NULL) {
		attr->wrhold = 0U;
		if (wrhold != 0) {
			attr->wrhold = 1U;
		}
	}
}

/* Initialize rwlock with shared attribute */
void raw_rwlock_init_shared(struct raw_rwlock *rwlock)
{
	struct raw_rwlock_attr attr;

	mem_zero_s(attr);
	raw_rwlock_attr_setshared(&attr, 1);
	raw_rwlock_init(rwlock, &attr);
}

#ifdef CONFIG_XDEP_RAW_RWLOCK_MULTI
#define XDEP_HOLDER_HASH_MASK   	(RAW_RWLOCK_XDEP_MAX_HOLDERS - 1U)
#define XDEP_HASH_SHIFT_1 		(5U)
#define XDEP_HASH_SHIFT_2 		(4U)
#define XDEP_TRIES_ON_COLLISION 	(2U)
#define XDEP_HOLDER_BOOSTING    	(1ULL << 63) /* boosting barrier */

static inline __u64 xdep_cref_hash_index(cref_t cref, __u32 shift)
{
	return (cref ^ (cref >> shift)) & XDEP_HOLDER_HASH_MASK;
}

static void xdep_trace_holder_add(struct raw_rwlock_xdep *rwlock, cref_t cref)
{
	__u64 index = RAW_RWLOCK_XDEP_MAX_HOLDERS;
	__u64 index_mask;
	__u64 prev_bm;

	BUG_ON_D(cref == 0ULL);

	index = xdep_cref_hash_index(cref, XDEP_HASH_SHIFT_1);
	for (__u32 tries = 0; tries < XDEP_TRIES_ON_COLLISION; tries++) {
		index_mask = 1ULL << index;
		/*
		Incoming TCBs cannot be tracked when boosting is already in process.
		We reserve the spot for the cref unconditionally,
		then unset the reserved bit later if we detect ongoing boosting.
		*/
		prev_bm = vatomic64_get_or(&rwlock->holders_bm, index_mask);
		/* Conditions below must be met to track incoming cref */
		/* Cond  Val	Desc
			1 	true => no boosting active
			2	true =>	no collision due to bit being set by atomic_or, first request
			3	true =>	no collision due to recursive locking
			Only one of conds 2 & 3 needed for holder tracking */
		if (((prev_bm & XDEP_HOLDER_BOOSTING) == 0) &&
			(((prev_bm & index_mask) != index_mask) ||
			(((prev_bm & index_mask) == index_mask) && (rwlock->holders[index] == cref)))) {
			rwlock->holders[index] = cref;
			++rwlock->recursive_locks[index];
			break;
		} else if ((prev_bm & index_mask) != index_mask) {
			/* boosting in progress ==> must reset bit for previously reserved slot */
			vatomic64_and(&rwlock->holders_bm, ~index_mask);
			break;
		}

		/* get second hash because failed to store cref in first iteration */
		index = xdep_cref_hash_index(cref, XDEP_HASH_SHIFT_2);
	}
}

/* return true when removed holder need to unboost */
static bool xdep_trace_holder_remove(struct raw_rwlock_xdep *rwlock, cref_t cref)
{
	__u64 index;
	__u64 prev_bm;
	bool ret = false;

	BUG_ON_D(cref == 0ULL);

	index = xdep_cref_hash_index(cref, XDEP_HASH_SHIFT_1);
	for (__u32 tries = 0; tries < XDEP_TRIES_ON_COLLISION; tries++) {
		/* Verify the cref is the one that can be removed. Do not allow a colliding cref may try remove itself */
		if (rwlock->holders[index] == cref) {
			/* Do not merge these ifs. Notice pre-decrement in if statement */
			if (--rwlock->recursive_locks[index] == 0) {
				rwlock->holders[index] = 0x00ULL;
				/* Prevents compiler from reordering bitmap update with array write */
				asm volatile("": : :"memory");
				prev_bm = vatomic64_and_get(&rwlock->holders_bm, ~(1ULL << index));
				/* unboost needed when boosting detected at time of holder removal */
				ret = (prev_bm & XDEP_HOLDER_BOOSTING) ? true : false;
			}
			/* cref either accounted for or removed ==> exit the loop */
			break;
		}
		/* get second hash because cref not found in first iteration */
		index = xdep_cref_hash_index(cref, XDEP_HASH_SHIFT_2);
	}

	return ret;
}

#define CREF_SAMPLE_RATE	5000U
static unsigned long long sched_thread_cref_of(void)
{
	unsigned long long cref = hm_actv_local_sched_thread_cref_of();
#ifdef CONFIG_DEBUG_BUILD
	static unsigned int cref_sample_cnt = 0;
	cref_sample_cnt++;
	if (cref_sample_cnt > CREF_SAMPLE_RATE) {
		unsigned long long syscall_cref = 0ULL;
		sysfast_thread_cref(&syscall_cref);
		BUG_ON_D(cref != syscall_cref, "cref is different from actv local and syscall!\n");
		cref_sample_cnt = 0U;
	}
#endif
	return cref;
}

static inline void xdep_trace_holder_conditional(struct raw_rwlock *rwlock)
{
	if (xdep_trace_flag_of(rwlock) != 0) {
		xdep_trace_holder_add(raw_rwlock_xdep_cast_of(rwlock), sched_thread_cref_of());
	}
}

/*
 * Conditionally set the xdep trace flag based on whether thread is critical.
 * If the rwlock has set the xdep attr, this function will judge whether critical
 * threads wait on this rwlock. If the rwlock engaged with critical threads, the
 * xdep_trace attr will be set, and the rwlock begins to trace holders. Otherwise,
 * xdep will not waste power to trace holders.
 *
 * return:
 * 	true: need to notify kernel futex to do the boost
 */
static bool xdep_trace_set_flag_conditional(struct raw_rwlock *rwlock)
{
	bool ret = false;

	if (xdep_flag_of(rwlock) != 0) { /* if this rwlock has enable the xdep for multi holders */
		if (xdep_trace_flag_of(rwlock) != 0) {
			ret = true; /* this rwlock has marked as tracing, then notify kernel futex to do the boost */
			/* lable this rwlock is boosting now, new holders will not be tracked during the boosting */
			vatomic64_or(&(raw_rwlock_xdep_cast_of(rwlock))->holders_bm, XDEP_HOLDER_BOOSTING);
		} else {
			/* if this lock is the first time to identify that it involves critical tasks,
			set the flag which begins to trace holders */
			if (hm_actv_local_thread_xdep_critical_of()) {
				xdep_trace_set_flag(rwlock); /* once set, it will not unset anymore */
			}
		}
	}

	return ret;
}

static void do_xdep_clean_holders(struct raw_rwlock *rwlock)
{
	mem_zero_a((raw_rwlock_xdep_cast_of(rwlock))->holders);
	mem_zero_a((raw_rwlock_xdep_cast_of(rwlock))->recursive_locks);
	vatomic64_write(&((raw_rwlock_xdep_cast_of(rwlock))->holders_bm), 0ULL);
}
#else
void raw_rwlock_init_xdep(struct raw_rwlock_xdep *rwlock_xdep, struct raw_rwlock_attr *attr)
{
	BUG_ON_D("xdep is disabled, but trying to init rwlock as xdep. Please check!\n");
	raw_rwlock_init(raw_rwlock_cast_of(rwlock_xdep), attr);
}
 
#define xdep_trace_holder_conditional(rwlock)
#endif

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

	return rc;
}

static int raw_rwlock_timedrdlock(struct raw_rwlock *rwlock, const struct timespec *at,
				  unsigned long pc)
{
	uint32_t r, v;
	int ret = E_HM_OK;
	unsigned int flags = FUTEX_TYPE_PRIVATE;
	int spins = SPIN_TIMES;
	unsigned long bitset2_xdep = 0x00UL;
	volatile cref_t *holder_crefs = NULL;
	struct __block_param_s block_param = {0};
	UNUSED(at);

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
#ifdef CONFIG_XDEP_RAW_RWLOCK_MULTI
			if (xdep_trace_set_flag_conditional(rwlock)) {
				holder_crefs = (raw_rwlock_xdep_cast_of(rwlock))->holders;
				bitset2_xdep = __FUTEX_BITSET2_XDEP_RWLOCK_MULTI;
			}
#endif
			ret = raw_futex_wait_ex(ptr_to_type(&rwlock->lock, int *), (int)v, flags,
						__FUTEX_BITSET2_RD, bitset2_xdep, ptr_to_ulong(holder_crefs),
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
			xdep_trace_holder_conditional(rwlock);
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
	if ((rc == E_HM_OK) && likely(!raw_rwlock_disabled())) {
		xdep_trace_holder_conditional(rwlock);
	}
	return rc;
}

static int __raw_rwlock_trywrlock(struct raw_rwlock *rwlock)
{
	int rc = E_HM_OK;

	rc = (vatomic32_cmpxchg_acq(&rwlock->lock, 0, (unsigned int)WRLOCK_VALUE) != 0U) ? E_HM_BUSY : E_HM_OK;

	return rc;
}

static inline __always_inline int raw_rwlock_wrlock_fast(struct raw_rwlock *rwlock)
{
	int rc = E_HM_OK;

	if (unlikely(rwlock == NULL)) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK && vatomic32_cmpxchg_acq(&rwlock->lock, 0, (unsigned int)WRLOCK_VALUE) != 0U) {
		rc = E_HM_BUSY;
	}

#ifdef CONFIG_XDEP_RAW_RWLOCK_MULTI
	if (rc == E_HM_OK) {
		xdep_trace_holder_conditional(rwlock);
	}
#endif

	return rc;
}

static int raw_rwlock_timedwrlock(struct raw_rwlock *rwlock, const struct timespec *at,
				  unsigned long pc)
{
	uint32_t r, v;
	int ret = E_HM_OK;
	unsigned int flags = ((shared_flag_of(rwlock) != 0) ? FUTEX_TYPE_SHARED : FUTEX_TYPE_PRIVATE);
	int spins = SPIN_TIMES;
	unsigned long bitset2_xdep = 0x00UL;
	volatile cref_t *holder_crefs = NULL;
	struct __block_param_s block_param = {0};
	UNUSED(at);

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
#ifdef CONFIG_XDEP_RAW_RWLOCK_MULTI
			if (xdep_trace_set_flag_conditional(rwlock)) {
				holder_crefs = (raw_rwlock_xdep_cast_of(rwlock))->holders;
				bitset2_xdep = __FUTEX_BITSET2_XDEP_RWLOCK_MULTI;
			}
#endif
			ret = raw_futex_wait_ex(ptr_to_type(&rwlock->lock, int *), (int)v, flags,
						__FUTEX_BITSET2_WR, bitset2_xdep, ptr_to_ulong(holder_crefs),
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
			xdep_trace_holder_conditional(rwlock);
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
	if ((rc == E_HM_OK) && likely(!raw_rwlock_disabled())) {
		xdep_trace_holder_conditional(rwlock);
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
#ifdef CONFIG_XDEP_RAW_RWLOCK_MULTI
	if ((rc == E_HM_OK || rc == E_HM_BUSY) && xdep_trace_flag_of(rwlock) != 0) {
		(void)xdep_trace_holder_remove(raw_rwlock_xdep_cast_of(rwlock), sched_thread_cref_of());
	}
#endif
	if (rc == E_HM_OK) {
		cnt = (unsigned int)(val & RWLOCK_LOCK_MASK);
		new = (cnt == WRLOCK_VALUE || cnt == 1U) ? (uint32_t)0 : val - 1U;
		/* try to unlock the rwlock */
		if (vatomic32_cmpxchg_rel(&rwlock->lock, val, new) != val) {
			/* Try unlock failure, and currently the rwlock is busy now.
			   Thus, the unlock needs to be tooken over by slow path. */
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
	uint32_t val, cnt, waiters, new, rw_bitset, bitset2_rw;
	int priv;

#ifdef CONFIG_RWLOCK_REDUCE_USELESS_WAKE
	rw_bitset = __FUTEX_BITSET2_RD;
	bitset2_rw = __FUTEX_BITSET2_WR;
#else
	rw_bitset = FUTEX_BITSET_MATCH_ANY;
	bitset2_rw = 0UL;
#endif

	if (rwlock == NULL) {
		__hmattr_sideff_resolved("no relative side effect");
		return E_HM_INVAL;
	}

#ifdef CONFIG_XDEP_RAW_RWLOCK_MULTI
	int attr_xdep_trace = xdep_trace_flag_of(rwlock);
#endif

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

	/* guarantee the rwlock must be unlocked */
	do {
		val = vatomic32_read_rlx(&rwlock->lock);
		cnt = (unsigned int)(val & RWLOCK_LOCK_MASK);
		waiters = vatomic32_read_rlx(&rwlock->waiters);
		new = (cnt == WRLOCK_VALUE || cnt == 1U) ? (uint32_t)0 : val - 1U;
	} while (vatomic32_cmpxchg_rel(&rwlock->lock, val, new) != val);

	/* wakeup blocked waiters */
	if (((new & RWLOCK_LOCK_MASK) == 0U) && ((waiters != 0U) || ((val & RWLOCK_WAITER_BIT) != 0U))) {
		uint32_t bitset2_xdep = 0x00U;
#ifdef CONFIG_XDEP_RAW_RWLOCK_MULTI
		if (attr_xdep_trace != 0) {
			bitset2_xdep = __FUTEX_BITSET2_XDEP_RWLOCK_MULTI;
			/* all holders are gone, and clear the boosting bit to welcome new holders */
			do_xdep_clean_holders(rwlock);
		}
#endif
		(void)raw_futex_wake_ex(ptr_to_type(&rwlock->lock, int *), (int)cnt, priv,
					rw_bitset, bitset2_xdep | bitset2_rw);
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

static inline __always_inline bool raw_rwlock_downgrade_fast(struct raw_rwlock *rwlock)
{
	BUG_ON_D(rwlock == NULL);
	return vatomic32_cmpxchg_acq(&rwlock->lock, WRLOCK_VALUE, 1U) == WRLOCK_VALUE;
}

static int __noinline raw_rwlock_downgrade_slow(struct raw_rwlock *rwlock)
{
	uint32_t val, cnt, waiters, new, rw_bitset, rw_bitset2;
	int priv;

#ifdef CONFIG_RWLOCK_REDUCE_USELESS_WAKE
	rw_bitset = __FUTEX_BITSET2_RD;
	rw_bitset2 = __FUTEX_BITSET2_WR;
#else
	rw_bitset = FUTEX_BITSET_MATCH_ANY;
	rw_bitset2 = 0UL;
#endif

	priv = (int)(shared_flag_of(rwlock) == 0);
	/* skip downgrade in special cases */
	if (unlikely(raw_rwlock_disabled())) {
		__hmattr_sideff_resolved("no relative side effect");
		return 0;
	}

	lock_trace_log(rwlock, LOCKTRACE_EVENT_TYPE_RWLOCK_DOWNGRADE);
#ifdef CONFIG_XDEP_RAW_RWLOCK_MULTI
	int attr_xdep_trace = xdep_trace_flag_of(rwlock);
#endif
	/* downgrade a wrlocked rwlock */
	do {
		val = vatomic32_read_rlx(&rwlock->lock);
		cnt = (unsigned int)(val & RWLOCK_LOCK_MASK);
		waiters = vatomic32_read_rlx(&rwlock->waiters);
		BUG_ON(cnt != WRLOCK_VALUE,
		       "try to downgrade a non-wrlocked rwlock: rwlock=0x%lx, lock=0x%x, waiters=%u.\n",
		       vaddr_of(rwlock), val, waiters);
	} while (vatomic32_cmpxchg(&rwlock->lock, val, 1U) != val);

	/* wakeup blocked waiters */
	if (((new & RWLOCK_LOCK_MASK) == 0U) && ((waiters != 0U) || ((val & RWLOCK_WAITER_BIT) != 0U))) {
		uint32_t bitset2_xdep = 0x00U;
#ifdef CONFIG_XDEP_RAW_RWLOCK_MULTI
		if (attr_xdep_trace != 0) {
			bitset2_xdep = __FUTEX_BITSET2_XDEP_RWLOCK_MULTI;
			/* all holders are gone, and clear the boosting bit to welcome new holders */
			do_xdep_clean_holders(rwlock);
		}
#endif
		(void)raw_futex_wake_ex(ptr_to_type(&rwlock->lock, int *), (int)cnt, priv,
					rw_bitset, bitset2_xdep | rw_bitset2);
	}

	return 0;
}

/*
 * downgrade a wrlock to rdlock
 * MUST called with wrlock hold by current thread.
 */
int raw_rwlock_downgrade(struct raw_rwlock *rwlock)
{
	int rc = E_HM_OK;

	if (!raw_rwlock_downgrade_fast(rwlock)) {
		rc = raw_rwlock_downgrade_slow(rwlock);
	}
	return rc;
}

/*
 * try upgrade a rdlock to wrlock
 * MUST called with rdlock hold by current thread,
 * and success ONLY IF there's no other threads hold rdlock,
 * return E_HM_BUSY otherwise
 */
int raw_rwlock_try_upgrade(struct raw_rwlock *rwlock)
{
	BUG_ON_D(rwlock == NULL);
	int ret = (vatomic32_cmpxchg_acq(&rwlock->lock, 1U, WRLOCK_VALUE) == 1U) ? E_HM_OK : E_HM_BUSY;
	if (ret == E_HM_OK) {
		lock_trace_log(rwlock, LOCKTRACE_EVENT_TYPE_RWLOCK_TRY_UPGRADE_OK);
	} else {
		lock_trace_log(rwlock, LOCKTRACE_EVENT_TYPE_RWLOCK_TRY_UPGRADE_FAIL);
	}
	return ret;
}

void raw_rwlock_destroy(struct raw_rwlock *rwlock)
{
	UNUSED(rwlock);
}

int raw_rwlock_getwaiter(struct raw_rwlock *rwlock)
{
	return vatomic32_read_rlx(&rwlock->waiters);
}
