/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: lsyscall trace interface
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 15 12:15:00 2023
 */
#ifndef HMASM_LSYSCALL_TRACE_H
#define HMASM_LSYSCALL_TRACE_H
#include <hmkernel/types.h>
#include <hmasm/ucontext.h>

void arch_trace_lsyscall_enter(struct arch_uctx *uctx);
void arch_trace_sys_enter(struct arch_uctx *uctx, unsigned long *args);
void arch_trace_lsyscall_exit(struct arch_uctx *uctx);
void arch_trace_sys_exit(struct arch_uctx *uctx, long *args);
#endif
