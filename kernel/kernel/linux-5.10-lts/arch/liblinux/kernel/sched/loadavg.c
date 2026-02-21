// SPDX-License-Identifier: GPL-2.0-only

/* Liblinux does not need to calculate load */

#include <linux/sched/nohz.h>

#include <liblinux/kapi.h>

#ifdef CONFIG_NO_HZ_COMMON
LIBLINUX_KAPI_DIFF
void calc_load_nohz_start(void)
{
}

LIBLINUX_KAPI_DIFF
void calc_load_nohz_stop(void)
{
}
#endif

LIBLINUX_KAPI_DIFF
void calc_global_load(void)
{
}
