/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: Stackunwind for modules
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 12 17:01:18 2020
 */
#ifndef AARCH64_MAPI_ASM_STACKUNWIND_H
#define AARCH64_MAPI_ASM_STACKUNWIND_H

#include <hmasm/registers.h>

struct stack_frame_s {
	uptr_t fp;
	uptr_t pc;
};

extern struct stack_frame_s
arch_stack_current_frame(void);

#endif
