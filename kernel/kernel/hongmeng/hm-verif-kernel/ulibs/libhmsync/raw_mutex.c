/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Interfaces of raw_mutex
 * Author: Huawei OS Kernel Lab
 * Create: Sat Oct 27 15:11:53 2018
 */
#include <stdbool.h>
#include <hongmeng/errno.h>
#include <hmkernel/barrier.h>
#include <hmkernel/rt_futex.h>
#include <hmkernel/activation/local_area.h>
#include <libhmsync/raw_thread.h>
#include <libhmsrv_sys/hm_futex.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmactv/actv.h>
#include <libhwsecurec/securec.h>
#include <hongmeng/types.h>
#include <libstrict/strict.h>
#include <vsync/atomic.h>
#include <libhmsync/raw_mutex.h>

#include "raw_thread_impl.h"
#include "uev_helper.h"
#include "lockdep_impl.h"

#define SPIN_TIMES 100

#ifdef __HOST_LLT__
static int llt_thread_get_current_tid(void);
#define thread_get_current_tid() llt_thread_get_current_tid()
#endif

/*
 * The mutex operation and memory ordering.
 * ======================================
 * Note that smp_mb() is used to show where the ordering must be guaranteed, but the
 * raw_mutex is implemented by load/store/cas function with acquire/release semantic.
 *
 *     raw_mutex_lock                                          raw_mutex_unlock
 *
 * loop:                                                       val=mutex->val
 *
 *     val=mutex->val
 *
 *     if(val==0)                        +-paired(A) with----->smp_mb()
 *                                       |
 *          cas(&mutex->val,0,LOCK)      |                     cas(&mutex->val,val,0)
 *                                       |
 *          smp_mb()<--------------------+     +-------------->smp_mb()
 *                                             |
 *     else                                    |               if(val&WAIT_BIT || waiters)
 *                                             |
 *          waiters++                          |                    futex_wake(&mutex)
 *                                             |
 *          cas(&mutex->val,val,val|WAIT_BIT)  |
 *                                             |
 *          smp_mb()<--------paired(B) with----+
 *
 *          futex_wait(&mutex)
 *
 *          waiters--
 *
 *          goto loop
 *
 * The pair(A) can guarantee that the code in critical zone will not
 * be executed outside the critical zone.
 *
 * The pair(B) can avoid the following scenarios(waiters=0, val=0):
 *           raw_mutex_lock:       raw_mutex_unlock:
 *
 *           if (mutex->val != 0)
 *             futex_wait          if(waiters) futex_wake
 *             waiters++           cas(&mutex->val, val, 0)
 * Under this case, the waiter will not be woken up.
 */

/*
 * Disable mutex in special cases such as dump all process info
 * when trigger fiq or system hangup. It's safe to disable locks
 * as the system is already abnormal and will reboot.
 */
static int disable_mutex_flag = 0;
#define DISABLE_MAGIC_NUM 0x55AA
#define raw_mutex_disabled() (disable_mutex_flag == DISABLE_MAGIC_NUM)

void disable_raw_mutex(void)
{
	disable_mutex_flag = DISABLE_MAGIC_NUM;
}

void enable_raw_mutex(void)
{
	disable_mutex_flag = 0;
}

_Bool is_raw_mutex_disabled(void)
{
	return raw_mutex_disabled();
}

static unsigned int
type_of(const struct raw_mutex *mutex)
{
	/*
	 * don't need protection, type is initialized after mutex initialization
	 * and will be never changed.
	 */
	return mutex->attr.__v.type;
}

static int
shared_flag_of(const struct raw_mutex *mutex)
{
	return (int)mutex->attr.__v.shared;
}

static int
waiters_of(const struct raw_mutex *mutex)
{
	return (int)mutex->attr.__v.waiters;
}

static int
inc_counters(struct raw_mutex *mutex)
{
	raw_mutex_attr_t val;
	uint32_t old_v, new_v;
	int rc = E_HM_OK;

	do {
		/* Check comment in raw_mutex_dec_waiters() */
		val.v._v = __read_once(mutex->attr.v._v);
		if (unlikely(val.__v.counters == RAW_MUTEX_COUNTERS_MAX)) {
			rc = E_HM_AGAIN;
			break;
		}
		old_v = val.v._v;
		val.__v.counters++;
		new_v = val.v._v;
	} while (vatomic32_cmpxchg_rlx(&mutex->attr.v, old_v, new_v) != old_v);

	return rc;
}

