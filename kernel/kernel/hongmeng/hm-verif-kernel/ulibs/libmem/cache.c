/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Cache
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 1 15:57:52 2019
 */

#include <libmem/cache.h>
#include <hongmeng/panic.h>
#include <hmkernel/cache.h>
#include <hongmeng/syscall.h>

#define HM_VSPACE_OP_CACHE(OP, Op, op, CACHE, Cache, cache)		\
static void								\
hm_vspace_##op##_my_##cache(unsigned long vstart, unsigned long vend)	\
{									\
	unsigned long cur_va = vstart;					\
	int err;							\
	while (cur_va < vend) {						\
		err = sysfast_flush_cache(__##CACHE##_##OP##_RANGE,	\
					  cur_va, vend);		\
		BUG_ON(err <= 0);					\
		cur_va += (unsigned long)(unsigned int)err;		\
	}								\
}									\
									\
static void								\
hm_vspace_##op##_others_##cache(rref_t rref_vspace,			\
				unsigned long vstart,			\
				unsigned long vend)			\
{									\
	unsigned long cur_va = vstart;					\
	int err;							\
	while (cur_va < vend) {						\
		err = syscap_VSpaceFlushCache(rref_vspace,		\
					      __##CACHE##_##OP##_RANGE,	\
					      cur_va, vend);		\
		BUG_ON(err <= 0);					\
		cur_va += (unsigned long)(unsigned int)err;		\
	}								\
}									\
									\
void									\
hm_vspace_##op##_##cache(rref_t rref_vspace,				\
			 unsigned long vstart,				\
			 unsigned long vend)				\
{									\
	if (rref_vspace == (rref_t)0) {					\
		hm_vspace_##op##_my_##cache(vstart, vend);		\
	} else {							\
		hm_vspace_##op##_others_##cache(rref_vspace,		\
						vstart, vend);		\
	}								\
}

HM_VSPACE_OP_CACHE(FLUSH, Flush, flush, CACHE, Cache, cache)
HM_VSPACE_OP_CACHE(FLUSH, Flush, flush, DCACHE, Dcache, dcache)
HM_VSPACE_OP_CACHE(CLEAN, Clean, clean, DCACHE, Dcache, dcache)
HM_VSPACE_OP_CACHE(INVAL, Inval, inval, DCACHE, Dcache, dcache)
HM_VSPACE_OP_CACHE(FLUSH, Flush, flush, ICACHE, Icache, icache)
