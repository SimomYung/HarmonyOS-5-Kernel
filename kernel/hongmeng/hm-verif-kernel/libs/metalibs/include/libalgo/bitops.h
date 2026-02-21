/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Wrappers for builtin bit operations
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 05 11:40:46 2021
 */
#ifndef ULIBS_LIBALGO_BITOPS_H
#define ULIBS_LIBALGO_BITOPS_H

#include <stdint.h>

#define BITS_PER_BYTE		8U

/*
 * count the trailing zeros of the given unsigned long long integer
 *
 * @param val [I] Value to count
 *
 * @return    Count of tail zeros
 *
 * @note      Input 0 is undefined for __builtin_ctzll.
 */
static inline uint32_t bit_ctzll(uint64_t val)
{
	return (val == 0ULL) ? 0U : (uint32_t)__builtin_ctzll(val);
}

/*
 * count the leading zeros of the given unsigned long long integer
 *
 * @param val [I] Value to count
 *
 * @return    Count of leading zeros
 *
 * @note      Input 0 is undefined for __builtin_clzll.
 */
static inline uint32_t bit_clzll(uint64_t val)
{
	return (val == 0ULL) ? 0U : (uint32_t)__builtin_clzll(val);
}

/*
 * find last (most-significant) set bit in a word.
 */
static inline uint32_t bit_flsll(uint64_t val)
{
	return (val == 0ULL) ? 0U
			    : (((uint32_t)sizeof(val) * BITS_PER_BYTE) -
			       (uint32_t)__builtin_clzll(val));
}

/*
 * one plus the index of the least significant 1-bit of val, or if x is zero, returns zero
 *
 * @param val [I] Value to count
 *
 * @return    Count of leading zeros
 *
 * @note      It will return 0 if value is 0
 */
static inline uint32_t bit_ffs(uint32_t val)
{
	return (uint32_t)__builtin_ffs(val);
}
#endif /* ULIBS_LIBALGO_BITOPS_H */
