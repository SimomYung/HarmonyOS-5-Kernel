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

__hmattr_tag(LOCK_TRACE);
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
 * RWLOCK_HAS_WAITERS means the lock has waiters
 * RWLOCK_W_LOCKED means the lock's owner is a writer
 * RWLOCK_W_REQUESTED means the lock requeseted by writers
 */
#define LOCK_NUMBER(val)		((uint32_t)(val) >> RWLOCK_R_SHIFT)
#define RLOCK_INC(val)			((val) + RWLOCK_R_BIAS)
#define RLOCK_DEC(val)			((val) - RWLOCK_R_BIAS)

#define RWLOCK_IS_WAITING(val)		(RWLOCK_GET_FLAG(val, RWLOCK_HAS_WAITERS) != 0U)
#define RWLOCK_IS_W_REQUESTED(val)	(RWLOCK_GET_FLAG(val, RWLOCK_W_REQUESTED) != 0U)
#define RWLOCK_IS_LAST_READER(val)	(LOCK_VALUE(val) == RWLOCK_R_BIAS)
#define RWLOCK_IS_R_OVERFLOW(val)	(LOCK_VALUE(val) == RWLOCK_RLOCK_MAX)
#define RWLOCK_IS_R_EMPTY(val)		(LOCK_VALUE(val) == 0U)

#define SPIN_TIMES 100

#define raw_rwlock_xdep_cast_of(rwlock)	ptr_to_type(rwlock, struct raw_rwlock_xdep *)
#define raw_rwlock_cast_of(rwlock_xdep)	ptr_to_type(rwlock_xdep, struct raw_rwlock *)

/*
 * The owner is the thread's tpidr_el0 which holding the lock.
 * Usually aligned with 8 bytes, therefore bit0~bit2 is zero.
 *
 * But non aligned situations are also allowed,
 * thus we clear bit0~bit2 all the times.
 */
static uint32_t get_lock_owner(void)
{
	unsigned long owner = raw_get_tls();
	return LOCK_VALUE((uint32_t)owner);
}

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
#ifdef CONFIG_RAW_RWLOCK_RDLOCKOWNER
		uspinlock_init(&rwlock->rd_owner_lock);
		dlist_init(&rwlock->rd_owner_list);
#endif
		lock_trace_init_lock_id(rwlock);
		lock_trace_log(rwlock, LOCKTRACE_EVENT_TYPE_RWLOCK_INIT);
		rwlock->attr.reserved = RWLOCK_INIT_MAGIC;
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

static inline __always_inline
void do_xdep_clean_holders(struct raw_rwlock *rwlock)
{
	mem_zero_a(&((raw_rwlock_xdep_cast_of(rwlock))->holders));
	mem_zero_a(&((raw_rwlock_xdep_cast_of(rwlock))->recursive_locks));
	vatomic64_write(&((raw_rwlock_xdep_cast_of(rwlock))->holders_bm), 0ULL);
}

static inline __always_inline
void do_xdep_trace_remove(struct raw_rwlock *rwlock)
{
	if (xdep_trace_flag_of(rwlock) != 0) {
		(void)xdep_trace_holder_remove(raw_rwlock_xdep_cast_of(rwlock), sched_thread_cref_of());
	}
}
#else
void raw_rwlock_init_xdep(struct raw_rwlock_xdep *rwlock_xdep, struct raw_rwlock_attr *attr)
{
	BUG_ON_D("xdep is disabled, but trying to init rwlock as xdep. Please check!\n");
	raw_rwlock_init(raw_rwlock_cast_of(rwlock_xdep), attr);
}

#define xdep_trace_holder_conditional(rwlock)
#define do_xdep_trace_remove(rwlock)
#endif

