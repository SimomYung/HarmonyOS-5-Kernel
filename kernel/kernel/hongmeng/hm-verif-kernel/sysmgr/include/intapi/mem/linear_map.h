/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Definition of linear map
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 17 16:50:23 2020
 */
#ifndef SYSMGR_MEM_LINEAR_MAP_H
#define SYSMGR_MEM_LINEAR_MAP_H

#include <stdint.h>
#include <stdlib.h>
#include <hmasm/page.h>

extern uint64_t __sysmgr_pv_offset;
extern uintptr_t __sysmgr_linear_map_start;
extern uintptr_t __sysmgr_linear_map_end;
extern uintptr_t __sysmgr_linear_pa_start;

#ifdef CONFIG_CTRLMEM_SYSCONF_DIRECT_KLINEAR

extern uint64_t __sysmgr_pv_offset_h;
extern uintptr_t __sysmgr_linear_map_start_h;
extern uintptr_t __sysmgr_linear_map_end_h;

#else

/*
 * When DIRECT_KLINEAR is off, following constant make sure all pointers
 * are skipped by compiler.
 */
#define __sysmgr_pv_offset_h		__sysmgr_pv_offset
#define __sysmgr_linear_map_start_h	(~(uintptr_t)(0UL))
#define __sysmgr_linear_map_end_h	0UL

#endif

extern void linear_pa_invalid_report(uint64_t pst, uint64_t ped);
extern void linear_info_print(void);

static inline bool __sys_linear_mem_is_range_overlap(uintptr_t xstart, uintptr_t xend,
						     uintptr_t ystart, uintptr_t yend)
{
	return !(xend <= ystart || xstart >= yend);
}

#ifdef CONFIG_KDP_PROTECT_KLINEAR
extern uint64_t __kernel_pv_offset;
extern uintptr_t __kernel_linear_map_start;
extern uintptr_t __kernel_linear_map_end;

extern uint64_t __kernel_pv_offset_h;
extern uintptr_t __kernel_linear_map_start_h;
extern uintptr_t __kernel_linear_map_end_h;

static inline uint64_t sys_va_to_pa(uintptr_t vaddr)
{
	return (vaddr < __sysmgr_linear_map_end) ?
		((uint64_t)vaddr - __sysmgr_pv_offset) :
		(vaddr < __kernel_linear_map_end ?
		 ((uint64_t)vaddr - __kernel_pv_offset) :
		 ((uint64_t)vaddr - __kernel_pv_offset_h));
}

/* translate paddr to sysmgr vaddr */
static inline uintptr_t pa_to_sys_va(uint64_t paddr)
{
	uintptr_t va = (uintptr_t)(paddr + __sysmgr_pv_offset);
	if (va < __sysmgr_linear_map_start) {
		va = (uintptr_t)(paddr + __kernel_pv_offset_h);
	} else if (va >= __sysmgr_linear_map_end) {
		va = (uintptr_t)(paddr + __kernel_pv_offset);
	}
	return va;
}

static inline uint64_t mem_sys_pa_start(void)
{
	if (__kernel_linear_map_start_h != 0) {
		return sys_va_to_pa(__kernel_linear_map_start_h);
	} else {
		return sys_va_to_pa(__kernel_linear_map_start);
	}
}

static inline uint64_t mem_sys_pa_end(void)
{
	return ((uint64_t)__kernel_linear_map_end - __kernel_pv_offset);
}

static inline bool sys_va_range_overlap_linear_map(uintptr_t vstart, uintptr_t vend)
{
	return __sys_linear_mem_is_range_overlap(vstart, vend, __sysmgr_linear_map_start, __sysmgr_linear_map_end) ||
		__sys_linear_mem_is_range_overlap(vstart, vend, __kernel_linear_map_start, __kernel_linear_map_end) ||
		__sys_linear_mem_is_range_overlap(vstart, vend, __kernel_linear_map_start_h, __kernel_linear_map_end_h);
}

