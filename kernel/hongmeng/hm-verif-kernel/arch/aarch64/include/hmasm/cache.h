/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 23 09:19:38 2018
 */
#ifndef ASM_CACHE_H
#define ASM_CACHE_H

#include <hmasm/memory.h>
#include <uapi/hmkernel/cache.h>
#include <hmkernel/errno.h>

extern void flush_dcache_all(void);
extern void clean_dcache_all(void);
extern int flush_cache_range(vaddr_t vstart, vaddr_t vend);
extern int flush_dcache_range(vaddr_t start, vaddr_t end);
extern int clean_dcache_range(vaddr_t start, vaddr_t end);
extern int clean_dcache_pou_range(vaddr_t start, vaddr_t end);
extern int inval_dcache_range(vaddr_t start, vaddr_t end);
extern int flush_icache_range(vaddr_t start, vaddr_t end);
extern void flush_cache_all_smp(void);
extern void flush_icache_all_smp(void);
extern void flush_cache_all_non_smp(void);
extern void flush_icache_all_non_smp(void);
extern void inval_cache_all_non_smp(void);

#ifdef CONFIG_SMP
#define flush_cache_all flush_cache_all_smp
#define flush_icache_all flush_icache_all_smp
#else
#define flush_cache_all flush_cache_all_non_smp
#define flush_icache_all flush_icache_all_non_smp
#endif

#endif