static int
dec_counters(struct raw_mutex *mutex)
{
	raw_mutex_attr_t val;
	uint32_t old_v, new_v;

	do {
		/* Check comment in raw_mutex_dec_waiters() */
		val.v._v = __read_once(mutex->attr.v._v);
		if (unlikely(val.__v.counters == 0)) {
			hm_panic("counters should be nonzero, mutex: 0x%lx, v: %u\n",
				 vaddr_of(mutex), vatomic32_read_rlx(&val.v));
		}
		old_v = val.v._v;
		val.__v.counters--;
		new_v = val.v._v;
	} while (vatomic32_cmpxchg_rlx(&mutex->attr.v, old_v, new_v) != old_v);

	return mutex->attr.__v.counters;
}

static uint32_t owner_of(struct raw_mutex *mutex)
{
	uint32_t owner = 0U;
#ifdef CONFIG_XDEP_RAW_MUTEX
	if (shared_flag_of(mutex) == 0) {
		uint32_t actv_cref = (uint32_t)raw_thread_cref_of(raw_thread_self());
		owner = actv_cref << __FUTEX_XDEP_VAL_SHIFT_OWNER_ID;
	} else {
		owner = (uint32_t)raw_get_and_check_tls();
	}
#else
	UNUSED(mutex);
	owner = (uint32_t)raw_get_and_check_tls();
#endif
	return owner;
}

static int
try_lock_recursive(struct raw_mutex *mutex)
{
	unsigned int old;
	int ret;
	unsigned int owner = owner_of(mutex);

	/*
	 * owner == 0:
	 *   1. New owner not visible, because cas_acq. But ok because it
	 *      gets rejected by next cas_acq.
	 *   2. Unlocked, nice.
	 *   3. Self just get lock, but get scheduled. Acquire is better.
	 *      [impossible with migration full fence]
	 *
	 * owner == tls & 0xffffffff:
	 *   1. Holding the lock, nice.
	 *   2. Just before unlock's swp_acq_rel, problematic with reentrance.
	 *
	 * owner == others: good
	 */
	old = vatomic32_cmpxchg_acq(&mutex->lock, 0, owner);
	if ((old & ~MUTEX_FLAGS) == owner || old == 0) {
		ret = inc_counters(mutex);
	} else {
		ret = E_HM_BUSY;
	}
	return ret;
}

