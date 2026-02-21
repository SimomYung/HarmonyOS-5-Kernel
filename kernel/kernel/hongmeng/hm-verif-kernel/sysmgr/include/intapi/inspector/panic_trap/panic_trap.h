/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Declarations for panic_trap
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 25 12:25:50 2024
 */

#ifndef SYSMGR_INSPECTOR_PANIC_TRAP_H
#define SYSMGR_INSPECTOR_PANIC_TRAP_H

#include <stdint.h>
#include <hongmeng/errno.h>
#include <hmkernel/compiler.h>

struct thread_s;
struct process_s;
struct __arch_exception_info;

int panic_trap_handle_exception(struct thread_s *thread, struct process_s *process,
		     const struct __arch_exception_info *info);

#endif
