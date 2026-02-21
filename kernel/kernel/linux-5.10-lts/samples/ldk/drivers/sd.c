// SPDX-License-Identifier: GPL-2.0
#include <liblinux/pal.h>
#include <trace/hooks/liblinux.h>

static void hook_sd_ida_alloc(void *args, int *sd_index)
{
		*sd_index = liblinux_pal_sd_ida_alloc();
}
INIT_VENDOR_HOOK(ldk_rvh_sd_ida_alloc, hook_sd_ida_alloc);

static void hook_sd_ida_free(void *args, int sd_index)
{
		liblinux_pal_sd_ida_free(sd_index);
}
INIT_VENDOR_HOOK(ldk_rvh_sd_ida_free, hook_sd_ida_free);
