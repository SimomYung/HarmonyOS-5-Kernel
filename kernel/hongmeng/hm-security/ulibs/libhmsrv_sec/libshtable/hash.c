/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Hash functions for shared hash table
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 25 10:49:31 2019
 */
#include "hash.h"

#include <libhmlog/hmlog.h>

#include "libshtable/shtable.h"

#define BITS_PER_BYTE	8U
#define SCRAMBLE_SHIFT_BIT 11
#define SCRAMBLE_KEY_SHIFT_BIT 17

static inline uint32_t get_uint32(const uint8_t *p, size_t size)
{
	size_t i;
	uint32_t n = 0;
	/*
	 * Loop termination condition:
	 * Following loop terminate when i reach 0, Since i keep decreasing
	 * in loop body, it has a upper bound.
	 * Loop condition is constant and reasonable:
	 * size is less than sht->key_desc.size, which is limited, so the loop condition is reasonable.
	 */
	for (i = size; i != 0U; --i) {
		n <<= BITS_PER_BYTE;
		n |= p[i - 1U];
	}
	return n;
}

static inline uint32_t rot_lshfit(uint32_t n, size_t shift)
{
	return (n << shift) | (n >> (sizeof(uint32_t) * BITS_PER_BYTE - shift));
}

static inline uint32_t scramble_key(uint32_t k)
{
	uint32_t lk = k;
	lk ^= 0x3c65ab25U;
	lk *= 0x9e3779dbU;
	lk = rot_lshfit(lk, SCRAMBLE_KEY_SHIFT_BIT);
	return lk;
}

static inline uint32_t scramble(uint32_t h, uint32_t k)
{
	uint32_t lh = h;
	lh ^= scramble_key(k);
	lh = rot_lshfit(lh, SCRAMBLE_SHIFT_BIT);
	lh *= 0x9e3779dbU;
	lh += 0xd38c59eaU;

	return lh;
}

static uint32_t hash(const uint8_t *key, size_t size)
{
	uint32_t h = (uint32_t)size * 0xe5687ac3U;
	uint32_t k;
	size_t i, aligned_size;

	aligned_size = (size / sizeof(uint32_t)) * sizeof(uint32_t);

	/*
	 * Loop termination condition:
	 * Following loop terminate when i reach aligned_size, Since i keep increasing
	 * in loop body, it has a upper bound.
	 * Loop condition is constant and reasonable:
	 * aligned_size is limited, so the loop condition is reasonable.
	 */
	for (i = 0; i < aligned_size; i += sizeof(uint32_t)) {
		k = get_uint32(key + i, sizeof(uint32_t));
		h = scramble(h, k);
	}

	k = get_uint32(key + aligned_size, size - aligned_size);
	h = scramble(h, k);

	return h;
}

uint32_t sht_hash(const struct shtable *sht, const void *key, size_t size)
{
	uint32_t ret = 0;
	if (sht == NULL || key == NULL) {
		hm_debug("parameters are invalid\n");
	} else {
		if (sht->hash_desc.hash != NULL) {
			ret = sht->hash_desc.hash(key, size);
		} else {
			ret = hash((const uint8_t *)key, size);
		}
	}

	return ret;
}
