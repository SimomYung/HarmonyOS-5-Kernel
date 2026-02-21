// SPDX-License-Identifier: GPL-2.0-only
#include <linux/kernel_stat.h>

#include <liblinux/kapi.h>

LIBLINUX_KAPI_DIFF
void account_idle_ticks(unsigned long ticks)
{
	(void)ticks;
}

LIBLINUX_KAPI_DIFF
void account_process_tick(struct task_struct *p, int user_tick)
{
	(void)p;
	(void)user_tick;
}