#else /* !CONFIG_KDP_PROTECT_KLINEAR */
#ifdef __HOST_LLT__
static inline uint64_t sys_va_to_pa(uintptr_t vaddr)
{
	return (uint64_t)vaddr;
}

static inline uintptr_t pa_to_sys_va(uint64_t paddr)
{
	return (uintptr_t)paddr;
}

static inline bool sys_va_range_overlap_linear_map(uintptr_t vstart, uintptr_t vend)
{
	return __sys_linear_mem_is_range_overlap(vstart, vend, __sysmgr_linear_map_start, __sysmgr_linear_map_end);
}

#else

static inline bool pa_is_l(uint64_t paddr)
{
	return paddr >= __sysmgr_linear_pa_start;
}

/* translate sysmgr vaddr to paddr */
/* if there is no high idmap, __sysmgr_linear_map_start_h will be 0,
 * so we cannot use __sysmgr_linear_map_start_h here. */
static inline uint64_t sys_va_to_pa(uintptr_t vaddr)
{
	return (vaddr < __sysmgr_linear_map_end) ?
		((uint64_t)vaddr - __sysmgr_pv_offset) :
		((uint64_t)vaddr - __sysmgr_pv_offset_h);
}

/* translate paddr to sysmgr vaddr */
static inline uintptr_t pa_to_sys_va(uint64_t paddr)
{
	if (pa_is_l(paddr)) {
		return (uintptr_t)(paddr + __sysmgr_pv_offset);
	} else {
		return (uintptr_t)(paddr + __sysmgr_pv_offset_h);
	}
}

static inline bool sys_va_range_overlap_linear_map(uintptr_t vstart, uintptr_t vend)
{
#ifdef CONFIG_CTRLMEM_SYSCONF_DIRECT_KLINEAR
	return __sys_linear_mem_is_range_overlap(vstart, vend, __sysmgr_linear_map_start, __sysmgr_linear_map_end) ||
			       __sys_linear_mem_is_range_overlap(vstart, vend, __sysmgr_linear_map_start_h, __sysmgr_linear_map_end_h);
#else
	return __sys_linear_mem_is_range_overlap(vstart, vend, __sysmgr_linear_map_start, __sysmgr_linear_map_end);
#endif
}

#endif

#ifdef CONFIG_CTRLMEM_SYSCONF_DIRECT_KLINEAR
static inline uint64_t mem_sys_pa_start(void)
{
	if (__sysmgr_linear_map_start_h != 0) {
		return sys_va_to_pa(__sysmgr_linear_map_start_h);
	} else {
		return sys_va_to_pa(__sysmgr_linear_map_start);
	}
}
#else
static inline uint64_t mem_sys_pa_start(void)
{
	return sys_va_to_pa(__sysmgr_linear_map_start);
}
#endif

static inline uint64_t mem_sys_pa_end(void)
{
	return ((uint64_t)__sysmgr_linear_map_end - __sysmgr_pv_offset);
}
#endif /* end CONFIG_KDP_PROTECT_KLINEAR */

static inline void mem_sys_pfn_range(uint64_t *start, uint64_t *end)
{
	if (start != NULL) {
		*start = mem_sys_pa_start() >> PAGE_SHIFT;
	}
	if (end != NULL) {
		*end = mem_sys_pa_end() >> PAGE_SHIFT;
	}
}

static inline void mem_sys_pa_range(uint64_t *start, uint64_t *end)
{
	if (start != NULL) {
		*start = mem_sys_pa_start();
	}
	if (end != NULL) {
		*end = mem_sys_pa_end();
	}
}

static inline uintptr_t pa_to_sys_va_st(uint64_t pst)
{
	return pa_to_sys_va(pst);
}

static inline uintptr_t pa_to_sys_va_ed(uint64_t ped)
{
	return pa_to_sys_va(ped - 1) + 1;
}
#endif
