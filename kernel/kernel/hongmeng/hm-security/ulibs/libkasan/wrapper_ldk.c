/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Implementation of wrapped LDK functon
 * Author: Huawei OS Kernel Lab
 * Create: Fri Dec 03 11:11:11 2021
 */
/* direct header */
#include "macro.h"
#include "wrapper_ldk.h"

/* libc */
#include <stdint.h>
#include <dlfcn.h>

#include <libsysif/sysmgr/api.h>

/* hm-security */
#include <libkasan/kasan.h>
#include <libkasan/mutex.h>
#include <libkasan/kasan_init.h>
#include <libhwsecurec/securec.h>
#include "shadow.h"
#include "asan_print.h"
#include "wrapper_ldk_define.h"

#ifndef __HOST_LLT__
#define KASAN_WRAPPER_DEFINE
#define KASAN_WRAPPER_VOID(ldk_name, ARG) \
static void (*_##ldk_name)(_PARA(PACK(ARG))) = NULL; \
void kasan_reg_##ldk_name(void) \
{ \
	_##ldk_name = dlsym(RTLD_NEXT, #ldk_name); \
} \
void ori_##ldk_name(_PARA(PACK(ARG))) \
{ \
	if (_##ldk_name == NULL) { \
		kasan_reg_##ldk_name(); \
	} \
	BUG_ON(_##ldk_name == NULL); \
	_##ldk_name(_ARGS(PACK(ARG))); \
	return; \
}
#define KASAN_WRAPPER(ret_type, ldk_name, ARG) \
static ret_type (*_##ldk_name)(_PARA(PACK(ARG))) = NULL; \
void kasan_reg_##ldk_name(void) \
{ \
	_##ldk_name = dlsym(RTLD_NEXT, #ldk_name); \
} \
ret_type ori_##ldk_name(_PARA(PACK(ARG))) \
{ \
	if (_##ldk_name == NULL) { \
		kasan_reg_##ldk_name(); \
	} \
	BUG_ON(_##ldk_name == NULL); \
	return _##ldk_name(_ARGS(PACK(ARG))); \
}
#include "wrapper_ldk_api.h"
#undef KASAN_WRAPPER
#undef KASAN_WRAPPER_DEFINE
#endif /* __HOST_LLT__ */

#define PAL_VM_TO_USER		(1U << 31)

int liblinux_pal_vm_mmap(unsigned long paddr, unsigned long vaddr,
			 unsigned long size, unsigned long prot,
			 unsigned int flags)
{
	int ret;
	size_t aligned_len = (size_t)PAGE_ALIGN_UP(size);

	ret = ori_liblinux_pal_vm_mmap(paddr, vaddr, size, prot, flags);
	if (ret == E_HM_OK && (flags & PAL_VM_TO_USER) == 0) {
		(void)kasan_map_shadow_range((uintptr_t)vaddr, (uintptr_t)(vaddr + aligned_len));
	}

	return ret;
}

void liblinux_pal_kasan_poison(const void *address, unsigned long size,
			       unsigned char value)
{
	if (value == 0) {
		kasan_unpoison_shadow((uintptr_t)address, (size_t)size);
	} else {
		kasan_poison_shadow((uintptr_t)address, (size_t)size, value);
	}
	return;
}

void liblinux_pal_kasan_check(unsigned long addr, unsigned long size,
			      unsigned int is_write, unsigned long ret_addr)
{
	check_memory_region(addr, (size_t)size, (bool)is_write, ret_addr);
	return;
}

void liblinux_pal_kasan_report(unsigned long addr, unsigned long size,
			       unsigned int is_write, unsigned long ret_addr)
{
	asan_report_address(addr, (size_t)size, (bool)is_write, ret_addr);
	return;
}

/*
 * This function calls mprotect to map the memory. When a driver is loaded, this function is
 * called to map the memory and clear the memory. Then, the driver is loaded to this address
 * space. When global variables in the driver is registered, will poison the area between two
 * global variables. The mprotect interface does not unpoison. Therefore, unpoison needs to be
 * performed in this function. Otherwise, some addresses cannot be accessed after mapping.
 */
int liblinux_pal_module_alloc_prepare(const void *addr, unsigned long size)
{
	int ret;

	ret = ori_liblinux_pal_module_alloc_prepare(addr, size);
	if (ret == E_HM_OK) {
		kasan_unpoison_shadow((uintptr_t)addr, (size_t)size);
	}

	return ret;
}

static struct {
	unsigned long kmap_virt_start;
	unsigned long kmap_virt_end;
	unsigned long long phys_offset;
	unsigned int page_sizeorder;
} g_pool_base;

static bool iofast_inited;

static unsigned long __phy_to_virt(unsigned long long paddr)
{
	return g_pool_base.kmap_virt_start + (unsigned long)(paddr - g_pool_base.phys_offset);
}

static unsigned int __order_to_size(unsigned int order)
{
	return 1U << (g_pool_base.page_sizeorder + order);
}

int lnxbase_iofast_declare(const char *name, void *pool_base)
{
	int ret;

	ret = ori_lnxbase_iofast_declare(name, pool_base);
	if (ret == E_HM_OK) {
		NOFAIL(memcpy_s(&g_pool_base, sizeof(g_pool_base), pool_base, sizeof(g_pool_base)));
		iofast_inited = true;
	}
	return ret;
}

int lnxbase_iofast_acquire_regions(void **regions)
{
	int i;
	int ret;
	struct iofast_region *iofast_regions = NULL;

	ret = ori_lnxbase_iofast_acquire_regions(regions);
	if (ret > 0) {
		iofast_regions = void_to_ptr(*regions, struct iofast_region);
		for (i = 0; i < ret; i++) {
			(void)kasan_map_shadow_range(iofast_regions[i].vstart,
					       iofast_regions[i].vstart + iofast_regions[i].length);
		}
	}

	return ret;
}

int lnxbase_iofast_alloc_pfn_v(unsigned int order, unsigned long long *pa_array,
			       unsigned long num, unsigned int flags, unsigned int *pa_num)
{
	int ret;
	unsigned int i;
	unsigned long start;
	unsigned int size;

	ret = ori_lnxbase_iofast_alloc_pfn_v(order, pa_array, num, flags, pa_num);
	if (ret == E_HM_OK && iofast_inited) {
		for (i = 0; i < *pa_num; i++) {
			start = __phy_to_virt(pa_array[i] & PAGE_MASK);
			size = __order_to_size(pa_array[i] & (~PAGE_MASK));
			(void)kasan_map_shadow_range(start, start + size);
		}
	}

	return ret;
}

int lnxbase_iofast_free_pfn_v(unsigned long long *pa_array, unsigned int pa_num)
{
	int ret;
	unsigned int i;
	unsigned long start;
	unsigned int size;

	ret = ori_lnxbase_iofast_free_pfn_v(pa_array, pa_num);
	if (ret == E_HM_OK && iofast_inited) {
		for (i = 0; i < pa_num; i++) {
			start = __phy_to_virt(pa_array[i] & PAGE_MASK);
			size = __order_to_size(pa_array[i] & (~PAGE_MASK));
			kasan_unmap_shadow_range(start, start + size);
		}
	}

	return ret;
}

int libdh_iomem_map(void *vaddr, uint64_t paddr,
		    size_t size, unsigned int prot,
		    unsigned int flags)
{
	int ret;

	ret = ori_libdh_iomem_map(vaddr, paddr, size, prot, flags);
	if (ret == E_HM_OK) {
		(void)kasan_map_shadow_range((uintptr_t)vaddr, (uintptr_t)vaddr + size);
	}

	return ret;
}

static void __map_shadow_batch(void *vaddr, const struct mem_raw_ranges *pa_array,
			       unsigned int pa_array_size)
{
	unsigned int i;
	uint64_t len = 0UL;

	for (i = 0; i < pa_array_size; i++) {
		len += (pa_array[i].end - pa_array[i].start) << PAGE_SHIFT;
	}
	(void)kasan_map_shadow_range((uintptr_t)vaddr, (uintptr_t)vaddr + (size_t)len);
}

int libdh_vm_mmap_batch(const struct mem_raw_ranges *pa_array,
			unsigned int pa_array_size,
			void *vaddr,
			unsigned int prot, unsigned int flags)
{
	int ret;

	ret = ori_libdh_vm_mmap_batch(pa_array, pa_array_size, vaddr, prot, flags);
	if (ret == E_HM_OK && (flags & PAL_VM_TO_USER) == 0) {
		__map_shadow_batch(vaddr, pa_array, pa_array_size);
	}
	return ret;
}

void *libdh_ioremap_ex(uint64_t paddr, size_t size,
		       unsigned int prot, unsigned int flags)
{
	void *vaddr = NULL;

	vaddr = ori_libdh_ioremap_ex(paddr, size, prot, flags);
	if (vaddr != NULL) {
		(void)kasan_map_shadow_range((uintptr_t)vaddr, (uintptr_t)vaddr + size);
	}
	return vaddr;
}
