/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Provide interfaces of generating random numbers of different lengths
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 14 18:07:15 2019
 */
#include <sys/param.h>
#include <libhwsecurec/securec.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmactv/actv.h>

#include <libhmsrv_crypt/crypt_rng.h>
#include <libhmsrv_crypt/crypt_server.h>

static int raw_crypt_random_value(rref_t crypto_rref, enum crypt_rng_type rng_type,
				  enum crypt_rng_rand_val_type value_type,
				  union crypt_rng_rand_value *value,
				  int flags)
{
	int ret = REF_TO_ERR(crypto_rref);
	if (ret != E_HM_OK) {
		hm_error("crypto rref is invalid, %s\n", hmstrerror(ret));
		return ret;
	}

	hm_actv_set_accessible_mem_range(ptr_to_ulong(value), sizeof(*value));
	ret = actvcapcall_cryptcall_rng_random_value(crypto_rref, rng_type, value_type,
						     value, flags);
	if (ret < E_HM_OK) {
		hm_debug("crypt rng random value failed, %s\n",
			 hmstrerror(ret));
		return ret;
	}

	return ret;
}

/*
 * If failed, 'out' may still be changed
 */
int hmsrv_crypt_random_bytes_ex(rref_t crypto_rref, enum crypt_rng_type rng_type,
				unsigned char *out, size_t len, int flags)
{
	int ret = E_HM_OK;
	size_t need_len;
	unsigned int read_len;
	struct crypt_rng_rand_data udata;

	if (rng_type >= CRYPT_RNG_MAX || out == NULL || len == 0) {
		return E_HM_INVAL;
	}

	ret = REF_TO_ERR(crypto_rref);
	if (ret != E_HM_OK) {
		hm_error("crypto rref is invalid, %s\n", hmstrerror(ret));
		return ret;
	}

	need_len = len;
	while (need_len > 0) {
		read_len = (unsigned int)MIN(need_len, CRYPT_RNG_RAND_DATA_MAX);
		hm_actv_set_accessible_mem_range(ptr_to_ulong(&udata), sizeof(udata));
		ret = actvcapcall_cryptcall_rng_random_bytes(crypto_rref,
			rng_type, &udata, read_len, flags);
		if (ret < E_HM_OK) {
			hm_debug("crypt rng random bytes failed, %s\n",
				 hmstrerror(ret));
			break;
		}

		ret = memcpy_s(out, need_len, udata.buf, udata.len);
		if (ret != EOK) {
			hm_debug("memcpy_s failed, err:%d\n", ret);
			ret = posix2hmerrno(ret);
			break;
		}

		need_len -= udata.len;
		out += udata.len;
	}

	if (len < CRYPT_RNG_RAND_DATA_MAX) {
		NOFAIL(memset_s(udata.buf, CRYPT_RNG_RAND_DATA_MAX, 0, len));
	} else {
		NOFAIL(memset_s(udata.buf, CRYPT_RNG_RAND_DATA_MAX, 0, CRYPT_RNG_RAND_DATA_MAX));
	}
	return ret;
}

int hmsrv_crypt_random_u8_ex(rref_t crypto_rref, enum crypt_rng_type rng_type, uint8_t *value, int flags)
{
	int ret;
	union crypt_rng_rand_value uvalue;

	if (rng_type >= CRYPT_RNG_MAX || value == NULL) {
		return E_HM_INVAL;
	}

	ret = raw_crypt_random_value(crypto_rref, rng_type, CRYPT_RNG_RAND_VAL_U8, &uvalue, flags);
	if (ret == E_HM_OK) {
		*value = uvalue.data_u8;
	}

	NOFAIL(memset_s(&uvalue, sizeof(uvalue), 0, sizeof(uvalue)));
	return ret;
}

int hmsrv_crypt_random_u16_ex(rref_t crypto_rref, enum crypt_rng_type rng_type, uint16_t *value, int flags)
{
	int ret;
	union crypt_rng_rand_value uvalue;

	if (rng_type >= CRYPT_RNG_MAX || value == NULL) {
		return E_HM_INVAL;
	}

	ret = raw_crypt_random_value(crypto_rref, rng_type, CRYPT_RNG_RAND_VAL_U16, &uvalue, flags);
	if (ret == E_HM_OK) {
		*value = uvalue.data_u16;
	}

	NOFAIL(memset_s(&uvalue, sizeof(uvalue), 0, sizeof(uvalue)));
	return ret;
}

int hmsrv_crypt_random_u32_ex(rref_t crypto_rref, enum crypt_rng_type rng_type, uint32_t *value, int flags)
{
	int ret;
	union crypt_rng_rand_value uvalue;

	if (rng_type >= CRYPT_RNG_MAX || value == NULL) {
		return E_HM_INVAL;
	}

	ret = raw_crypt_random_value(crypto_rref, rng_type, CRYPT_RNG_RAND_VAL_U32, &uvalue, flags);
	if (ret == E_HM_OK) {
		*value = uvalue.data_u32;
	}

	NOFAIL(memset_s(&uvalue, sizeof(uvalue), 0, sizeof(uvalue)));
	return ret;
}

int hmsrv_crypt_random_u64_ex(rref_t crypto_rref, enum crypt_rng_type rng_type, uint64_t *value, int flags)
{
	int ret;
	union crypt_rng_rand_value uvalue;

	if (rng_type >= CRYPT_RNG_MAX || value == NULL) {
		return E_HM_INVAL;
	}

	ret = raw_crypt_random_value(crypto_rref, rng_type, CRYPT_RNG_RAND_VAL_U64, &uvalue, flags);
	if (ret == E_HM_OK) {
		*value = uvalue.data_u64;
	}

	NOFAIL(memset_s(&uvalue, sizeof(uvalue), 0, sizeof(uvalue)));
	return ret;
}

int hmsrv_crypt_random_range_ex(rref_t crypto_rref, enum crypt_rng_type rng_type, uint32_t start,
			     uint32_t end, uint32_t *value, int flags)
{
	int ret;
	union crypt_rng_rand_value uvalue;

	if (rng_type >= CRYPT_RNG_MAX || start >= end || value == NULL) {
		return E_HM_INVAL;
	}

	ret = raw_crypt_random_value(crypto_rref, rng_type, CRYPT_RNG_RAND_VAL_U32, &uvalue, flags);
	if (ret == E_HM_OK) {
		/*
		 * Calculate "start + (end - start) * rand_val" to get a random value in [start, end),
		 * and rand_val is in [0,1).
		 */
		// use a random in [0, 2^32) multiply (end - start) to get [end - start, 2^32 * (end - start))
		uint64_t tmp1 = (uint64_t)uvalue.data_u32 * (end - start);
		// divide 2^32 to get random in [0, end - start)
		uint32_t tmp2 = (uint32_t)(tmp1 >> 32);
		// add start to get [start, end)
		*value = tmp2 + start;
	}

	mem_zero_s(uvalue);
	return ret;
}

int hmsrv_crypt_random_entropy_avail(rref_t crypto_rref)
{
	int ret;

	ret = REF_TO_ERR(crypto_rref);
	if (ret != E_HM_OK) {
		hm_error("crypto rref is invalid, %s\n", hmstrerror(ret));
		return ret;
	}

	ret = actvcapcall_cryptcall_rng_random_entropy_avail(crypto_rref);
	if (ret < E_HM_OK) {
		hm_debug("crypt rng random entropy avail failed, %s\n",
			 hmstrerror(ret));
	}

	return ret;
}
