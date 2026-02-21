/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Boot - bootmem related API - MAPI
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 07 15:59:58 2022
 */

#ifndef MAPI_HMKERNEL_BOOT_MEM_H
#define MAPI_HMKERNEL_BOOT_MEM_H

#include <hmasm/page.h>
#include <hmasm/memory.h>
#include <hmkernel/types.h>
#include <hmkernel/const.h>

/*
 * Directly include hmkernel/pgstr.h whould be safe because we
 * have already cleaned memory.h. Now boot/mem.h is not fundamental
 * header and can depends on complex header. However, to make our
 * life easier, only include pgstr/pgstr.h because boot allocator
 * only need pgstr_type_e.
 */
#include <hmkernel/pgstr/pgstr.h>

/* alloc from bootmem. Doesn't support free */

/*
 * __boot_alloc_align is the fundamental boot allocator which only
 * adjust boot allocator internal state.
 *
 * boot_alloc_align would be used in most of the case which registers
 * pgstr. Only pgstr handler should use __boot_alloc_align. All other
 * code should use boot_alloc_align.
 */

/*
 * alloc_align is base allocator
 * based on alloc_aligned, alloc (with align 1) and alloc_pages_aligned
 * based on alloc_pages_aligned implement alloc_pages
 * based on alloc_pages implement alloc_page.
 */
extern void *__boot_alloc_align(size_t sz, size_t align);

static inline void *__boot_alloc(size_t sz)
{
	return __boot_alloc_align(sz, (size_t)(1UL));
}

static inline void *__boot_alloc_pages_align(unsigned long nr, unsigned long align)
{
	return __boot_alloc_align(UL(PAGE_SIZE) * nr, UL(PAGE_SIZE) * align);
}

static inline void *__boot_alloc_pages(unsigned long nr)
{
	return __boot_alloc_pages_align(nr, 1UL);
}

static inline void *__boot_alloc_page(void)
{
	return __boot_alloc_pages(1UL);
}

/*
 * normal memory allocator. Based on __boot_alloc_align, set allocated pages to
 * PGSTR_TYPE_KERNEL.
 */
extern void *boot_alloc_align(size_t sz, size_t align);

static inline void *boot_alloc(size_t sz)
{
	return boot_alloc_align(sz, (size_t)(1UL));
}

static inline void *boot_alloc_pages_align(unsigned long nr, unsigned long align)
{
	return boot_alloc_align(UL(PAGE_SIZE) * nr, UL(PAGE_SIZE) * align);
}

static inline void *boot_alloc_pages(unsigned long nr)
{
	return boot_alloc_pages_align(nr, 1UL);
}

static inline void *boot_alloc_page(void)
{
	return boot_alloc_pages(1UL);
}

/*
 * Typed allocator. NOTE that typed allocator always aligned with PAGE_TABLE, so
 * no boot_alloc_align_typed. All typed allocators are based on
 * boot_alloc_pages_align_typed.
 */
extern void *boot_alloc_pages_align_typed(unsigned long nr, unsigned long align,
					  enum pgstr_type_e type);

static inline void *boot_alloc_pages_typed(unsigned long nr, enum pgstr_type_e type)
{
	return boot_alloc_pages_align_typed(nr, 1UL, type);
}

static inline void *boot_alloc_page_typed(enum pgstr_type_e type)
{
	return boot_alloc_pages_typed(1UL, type);
}

#endif
