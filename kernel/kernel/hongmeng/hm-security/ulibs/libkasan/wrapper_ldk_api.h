/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Wrapped LDK api
 * Author: Huawei OS Kernel Lab
 * Create: Fri Dec 03 11:11:11 2021
 */
#include <libmem/types.h>

KASAN_WRAPPER(int, liblinux_pal_vm_mmap,
	      ARGS(unsigned long,
		   paddr,
		   unsigned long,
		   vaddr,
		   unsigned long,
		   size,
		   unsigned long,
		   prot,
		   unsigned int,
		   flags))

KASAN_WRAPPER_VOID(liblinux_pal_kasan_poison,
		   ARGS(const void *,
			address,
			unsigned long,
			size,
			unsigned char,
			value))

KASAN_WRAPPER_VOID(liblinux_pal_kasan_check,
		   ARGS(unsigned long,
			addr,
			unsigned long,
			size,
			unsigned int,
			is_write,
			unsigned long,
			ret_addr))

KASAN_WRAPPER_VOID(liblinux_pal_kasan_report,
		   ARGS(unsigned long,
			addr,
			unsigned long,
			size,
			unsigned int,
			is_write,
			unsigned long,
			ret_addr))

KASAN_WRAPPER(int, liblinux_pal_module_alloc_init,
	      ARGS(const void *,
		   base,
		   unsigned long,
		   size))

KASAN_WRAPPER(int, liblinux_pal_module_alloc_prepare,
	      ARGS(const void *,
		   addr,
		   unsigned long,
		   size))

KASAN_WRAPPER(int, liblinux_pal_module_alloc_release,
	      ARGS(const void *,
		   addr,
		   unsigned long *,
		   psize_out))

KASAN_WRAPPER(int, liblinux_pal_page_alloc_populate,
	      ARGS(const void *,
		   start,
		   unsigned long,
		   size))

KASAN_WRAPPER(int, liblinux_pal_page_alloc_unpopulate,
	      ARGS(void *,
		   start,
		   unsigned long,
		   size))

KASAN_WRAPPER(int, lnxbase_iofast_declare,
	      ARGS(const char *, name,
		   void *, pool_base))

KASAN_WRAPPER(int, lnxbase_iofast_acquire_regions,
	      ARGS(void **, regions))

KASAN_WRAPPER(int, lnxbase_iofast_alloc_pfn_v,
	      ARGS(unsigned int, order,
		   unsigned long long *, pa_array,
		   unsigned long, num,
		   unsigned int, flags,
		   unsigned int *, pa_num))

KASAN_WRAPPER(int, lnxbase_iofast_free_pfn_v,
	      ARGS(unsigned long long *, pa_array,
		   unsigned int, pa_num))

KASAN_WRAPPER(int, libdh_iomem_map,
	      ARGS(void *, vaddr,
		   uint64_t, paddr,
		   size_t, size,
		   unsigned int, prot,
		   unsigned int, flags))

KASAN_WRAPPER(void *, libdh_ioremap_ex,
	      ARGS(uint64_t, paddr,
		   size_t, size,
		   unsigned int, prot,
		   unsigned int, flags))

KASAN_WRAPPER(int, libdh_vm_mmap_batch,
	      ARGS(const struct mem_raw_ranges *, pa_array,
		   unsigned int, pa_array_size,
		   void *, vaddr,
		   unsigned int, prot,
		   unsigned int, flags))
