/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Interfaces of cache memory in ulibs
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 1 15:57:52 2019
 */

#ifndef ULIBS_INCLUDE_LIBMEM_CACHE_H
#define ULIBS_INCLUDE_LIBMEM_CACHE_H

#include <hmkernel/capability.h>

#ifdef __cplusplus
extern "C" {
#endif

void hm_vspace_flush_cache(rref_t rref_vspace, unsigned long vstart,
			   unsigned long vend);
void hm_vspace_flush_dcache(rref_t rref_vspace, unsigned long vstart,
			    unsigned long vend);
void hm_vspace_clean_dcache(rref_t rref_vspace, unsigned long vstart,
			    unsigned long vend);
void hm_vspace_inval_dcache(rref_t rref_vspace, unsigned long vstart,
			    unsigned long vend);
void hm_vspace_flush_icache(rref_t rref_vspace, unsigned long vstart,
			    unsigned long vend);

#ifdef __cplusplus
}
#endif

#endif
