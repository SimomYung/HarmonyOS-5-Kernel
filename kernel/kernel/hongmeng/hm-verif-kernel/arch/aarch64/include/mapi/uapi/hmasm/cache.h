/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Cache
 * Author: Huawei OS Kernel Lab
 * Create: Mon Oct 19 19:53:03 CST 2020
 */
#ifndef AARCH64_UAPI_ASM_CACHE_H
#define AARCH64_UAPI_ASM_CACHE_H

#include <hmasm/barrier.h>

#define CACHELINE_SIZE 64UL
#define dc(opt, addr)	asm volatile("dc " #opt ", %0" :: "r" (addr));
#define ic(opt, addr)	asm volatile("ic " #opt ", %0" :: "r" (addr));

static inline int
flush_dcache_range_user(unsigned long start, unsigned long end)
{
	unsigned long addr = start;

	for (addr &= ~(CACHELINE_SIZE - 1UL); addr < end; addr += CACHELINE_SIZE) {
		dc(civac, addr);
	}
	dsb(sy);
	return 0;
}

static inline int
clean_dcache_range_user(unsigned long start, unsigned long end)
{
	unsigned long addr = start;

	for (addr &= ~(CACHELINE_SIZE - 1UL); addr < end; addr += CACHELINE_SIZE) {
		dc(cvac, addr);
	}
	dsb(sy);
	return 0;
}

static inline int
flush_icache_range_user(unsigned long start, unsigned long end)
{
	unsigned long addr = start;

	for (addr &= ~(CACHELINE_SIZE - 1UL); addr < end; addr += CACHELINE_SIZE) {
		ic(ivau, addr);
	}
	dsb(sy);
	isb();
	return 0;
}

static inline int
flush_cache_range_user(unsigned long start, unsigned long end)
{
	(void)flush_dcache_range_user(start, end);
	(void)flush_icache_range_user(start, end);
	return 0;
}

#undef CACHELINE_SIZE

#endif
