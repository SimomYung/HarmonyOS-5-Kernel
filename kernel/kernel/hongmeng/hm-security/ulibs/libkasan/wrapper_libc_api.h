/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Wrapped libc api
 * Author: Huawei OS Kernel Lab
 * Create: Sun Sep 29 11:11:11 2019
 */
#include <stdint.h>

KASAN_WRAPPER(void *, malloc,
	      ARGS(size_t,
		   sz))

KASAN_WRAPPER_VOID(free,
	      ARGS(void *,
		   p))

KASAN_WRAPPER(void *, calloc,
	      ARGS(size_t,
		   m,
		   size_t,
		   n))

KASAN_WRAPPER(void *, realloc,
	      ARGS(void *,
		   p,
		   size_t,
		   n))

KASAN_WRAPPER(void *, mmap,
	       ARGS(void *,
		    addr,
		    size_t,
		    length,
		    int,
		    prot,
		    int,
		    flags,
		    int,
		    fd,
		    off_t,
		    offset))

KASAN_WRAPPER(int, munmap,
	      ARGS(void *,
		   addr,
		   size_t,
		   length))

KASAN_WRAPPER(int, mprotect,
	      ARGS(void *,
		   addr,
		   size_t,
		   length,
		   int,
		   prot))

#if defined(KASAN_SHARED) || defined(__HOST_LLT__)
KASAN_WRAPPER(void *, memalign,
	      ARGS(size_t,
		   alignment,
		   size_t,
		   size))

KASAN_WRAPPER(void *, aligned_alloc,
	      ARGS(size_t,
		   align,
		   size_t,
		   len))

KASAN_WRAPPER(size_t, malloc_usable_size,
	      ARGS(void *,
		   ptr))
#endif

#ifndef __HOST_LLT__
KASAN_WRAPPER(void *, memcpy,
	      ARGS(void * __restrict,
		   dst,
		   const void *__restrict,
		   src,
		   size_t,
		   n))

KASAN_WRAPPER(void *, memset,
	      ARGS(void * ,
		   s,
		   int ,
		   c,
		   size_t,
		   n))
#endif