static int
try_lock(struct raw_mutex *mutex)
{
	uint32_t old = 0;
	unsigned int type;
	int ret;

	type = type_of(mutex);
#ifndef CONFIG_LOCKDEP
	if ((type & RAW_MUTEX_TYPE_MASK) == 0) {
#else
	if ((type & RAW_MUTEX_TYPE_MASK) == 0 || (type & RAW_MUTEX_LOCKDEP) != 0) {
#endif
		/* Normal mutex */
		unsigned int owner = owner_of(mutex);
		old = vatomic32_cmpxchg_acq(&mutex->lock, 0, owner);
		ret = (old == 0 ? E_HM_OK : E_HM_BUSY);
	} else if ((type & RAW_MUTEX_RECURSIVE) != 0U) {
		/* recursive mutex */
		ret = try_lock_recursive(mutex);
#ifdef CONFIG_RT_FUTEX
	} else if ((type & RAW_MUTEX_PI) != 0) {
		int tid;
		/* PI mutex */
		tid = thread_get_current_tid();
		old = vatomic32_cmpxchg_acq(&mutex->lock, 0, (unsigned int)tid);
		ret = (old == 0 ? E_HM_OK : E_HM_BUSY);
#endif
	} else {
		hm_panic("couldn't reach here, mutex = 0x%lx, mutex.lock = %u, mutex.v = %u\n",
			 vaddr_of(mutex), vatomic32_read_rlx(&mutex->lock), vatomic32_read_rlx(&mutex->attr.v));
	}
	return ret;
}

void raw_mutex_init(struct raw_mutex *mutex)
{
	raw_mutex_init_with_attr(mutex, NULL);
}

void raw_mutex_init_recursive(struct raw_mutex *mutex)
{
	struct raw_mutex_attr attr;

	raw_mutex_attr_init(&attr);
	raw_mutex_attr_settype(&attr, RAW_MUTEX_RECURSIVE);
	raw_mutex_init_with_attr(mutex, &attr);
}

void raw_mutex_init_with_attr(struct raw_mutex *mutex,
			      const struct raw_mutex_attr *attr)
{
	if (mutex != NULL) {
		vatomic32_write_rlx(&mutex->lock, 0);
		vatomic32_write_rlx(&mutex->attr.v, 0);
		lock_trace_init_lock_id(mutex);
		lock_trace_log(mutex, LOCKTRACE_EVENT_TYPE_MUTEX_INIT);
		lock_dep_map_init(mutex);
		if (attr != NULL) {
			mutex->attr.__v.shared = ((attr->attr & RAW_MUTEX_SHARED_MASK) != 0U) ? 1 : 0;
			mutex->attr.__v.type   = attr->attr & RAW_MUTEX_TYPE_MASK;
		}
	}
}

void raw_mutex_attr_init(struct raw_mutex_attr *attr)
{
	if (attr != NULL) {
		mem_zero_s(*attr);
	}
}
void raw_mutex_attr_settype(struct raw_mutex_attr *attr, unsigned int type)
{
	if (attr != NULL && (type & ~RAW_MUTEX_TYPE_MASK) == 0) {
		attr->attr |= type;
	}
}

void raw_mutex_attr_setshared(struct raw_mutex_attr *attr, int shared)
{
	if (attr != NULL) {
		attr->attr &= ~RAW_MUTEX_SHARED_MASK;
		if (shared != 0) {
			attr->attr |= RAW_MUTEX_SHARED_MASK;
		}
	}
}

void raw_mutex_destroy(struct raw_mutex *mutex)
{
	UNUSED(mutex);
}

void raw_mutex_init_shared(struct raw_mutex *mutex)
{
	struct raw_mutex_attr attr;

	raw_mutex_attr_init(&attr);
	raw_mutex_attr_setshared(&attr, 1);
	raw_mutex_init_with_attr(mutex, &attr);
}

#ifdef CONFIG_RT_FUTEX
void raw_mutex_init_pi(struct raw_mutex *mutex)
{
	struct raw_mutex_attr attr;

	raw_mutex_attr_init(&attr);
	raw_mutex_attr_settype(&attr, RAW_MUTEX_PI);
	raw_mutex_init_with_attr(mutex, &attr);
}

static int raw_mutex_lock_pi(struct raw_mutex *mutex)
{
	int r;

	do {
		r = hm_futex_wait_pi(ptr_to_type(&mutex->lock, int *),
					RT_FUTEX_BLOCKED_TIMEDOUT_DETECT, NULL);
	} while (r == E_HM_INTR || r == E_HM_HMSIGINTR);

	if (r != E_HM_OK) {
		hm_panic("raw_mutex_lock_pi failed: %s\n", hmstrerror(r));
	}
	return r;
}

static int raw_mutex_unlock_pi(struct raw_mutex *mutex)
{
	uint32_t v;
	int r;
	bool b = true;

	v = vatomic32_read_rlx(&mutex->lock);
	if (((unsigned long)v & RT_FUTEX_HAS_WAITERS) == 0UL) {
		uint32_t ret;

		ret = vatomic32_cmpxchg_rel(&mutex->lock, v, 0);
		if (ret == v) {
			r = 0;
			b = false;
		}
	}
	if (b) {
		do {
			r = hm_futex_wake_pi(ptr_to_type(&mutex->lock, int *), 0u);
		} while (r == E_HM_INTR || r == E_HM_HMSIGINTR);
		if (r != E_HM_OK) {
			hm_panic("raw_mutex_unlock_pi failed: %s\n", hmstrerror(r));
		}
	}
	return r;
}
#else
void raw_mutex_init_pi(struct raw_mutex *mutex)
{
	UNUSED(mutex);
	hm_warn("unsupported pi mutex\n");
	return;
}

static int raw_mutex_lock_pi(struct raw_mutex *mutex)
{
	UNUSED(mutex);
	hm_warn("unsupported pi mutex\n");
	return E_HM_NOSYS;
}
static int raw_mutex_unlock_pi(struct raw_mutex *mutex)
{
	UNUSED(mutex);
	hm_warn("unsupported pi mutex\n");
	return E_HM_NOSYS;
}
#endif

static int raw_mutex_wait_lock(struct raw_mutex *mutex, unsigned int flags,
			       unsigned long pc)
{
	uint32_t r, v, old;
	unsigned long bitset2 = 0UL;
	int ret;
	struct __block_param_s block_param = {0};

	hm_set_block_param(&block_param, pc);

	while ((ret = try_lock(mutex)) == E_HM_BUSY) {
		/* if unlocked by others, continues to try-lock the mutex */
		if ((r = vatomic32_read_rlx(&mutex->lock)) == 0) {
			continue;
		}

		/*
		 * XXX: later, we can feel if the owner is on cpu or not,
		 * and spin for a while.
		 */
		raw_mutex_inc_waiters(mutex);

		/* set wait bit, will wakeup me when unlock the mutex */
		v = r | MUTEX_HAS_WAITERS;
		old = vatomic32_cmpxchg(&mutex->lock, r, v);
		if (old != r) {
			/*
			 * if old != r means failed to set lock, and lock is changed by others.
			 * continues to try lock.
			 */
			raw_mutex_dec_waiters(mutex);
			continue;
		}

#ifdef CONFIG_XDEP_RAW_MUTEX
		if (shared_flag_of(mutex) == 0) {
			bitset2 |= __FUTEX_BITSET2_XDEP_MUTEX;
		}
#endif
		/* futex wait until lock the mutex */
		ret = raw_futex_wait_ex(ptr_to_type(&mutex->lock, int *), (int)v,
					flags, FUTEX_BITSET_MATCH_ANY, bitset2, 0UL,
					&block_param);
		raw_mutex_dec_waiters(mutex);

		/* futex wait error, and not E_HM_INTR */
		if (ret != E_HM_OK && ret != E_HM_INTR) {
			break;
		}
	}

	return ret;
}

static __pure int raw_mutex_timedlock(struct raw_mutex *mutex, const struct timespec *at,
				      unsigned long pc)
{
	unsigned int flags = 0;
	int ret;
	int times = SPIN_TIMES;

	UNUSED(at);
	if (unlikely(mutex == NULL)) {
		return E_HM_INVAL;
	}

	/* when system is abnormal, lock operation will not be blocked */
	/* do not take the lock when the system is abnormal */
	if (unlikely(raw_mutex_disabled())) {
		return 0;
	}
	if (try_lock(mutex) == 0) {
		return 0;
	}

	if ((type_of(mutex) & RAW_MUTEX_PI) != 0) {
		return raw_mutex_lock_pi(mutex);
	}

	flags = ((shared_flag_of(mutex) != 0) ? FUTEX_TYPE_SHARED : FUTEX_TYPE_PRIVATE);
	flags |= (FUTEX_BLOCKED_TIMEDOUT_DETECT | FUTEX_BLOCKED_HAS_LOCK_OWNER);

	/* optimize spins */
	while ((times-- != 0) && (vatomic32_read_rlx(&mutex->lock) != 0U)) {
		vatomic_fence_rlx();
	}

#ifdef CONFIG_LOCKSTAT
	if ((type_of(mutex) & RAW_MUTEX_LOCKDEP) != 0) {
		lock_contended(&mutex->dep_map);
	}
#endif

	return raw_mutex_wait_lock(mutex, flags, pc);
}

int raw_mutex_trylock(struct raw_mutex *mutex)
{
	int rc = E_HM_OK;

	if (mutex == NULL) {
		rc = E_HM_INVAL;
	}
	/*
	 * When system is abnormal, lock operation will not be blocked.
	 * Do not take the lock when the system is abnormal.
	 */
	if (unlikely(raw_mutex_disabled())) {
		return E_HM_OK;
	}
	if (rc == E_HM_OK) {
		rc = try_lock(mutex);
		if (likely(rc == E_HM_OK)) {
			lock_trace_log(mutex, LOCKTRACE_EVENT_TYPE_MUTEX_TRY_ACQUIRE_OK);
		} else {
			lock_trace_log(mutex, LOCKTRACE_EVENT_TYPE_MUTEX_TRY_ACQUIRE_FAIL);
		}
	}

	return rc;
}

/* fastpath for normal mutex lock */
static inline __always_inline bool raw_mutex_lock_fast(struct raw_mutex *mutex)
{
	bool ret = false;
	unsigned int owner = owner_of(mutex);
	if ((type_of(mutex) & RAW_MUTEX_TYPE_MASK) == 0) {
		ret = (bool)(vatomic32_cmpxchg_acq(&mutex->lock, 0, owner) == 0);
	}
	return ret;
}

static void __noinline raw_mutex_lock_slowpath(struct raw_mutex *mutex)
{
	int rc = E_HM_OK;

#ifdef CONFIG_LOCKDEP
	if (mutex != NULL && (type_of(mutex) & RAW_MUTEX_LOCKDEP) != 0) {
		lock_acquire(&mutex->dep_map);
	}
#endif

	rc = raw_mutex_timedlock(mutex, NULL, ptr_to_ulong(__builtin_return_address(0U)));

#ifdef CONFIG_LOCKSTAT
	if (mutex != NULL && (type_of(mutex) & RAW_MUTEX_LOCKDEP) != 0) {
		lock_acquired(&mutex->dep_map);
	}
#endif
	if (rc != E_HM_OK && rc != E_HM_INVAL) {
		hm_panic("mutex lock failed, rc = %d, err: %s\n", rc, hmstrerror(rc));
	}
	if (rc == E_HM_INVAL) {
		hm_warn("mutex is invalid\n");
	}
	lock_trace_log(mutex, LOCKTRACE_EVENT_TYPE_MUTEX_ACQUIRE);
}

void raw_mutex_lock(struct raw_mutex *mutex)
{
	if (!raw_mutex_lock_fast(mutex)) {
		raw_mutex_lock_slowpath(mutex);
	}
}

/*
 * When waiters_of the mutex is exceeds the specified numbers of @waiters,
 * do sched_yiled before trylock.
 */
void raw_mutex_lock_yield(struct raw_mutex *mutex, int waiters)
{
	if ((type_of(mutex) & RAW_MUTEX_TYPE_MASK) == 0) {
		if (waiters_of(mutex) >= waiters) {
			hm_thread_yield();
		}
		if (!(vatomic32_cmpxchg_acq(&mutex->lock, 0, owner_of(mutex)) == 0)) {
			raw_mutex_lock_slowpath(mutex);
		}
	} else {
		hm_warn("only support normal lock\n");
	}
}

/*
 * Why unlock must provide release semantic
 *
 *          v = 0
 *
 *          ldaxr(l)
 * lock:    stxr(l) --- l=1
 *
 *          str(v=1)--+
 *                    | order not enforced
 * unlock:  str(l=0)--+----+
 *      (missing release)  |
 *                         +----> lock:  ldaxr(l)
 *                                       stxr(l) --- l=1
 *
 *                                       ld(v) = 0
 */

/* fastpath for normal mutex unlock */
static inline __always_inline bool raw_mutex_unlock_fast(struct raw_mutex *mutex)
{
	unsigned int owner = owner_of(mutex);
	bool ret = false;

	if ((type_of(mutex) & RAW_MUTEX_TYPE_MASK) == 0 && waiters_of(mutex) == 0) {
		ret = (bool)(vatomic32_cmpxchg_rel(&mutex->lock, owner, 0) == owner);
	}

	return ret;
}

static void __noinline raw_mutex_unlock_slowpath(struct raw_mutex *mutex)
{
	int priv = 0;
	unsigned int type = 0;
	unsigned long bitset2 = 0UL;
	bool ret = false;
	int waiters;

	if (unlikely(mutex == NULL)) {
		hm_warn("mutex is invalid\n");
		ret = true;
	}

#ifdef CONFIG_LOCKDEP
	if (!ret && (type_of(mutex) & RAW_MUTEX_LOCKDEP) != 0) {
		lock_release(&mutex->dep_map);
	}
#endif

	/* skip unlock in special cases */
	if (!ret && unlikely(raw_mutex_disabled())) {
		ret = true;
	}

	lock_trace_log(mutex, LOCKTRACE_EVENT_TYPE_MUTEX_UNLOCK);
	if (!ret) {
		type = type_of(mutex);
		priv = (int)(shared_flag_of(mutex) == 0);
		if ((type & RAW_MUTEX_PI) != 0) {
			int rc = raw_mutex_unlock_pi(mutex);
			BUG_ON(rc != E_HM_OK);
			ret = true;
		}
	}

	if (!ret) {
		int cnt = 0;

		/*
		 * XXX
		 * After all incorrect use of 'unlock not by owner' fixed,
		 * add some check here.
		 */
		if ((type & RAW_MUTEX_RECURSIVE) != 0U) {
			cnt = dec_counters(mutex);
		}

		/*
		 * cnt == 0, means last unlock of recursive lock or
		 * a non-recursive unlock.
		 */
		if (cnt == 0) {
#ifdef CONFIG_XDEP_RAW_MUTEX
			if (shared_flag_of(mutex) == 0) {
				bitset2 |= __FUTEX_BITSET2_XDEP_MUTEX;
			}
#endif
			waiters = waiters_of(mutex);
			uint32_t val = (unsigned int)vatomic32_xchg_rel(&mutex->lock, 0);
			if (waiters != 0 || (val & MUTEX_FLAGS) == 1) {
				(void)raw_futex_wake_ex(ptr_to_type(&mutex->lock, int *), 1, priv,
										FUTEX_BITSET_MATCH_ANY, bitset2);
			}
		}
	}
}

void raw_mutex_unlock(struct raw_mutex *mutex)
{
	if (!raw_mutex_unlock_fast(mutex)) {
		raw_mutex_unlock_slowpath(mutex);
	}
}

int raw_mutex_lock_pair(struct raw_mutex *mutex_a, struct raw_mutex *mutex_b)
{
	int err = E_HM_OK;

	if ((mutex_a == NULL) || (mutex_b == NULL)) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		if (ptr_to_ulong(mutex_a) < ptr_to_ulong(mutex_b)) {
			raw_mutex_lock(mutex_a);
			raw_mutex_lock(mutex_b);
		} else {
			raw_mutex_lock(mutex_b);
			raw_mutex_lock(mutex_a);
		}
	}

	lock_trace_log_pair(mutex_a, mutex_b, LOCKTRACE_EVENT_TYPE_MUTEX_PAIR_ACQUIRE);
	return err;
}

