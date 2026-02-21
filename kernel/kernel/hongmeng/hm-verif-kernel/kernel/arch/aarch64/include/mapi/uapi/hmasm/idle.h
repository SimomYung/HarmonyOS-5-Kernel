/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Idle
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 18 12:16:22 2021
 */
#ifndef ARCH_AARCH64_IDLE_INCLUDE_ASM_IDLE_H
#define ARCH_AARCH64_IDLE_INCLUDE_ASM_IDLE_H

#ifndef __HOST_LLT__

static inline void arch_default_idle(void)
{
	asm volatile ("wfi");
}

#else /* __HOST_LLT__ */

static inline void arch_default_idle(void)
{
}

#endif /* __HOST_LLT__ */

#endif