#ifdef CONFIG_RAW_RWLOCK_RDLOCKOWNER
static void raw_rwlock_print_info(struct raw_rwlock *rwlock, const char info[])
{
#define RDLOCK_LIST_MAX 20U
	struct dlist_node *next_node = NULL;
	struct dlist_node *head_node = &rwlock->rd_owner_list;
	unsigned int i = 0;

	hm_warn("%s, rwlock info: %p[value:%x, waiter: %d, attr: %x, head:%p-%p]\n",
		info, rwlock, vatomic32_read_rlx(&rwlock->lock), vatomic32_read_rlx(&rwlock->waiters),
		rwlock->attr.flags, head_node->prev, head_node->next);
	dlist_for_each(next_node, head_node) {
		++i;
		if (i > RDLOCK_LIST_MAX) {
			/* the node list may be abnormal, check max node, avoiding a dead loop */
			break;
		}
		hm_warn("rdlock record: list[%p] prev[%p] next[%p]\n",
			next_node, next_node->prev, next_node->next);
	}
#undef RDLOCK_LIST_MAX
}

static void do_raw_rdlock_record_owner(struct raw_rwlock *rwlock, raw_thread_t thread)
{
	if (raw_thread_rdlock_has_free_owner_node(thread)) {
		struct dlist_node *node = raw_thread_rdlock_alloc_free_owner_node(thread);
		uspinlock_lock(&rwlock->rd_owner_lock);
		dlist_insert(&rwlock->rd_owner_list, node);
		uspinlock_unlock(&rwlock->rd_owner_lock);
	} else {
		raw_thread_rdlock_print_node(thread, "No free owner node left");
	}
}

static void raw_rdlock_record_owner_node(struct raw_rwlock *rwlock)
{
	if (likely(shared_flag_of(rwlock) == 0)) {
		unsigned long tls = raw_get_tls();
		if (likely(tls != 0)) {
			raw_thread_t thread = TLS_TO_THREAD(tls);
			do_raw_rdlock_record_owner(rwlock, thread);
		}
	}
}

static struct dlist_node *rdlock_seacrch_node(struct dlist_node *cur_node, struct dlist_node *head_node)
{
	struct dlist_node *next_node = head_node;

	if (likely(!dlist_empty(cur_node))) {
		dlist_for_each(next_node, head_node) {
			if (next_node == cur_node || unlikely(next_node == next_node->next)) {
				break;
			}
		}
	}

	return next_node;
}

static int rdlock_free_thread_node(raw_thread_t thread, struct dlist_node *head_node)
{
	int err = E_HM_NODATA;
	struct dlist_node *cur_node = raw_thread_rdlock_current_using_owner_node(thread);

	if (unlikely((cur_node == NULL) || dlist_empty(head_node))) {
		err = E_HM_NOENT;
	} else {
		RAW_THREAD_RDLOCK_EACH_PREV(thread, cur_node) {
			if (rdlock_seacrch_node(cur_node, head_node) == cur_node) {
				dlist_delete(cur_node);
				raw_thread_rdlock_free_current_owner_node(thread);
				err = E_HM_OK;
				break;
			}
		}
	}

	return err;
}

static void do_raw_rdlock_release_owner(struct raw_rwlock *rwlock, raw_thread_t thread)
{
	int err;
	bool is_abort = __true;
	struct dlist_node *head_node = &rwlock->rd_owner_list;

	uspinlock_lock(&rwlock->rd_owner_lock);
	err = rdlock_free_thread_node(thread, head_node);
	if ((err == E_HM_NOENT) || ((err == E_HM_NODATA) && raw_thread_rdlock_has_free_owner_node(thread))) {
		raw_rwlock_print_info(rwlock, "rdlock record abnormal: owner list is empty or out-of-order");
		raw_thread_rdlock_print_node(thread, "All thread node");
	} else {
		is_abort = __false;
	}
	uspinlock_unlock(&rwlock->rd_owner_lock);

	BUG_ON_D(is_abort, "rdlock record abnormal\n");
}

