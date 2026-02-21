/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Prototypes and type definitions for tracefs module
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 04 17:23:12 2023
 */
#ifndef __DEVHOST_PLUGIN_TRACEFS_MODULE_TRACEFS_H__
#define __DEVHOST_PLUGIN_TRACEFS_MODULE_TRACEFS_H__

int hmtrace_enable_disable_ldk_event(unsigned int tp_index, bool enable);
int hmtrace_ldk_event_format(unsigned tp_index, char *format);
int hmtrace_print_ldk_event(unsigned int tp_index, void *entry, char *event_str);

#endif /* __DEVHOST_PLUGIN_TRACEFS_MODULE_TRACEFS_H__ */
