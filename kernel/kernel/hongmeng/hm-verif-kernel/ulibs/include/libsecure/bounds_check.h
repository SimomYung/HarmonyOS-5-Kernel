/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Library for bounds check
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 25 12:57:57 2018
 */
#ifndef LIBSEC_BOUNDS_CHECK_H
#define LIBSEC_BOUNDS_CHECK_H

#include <hongmeng/types.h>
#include <hmkernel/const.h>

#ifndef __BITS_PER_BYTE
#define __BITS_PER_BYTE __U(8)
#endif
/*
 * The function return a mask value for an array,
 * ~0 for index < size, 0 for index >= size.
 * index is a index num in an array. size is the size of an array.
 * If index < size, we get the mask 0xffffffffffffffff.
 * If index >= size, we get the mask 0x0.
 */
static inline unsigned long __array_index_mask(unsigned long index, unsigned long size)
{
	unsigned long distance = ~((size - index - 1UL) | index);
	return (0UL - (distance >> (__BITS_PER_BYTE * sizeof(distance) - 1UL)));
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
	unsigned long mask_value = __array_index_mask((unsigned long)_index, (unsigned long)_size); \
	(typeof(_index))(_index & mask_value);				\
})

/*
 * We define a new macro for Spec_v1.
 * Normally, without Spec_v1 mitigation, INDEXING means a[i].
 * On the other side, with Spec_v1 mitigation, INDEXING uses
 * array_index_fixup for a fixed up index.
 */
#define array_idxing(array, index)					\
	array[array_index_fixup(index, sizeof(array) / sizeof((array)[0]))]

#endif
