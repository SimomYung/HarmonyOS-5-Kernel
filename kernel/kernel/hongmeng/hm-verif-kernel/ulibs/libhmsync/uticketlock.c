/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2025. All rights reserved.
 * Description: Implementation of user space ticketlock
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 7 15:59:22 2025
 */
#include <hongmeng/errno.h>
#include <hongmeng/panic.h>
#include <libhmlog/hmlog.h>
#include <hongmeng/compiler.h>
#include <libhmsync/uticketlock.h>
#include <libpreempt/preempt.h>

static int disable_uticketlock_flag = 0;
#define DISABLE_MAGIC_NUM 0x55AA
#define uticketlock_disabled() (disable_uticketlock_flag == DISABLE_MAGIC_NUM)

void disable_uticketlock(void)
{
	disable_uticketlock_flag = DISABLE_MAGIC_NUM;
}

void enable_uticketlock(void)
{
	disable_uticketlock_flag = 0;
}

_Bool is_uticketlock_disabled(void)
{
	return uticketlock_disabled();
}

int __hot uticketlock_lock(struct uticketlock_s *utl)
{
	if (unlikely(uticketlock_disabled())) {
		return preempt_process_cpu_get();
	}

	int cpu = hm_user_preempt_disable();
	BUG_ON(cpu < 0);

	ticketlock_acquire(&utl->l);
	return cpu;
}

void __hot uticketlock_unlock(struct uticketlock_s *utl)
{
	if (unlikely(uticketlock_disabled())) {
		return;
	}

	ticketlock_release(&utl->l);
	(void)hm_user_preempt_enable();
}

int __hot uticketlock_trylock(struct uticketlock_s *utl)
{
	int ret = E_HM_OK;
	if (unlikely(uticketlock_disabled())) {
		return ret;
	}

	(void)hm_user_preempt_disable();
	if (ticketlock_tryacquire(&utl->l) == 0) {
		/* failed */
		(void)hm_user_preempt_enable();
		ret = E_HM_BUSY;
	}

	return ret;
}
