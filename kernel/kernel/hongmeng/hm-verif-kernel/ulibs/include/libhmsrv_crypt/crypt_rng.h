/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Provide interfaces of generating random numbers of different lengths
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 15 16:22:04 2019
 */
/*
 * Notice: random interface will block when entropy is not enougth
 */

#ifndef LIBHMSRV_CRYPT_RNG_H
#define LIBHMSRV_CRYPT_RNG_H

#include <string.h>
#include <sys/random.h>
#include <hongmeng/types.h>
#include <libhwsecurec/securec.h>

#include <libhmsrv_crypt/sysif/api.h>

int hmsrv_crypt_random_bytes_ex(rref_t crypto_rref, enum crypt_rng_type rng_type, unsigned char *out,
				size_t len, int flags);
int hmsrv_crypt_random_u8_ex(rref_t crypto_rref, enum crypt_rng_type rng_type, uint8_t *value, int flags);
int hmsrv_crypt_random_u16_ex(rref_t crypto_rref, enum crypt_rng_type rng_type, uint16_t *value, int flags);
int hmsrv_crypt_random_u32_ex(rref_t crypto_rref, enum crypt_rng_type rng_type, uint32_t *value, int flags);
int hmsrv_crypt_random_u64_ex(rref_t crypto_rref, enum crypt_rng_type rng_type, uint64_t *value, int flags);
int hmsrv_crypt_random_range_ex(rref_t crypto_rref, enum crypt_rng_type rng_type, uint32_t start,
				uint32_t end, uint32_t *value, int flags);
int hmsrv_crypt_random_entropy_avail(rref_t crypto_rref);
/* unblock interface */
static inline int hmsrv_crypt_urandom_bytes(rref_t crypto_rref, enum crypt_rng_type rng_type,
					    unsigned char *out, size_t len)
{
	return hmsrv_crypt_random_bytes_ex(crypto_rref, rng_type, out, len, 0);
}

static inline int hmsrv_crypt_urandom_u8(rref_t crypto_rref, enum crypt_rng_type rng_type, uint8_t *value)
{
	return hmsrv_crypt_random_u8_ex(crypto_rref, rng_type, value, 0);
}

static inline int hmsrv_crypt_urandom_u16(rref_t crypto_rref, enum crypt_rng_type rng_type, uint16_t *value)
{
	return hmsrv_crypt_random_u16_ex(crypto_rref, rng_type, value, 0);
}

static inline int hmsrv_crypt_urandom_u32(rref_t crypto_rref, enum crypt_rng_type rng_type, uint32_t *value)
{
	return hmsrv_crypt_random_u32_ex(crypto_rref, rng_type, value, 0);
}

static inline int hmsrv_crypt_urandom_u64(rref_t crypto_rref, enum crypt_rng_type rng_type, uint64_t *value)
{
	return hmsrv_crypt_random_u64_ex(crypto_rref, rng_type, value, 0);
}

static inline int hmsrv_crypt_urandom_range(rref_t crypto_rref, enum crypt_rng_type rng_type, uint32_t start,
					    uint32_t end, uint32_t *value)
{
	return hmsrv_crypt_random_range_ex(crypto_rref, rng_type, start, end, value, 0);
}

/* block interface */
static inline int hmsrv_crypt_random_bytes(rref_t crypto_rref, enum crypt_rng_type rng_type,
					   unsigned char *out, size_t len)
{
	return hmsrv_crypt_random_bytes_ex(crypto_rref, rng_type, out, len, GRND_RANDOM);
}

static inline int hmsrv_crypt_random_u8(rref_t crypto_rref, enum crypt_rng_type rng_type, uint8_t *value)
{
	return hmsrv_crypt_random_u8_ex(crypto_rref, rng_type, value, GRND_RANDOM);
}

static inline int hmsrv_crypt_random_u16(rref_t crypto_rref, enum crypt_rng_type rng_type, uint16_t *value)
{
	return hmsrv_crypt_random_u16_ex(crypto_rref, rng_type, value, GRND_RANDOM);
}

static inline int hmsrv_crypt_random_u32(rref_t crypto_rref, enum crypt_rng_type rng_type, uint32_t *value)
{
	return hmsrv_crypt_random_u32_ex(crypto_rref, rng_type, value, GRND_RANDOM);
}

static inline int hmsrv_crypt_random_u64(rref_t crypto_rref, enum crypt_rng_type rng_type, uint64_t *value)
{
	return hmsrv_crypt_random_u64_ex(crypto_rref, rng_type, value, GRND_RANDOM);
}

static inline int hmsrv_crypt_random_range(rref_t crypto_rref, enum crypt_rng_type rng_type, uint32_t start,
					   uint32_t end, uint32_t *value)
{
	return hmsrv_crypt_random_range_ex(crypto_rref, rng_type, start, end, value, GRND_RANDOM);
}

#endif
