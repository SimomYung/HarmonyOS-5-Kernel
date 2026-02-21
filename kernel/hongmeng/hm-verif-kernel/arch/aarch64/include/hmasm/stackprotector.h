/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 10 15:52:25 2018
 */
#ifndef AARCH64_ASM_STACKPROTECTOR_H
#define AARCH64_ASM_STACKPROTECTOR_H

#include <hmkernel/compiler.h>

#if defined(__hmfeature_stackprotector__) || defined(CONFIG_CC_STACKPROTECTOR)
extern unsigned long __stack_chk_guard;
static inline __always_inline void stack_canary_update(unsigned long v)
{
	__stack_chk_guard = v;
	*(char*)(&__stack_chk_guard) = '\0';
}
#else
static inline void stack_canary_update(unsigned long v __maybe_unused)
{
}
#endif

void __cold __stack_chk_fail(void);

#endif
