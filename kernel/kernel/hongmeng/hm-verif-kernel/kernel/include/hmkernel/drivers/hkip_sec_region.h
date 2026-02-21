/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: kip protection header for kernel
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 17 16:48:30 2024
 */

#ifndef HMKERNEL_DRIVERS_HKIP_SEC_REGION_H
#define HMKERNEL_DRIVERS_HKIP_SEC_REGION_H

#include <hmasm/memory.h>
#include <hmkernel/compiler.h>
#include <hmkernel/boot/mem.h>

#ifdef CONFIG_HKIP_SEC_REGION

extern void hkip_sec_region_bootmem_init(void);

extern paddr_t hkip_sec_region_bootmem_alloc_phy_pages_align(unsigned long nr, unsigned long align);

void __hkip_sec_region_set_range(void);

void hkip_sec_region_get_range(paddr_t *start, paddr_t *end);

static inline void hkip_sec_region_set_range(void)
{
	__hkip_sec_region_set_range();
	hkip_sec_region_bootmem_init();
}

#else

static inline void hkip_sec_region_bootmem_init(void) { }

static inline paddr_t hkip_sec_region_bootmem_alloc_phy_pages_align(
		__maybe_unused unsigned long nr, __maybe_unused unsigned long align)
{
	return __PADDR_T_MAX;
}

static inline void hkip_sec_region_set_range(void) {}

static inline void hkip_sec_region_get_range(paddr_t *start, paddr_t *end)
{
	if (start != NULL) {
		*start = 0;
	}

	if (end != NULL) {
		*end = 0;
	}
}

#endif

#endif
