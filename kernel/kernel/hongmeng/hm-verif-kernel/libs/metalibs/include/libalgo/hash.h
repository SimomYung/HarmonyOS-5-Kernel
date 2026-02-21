/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Hash functions
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 15 21:27:14 2020
 */
#ifndef ULIBS_LIBALGO_HASH_H
#define ULIBS_LIBALGO_HASH_H

#include <stdint.h>

/*
 * Fibonacci hashing based on golden section is used for integer.
 * It uses golden ratio as multiplier for hashing for both 32 and
 * 64 bits integer. And input is processed by shifting the span
 * of retained top bits down and XORing them before multiplication
 * to avoid the problem of higher-value input bits do not affect
 * lower-value output bits.
 */
#define h32_shift(x) ((uint32_t)32 - (uint32_t)(x))
#define h64_shift(x) ((uint32_t)64 - (uint32_t)(x))
#define GOLD_MEAN_32 ((uint32_t)2654435769)
#define GOLD_MEAN_64 ((uint64_t)1400714813093931008)

/*
 * Generate hash code for a 32 bit interger
 *
 * @param val    [I] 32 bits key for the hash function
 *
 * @param bits   [I] bits hash range used
 *
 * @return       Hash code generated
 *
 * @note         Caller should ensure that bits is less
 *               than size of key type
 */
static inline uint32_t hash_32(uint32_t val, uint32_t bits)
{
	return (((val ^ (val >> h32_shift(bits))) * GOLD_MEAN_32) >> h32_shift(bits));
}

/*
 * Generate hash code for a 64 bit interger
 *
 * @param val    [I] 64 bits key for the hash function
 *
 * @param bits   [I] bits hash range used
 *
 * @return       Hash code generated
 *
 * @note         Caller should ensure that bits is less
 *               than size of key type
 */
static inline uint32_t hash_64(uint64_t val, uint32_t bits)
{
	return (uint32_t)(((val ^ (val >> h64_shift(bits))) * GOLD_MEAN_64) >> h64_shift(bits));
}

uint32_t str_to_hash(const unsigned char *str);

/* case-insensitive */
uint32_t str_to_hash_nocase(const unsigned char *str, uint32_t len);

uint32_t int_sqrtll(uint64_t arg);

#endif  /* ULIBS_LIBALGO_HASH_H */
