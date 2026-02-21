/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Header for bootargs
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 21 11:03:29 2021
 */
#ifndef AARCH64_ASM_BOOTARGS_H
#define AARCH64_ASM_BOOTARGS_H

#include <hmkernel/types.h>

#ifndef __ASSEMBLY__
struct arch_bootargs_s {
	u64 dtb_start_pa;
	u64 freemem_start_pa;
	u64 pa_va_offset;
	u64 random_seed;
	u64 load_start;
	u64 hkip_flag;
	u64 iregion_load_start;

	/* arguments passed from elfloader to boot */
	u64 loadinfo_paddr;
};

#define ARCH_BOOTARGS_ZERO { \
	.dtb_start_pa = 0UL, \
	.freemem_start_pa = 0UL, \
	.pa_va_offset = 0UL, \
	.random_seed = 0UL, \
	.load_start = 0UL, \
	.hkip_flag = 0UL, \
}

#define __arch_bootargs_set_dtb_start_pa(x, v)		((x)->dtb_start_pa = (u64)(v))

#endif /* !__ASSEMBLY__ */

#ifndef __assert_offset
#define __assert_offset(...)
#endif

#ifndef __assert_size
#define __assert_size(...)
#endif

/* for two instruction: nop; b 1f, see boot/head.S */
#define ARCH_BOOTARGS_OFFSET 8

#define ARCH_BOOTARGS_OFFSET_DTB_START_PA 0
__assert_offset(dtb_start_pa, DTB_START_PA);
#define ARCH_BOOTARGS_OFFSET_FREEMEM_START_PA 8
__assert_offset(freemem_start_pa, FREEMEM_START_PA);
#define ARCH_BOOTARGS_OFFSET_PA_VA_OFFSET 16
__assert_offset(pa_va_offset, PA_VA_OFFSET);
#define ARCH_BOOTARGS_OFFSET_RANDOM_SEED 24
__assert_offset(random_seed, RANDOM_SEED);
#define ARCH_BOOTARGS_OFFSET_LOAD_START 32
__assert_offset(load_start, LOAD_START);
#define ARCH_BOOTARGS_OFFSET_HKIP_FLAG 40
__assert_offset(hkip_flag, HKIP_FLAG);
#define ARCH_BOOTARGS_OFFSET_IREGION_LOAD_START 48
__assert_offset(iregion_load_start, IREGION_LOAD_START);
#define ARCH_BOOTARGS_SIZE 64
__assert_size(struct arch_bootargs_s, ARCH_BOOTARGS_SIZE);

#endif
