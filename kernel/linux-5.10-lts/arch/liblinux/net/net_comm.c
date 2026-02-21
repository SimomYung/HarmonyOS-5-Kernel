// SPDX-License-Identifier: GPL-2.0-only
#include <liblinux/pal.h>
#include <liblinux/internal.h>

#include <trace/hooks/liblinux.h>

static void hook_devhost_current_mm_set(void *args, struct task_struct *cur)
{
	__init_current_mm_info(cur, liblinux_pal_current_mm());
}
INIT_VENDOR_HOOK(ldk_devhost_current_mm_set, hook_devhost_current_mm_set);

static void hook_devhost_current_mm_clear(void *args, struct task_struct *cur)
{
	cur->mm = NULL;
}
INIT_VENDOR_HOOK(ldk_devhost_current_mm_clear, hook_devhost_current_mm_clear);
