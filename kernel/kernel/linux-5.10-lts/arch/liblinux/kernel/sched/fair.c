// SPDX-License-Identifier: GPL-2.0-only
#include <linux/sched/nohz.h>

#include <liblinux/kapi.h>

#if defined(CONFIG_SMP) && defined(CONFIG_NO_HZ_COMMON)
LIBLINUX_KAPI_DIFF
void nohz_balance_enter_idle(int cpu)
{
	(void)cpu;
}
#endif
