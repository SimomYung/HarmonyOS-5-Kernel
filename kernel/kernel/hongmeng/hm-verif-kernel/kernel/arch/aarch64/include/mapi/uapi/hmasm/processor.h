/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 25 10:52:02 2019
 */
#ifndef AARCH64_UAPI_ASM_PROCESSOR_H
#define AARCH64_UAPI_ASM_PROCESSOR_H

#define PSR_T_BIT	(1UL << 5UL)

/* PSR bits */
#define PSR_MODE_MASK	0x0000000F
#define PSR_MODE_EL3t	0x0000000C
#define PSR_MODE_EL3h	0x0000000D
#define PSR_MODE_EL2t	0x00000008
#define PSR_MODE_EL2h	0x00000009
#define PSR_MODE_EL1t	0x00000004
#define PSR_MODE_EL1h	0x00000005
#define PSR_MODE_EL0t	0x00000000

#ifndef __ASSEMBLY__

static inline void cpu_relax(void)
{
	asm volatile("yield" ::: "memory");
}

#endif

#endif
