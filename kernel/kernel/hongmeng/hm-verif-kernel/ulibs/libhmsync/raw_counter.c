/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Function implementation of raw conuter
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 07 10:37:13 2020
 */
#include <hongmeng/errno.h>
#include <libhmsrv_sys/hm_futex.h>
#include <hongmeng/panic.h>
#include <libhmsync/raw_counter.h>

int raw_counter_inc(struct raw_counter *cnter)
{
	int new;
	int cnt;

	new = raw_atomic_int_inc(&cnter->count);
	cnt = (int)((unsigned int)new & COUNTER_MASK);
	if (cnt == COUNTER_MAX) {
		hm_panic("cnt is overfolw, counter=%lx, val=%d\n", vaddr_of(cnter), new);
	}

	return cnt;
}

int raw_counter_dec_wake(struct raw_counter *cnter)
{
	int new;
	int cnt;
	int old;

	new = raw_atomic_int_dec(&cnter->count);
	cnt = (int)((unsigned int)new & COUNTER_MASK);
	if (cnt == COUNTER_MAX) {
		hm_panic("cnt should not less than 0, counter=%lx, val=%d\n", vaddr_of(cnter), new);
	}
	/* Concurrent inc and count values may increase. It's still gonna wake up. */
	if (cnt == cnter->wa_val) {
		/*
		 * Set futex_val to 1. To ensure that the wake function is executed before
		 * the wait function in the concurrency scenario, the wait function can return E_HM_AGAIN.
		 * When more than one thread is waking up at the same time, only one is guaranteed to succeed.
		 */
		old = raw_atomic_int_cas(ptr_to_type(&cnter->futex_val, raw_atomic_int_t *), 0, 1);
		if (old != 0) {
			__hmattr_sideff_resolved("no relative side effect");
			return E_HM_BUSY;
		}
		old = raw_atomic_int_read(&cnter->count);
		if (((unsigned int)old & (1U << COUNTER_WAITER_BIT)) != 0U) {
			(void)hm_futex_wake(&cnter->futex_val, (unsigned int)(-1), 0);
		}
	}

	return cnt;
}

static void raw_counter_set_wait_bit(struct raw_counter *cnter)
{
	int old;
	int new_val;
	int ok;

	while (1) {
		old = raw_atomic_int_read(&cnter->count);
		if (((unsigned int)old & (1U << COUNTER_WAITER_BIT)) != 0U) {
			__hmattr_sideff_resolved("no relative side effect");
			return;
		}
		new_val = (int)((unsigned int)old | (1U << COUNTER_WAITER_BIT));
		/*
		 * When two threads are running at the same time,
		 * the atomic operation ensures that one of them fails.
		 */
		ok = raw_atomic_int_cas(&cnter->count, old, new_val);
		if (ok == old) {
			__hmattr_sideff_resolved("no relative side effect");
			return;
		}
	}
	__builtin_unreachable();
}

/*
 * When multiple threads concurrently invoke raw_counter_wait,
 * the current behavior is uncertain. The thread may not be woken up.
 */
int raw_counter_wait(struct raw_counter *cnter)
{
	int rc = E_HM_OK;
	int count;

	cnter->futex_val = 0;
	count = raw_counter_val(cnter);
	/* Concurrent addition may occur, but this scenario is not considered currently. */
	if (count > cnter->wa_val) {
		raw_counter_set_wait_bit(cnter);
		rc = hm_futex_wait(&cnter->futex_val, 0, NULL, 0);
	}

	return rc;
}
