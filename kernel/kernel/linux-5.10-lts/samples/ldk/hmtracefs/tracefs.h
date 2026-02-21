/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBDH_LINUX_TRACEFS_H__
#define __LIBDH_LINUX_TRACEFS_H__
#include <linux/list.h>
#include <linux/types.h>

int hmtrace_enable_disable_ldk_event(unsigned int tp_index, bool enable);
int hmtrace_ldk_event_format(unsigned int tp_index, char *format);
int hmtrace_print_ldk_event(unsigned int tp_index, void *entry, char *event_str);
#endif /* __LIBDH_LINUX_TRACEFS_H__ */
