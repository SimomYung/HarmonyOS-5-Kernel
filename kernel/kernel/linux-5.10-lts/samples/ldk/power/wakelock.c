// SPDX-License-Identifier: GPL-2.0
#include <linux/debugfs.h>

#include <trace/hooks/liblinux.h>

static struct wakeup_source *deleted_ws = NULL;

static void hook_ws_debugfs_init(void *args, struct wakeup_source *ws)
{
	deleted_ws = (struct wakeup_source *)ws;
}
INIT_VENDOR_HOOK(ldk_vh_ws_debugfs_init, hook_ws_debugfs_init);

struct wakeup_source *liblinux_acquire_deleted_ws(void)
{
	return (struct wakeup_source *)deleted_ws;
}
EXPORT_SYMBOL(liblinux_acquire_deleted_ws);

static int __init wakeup_sources_init(void)
{
	debugfs_create_symlink("wakeup_sources", NULL, "/sys/power/wakeup_sources");

	return 0;
}
postcore_initcall(wakeup_sources_init);
