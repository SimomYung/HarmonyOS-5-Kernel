/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Hash
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 15 21:27:14 2020
 */

#include <libalgo/hash.h>
#include <libalgo/bitops.h>
#include <stddef.h>

/*
 * Parameters for int_sqrtll
 */
#define INT_SQRT_SHIFT1		1ULL
#define INT_SQRT_SHIFT2		2ULL

/* equal to: hash = 131203 * hash */
#define hash_multi(hash) (((hash) << 17) + /* 17: 131072 * hash */ \
			  ((hash) << 7)  + /* 7: 128 * hash */ \
			  ((hash) << 1)  +  (hash))

/* similar SDBMHash */
uint32_t str_to_hash(const unsigned char *str)
{
	uint32_t hash = 0;

	if (str != NULL) {
		while (*str != (unsigned char)'\0') {
			hash = (*str++) + hash_multi(hash);
		}
	}

	return hash;
}

/* case-insensitive */
uint32_t str_to_hash_nocase(const unsigned char *str, uint32_t len)
{
	uint32_t hash = 0;
	unsigned int offset = 'a' - 'A';
	const unsigned char *cur_str = str;

	if (cur_str != NULL) {
		while (*cur_str != (unsigned char)'\0') {
			if (len == 0U) {
				break;
			}

			if ((*cur_str >= (unsigned char)'a') &&
			    (*cur_str <= (unsigned char)'z')) {
				hash = (*cur_str - offset) + hash_multi(hash);
			} else {
				hash = (*cur_str) + hash_multi(hash);
			}
			cur_str++;
			len--;
		}
	}
	return hash;
}

uint32_t int_sqrtll(uint64_t arg)
{
	uint32_t ret = 0;
	uint64_t val = arg;
	uint64_t b, m, y = 0;

	if (val <= 1ULL) {
		ret = (uint32_t)val;
	} else {
		m = ((uint64_t)bit_flsll(val)) & (~(1ULL));
		m = (1ULL) << m;
		while (m != 0ULL) {
			b = y + m;
			y >>= INT_SQRT_SHIFT1;
			if (val >= b) {
				val -= b;
				y += m;
			}
			m >>= INT_SQRT_SHIFT2;
		}
		ret = (uint32_t)y;
	}

	return ret;
}
