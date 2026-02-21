/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Prototypes and type definitions for devhost_probe module
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 14 04:44:42 2023
 */
#ifndef __DEVHOST_PLUGIN_PROBE_MODULE_DEVHOST_PROBE_H__
#define __DEVHOST_PLUGIN_PROBE_MODULE_DEVHOST_PROBE_H__

int kprobe_breakpoint_ss_handler(struct pt_regs *regs, unsigned int esr);
int kprobe_breakpoint_handler(struct pt_regs *regs, unsigned int esr);

#endif /* __DEVHOST_PLUGIN_PROBE_MODULE_DEVHOST_PROBE_H__ */
