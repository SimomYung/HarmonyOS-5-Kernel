/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: implement the raw bitlock which will use only two bits to implement
 *		the lock in userspace.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 10 11:34:28 2023
 */

#include <libstrict/strict_base.h>
#include <stdio.h>
#include <pthread.h>
#include <vsync/atomic.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <libstrict/strict.h>
#include <libhmsync/raw_bitlock.h>
#include <hmasm/processor.h>
#include <libhmsrv_sys/hm_futex.h>
#include <libpreempt/preempt.h>
#include <libhmsrv_sys/hm_futex.h>

/*
 * Disable bitlock in special cases such as dump all process info
 * when trigger fiq or system hangup. It's safe to disable locks
 * as the system is already abnormal and will reboot.
 */
static int disable_bitlock_flag = 0;
#define DISABLE_MAGIC_NUM 0x55AA
#define raw_bitlock_disabled() (disable_bitlock_flag == DISABLE_MAGIC_NUM)

void disable_raw_bitlock(void)
{
	disable_bitlock_flag = DISABLE_MAGIC_NUM;
}

void enable_raw_bitlock(void)
{
	disable_bitlock_flag = 0;
}

_Bool is_raw_bitlock_disabled(void)
{
	return raw_bitlock_disabled();
}

static inline int do_bitlock_wait(int *addr, int lock_bit, int wait_bit, unsigned long long *timeout)
{
#ifdef CONFIG_USER_FUTEX
	/*
	 * The futex side of bitlock does not care which is the lock and the wait bit
	 * All it matters is that they both need to be set for a "wait" to succeed
	*/
	unsigned int val = 1 << lock_bit;
	if (wait_bit < INVALID_BITS_NUM) {
		val |= 1 << wait_bit;
	}
	unsigned int flags = (__FUTEX_FLAG_BITLOCK | FUTEX_BLOCKED_TIMEDOUT_DETECT);
	return hm_user_futex_wait(addr, (int)val, FUTEX_BITSET_MATCH_ANY, flags, NULL, timeout);
#else
	struct __block_param_s block_param = {0};

	hm_set_block_param(&block_param, ptr_to_ulong(__builtin_return_address(0U)));
	return sysfast_bitlock_wait(addr, lock_bit, wait_bit, &block_param);
#endif
}

static inline int do_bitlock_wake(int *addr, int lock_bit, int wait_bit, unsigned int num)
{
	/* My thought is we can use the bitset field to target the specific lock bit on a wake call */
#ifdef CONFIG_USER_FUTEX
	unsigned int lock_mask = (1 << lock_bit);
	unsigned int wait_mask = (wait_bit >= INVALID_BITS_NUM) ? 0U : (1U << wait_bit);
	return hm_user_futex_wake_ex(addr, num, lock_mask, wait_mask, __FUTEX_FLAG_BITLOCK);
#else
	UNUSED(lock_bit, wait_bit);
	return sysfast_bitlock_wake(addr, num, NULL);
#endif
}

/*
 * raw_bitlock_trylock - use only two bits to realize a lock
 * @addr: address of lock
 * @lock_bit: which bit of addr means lock (1: someone takes lock, 0: nobody uses this lock)
 * @wait_bit: which bit of addr means wait, used to determine if need to wake up waiter in unlock
 *
 * Return E_HM_OK when getting lock successfully
 * Return E_HM_BUSY when the lock is taken by others.
 */
int raw_bitlock_trylock(int *addr, int lock_bit, int wait_bit)
{
	UNUSED(wait_bit);

	unsigned int old;
	int ret = E_HM_OK;
	/* when system is abnormal, lock operation will not be blocked */
	/* do not take the lock when the system is abnormal */
	if (likely(!raw_bitlock_disabled())) {
		/* try lock in userspace */
		old = vatomic32_get_or((vatomic32_t *)addr, 1U << lock_bit);
		if (old & (1U << lock_bit)) {
			ret = E_HM_BUSY;
		}
	}

	return ret;
}

/*
 * raw_onebit_trylock - use only two bits to realize a lock
 * @addr: address of lock
 * @lock_bit: which bit of addr means lock (1: someone takes lock, 0: nobody uses this lock)
 *
 * Return E_HM_OK when getting lock successfully
 * Return E_HM_BUSY when the lock is taken by others.
 */
int raw_onebit_trylock(int *addr, int lock_bit)
{
	unsigned int old;
	int ret = E_HM_OK;

	if (likely(!raw_bitlock_disabled())) {
		/* try lock in userspace */
		old = vatomic32_get_or((vatomic32_t *)(uintptr_t)addr, 1U << lock_bit);
		if (old & (1U << lock_bit)) {
			ret = E_HM_BUSY;
		}
	}
	return ret;
}

#ifndef CONFIG_USER_FUTEX
static unsigned int update_lock_state(int *addr, int lock_bit, int wait_bit)
{
	unsigned int oldval;
	unsigned int new_val;
	do {
		oldval = new_val = vatomic32_read_rlx((vatomic32_t *)addr);
		new_val |= (1U << lock_bit);
		new_val &= ~(1U << wait_bit);
	} while(oldval != vatomic32_cmpxchg((vatomic32_t *)addr, oldval, new_val));
	return oldval;
}
#endif

