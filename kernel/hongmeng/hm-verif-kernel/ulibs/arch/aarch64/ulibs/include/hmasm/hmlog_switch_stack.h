/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: hmlog switch stack
 * Author: Huawei OS Kernel Lab
 * Create: Sep 3 16:00:20 2024
 */
#ifndef AARCH64_ULIBS_ASM_HMLOG_SWTICH_STACK_H
#define AARCH64_ULIBS_ASM_HMLOG_SWTICH_STACK_H

int arch_hmlog_vprintf_switch_stack(enum hmlog_level level,const char *func,
				    int line,const char *fmt, va_list args,
				    void *stack, void *vrpintf_func);

#endif
