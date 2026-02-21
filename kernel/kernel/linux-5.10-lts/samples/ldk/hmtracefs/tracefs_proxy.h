/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBDH_LINUX_TRACEFS_PROXY_H__
#define __LIBDH_LINUX_TRACEFS_PROXY_H__

#define TRACE_EVENT_NAME_SIZE		64
#define TRACE_EVENT_GROUP_NAME_SIZE	32
struct tracepoint_config {
	char group_name[TRACE_EVENT_GROUP_NAME_SIZE];
	char event_name[TRACE_EVENT_NAME_SIZE];
};

int hmtrace_register_ldk_events(struct tracepoint_config *data, unsigned int num, unsigned int *tp_type);
int hmtrace_add_ldk_printk_format(char *str);
#endif /* __LIBDH_LINUX_TRACEFS_PROXY_H__ */
