/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: User interface of coobj related functions
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 9 20:48:28 2023
 */
#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_COOBJ_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_COOBJ_H

#include <sys/stat.h>
#include <vsync/atomic.h>
#include <lib/coobj/coobj.h>
#include <hongmeng/types.h>
#include "hm_stat.h"

#define HM_COOBJ_FLAGS_KASAN_ENABLE  0x01

int coobj_init(const char *name, uintptr_t *ator, coobj_role role, __bool resv_mem, mode_t mode);
void coobj_free(uintptr_t ator, size_t idx);
size_t coobj_malloc(uintptr_t ator, size_t size);

#if defined(__HOST_LLT__) || defined(__HOST_UT__)
void *coobj_idx_to_ptr(uintptr_t ator, size_t idx);
#ifdef __aarch64__
void* coobj_offset_to_ptr(size_t offset);
#endif

#else
extern uintptr_t coobj_resv_va;

static inline __always_inline
void *coobj_idx_to_ptr(uintptr_t ator_addr, size_t idx)
{
	void *ret = NULL;
	struct coobj_var_user_s *ator = ulong_to_ptr(ator_addr, struct coobj_var_user_s);

	if(ator != NULL) {
#ifdef __aarch64__
		ret = coobj_user_idx_to_ptr(ator, (__relpos_t)(idx >> COOBJ_RESERVED_MEM_ORDER));
#else
		ret = coobj_user_idx_to_ptr(ator, (__relpos_t)idx);
#endif
	}
	return ret;
}

#ifdef __aarch64__
static inline __always_inline
void* coobj_offset_to_ptr(size_t idx)
{
	void *ret = NULL;
	size_t addr = (idx & COOBJ_RESERVED_MEM_MASK) + coobj_resv_va;
	ret = size_t_to_ptr(addr, void);
	return ret;
}
#endif
#endif

#endif
