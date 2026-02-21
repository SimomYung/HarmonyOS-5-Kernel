/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Prototypes and type definitions for tracefs APIs
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 10 12:29:39 2023
 */
#ifndef __DEVHOST_PLUGIN_TRACEFS_API_H__
#define __DEVHOST_PLUGIN_TRACEFS_API_H__

struct devhost_tracefs_ops {
	int (*enable_disable_ldk_event)(unsigned int tp_index, bool enable);
	int (*ldk_event_format)(unsigned int tp_index, char *format);
	int (*print_ldk_event)(unsigned int tp_index, void *entry, char *event_str);
};
int devhost_tracefs_ops_init(struct devhost_tracefs_ops *ops);

#endif /* __DEVHOST_PLUGIN_TRACEFS_API_H__ */
