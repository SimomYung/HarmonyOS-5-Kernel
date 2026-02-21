/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Extract ttl computation from tbl.h so LLT can use it.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Dec 25 03:53:19 2024
 */

#ifndef A64_HMASM_TLBTTL_H
#define A64_HMASM_TLBTTL_H

#include <hmasm/page.h>
#include <hmkernel/compiler.h>
#include <hmkernel/cpufeatures.h>

#if defined CONFIG_AARCH64_PAGE_4K
# define A64_TLBI_TTL_GRANULE	1
#elif defined CONFIG_AARCH64_PAGE_16K
# define A64_TLBI_TTL_GRANULE	2
#elif defined CONFIG_AARCH64_PAGE_64K
# define A64_TLBI_TTL_GRANULE	3
#else
# error "Impossible"
#endif
#define A64_TLBI_TTL_LEVEL_3	3
#define A64_TLBI_TTL_LEVEL_2	2
#define A64_TLBI_TTL_LEVEL_1	1

#define A64_TLBI_TTL_LEVEL_FACTOR	512

#define A64_TLBI_TTL_GRANUAL_SHIFT	2
#define A64_TLBI_TTL_SHIFT	44

static inline unsigned long __tlb_invalidate_ttl_by_size(unsigned long size)
{
	unsigned long ttl;

	if (likely(size <= PAGE_SIZE)) {
		ttl = A64_TLBI_TTL_LEVEL_3;
	} else if (size <= PAGE_SIZE * A64_TLBI_TTL_LEVEL_FACTOR) {
		ttl = A64_TLBI_TTL_LEVEL_2;
	} else {
		ttl = A64_TLBI_TTL_LEVEL_1;
	}
	return ttl;
}

/*
 * See D5-53 TTL field encodings in TLB instructions that apply to a single address
 */
static inline __pure unsigned long tlb_invalidate_ttl_by_size(unsigned long size)
{
	unsigned long ttl = 0;
	if (arch_cpu_features_has_a64ttl() && (size != 0)) {
		ttl = __tlb_invalidate_ttl_by_size(size) |
			(A64_TLBI_TTL_GRANULE << A64_TLBI_TTL_GRANUAL_SHIFT);
		ttl <<= A64_TLBI_TTL_SHIFT;
	}
	return ttl;
}

#endif
