/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: flush cache API
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jun 12 09:40:43 2023
 */
#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_FLUSH_CACHE_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_FLUSH_CACHE_H

#include <hmkernel/cache.h>

#define HM_FLUSH_CACHE_ALL 0x1
#define HM_FLUSH_CACHE_DIRECT 0x2

int hm_flush_cache_init(unsigned int flags);

/* By cmd, I-cache/D-cache can be invalidated, cleaned, invalidated and cleaned.
* Here cmd for details,
* __CACHE_FLUSH_RANGE : I-cache ane D-cache will be invalidated and cleaned.
* __DCACHE_FLUSH_RANGE : D-cache will be invalidated and cleaned.
* __ICACHE_FLUSH_RANGE : I-cache will be invalidated and cleaned.
* __DCACHE_CLEAN_RANGE : D-cache will be cleaned.
* __DCACHE_INVAL_RANGE : D-cache will be invalidated.
*/
void hm_flush_cache(unsigned int cmd, unsigned long start, unsigned long end);

void hm_flush_cache_by_pa(unsigned int cmd, unsigned long long start, unsigned long long end);

void hm_flush_caller_pgtbl_dcache(int pid, unsigned long vstart, unsigned long vend);

#endif
