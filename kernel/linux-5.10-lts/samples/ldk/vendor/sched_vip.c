// SPDX-License-Identifier: GPL-2.0-only

#include <liblinux/pal.h>
#include <linux/sched.h>
#include <trace/hooks/liblinux.h>

static void hook_set_vip_prio(void* __data, struct task_struct *p, int prio)
{
	liblinux_pal_thread_set_vip_prio(&task_thread_info(p)->thread_handle, prio);
}
INIT_VENDOR_HOOK(ldk_rvh_set_vip_prio, hook_set_vip_prio);
