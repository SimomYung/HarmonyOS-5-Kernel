/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 25 07:31:12 2018
 */
#ifndef AARCH64_ASM_PGTABLE_H
#define AARCH64_ASM_PGTABLE_H

#include <hmasm/pgtable-internal/pgtable_hw.h>
#include <hmkernel/types.h>
#include <hmasm/page.h>

#ifndef HMKERNLE_PGTABLE_H
# error This file should be included by hmkernel/mm/pgtable.h
#endif

#ifdef CONFIG_AARCH64_PAGE_4K
#include <hmasm/pgtable-internal/pgtable_4k.h>
#elif defined(CONFIG_AARCH64_PAGE_16K)
#include <hmasm/pgtable-internal/pgtable_16k.h>
#elif defined(CONFIG_AARCH64_PAGE_64K)
#include <hmasm/pgtable-internal/pgtable_64k.h>
#else
# error invalid page size
#endif

#define PGTABLE_ATTR_SW_SHIFT 25U
#define PGTABLE_ATTR_SW_BITS 4U

#define pgtable_attr_sw(swbits) \
	(__u64)((unsigned long)(swbits) << PGTABLE_ATTR_SW_SHIFT)
#define pgtable_attr_sw_of(attr) \
	((unsigned)((attr) >> PGTABLE_ATTR_SW_SHIFT) & \
	 ((U(1) << PGTABLE_ATTR_SW_BITS) - U(1)))

#endif
