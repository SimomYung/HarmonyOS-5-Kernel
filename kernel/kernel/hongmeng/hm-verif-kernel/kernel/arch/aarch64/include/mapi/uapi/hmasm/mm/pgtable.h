/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 24 21:51:08 2018
 */
#ifndef AARCH64_UAPI_ASM_MM_PGTABLE_H
#define AARCH64_UAPI_ASM_MM_PGTABLE_H

#include <hmasm/types.h>

/* share with enum pgtable_attr */
/* combination of secure and tee:
 *
 * NONSECURE  CONFIG_A64_TEE   NS-bit
 * 0          0                unset (SBZ/RES0)
 * 0          1                unset
 * 1          0                unset (SBZ/RES0)
 * 1          1                set
 * */
#define PGTABLE_ATTR_A64_NONSECURE	(__u64)(1UL << 32)

#define PGTABLE_ATTR_A64_DEV_nGnRnE	(__u64)(1UL << 33)
#define PGTABLE_ATTR_A64_DEV_nGnRE	(__u64)(1UL << 34)
#define PGTABLE_ATTR_A64_DEV_GRE	(__u64)(1UL << 35)

#define PGTABLE_ATTR_A64_STAGE2		(__u64)(1UL << 36)

#define PGTABLE_ATTR_A64_SH_SHIFT	38
#define PGTABLE_ATTR_A64_SH		    (__u64)(3UL << PGTABLE_ATTR_A64_SH_SHIFT)
#define PGTABLE_ATTR_A64_PBHA_SHIFT	40
#define PGTABLE_ATTR_A64_PBHA		(__u64)(0xFUL << PGTABLE_ATTR_A64_PBHA_SHIFT)

#define PGTABLE_ROOT_COUNT	1

#endif
