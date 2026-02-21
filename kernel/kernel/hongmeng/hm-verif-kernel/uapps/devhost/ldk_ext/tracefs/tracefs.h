/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Interface of tracefs
 * Author: Huawei OS Kernel Lab
 * Create: Mon Aug 14 02:25:27 2023
 */
#ifndef DEVHOST_LDK_EXT_TRACEFS_EX_H
#define DEVHOST_LDK_EXT_TRACEFS_EX_H
int devhost_trace_enable_disable_ldk_event(unsigned int tp_index, bool enable);
int devhost_trace_ldk_event_format(unsigned int tp_index, char *format);
int devhost_trace_print_ldk_event(unsigned int tp_index, void *entry, char *event_str);
#endif /* DEVHOST_LDK_EXT_TRACEFS_EX_H */
