/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Memory cgroup control intapi
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 17 11:19:59 2023
 */
#ifndef SYSMGR_INTAPI_MEM_STAT_H
#define SYSMGR_INTAPI_MEM_STAT_H
#include <intapi/mem/mcache.h>

DECLARE_MCACHE(sys_ebr_dev_block)
DECLARE_MCACHE(sys_ebr_fscache)
DECLARE_MCACHE(sys_ebr_hmfs_ntt)
DECLARE_MCACHE(sys_ebr_hmfs_mgr)
DECLARE_MCACHE(sys_ebr_hmfs_gc)
DECLARE_MCACHE(sys_ebr_default)

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
