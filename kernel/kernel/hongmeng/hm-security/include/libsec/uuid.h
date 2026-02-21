/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Some utils about UUID
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 25 10:10:04 2024
 */
#ifndef CRYPTO_UUID_H
#define CRYPTO_UUID_H

#include <stdint.h>
#include <libmem/utils.h>
#include <libhmsync/bitops.h>

#define UUID_SIZE 16U

struct uuid {
	uint8_t bytes[UUID_SIZE];
};

#define UUID_VERSION_BYTE 6U
#define UUID_VERSION_BITS 4U
#define UUID_VARIANT_BYTE 8U

#define UUID_VERSION_DATE_MAC			1U
#define UUID_VERSION_DATE_MAC_DCE		2U
#define UUID_VERSION_NAMESPACE_NAME_MD5		3U
#define UUID_VERSION_RANDOM			4U
#define UUID_VERSION_NAMESPACE_NAME_SHA1	5U

#define UUID_VARIANT_COMPAT	0U
#define UUID_VARIANT_DCE	1U
#define UUID_VARIANT_GUID	2U

static inline uint8_t uuid_uint8_shl(uint8_t num, unsigned int bit)
{
	return (uint8_t)((unsigned)num << bit);
}

static inline void uuid_set_prefix(uint8_t *byte, uint8_t prefix, unsigned int prefix_bits)
{
	unsigned keep_bits = ((unsigned)BITS_PER_BYTE - prefix_bits);
	*byte = (uint8_t)mask((unsigned)*byte, keep_bits) | uuid_uint8_shl(prefix, keep_bits);
}

static inline void uuid_set_format(struct uuid *uuid, uint8_t version,
				   unsigned int varient)
{
	uuid_set_prefix(&uuid->bytes[UUID_VERSION_BYTE], version, UUID_VERSION_BITS);
	/*
	 * varient 0: 0xxxxxxx
	 * varient 1: 10xxxxxx
	 * varient 2: 110xxxxx
	 * We shift 0xfe(0b1111,1110) in 8 bits to get variant-prefix.
	 */
	uuid_set_prefix(&uuid->bytes[UUID_VARIANT_BYTE], 0xfe, varient + 1);
}

#endif
