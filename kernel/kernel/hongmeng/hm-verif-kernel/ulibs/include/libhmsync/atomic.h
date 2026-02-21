/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: Atomic
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 19 15:00:03 2018
 */
#ifndef ULIBS_INCLUDE_LIBHMSYNC_ATOMIC_H
#define ULIBS_INCLUDE_LIBHMSYNC_ATOMIC_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#if __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_ATOMICS__)
#include <libhmsync/atomic/c11.h>
#elif defined(__GNUC__)
#include <libhmsync/atomic/gcc47.h>
#else
raw_static_assert(0, _unknown_compiler_atomic_primrives_is_missing);
#endif

static inline uint8_t __raw_atomic_xchg8(uint8_t *ptr, uint8_t x)
{
	return __raw_atomic_swap_relexed(ptr, x);
}

static inline uint16_t __raw_atomic_xchg16(uint16_t *ptr, uint16_t x)
{
	return __raw_atomic_swap_relexed(ptr, x);
}

static inline uint32_t __raw_atomic_xchg32(uint32_t *ptr, uint32_t x)
{
	return __raw_atomic_swap_relexed(ptr, x);
}

static inline uint64_t __raw_atomic_xchg64(uint64_t *ptr, uint64_t x)
{
	return __raw_atomic_swap_relexed(ptr, x);
}

#define raw_atomic_xchg(ptr, x)						\
({									\
	typeof(*(ptr)) __old;						\
	size_t __size = sizeof(*(ptr));				\
									\
	BUG_ON(__size > sizeof(unsigned long));				\
	__old = (typeof(*(ptr)))__raw_atomic_xchg(			\
					(unsigned long *)(ptr),		\
					(unsigned long)(x), __size);	\
	__old;								\
})

#define raw_atomic_xchg64(ptr, x)					\
({									\
	typeof(*(ptr)) __old;						\
									\
	BUG_ON(sizeof(*(ptr)) != sizeof(uint64_t));			\
	__old = (typeof(*(ptr)))__raw_atomic_xchg64(			\
					(uint64_t *)(ptr),		\
					(uint64_t)(x));			\
	__old;								\
})

static inline uint8_t __raw_atomic_cmpxchg8(uint8_t *ptr, uint8_t o, uint8_t n)
{
	return __raw_atomic_compare_exchange_relaxed(ptr, o, n);
}

static inline uint16_t __raw_atomic_cmpxchg16(uint16_t *ptr,
					      uint16_t o, uint16_t n)
{
	return __raw_atomic_compare_exchange_relaxed(ptr, o, n);
}

static inline uint32_t __raw_atomic_cmpxchg32(uint32_t *ptr,
					      uint32_t o, uint32_t n)
{
	return __raw_atomic_compare_exchange_relaxed(ptr, o, n);
}

static inline uint64_t __raw_atomic_cmpxchg64(uint64_t *ptr,
					      uint64_t o, uint64_t n)
{
	return __raw_atomic_compare_exchange_relaxed(ptr, o, n);
}

#define raw_atomic_cmpxchg(ptr, o, n)						\
({									\
	typeof(*(ptr)) __old;						\
	size_t __size = sizeof(*(ptr));					\
									\
	BUG_ON(__size > sizeof(unsigned long));                         \
	__old = (typeof(*(ptr)))__raw_atomic_cmpxchg(			\
					(unsigned long *)(ptr),	(unsigned long)(o), \
					(unsigned long)(n), __size);		\
	__old;								\
})

#define raw_atomic_cmpxchg64(ptr, o, n)					\
({									\
	typeof(*(ptr)) __old;						\
									\
	BUG_ON(sizeof(*(ptr)) != sizeof(uint64_t));                     \
	__old = (typeof(*(ptr)))__raw_atomic_cmpxchg64(			\
					(uint64_t *)(ptr),		\
					(uint64_t)(o), (uint64_t)(n));	\
	__old;								\
})

unsigned long __raw_atomic_xchg(unsigned long *ptr, unsigned long x, size_t size);
unsigned long __raw_atomic_cmpxchg(unsigned long *ptr, unsigned long o,
				   unsigned long n, size_t size);

#endif