int raw_mutex_unlock_pair(struct raw_mutex *mutex_a, struct raw_mutex *mutex_b)
{
	int err = E_HM_OK;

	lock_trace_log_pair(mutex_a, mutex_b, LOCKTRACE_EVENT_TYPE_MUTEX_PAIR_UNLOCK);
	if ((mutex_a == NULL) || (mutex_b == NULL)) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		if (ptr_to_ulong(mutex_a) < ptr_to_ulong(mutex_b)) {
			raw_mutex_unlock(mutex_b);
			raw_mutex_unlock(mutex_a);
		} else {
			raw_mutex_unlock(mutex_a);
			raw_mutex_unlock(mutex_b);
		}
	}

	return err;
}

void raw_mutex_inc_waiters(struct raw_mutex *mutex)
{
	raw_mutex_attr_t val;
	uint32_t old_v, new_v;
	do {
		/* Check comment in raw_mutex_dec_waiters() */
		val.v._v = __read_once(mutex->attr.v._v);
		if (unlikely(val.__v.waiters == RAW_MUTEX_WAITERS_MAX)) {
			hm_panic("mutex waiters is overflow: mutex = 0x%lx, v = %u, lock = %u\n",
				 vaddr_of(mutex), vatomic32_read_rlx(&val.v), vatomic32_read_rlx(&mutex->lock));
		}
		old_v = val.v._v;
		val.__v.waiters++;
		new_v = val.v._v;
	} while (vatomic32_cmpxchg_rlx(&mutex->attr.v, old_v, new_v) != old_v);
	lock_trace_log(mutex, LOCKTRACE_EVENT_TYPE_MUTEX_INC_WAITERS);
}