/*
 * raw_bitlock_lock - use only two bits to realize a lock
 * @addr: address of lock
 * @lock_bit: which bit of addr means lock (1: someone takes lock, 0: nobody uses this lock)
 * @wait_bit: which bit of addr means wait, used to determine if need to wake up waiter in unlock
 *
 * Return 0 when getting lock successfully
 * Return E_HM_xx when an error occurred
 *
 */
int raw_bitlock_lock(int *addr, int lock_bit, int wait_bit, unsigned long long *timeout)
{
	unsigned int old;
	int rc;

	if (likely(!raw_bitlock_disabled())) {
		/* try lock in userspace */
		old = vatomic32_get_or((vatomic32_t *)(uintptr_t)addr, 1U << lock_bit);

		/* origin bit is 1, means someone takes this bitlock. current thread needs to wait */
		while (old & (1U << lock_bit)) {
			/* there is only one bit for waiter bits, to set when ready to sleep. */
			(void)vatomic32_get_or((vatomic32_t *)addr, 1U << wait_bit);
			rc = do_bitlock_wait(addr, lock_bit, wait_bit, timeout);
			if (rc == E_HM_TIMEDOUT) {
				return rc;
			}

			/* wakeup by previous bitlock owner, retry to acquire the bitlock. */
#ifdef CONFIG_USER_FUTEX
			old = vatomic32_get_or((vatomic32_t *)(uintptr_t)addr, 1U << lock_bit);
#else
			old = update_lock_state(addr, lock_bit, wait_bit);
#endif
		}
	}
	return E_HM_OK;
}

/*
 * raw_onebit_spin_lock - use only one bits to realize a spin_lock
 */
int raw_bitlock_spin_lock(int *addr, int lock_bit)
{
	int old;

	/* disable preempt */
	(void)preempt_process_disable();
	/* try lock in userspace */
	old = vatomic32_get_or((vatomic32_t *)addr, 1 << lock_bit);
	while (old & (1U << lock_bit)) {
		(void)preempt_process_enable();
		cpu_relax();
		(void)preempt_process_disable();
		old = vatomic32_get_or((vatomic32_t *)addr, 1U << lock_bit);
	}
	return E_HM_OK;
}


/*
 * raw_onebit_lock - use only one bits to realize a lock
 * @addr: address of lock
 * @lock_bit: which bit of addr means lock (1: someone takes lock, 0: nobody uses this lock)
 *
 * Return 0 when getting lock successfully
 * Return E_HM_xx when an error occurred
 */
int raw_onebit_lock(int *addr, int lock_bit)
{
	unsigned int old;
	if (likely(!raw_bitlock_disabled())) {
		/* try lock in userspace */
		old = vatomic32_get_or((vatomic32_t *)(uintptr_t)addr, 1U << lock_bit);

		/* origin bit is 1, means someone takes this bitlock. current thread needs to wait */
		while (old & (1U << lock_bit)) {
			(void)do_bitlock_wait(addr, lock_bit, INVALID_BITS_NUM, NULL);
			/* wakeup by previous bitlock owner, retry to acquire the bitlock. */
			old = vatomic32_get_or((vatomic32_t *)(uintptr_t)addr, 1U << lock_bit);
		}
	}
	return E_HM_OK;
}

int raw_bitlock_unlock(int *addr, int lock_bit, int wait_bit)
{
#ifdef CONFIG_USER_FUTEX
	unsigned int old;
	/* only clear the lock bit */
	old = vatomic32_get_and((vatomic32_t *)(uintptr_t)addr, ~(1U << lock_bit));
	/*
	 * wait bit is cleared in hm_user_futex_wake_ex when there is no more waiter on
	 * waiter rbtree.
	 * If wait bit is cleared, we just clear the lock bit, no need to call do_bitlock_wake().
	 */
	if (old & (1U << wait_bit)) {
		/*
		 * if wait bit a is set, there is at least one waiter waiting on bitlock (on the rbtree).
		 * only wakeup one waiter.
		 */
		do_bitlock_wake(addr, lock_bit, wait_bit, 1);
	}
#else
	(void)vatomic32_get_and((vatomic32_t *)(uintptr_t)addr, ~(1U << lock_bit));

	do_bitlock_wake(addr, lock_bit, wait_bit, 1);
#endif

	return 0;
}

int raw_bitlock_spin_unlock(int *addr, int lock_bit)
{
	vatomic32_and((vatomic32_t *)addr, ~(1U << lock_bit));
	/* enable preempt */
	(void)preempt_process_enable();
	return 0;
}

int raw_onebit_unlock(int *addr, int lock_bit)
{
	vatomic32_and((vatomic32_t *)(uintptr_t)addr, ~(1U << lock_bit));
	/* wake all waiters */
	(void)do_bitlock_wake(addr,lock_bit, INVALID_BITS_NUM, 1);
	return 0;
}

static inline int bit_locked(int *addr, int lock_bit)
{
	return (vatomic32_read((vatomic32_t *)addr) & (1 << lock_bit)) == 0 ? 0 : 1;
}

int raw_bitlock_locked(int *addr, int lock_bit, int wait_bit)
{
	UNUSED(wait_bit);
	if (likely(!disable_bitlock_flag)) {
		return bit_locked(addr, lock_bit);
	}
	return 1;
}

int raw_onebit_locked(int *addr, int lock_bit)
{
	if (likely(!disable_bitlock_flag)) {
		return bit_locked(addr, lock_bit);
	}
	return 0;
}
