/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: Declaration mem types
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 27 20:40:56 2018
 */
#ifndef ULIBS_INCLUDE_LIBMEM_MEM_TYPES_H
#define ULIBS_INCLUDE_LIBMEM_MEM_TYPES_H

#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>
#include <lib/utils.h>
#include <hmasm/page.h>

#ifndef __INVALID_VADDR__
#define __INVALID_VADDR__
#define INVALID_VADDR UINTPTR_MAX
#endif

#ifndef INVALID_PADDR
#define INVALID_PADDR UINT64_MAX
#endif

#ifndef INVALID_PFN
#define INVALID_PFN UINT64_MAX
#endif

#ifndef __MAX_VMEM_ADDR__
#define __MAX_VMEM_ADDR__
#define VMEM_GAP_SIZE 0x100000 /* 1M gap */
#define MAX_VMEM_ADDR ((uintptr_t)-VMEM_GAP_SIZE)
#endif

#define PAGE_MAX_COUNT  (UINT64_MAX / PAGE_SIZE)

#ifndef MAP_FAILED_VADDR
#define MAP_FAILED_VADDR INVALID_VADDR
#endif

#ifndef INVALID_FD
#define INVALID_FD (-1)
#endif

#define PAGE_OFFMASK          ((unsigned long long)PAGE_SIZE - 1UL)
#define PAGE_MASK             (~PAGE_OFFMASK)

/*
 * This can be represent a range of
 * virtual addresses or a range of physical addresses
 */
struct mem_raw_ranges {
	/* for compatibility with 64-bit address length */
	uint64_t start;
	uint64_t end;
};

struct mem_resource_usage_info {
	uint64_t total;
	uint64_t free;
	uint64_t meta;
};

struct proc_status_mem_info {
	uint64_t vmem_peak;       /* size of maximum virtual memory used */
	uint64_t vmem_size;       /* size of current virtual memory used */
	uint64_t vmem_mlocked;    /* size of locked memory */
	uint64_t vmem_pin;        /* size of pinned memory */
	uint64_t rss_peak;        /* maximum resident set size used */
	uint64_t rss;             /* current resident set size used */
	uint64_t rss_anon;        /* size of resident anonymous memory */
	uint64_t rss_file;        /* size of resident file mappings */
	uint64_t rss_shmem;       /* size of resident shmem memory */
	uint64_t vmem_data;	  /* size of private data segments */
	uint64_t vmem_stack;	  /* size of stack segments */
	uint64_t vmem_text;	  /* size of text segment */
	uint64_t vmem_lib;	  /* size of shared library code */
	uint64_t vmem_pte;	  /* size of page table entries */
	uint64_t vmem_swap;	  /* size of swap used by anonymous private data */
	uint64_t mem_hugtlb;	  /* size of hugetlb memory */
};

struct mem_size_info {
	/* virtual memory */
	uint64_t vmem_rw_size;
	uint64_t vmem_mlocked;
	uint64_t vmem_none_size;
	uint64_t vmem_anon_size;
	uint64_t vmem_swap_size;
	uint64_t vmem_total_size;
	uint64_t vmem_using_size;
	uint64_t vmem_share_size;
	/* physical memory */
	uint64_t p_pmem_size; /* pmem from page table of process */
	uint64_t a_pmem_size; /* pmem from alloc pmem */
	uint64_t priv_size; /* private memory size */
	uint64_t share_size; /* share memory size */
	uint64_t aver_size; /* average memory size in shared memory */
	uint64_t phymem_size; /* total physical memory */
	/* process memory */
	uint64_t dirty_share_size;
	uint64_t dirty_total_size;
	uint64_t vm_total_mem_size;
	uint64_t vm_anon_mem_size;
	uint64_t vm_file_mem_size;
	uint64_t vm_static_shm_size;
	uint64_t vm_dynamic_shm_size;
	uint64_t vm_average_shm_size;
	uint64_t vm_reserve_mem_size;
	uint64_t stack_size;
	uint64_t actv_size;
};

#endif
