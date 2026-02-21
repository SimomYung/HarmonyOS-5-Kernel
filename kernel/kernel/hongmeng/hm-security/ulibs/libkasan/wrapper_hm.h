/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Delcaration of wrapped hongmeng function
 * Author: Huawei OS Kernel Lab
 * Create: Sun Sep 29 11:11:11 2019
 */
#ifndef __WRAPPER_HM_H_
#define __WRAPPER_HM_H_
#include <stddef.h>
#include <stdint.h>

#ifdef KASAN_SHARED
typedef void (*hmcrt_posthook_type)(void);
void hmcrt_posthook(void);
#else
void __wrap_hmcrt_posthook(void);
void __real_hmcrt_posthook(void);
void *__wrap_hm_mem_mmap(void *addr, size_t len,
			 uint32_t prot, uint32_t flags,
			 int32_t fd, int64_t offset);
void *__wrap_hm_mem_mmap_ex(void *addr, size_t len,
			 uint32_t prot, uint32_t flags,
			 int32_t fd, int64_t offset, size_t guard_size);
#endif
#include "wrapper_hm_decl.h"

#endif

