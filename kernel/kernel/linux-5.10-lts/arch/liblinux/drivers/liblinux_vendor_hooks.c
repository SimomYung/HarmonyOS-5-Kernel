// SPDX-License-Identifier: GPL-2.0-only

#define CREATE_TRACE_POINTS
#include <net/net_namespace.h>
#include <trace/hooks/liblinux.h>

/*
 * Export tracepoints that act as a bare tracehook (ie: have no trace event
 * associated with them) to allow external modules to probe them.
 */
EXPORT_TRACEPOINT_SYMBOL_GPL(ldk_vh_pm_qos_notify);
EXPORT_TRACEPOINT_SYMBOL_GPL(ldk_vh_power_supply_charge_notify);
EXPORT_TRACEPOINT_SYMBOL_GPL(ldk_rvh_wakeup_source_activate);
EXPORT_TRACEPOINT_SYMBOL_GPL(ldk_rvh_get_thread_name);

#if IS_ENABLED(CONFIG_BLK_DEV_SD)
EXPORT_TRACEPOINT_SYMBOL_GPL(ldk_rvh_sd_ida_alloc);
EXPORT_TRACEPOINT_SYMBOL_GPL(ldk_rvh_sd_ida_free);
#endif

#ifdef CONFIG_NET_NS
EXPORT_TRACEPOINT_SYMBOL_GPL(ldk_vh_devhost_net_socket_get_ns_by_fd);
EXPORT_TRACEPOINT_SYMBOL_GPL(ldk_vh_devhost_net_socket_get_ns_by_pid);
EXPORT_TRACEPOINT_SYMBOL_GPL(ldk_vh_devhost_net_socket_get_netns);

static void hook_devhost_net_socket_put_netns(void *args, struct net *net)
{
	put_net(net);
}
INIT_VENDOR_HOOK(ldk_vh_devhost_net_socket_put_netns, hook_devhost_net_socket_put_netns);

#endif
