/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: pgtblpool related API
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 25 11:40:39 2024
 */

#ifndef HMKERNEL_BOOT_PGTBLPOOL_H
#define HMKERNEL_BOOT_PGTBLPOOL_H

#include <hmkernel/boot/mem.h>
#include <hmasm/page.h>
#include <hmasm/memory.h>

/*
 * Directly include hmkernel/pgstr.h whould be safe because we
 * have already cleaned memory.h. Now boot/mem.h is not fundamental
 * header and can depends on complex header. However, to make our
 * life easier, only include pgstr/pgstr.h because boot allocator
 * only need pgstr_type_e.
 */
#include <hmkernel/pgstr/pgstr.h>

struct bootpgtblpool_s {
	void *va_st;
	void *va_ed;
	void *ptr;
};

#ifndef CONFIG_BOOTPGTBLPOOL_ENABLE
static inline void bootpgtblpool_init(void) { }

static inline void *bootpgtblpool_alloc_pages_align_typed(vaddr_t va, unsigned long nr, unsigned long align,
							  enum pgstr_type_e type)
{
	UNUSED(va);
	return boot_alloc_pages_align_typed(nr, align, type);
}

static inline void bootpgtblpool_phy_range_of(paddr_t *start, paddr_t *end)
{
	UNUSED(start, end);
}

static inline bool is_pa_in_bootpgtblpool(paddr_t pa)
{
	UNUSED(pa);
	return false;
}

static inline void bootpgtblpool_mprotect_ro(void) {}

#else

void bootpgtblpool_init(void);

void *bootpgtblpool_alloc_pages_align_typed(vaddr_t va, unsigned long nr, unsigned long align,
					    enum pgstr_type_e type);

void bootpgtblpool_phy_range_of(paddr_t *start, paddr_t *end);

bool is_pa_in_bootpgtblpool(paddr_t pa);

void bootpgtblpool_mprotect_ro(void);

#endif

#endif
