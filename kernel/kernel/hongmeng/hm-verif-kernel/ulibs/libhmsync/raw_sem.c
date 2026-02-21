/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation of semaphore
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 08 10:08:17 2019
 */
#include <hongmeng/errno.h>
#include "raw_thread_impl.h"

#include <libhmsync/raw_sem.h>
#include <vsync/atomic.h>
/*
 * The semaphore operation and memory ordering.
 * ======================================
 * Note that smp_mb() is used to show where the ordering must be guaranteed, but the
 * raw_sem is implemented by load/store/cas function with acquire/release semantic.
 *
 *     raw_sem_wait:                                           raw_sem_post:
 *
 * loop:                                                       val=sem->val
 *
 *     val=sem->val                    +---paired(A) with----->smp_mb()
 *                                     |
 *     if(val>0)                       |                       cas(&sem->val,val,val+1)
 *                                     |
 *          cas(&sem->val,val,val-1)   |       +-------------->smp_mb()
 *                                     |       |
 *          smp_mb()<------------------+       |               if(val&WAIT_BIT || waiters)
 *                                             |
 *     else                                    |                    futex_wake(&sem)
 *                                             |
 *          waiters++                          |
 *                                             |
 *          cas(&sem->val, val, val|WAIT_BIT)  |
 *                                             |
 *          smp_mb()<---------paired(B) with---+
 *
 *          futex_wait(&sem)
 *
 *          waiters++
 *
 *          goto loop
 *
 * The pair(A) can guarantee that the code after sem_wait will not be executed before the
 * semaphore is available, and the code before sem_post will be executed before it make
 * semaphore available.
 *
 * The pair(B) can avoid the following scenarios(waiters=0, val=0):
 *           sem_wait:             sem_post:
 *
 *           if (sem->val<=0)
 *             futex_wait          if(waiters) futex_wake
 *             waiters++           cas(&sem->val, val, val+1)
 * Under this case, the waiter will not be woken up.
 */
int raw_sem_init(struct raw_sem *sem, unsigned int value)
{
	int rc = E_HM_OK;

	if (sem == NULL || value > __U(RAW_SAM_VALUE_MAX)) {
		rc = E_HM_INVAL;
	} else {
		vatomic32_write_rlx(&sem->value, value);
		vatomic32_write_rlx(&sem->waiters, 0);
	}
	return rc;
}

int raw_sem_trywait(struct raw_sem *sem)
{
	int val;
	int new;
	uint32_t waiters;
	int err = E_HM_OK;

	if (sem == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		err = E_HM_AGAIN;
		while ((val = (int)vatomic32_read_rlx(&sem->value)) > 0) {
			/* Special treatment of sem->value:
			 *  0 no waiters
			 * -1 there are waiters sleeping
			 *
			 * This is for raw_sem_post to judge if there are blocking waiters
			 * (thread or process) to wakeup. Whether sem->value is 0 or -1,
			 * raw_sem_post will set sem->value to 1.
			 */
			waiters = vatomic32_read_rlx(&sem->waiters);
			new = (val - 1) - (int)((val == 1) && (waiters != 0U));
			if (vatomic32_cmpxchg_acq(&sem->value, (uint32_t)val, (uint32_t)new) == (uint32_t)val) {
				err = E_HM_OK;
				break;
			}
		}
	}
	return err;
}

static int raw_sem_timedwait_with_sigmask(struct raw_sem *sem, const struct timespec *at, struct __hmsigmask *mask)
{
	int r = E_HM_OK;
	int spins = 100;

	if (sem == NULL) {
		r = E_HM_INVAL;
	}

	if ((r == E_HM_OK) && (raw_sem_trywait(sem) != E_HM_OK)) {
		/* optimize spin if there is no value and no waiters for the semaphore */
		while ((spins-- != 0) &&
		       (int)vatomic32_read_rlx(&sem->value) <= 0 &&
		       (int)vatomic32_read_rlx(&sem->waiters) == 0) {
			raw_spin();
		}

		/* sleep until futex wake, and get semphore by raw_sem_trywait */
		while ((r = raw_sem_trywait(sem)) == E_HM_AGAIN) {
			(void)vatomic32_inc_get_rlx(&sem->waiters);
			(void)vatomic32_cmpxchg(&sem->value, 0, (uint32_t)-1);
			r = raw_futex_timedwait(ptr_to_type(&sem->value, int *), -1, CLOCK_MONOTONIC, at, mask);
			/* return value r:
			 * 0: caller was woken up;
			 * E_HM_TIMEDOUT: timeout;
			 * E_HM_INTR: signals or other spurious wakeups.
			 */
			(void)vatomic32_dec_get_rlx(&sem->waiters);
			if (r != E_HM_OK) {
				break;
			}
		}
	}
	return r;
}

int raw_sem_timedwait(struct raw_sem *sem, const struct timespec *at)
{
	return raw_sem_timedwait_with_sigmask(sem, at, NULL);
}

int raw_sem_wait(struct raw_sem *sem)
{
	return raw_sem_timedwait(sem, NULL);
}

int raw_sem_wait_killable(struct raw_sem *sem)
{
	struct __hmsigmask maskkill = {(__u64)HMSIGNAL_KILL};
	return raw_sem_timedwait_with_sigmask(sem, NULL, &maskkill);
}

int raw_sem_post(struct raw_sem *sem)
{
	int val;
	uint32_t waiters;
	int rc = E_HM_OK;

	if (sem == NULL) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		do {
			val = (int)vatomic32_read_rlx(&sem->value);
			waiters = vatomic32_read_rlx(&sem->waiters);
			if (val == RAW_SAM_VALUE_MAX) {
				rc = E_HM_OVERFLOW;
				break;
			}
		} while (vatomic32_cmpxchg(&sem->value, (uint32_t)val,
					(uint32_t)(val + 1 + (int)(val < 0))) != (uint32_t)val);
		if (rc == E_HM_OK) {
			if (val < 0 || waiters != 0U) {
				(void)raw_futex_wake(ptr_to_type(&sem->value, int *), 1, 1);
			}
		}
	}

	return rc;
}

int raw_sem_getvalue(struct raw_sem *sem)
{
	return (sem == NULL) ? E_HM_INVAL : (int)vatomic32_read_rlx(&sem->value);
}

int raw_sem_getwaiter(struct raw_sem *sem)
{
	return (sem == NULL) ? E_HM_INVAL : (int)vatomic32_read_rlx(&sem->waiters);
}