void raw_mutex_dec_waiters(struct raw_mutex *mutex)
{
	raw_mutex_attr_t val;
	uint32_t old_v, new_v;
	lock_trace_log(mutex, LOCKTRACE_EVENT_TYPE_MUTEX_DEC_WAITERS);
	do {
		/*
		 * Reading _v instead of mutex->attr is to prevent the following line from being
		 * compiled to memcpy (e.g. when KASAN is enabled), which might do one-by-one copy
		 * and therefore ruin the atomicity.
		 */
		val.v._v = __read_once(mutex->attr.v._v);
		if (unlikely(val.__v.waiters == 0)) {
			hm_panic("waiters should be nonzero: mutex = 0x%lx, v = %u, lock = %u\n",
				 vaddr_of(mutex), vatomic32_read_rlx(&val.v), vatomic32_read_rlx(&mutex->lock));
		}
		old_v = val.v._v;
		val.__v.waiters--;
		new_v = val.v._v;
	} while (vatomic32_cmpxchg_rlx(&mutex->attr.v, old_v, new_v) != old_v);
}

#ifdef __HOST_LLT__
static int llt_thread_get_current_tid(void)
{
	return 1;
}
#endif

void raw_mutex_init_lockdep(struct raw_mutex *mutex, const char *name,
			    struct lockdep_lock_class_key_s *key)
{
#ifdef CONFIG_LOCKDEP
	struct raw_mutex_attr attr;

	raw_mutex_attr_init(&attr);
	raw_mutex_attr_settype(&attr, RAW_MUTEX_LOCKDEP);
	raw_mutex_init_with_attr(mutex, &attr);

	lockdep_init_map(&mutex->dep_map, name, key);
#else
	UNUSED(name, key);
	raw_mutex_init_with_attr(mutex, NULL);
#endif
}
