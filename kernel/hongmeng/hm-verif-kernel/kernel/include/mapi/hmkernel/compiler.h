/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Export kernel complier.h to mapi
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 08 10:44:29 2019
 */
#ifndef MAPI_HMKERNEL_COMPILER_H
#define MAPI_HMKERNEL_COMPILER_H

#include <uapi/hmkernel/compiler.h>
#include <hmkernel/strict.h>
#include <hmasm/types.h>

#define __packed __attribute__((packed))
#define __unused __attribute__((unused))
#define __pure __attribute__((pure))
#define __no_utsan __attribute__((no_sanitize_thread))

#define __align_of(v) __alignof__(v)

#ifdef __HOST_LLT__

/*
 * If some function need to be run before the test main body in llt,
 * use following __llt_xxx_init series specifier
 */
#define MODULE_EARLY_DEVICE		101
#define MODULE_KEY_DEVICE		102
#define MODULE_DEVICE			103
#define MODULE_LATE_DEVICE		104

#define __llt_module_init(lv) __attribute__((constructor(lv)))
#define __llt_early_device_init __llt_module_init(MODULE_EARLY_DEVICE)
#define __llt_key_device_init __llt_module_init(MODULE_KEY_DEVICE)
#define __llt_device_init __llt_module_init(MODULE_DEVICE)
#define __llt_late_device_init __llt_module_init(MODULE_LATE_DEVICE)
#else
#define __llt_early_device_init
#define __llt_key_device_init
#define __llt_device_init
#define __llt_late_device_init
#endif

/*
 * Indicate that the kernel part of a object is kept unmodified during module usage.
 * For instance, "const struct scheduler_s *scheduler" means the scheduler's struct it self
 * is not modified, but its mdata maybe modified.
 */
#define __kconst const

#define unreachable()  __builtin_unreachable()

#define static_assert(condition, name)  extern int __STATIC_ASSERT_##name[1 - 2 * ((condition) ? 0 : 1)] __unused

#define __user

/* Mark the guest domain memory */
#define __guest

#ifndef CONFIG_SPECV1_MITIGATION
#define array_index_fixup(index, size)  index

#define array_idxing(array, index)  array[index]

#else
/*
 * The function return a mask value for an array,
 * ~0 for index < size, 0 for index >= size.
 * index is a index num in an array. size is the size of an array.
 * If index < size, we get the mask 0xffffffffffffffff.
 * If index >= size, we get the mask 0x0.
 */
#ifndef __ASSEMBLY__
static inline unsigned long __array_index_mask(unsigned long index, unsigned long size)
{
	unsigned long distance = ~((size - index - 1UL) | index);
	return (0UL - (distance >> (BITS_PER_LONG - 1UL)));
}

/*
 * We use this function to fix up the index of an array.
 * If index >= size, the fixup index value will be 0.
 * If index < size, the fixup index valuse will be the original
 * index value.
 */
#define array_index_fixup(index, size)					\
({									\
	typeof(index) _index = (index);					\
	typeof(size) _size = (size);					\
									\
	unsigned long mask = __array_index_mask((unsigned long)_index, (unsigned long)_size);		\
	(typeof(_index))(_index & mask);				\
})

/*
 * We define a new macro for Spec_v1.
 * Normally, without Spec_v1 mitigation, INDEXING means a[i].
 * On the other side, with Spec_v1 mitigation, INDEXING uses
 * array_index_fixup for a fixed up index.
 */
#define array_idxing(array, index)					\
	array[array_index_fixup(index, sizeof(array) / sizeof(array[0]))]
#endif
#endif

/* merge jam helper marcos */
#ifdef __JAM_BUILD
#define JAM_INLINE static inline __always_inline
#define JAM_EXTERN_INLINE extern inline
#else
#define JAM_INLINE
#define JAM_EXTERN_INLINE
#endif

#ifndef __hmbuild__
/* Use legacy kconfig option to define morden hmbuild feature macro */
# ifdef CONFIG_KCFI
#  ifndef __hmfeature_cfi__
#   define __hmfeature_cfi__
#  endif
# endif
#endif

#ifdef __hmfeature_cfi__
#define __nocfi		__attribute__((nocfi))
#else
#define __nocfi
#endif

#ifdef CONFIG_UBSAN
#ifdef __clang__
#define __no_sanitize_undefined __attribute__((no_sanitize("undefined")))
#elif __GNUC__
#define __no_sanitize_undefined __attribute__((no_sanitize_undefined))
#else
#error "unknown compiler"
#endif
#else
#define __no_sanitize_undefined
#endif

#define likely(x)	__likely(x)
#define unlikely(x)	__unlikely(x)

#endif
