/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Implementation of user space spinlock
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 29 21:12:12 2023
 */
#include <hongmeng/errno.h>
#include <hongmeng/panic.h>
#include <libhmlog/hmlog.h>
#include <hongmeng/compiler.h>
#include <libhmsync/uspinlock.h>
#include <libpreempt/preempt.h>

static int disable_uspinlock_flag = 0;
#define DISABLE_MAGIC_NUM 0x55AA
#define uspinlock_disabled() (disable_uspinlock_flag == DISABLE_MAGIC_NUM)

void disable_uspinlock(void)
{
	disable_uspinlock_flag = DISABLE_MAGIC_NUM;
}

void enable_uspinlock(void)
{
	disable_uspinlock_flag = 0;
}

_Bool is_uspinlock_disabled(void)
{
	return uspinlock_disabled();
}

int __hot uspinlock_lock(struct uspinlock_s *usl)
{
	if (unlikely(uspinlock_disabled())) {
		return preempt_process_cpu_get();
	}

	int cpu = hm_user_preempt_disable();
	BUG_ON(cpu < 0);

	spinlock_acquire(&usl->s);
	return cpu;
}

void __hot uspinlock_unlock(struct uspinlock_s *usl)
{
	if (unlikely(uspinlock_disabled())) {
		return;
	}

	spinlock_release(&usl->s);
	(void)hm_user_preempt_enable();
}

inline void __hot uspinlock_lock_lite(struct uspinlock_s *usl)
{
	spinlock_acquire(&usl->s);
}

inline void __hot uspinlock_unlock_lite(struct uspinlock_s *usl)
{
	spinlock_release(&usl->s);
}

int __hot uspinlock_trylock(struct uspinlock_s *usl)
{
	int ret = E_HM_OK;
	if (unlikely(uspinlock_disabled())) {
		return ret;
	}

	(void)hm_user_preempt_disable();
	if (spinlock_tryacquire(&usl->s) == 0) {
		/* failed */
		(void)preempt_process_enable();
		ret = E_HM_BUSY;
	}

	return ret;
}