static void raw_rdlock_release_owner_node(struct raw_rwlock *rwlock)
{
	if (likely(shared_flag_of(rwlock) == 0)) {
		unsigned long tls = raw_get_tls();
		if (likely(tls != 0)) {
			raw_thread_t thread = TLS_TO_THREAD(tls);
			do_raw_rdlock_release_owner(rwlock, thread);
		}
	}
}
#else
#define raw_rdlock_release_owner_node(rwlock)
#define raw_rdlock_record_owner_node(rwlock)
#endif

static int __raw_rwlock_tryrdlock(struct raw_rwlock *rwlock)
{
	uint32_t val;
	int rc = E_HM_OK;

	do {
		val = vatomic32_read_rlx(&rwlock->lock);
		/* write lock is occupied */
		if (RWLOCK_IS_W_LOCKED(val)) {
			rc = E_HM_BUSY;
			break;
		}
#ifdef CONFIG_WRHOLD_RAW_RWLOCK
		/* RWLOCK_WRITER_BIT bit means a writing request is waiting for the lock. */
		if (RWLOCK_IS_W_REQUESTED(val)) {
			rc = E_HM_BUSY;
			break;
		}
#endif
		/* nonblock when rwlock overflow */
		if (unlikely(RWLOCK_IS_R_OVERFLOW(val))) {
			rc = E_HM_AGAIN;
			break;
		}
	} while (vatomic32_cmpxchg_acq(&rwlock->lock, val, RLOCK_INC(val)) != val);

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

	/* try fastpath for apply a rlock, first */
	if (__raw_rwlock_tryrdlock(rwlock) != E_HM_OK) {
		/* optimize spins */
		while ((spins-- != 0) &&
		       vatomic32_read_rlx(&rwlock->lock) != 0 &&
		       vatomic32_read_rlx(&rwlock->waiters) == 0) {
			raw_spin();
		}

		flags |= (FUTEX_BLOCKED_TIMEDOUT_DETECT | __FUTEX_BLOCKED_HAS_LOCK_OWNER);
		while ((ret = __raw_rwlock_tryrdlock(rwlock)) == E_HM_BUSY) {
			/* if unlocked by others, continue to try-lock the rwlock */
			r = vatomic32_read_rlx(&rwlock->lock);
			if (RWLOCK_IS_W_LOCKED(r)) {
				/* wrlock is holding, owner is saved in rwlock->lock */
#ifdef CONFIG_WRHOLD_RAW_RWLOCK
			} else if (RWLOCK_IS_W_REQUESTED(r)) {
				/* writer is waiting, new reader is not allowed to get lock */
				/* rdlock is holding, owner info is recorded in rwlock->rd_owner_list */
#endif
			} else {
				/* if unlocked by others, continue to try-lock the rwlock */
				continue;
			}

			/* increse waiters */
			vatomic32_inc_rlx(&rwlock->waiters);

			/* set wait bit, will wakeup me when unlock the rwlock */
			v = RWLOCK_SET_FLAG(r, RWLOCK_HAS_WAITERS);
			if (vatomic32_cmpxchg(&rwlock->lock, r, v) != r) {
				vatomic32_dec_rlx(&rwlock->waiters);
				continue;
			}

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
	if (likely(!raw_rwlock_disabled())) {
		rc = __raw_rwlock_tryrdlock(rwlock);
		if (rc == E_HM_OK) {
			/*
			 * try to save last owner of this rdlock, but current owner
			 * will not block other thread on the rdlock, so they maybe
			 * overwrite the owner of the rdlock, but it can be ignore
			 * due to rdlock owner is no guarantee of correctness.
			 */
			lock_trace_log(rwlock, LOCKTRACE_EVENT_TYPE_RWLOCK_READ_TRY_ACQUIRE_OK);
			raw_rdlock_record_owner_node(rwlock);
			xdep_trace_holder_conditional(rwlock);
		} else {
			lock_trace_log(rwlock, LOCKTRACE_EVENT_TYPE_RWLOCK_READ_TRY_ACQUIRE_FAIL);
		}
	}

	return rc;
}

static inline __always_inline int raw_rwlock_rdlock_fast(struct raw_rwlock *rwlock)
{
	uint32_t val;
	int rc = E_HM_OK;

	if (rc == E_HM_OK) {
		val = vatomic32_read_rlx(&rwlock->lock);
	}

	if ((rc == E_HM_OK) &&
	    (unlikely(RWLOCK_IS_R_OVERFLOW(val)) ||
#ifdef CONFIG_WRHOLD_RAW_RWLOCK
	     (RWLOCK_IS_W_REQUESTED(val)) ||
#endif
	     RWLOCK_IS_W_LOCKED(val))) {
		rc = E_HM_BUSY;
	}

	if (rc == E_HM_OK && vatomic32_cmpxchg_acq(&rwlock->lock, val, RLOCK_INC(val)) != val) {
		rc = E_HM_BUSY;
	}

	return rc;
}

/*
 * Apply for a read lock.
 */
static int __noinline raw_rwlock_rdlock_slow(struct raw_rwlock *rwlock)
{
	int rc = raw_rwlock_timedrdlock(rwlock, NULL, ptr_to_ulong(__builtin_return_address(0U)));
	lock_trace_log(rwlock, LOCKTRACE_EVENT_TYPE_RWLOCK_READ_ACQUIRE);
	return rc;
}

int raw_rwlock_rdlock(struct raw_rwlock *rwlock)
{
	int rc = E_HM_OK;
	if (likely(!raw_rwlock_disabled())) {
		rc = raw_rwlock_rdlock_fast(rwlock);
		if (rc == E_HM_BUSY) {
			rc = raw_rwlock_rdlock_slow(rwlock);
		}
		if (rc == E_HM_OK) {
			raw_rdlock_record_owner_node(rwlock);
			xdep_trace_holder_conditional(rwlock);
		}
	}

	return rc;
}

static int __raw_rwlock_trywrlock(struct raw_rwlock *rwlock, uint32_t owner)
{
	unsigned int new = RWLOCK_SET_FLAG(owner, RWLOCK_W_LOCKED);

	int rc = (vatomic32_cmpxchg_acq(&rwlock->lock, 0U, new) != 0U) ? E_HM_BUSY : E_HM_OK;

	return rc;
}

static inline __always_inline int raw_rwlock_wrlock_fast(struct raw_rwlock *rwlock, uint32_t owner)
{
	return __raw_rwlock_trywrlock(rwlock, owner);
}

static int raw_rwlock_timedwrlock(struct raw_rwlock *rwlock, const struct timespec *at,
				  unsigned long pc, uint32_t owner)
{
	uint32_t r, v;
	int ret = E_HM_OK;
	unsigned int flags = ((shared_flag_of(rwlock) != 0) ? FUTEX_TYPE_SHARED : FUTEX_TYPE_PRIVATE);
	int spins = SPIN_TIMES;
	unsigned long bitset2_xdep = 0x00UL;
	volatile cref_t *holder_crefs = NULL;
	struct __block_param_s block_param = {0};
	UNUSED(at);

	/* try fastpath for apply a wlock, first */
	if (__raw_rwlock_trywrlock(rwlock, owner) != E_HM_OK) {
		/* optimize spins */
		while (spins-- != 0 &&
		       vatomic32_read_rlx(&rwlock->lock) != 0 &&
		       vatomic32_read_rlx(&rwlock->waiters) == 0) {
			raw_spin();
		}

		flags |= (FUTEX_BLOCKED_TIMEDOUT_DETECT | __FUTEX_BLOCKED_HAS_LOCK_OWNER);
		while ((ret = __raw_rwlock_trywrlock(rwlock, owner)) == E_HM_BUSY) {
			r = vatomic32_read_rlx(&rwlock->lock);
			if (unlikely(!RWLOCK_IS_LOCKED(r))) {
				/* if unlocked by others, continue to try-lock the rwlock */
				continue;
			} else {
				/* rdlock is holding, owner info is recorded in rwlock->rd_owner_list */
				/* wrlock is holding, owner is saved in rwlock->lock */
			}

			/* increse waiters */
			vatomic32_inc_rlx(&rwlock->waiters);

			/* writer before waiting on futex, set the writer bits. */
			v = RWLOCK_SET_FLAG(r, RWLOCK_HAS_WAITERS);
#ifdef CONFIG_WRHOLD_RAW_RWLOCK
			if (wrhold_flag_of(rwlock) != 0) {
				v = RWLOCK_SET_FLAG(v, RWLOCK_W_REQUESTED);
			}
#endif
			if (vatomic32_cmpxchg(&rwlock->lock, r, v) != r) {
				vatomic32_dec_rlx(&rwlock->waiters);
				continue;
			}

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
	uint32_t owner = get_lock_owner();
	int rc = __raw_rwlock_trywrlock(rwlock, owner);
	if (rc == E_HM_OK) {
		lock_trace_log(rwlock, LOCKTRACE_EVENT_TYPE_RWLOCK_WRITE_TRY_ACQUIRE_OK);
		xdep_trace_holder_conditional(rwlock);
	} else if (unlikely(raw_rwlock_disabled())) {
		rc = E_HM_OK;
	} else {
		lock_trace_log(rwlock, LOCKTRACE_EVENT_TYPE_RWLOCK_WRITE_TRY_ACQUIRE_FAIL);
	}

	return rc;
}

/*
 * Apply for a write lock.
 */
static int __noinline raw_rwlock_wrlock_slow(struct raw_rwlock *rwlock, uint32_t owner)
{
	int rc = raw_rwlock_timedwrlock(rwlock, NULL, ptr_to_ulong(__builtin_return_address(0U)), owner);
	lock_trace_log(rwlock, LOCKTRACE_EVENT_TYPE_RWLOCK_WRITE_ACQUIRE);
	return rc;
}

int raw_rwlock_wrlock(struct raw_rwlock *rwlock)
{
	uint32_t owner = get_lock_owner();
	int rc = raw_rwlock_wrlock_fast(rwlock, owner);
	if (rc == E_HM_BUSY) {
		if (unlikely(raw_rwlock_disabled())) {
			rc = E_HM_OK;
		} else {
			rc = raw_rwlock_wrlock_slow(rwlock, owner);
		}
	}
	if ((rc == E_HM_OK) && likely(!raw_rwlock_disabled())) {
		xdep_trace_holder_conditional(rwlock);
	}

	return rc;
}

#define RAW_RWLOCK_PRINT_UNLOCK_FAILED(rwlock, val, waiters)		\
	hm_warn("unlock not allowed, becase of not locked by self"	\
		": rwlock=%p, lock=0x%x, waiters=%u\n",			\
		(rwlock), (val), (waiters))

static inline __always_inline
int do_raw_wrlock_unlock_fast(struct raw_rwlock *rwlock, uint32_t val, uint32_t waiters)
{
	int err = E_HM_OK;
	if (unlikely(LOCK_VALUE(val) != get_lock_owner())) {
		RAW_RWLOCK_PRINT_UNLOCK_FAILED(rwlock, val, waiters);
		err = E_HM_INVAL;
	} else {
		do_xdep_trace_remove(rwlock);
		if ((waiters != 0) || RWLOCK_IS_WAITING(val)) {
			/* Someone is waiting, neeed wake up by slow path */
			err = E_HM_BUSY;
		} else if (vatomic32_cmpxchg_rel(&rwlock->lock, val, 0U) != val) {
			/* Currently, the rwlock is busy, need unlock by slow path */
			err = E_HM_BUSY;
		}
	}

	return err;
}

static inline __always_inline
int do_raw_rdlock_unlock_fast(struct raw_rwlock *rwlock, uint32_t val, uint32_t waiters)
{
	int err = E_HM_OK;
	if (unlikely(RWLOCK_IS_R_EMPTY(val))) {
		RAW_RWLOCK_PRINT_UNLOCK_FAILED(rwlock, val, waiters);
		err = E_HM_INVAL;
	} else {
		/*
		 * release owner node must before write rwlock->lock,
		 * if not the memory of rwlock may be free.
		 */
		raw_rdlock_release_owner_node(rwlock);
		do_xdep_trace_remove(rwlock);
		if ((waiters != 0) || RWLOCK_IS_WAITING(val)) {
			/* Someone is waiting, neeed wake up by slow path */
			err = E_HM_BUSY;
		} else if (vatomic32_cmpxchg_rel(&rwlock->lock, val, RLOCK_DEC(val)) != val) {
			/* Currently, the rwlock is busy, need unlock by slow path */
			err = E_HM_BUSY;
		}
	}

	return err;
}

static inline __always_inline
int raw_rwlock_unlock_fast(struct raw_rwlock *rwlock, uint32_t val, uint32_t waiters)
{
	int err;
	if (RWLOCK_IS_W_LOCKED(val)) {
		err = do_raw_wrlock_unlock_fast(rwlock, val, waiters);
	} else {
		err = do_raw_rdlock_unlock_fast(rwlock, val, waiters);
	}

	return err;
}

static __noinline
int raw_rwlock_unlock_slow(struct raw_rwlock *rwlock, uint32_t val)
{
	/*
	 * when in the path means:
	 * 1. lock value is check ok and needn't check again;
	 * 2.1 unlock for write: need wake up others always;
	 * 2.2 unlock for read: count == 0 mean need wake up;
	 * 2.3 thus, when new == 0, need wake up;
	 * 3.1 the lock owner node has been released in fastpath;
	 * 3.2 the xdep holder has been removed in fastpath;
	 * 4. always return E_HM_OK;
	 */
	uint32_t cnt;
	uint32_t old = val;
	uint32_t new;
	uint32_t last;
	int private = (int)(shared_flag_of(rwlock) == 0);

#ifdef CONFIG_RWLOCK_REDUCE_USELESS_WAKE
	uint32_t base_bitset = __FUTEX_BITSET2_RD;
	uint32_t first_bitset = __FUTEX_BITSET2_WR;
#else
	uint32_t base_bitset = FUTEX_BITSET_MATCH_ANY;
	uint32_t first_bitset = 0UL;
#endif

#ifdef CONFIG_XDEP_RAW_RWLOCK_MULTI
	int attr_xdep_trace = xdep_trace_flag_of(rwlock);
#endif
	uint32_t xdep_bitset = 0U;

	lock_trace_log(rwlock, LOCKTRACE_EVENT_TYPE_RWLOCK_UNLOCK);
	if (RWLOCK_IS_W_LOCKED(old)) {
		cnt = (uint32_t)(-1);
		new = 0U;
		do {
			last = old;
		} while((old = vatomic32_cmpxchg_rel(&rwlock->lock, last, new)) != last);
	} else {
		cnt = 1U;
		do {
			last = old;
			new = RWLOCK_IS_LAST_READER(old) ? 0U : RLOCK_DEC(old);
			old = vatomic32_cmpxchg_rel(&rwlock->lock, last, new);
		} while (old != last);
		BUG_ON_D(RWLOCK_IS_R_OVERFLOW(new));
	}

	/* wakeup blocked waiters */
	if (new == 0) {
#ifdef CONFIG_XDEP_RAW_RWLOCK_MULTI
		if (attr_xdep_trace != 0) {
			xdep_bitset = __FUTEX_BITSET2_XDEP_RWLOCK_MULTI;
			/* all holders are gone, and clear the boosting bit to welcome new holders */
			do_xdep_clean_holders(rwlock);
		}
#endif
		(void)raw_futex_wake_ex(ptr_to_type(&rwlock->lock, int *), (int)cnt, private,
					base_bitset, first_bitset | xdep_bitset);
	}

	return E_HM_OK;
}

int raw_rwlock_unlock(struct raw_rwlock *rwlock)
{
	int err = E_HM_BUSY;
	uint32_t val = vatomic32_read_rlx(&rwlock->lock);
	uint32_t waiters = vatomic32_read_rlx(&rwlock->waiters);

	/*
	 * must check before do unlock:
	 * when lock is disabled,
	 * 1. allow A wrlock, and B unlock, but do nothing;
	 * 2. unlock rdlock do nothing, and return E_HM_OK directly,
	 *    otherwise, the owner information will be cleared.
	 */
	if (unlikely(raw_rwlock_disabled())) {
		err = E_HM_OK;
	}

	if (likely(err == E_HM_BUSY)) {
		err = raw_rwlock_unlock_fast(rwlock, val, waiters);
	}
	if (err == E_HM_BUSY) {
		err = raw_rwlock_unlock_slow(rwlock, val);
	}

	return err;
}

static inline __always_inline bool raw_rwlock_downgrade_fast(struct raw_rwlock *rwlock, unsigned int old)
{
	return vatomic32_cmpxchg_acq(&rwlock->lock, old, RWLOCK_R_BIAS) == old;
}

static int __noinline raw_rwlock_downgrade_slow(struct raw_rwlock *rwlock, unsigned int old)
{
	uint32_t val;
	int priv;

#ifdef CONFIG_RWLOCK_REDUCE_USELESS_WAKE
	uint32_t rw_bitset = __FUTEX_BITSET2_RD;
	uint32_t rw_bitset2 = __FUTEX_BITSET2_WR;
#else
	uint32_t rw_bitset = FUTEX_BITSET_MATCH_ANY;
	uint32_t rw_bitset2 = 0UL;
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
		BUG_ON(old != RWLOCK_VALUE(val),
		       "try to downgrade a non-wrlocked rwlock: rwlock=0x%lx, lock=0x%x, waiters=%u, owner=0x%x\n",
		       vaddr_of(rwlock), val, vatomic32_read_rlx(&rwlock->waiters), old);
	/* here, don't consider writing requests, change to rdlock directly */
	} while (vatomic32_cmpxchg(&rwlock->lock, val, RWLOCK_R_BIAS) != val);

	/* wakeup blocked waiters */
	if (RWLOCK_IS_WAITING(val) || (vatomic32_read_rlx(&rwlock->waiters) > 0)) {
		uint32_t bitset2_xdep = 0x00U;
#ifdef CONFIG_XDEP_RAW_RWLOCK_MULTI
		if (attr_xdep_trace != 0) {
			bitset2_xdep = __FUTEX_BITSET2_XDEP_RWLOCK_MULTI;
			/* all holders are gone, and clear the boosting bit to welcome new holders */
			do_xdep_clean_holders(rwlock);
		}
#endif
		/* cnt = -1 maen wake all waiters */
		(void)raw_futex_wake_ex(ptr_to_type(&rwlock->lock, int *), (int)(-1), priv,
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
	unsigned int old = RWLOCK_SET_FLAG(get_lock_owner(), RWLOCK_W_LOCKED);

	if (!raw_rwlock_downgrade_fast(rwlock, old)) {
		rc = raw_rwlock_downgrade_slow(rwlock, old);
	}
	if (likely(!raw_rwlock_disabled())) {
		raw_rdlock_record_owner_node(rwlock);
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
	unsigned int new = RWLOCK_SET_FLAG(get_lock_owner(), RWLOCK_W_LOCKED);
	int ret = (vatomic32_cmpxchg_acq(&rwlock->lock, RWLOCK_R_BIAS, new) ==
		   RWLOCK_R_BIAS) ? E_HM_OK : E_HM_BUSY;

	lock_trace_log(rwlock, ret == E_HM_OK?
		       LOCKTRACE_EVENT_TYPE_RWLOCK_TRY_UPGRADE_OK: LOCKTRACE_EVENT_TYPE_RWLOCK_TRY_UPGRADE_FAIL);
	if (ret == E_HM_OK) {
		raw_rdlock_release_owner_node(rwlock);
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
