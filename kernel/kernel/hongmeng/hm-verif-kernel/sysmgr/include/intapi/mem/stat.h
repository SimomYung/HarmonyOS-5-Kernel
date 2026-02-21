/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Memory cgroup control intapi
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 17 11:19:59 2023
 */
#ifndef SYSMGR_INTAPI_MEM_STAT_H
#define SYSMGR_INTAPI_MEM_STAT_H
#include <hongmeng/panic.h>
#include <libhmsync/atomic.h>
#include <intapi/mem/mcache.h>
#include <intapi/mem/paf.h>

DECLARE_MCACHE(sys_ebr_dev_block)
DECLARE_MCACHE(sys_ebr_fscache)
DECLARE_MCACHE(sys_ebr_hmfs_ntt)
DECLARE_MCACHE(sys_ebr_hmfs_mgr)
DECLARE_MCACHE(sys_ebr_hmfs_gc)
DECLARE_MCACHE(sys_ebr_default)

enum sys_stat_type {
	SYS_ANON_MAPPED, /* size of non-file backed pages mapped into userspace page tables */
	SYS_MLOCKED,     /* size of physical memory locked by mlock() system call */
	SYS_PAGETABLE,   /* size of physical memory used by page tables */
	SYS_VMEM_COMMITTED_AS,   /* size of virtual memory presently allocated on the system */
	SYS_DEVHOSTRSV, /* size of physical memory used by devhost reserve */
	SYS_DEVHOSTDMA, /* size of dma physical memory used by devhost */
	SYS_FPOOL, /* size of physical memory used by fpool */
	SYS_LINEAR_ANON, /* Anonymous pages used via sysmgr linear map */
	SYS_SHMM_ANON, /* Unmapped anonymous pages backed by shm node pset */
	SYS_TRACE_ANON, /* Trace_shm anonymous pages used by trace shm */
	SYS_HMPSF, /* Programmable system framework pages for xvm modules and xvm maps */
	NR_SYS_STAT_TYPE
};

extern raw_atomic_ullong_t sys_stat_size[(int)NR_SYS_STAT_TYPE];

static inline void sys_stat_inc_size(enum sys_stat_type type, uint64_t value)
{
	BUG_ON_D(type >= NR_SYS_STAT_TYPE);
	(void)raw_atomic_ullong_add_relaxed(&(sys_stat_size[type]), value);
}

static inline void sys_stat_dec_size(enum sys_stat_type type, uint64_t value)
{
	BUG_ON_D(type >= NR_SYS_STAT_TYPE);
	(void)raw_atomic_ullong_sub_relaxed(&(sys_stat_size[type]), value);
}

static inline uint64_t sys_stat_read_size(enum sys_stat_type type)
{
	BUG_ON_D(type >= NR_SYS_STAT_TYPE);
	return raw_atomic_ullong_read_relaxed(&(sys_stat_size[type]));
}

static inline void sys_stat_anon_mapped_update(enum pg_type tid, uint32_t mapcnt,
					       uint64_t value, bool inc)
{
	if (inc && (mapcnt == 1U) && ((tid == PAGE_ANON) || (tid == PAGE_HUGE))) {
		(void)raw_atomic_ullong_add_relaxed(&(sys_stat_size[SYS_ANON_MAPPED]), value);
	} else if (!inc && (mapcnt == 0U) && ((tid == PAGE_ANON) || (tid == PAGE_HUGE))) {
		(void)raw_atomic_ullong_sub_relaxed(&(sys_stat_size[SYS_ANON_MAPPED]), value);
	} else {
		/* Do nothing. */
	}
}

/*
 * Get total size of physical memory
 *
 * @return  Size of the memory sysmgr managed
 *
 * @note：All memory is managed by buddy allocators of
 *        the memory subsystem excepth that reserved
 *        and used by kernel. Therefore, total memory
 *        desn't cover kernel memory and reserve memory
 *        when boot.
 */
uint64_t mem_size_total(void);

/*
 * Get free size of physical memory
 *
 * @return  Size of the memory free
 *
 * @note: Free memory covers free pages in buddy zones
 *        and free pages in cache. But free in slab is
 *        not counted in.
 */
uint64_t mem_size_free(void);

uint64_t mem_wmark_min(void);
uint64_t mem_wmark_low(void);
uint64_t mem_wmark_high(void);

uint64_t normal_zone_size_total(void);
uint64_t normal_zone_size_free(void);

uint64_t mcache_sys_ebr_reclaimable(void);

#endif /* SYSMGR_INTAPI_MEM_STAT_H */
