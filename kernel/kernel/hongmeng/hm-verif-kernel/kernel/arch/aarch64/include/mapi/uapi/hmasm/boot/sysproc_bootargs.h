/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Definition of sysproc bootargs
 * Author: Huawei OS Kernel Lab
 * Create: Wed Dec 22 17:09:45 2021
 */
#ifndef UAPI_ASM_BOOT_SYSPROC_BOOTARGS_H
#define UAPI_ASM_BOOT_SYSPROC_BOOTARGS_H

#ifndef __ASSEMBLY__
#include <hmkernel/types.h>
struct __sysproc_bootargs_s {
	__u64 load_start;
	__u64 canary;
	/* originally linear_map_start, linear_map_end and pv_offset */
	__u64 pad0;
	__u64 pad1;
	__u64 pad2;
	__u64 pgd;
	__u64 uapps_start;
	__u64 uapps_end;
	__u64 mmap_rnd;
};
#endif

#ifndef __sysproc_bootargs_assert_offset
#define __sysproc_bootargs_assert_offset(...)
#endif

#ifndef __sysproc_bootargs_assert_size
#define __sysproc_bootargs_assert_size(...)
#endif

/* for two instruction: nop; b 1f, see sysmgr/head.S */
#define __SYSPROC_BOOTARGS_OFFSET 8

#define __SYSPROC_BOOTARGS_OFFSET_LOAD_START 0
__sysproc_bootargs_assert_offset(load_start, LOAD_START);
#define __SYSPROC_BOOTARGS_OFFSET_CANARY 8
__sysproc_bootargs_assert_offset(canary, CANARY);
#define __SYSPROC_BOOTARGS_OFFSET_PGD 40
__sysproc_bootargs_assert_offset(pgd, PGD);
#define __SYSPROC_BOOTARGS_OFFSET_UAPPS_START 48
__sysproc_bootargs_assert_offset(uapps_start, UAPPS_START);
#define __SYSPROC_BOOTARGS_OFFSET_UAPPS_END 56
__sysproc_bootargs_assert_offset(uapps_end, UAPPS_END);

#define __SYSPROC_BOOTARGS_OFFSET_MMAP_RND 64
__sysproc_bootargs_assert_offset(mmap_rnd, MMAP_RND);

#define __SYSPROC_BOOTARGS_SIZE 72
__sysproc_bootargs_assert_size(struct __sysproc_bootargs_s, __SYSPROC_BOOTARGS_SIZE);

#endif
