/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Credential base inline functions
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 25 10:58:09 2024
 */
#ifndef ULIBS_LIBSEC_CRED_COMMON_H
#define ULIBS_LIBSEC_CRED_COMMON_H
#include "cred_common_struct.h"

#define __always_inline        __attribute__((always_inline))
#ifdef ENABLE_PAC_HARD_CODE

#define ENCAPS_SIGNED_LEN (sizeof(cred_idx_t) + sizeof(enum encaps_proc_type))

static inline __always_inline __u64 hm_pacga_encaps_mem(struct sec_cred_encaps *cred_encaps, cred_idx_t bcred_idx)
{
	if (cred_encaps == NULL) {
		return;
	}
	return hm_pacga_mem(cred_encaps, ENCAPS_SIGNED_LEN, bcred_idx);
}

static inline __always_inline __u64 hm_pacdb_encaps_mem(struct sec_cred_encaps *cred_encaps, cred_idx_t bcred_idx)
{
	if (cred_encaps == NULL) {
		return;
	}
	return hm_pacdb_mem(cred_encaps, ENCAPS_SIGNED_LEN, bcred_idx);
}

static inline __always_inline __u64 hm_pacga_encaps(struct base_cred *b_cred)
{
	__u64 modifier = b_cred->idx;
	if (b_cred->encaps_idx != 0) {
		/* protect encaps's data */
		modifier = hm_pacga_encaps_mem(sec_cred_idx_to_ptr(b_cred->encaps_idx), b_cred->idx);
	}
	return hm_pacga(b_cred->encaps_idx, modifier);
}

static inline __always_inline int hm_pacga_encaps_check(struct base_cred *b_cred, bool retry)
{
	int ret = E_HM_OK;
	__u64 modifier = b_cred->idx;
	if (b_cred->encaps_idx != 0) {
		/* protect encaps's data */
		modifier = hm_pacga_encaps_mem(sec_cred_idx_to_ptr(b_cred->encaps_idx), b_cred->idx);
	}
	ret = hm_pacga_check_with_BID(b_cred->encaps_idx, modifier, b_cred->pac[SEC_PAC_BASE_ENCAPS],
				      SEC_PAC_BASE_ENCAPS, retry);
	return ret;
}

static inline __always_inline __u64 hm_pacdb_encaps(struct base_cred *b_cred)
{
	__u64 modifier = b_cred->idx;
	if (b_cred->encaps_idx != 0) {
		/* protect encaps's data */
		modifier = hm_pacdb_encaps_mem(sec_cred_idx_to_ptr(b_cred->encaps_idx), b_cred->idx);
	}
	return hm_pacdb(b_cred->encaps_idx, modifier);
}

static inline __always_inline int hm_pacdb_encaps_check(struct base_cred *b_cred, bool retry)
{
	int ret = E_HM_OK;
	__u64 modifier = b_cred->idx;
	if (b_cred->encaps_idx != 0) {
		/* protect encaps's data */
		modifier = hm_pacdb_encaps_mem(sec_cred_idx_to_ptr(b_cred->encaps_idx), b_cred->idx);
	}
	ret = hm_pacdb_check_with_BID(b_cred->encaps_idx, modifier, b_cred->pac[SEC_PAC_BASE_ENCAPS],
				      SEC_PAC_BASE_ENCAPS, retry);
	return ret;
}

static inline __always_inline void hm_pacga_bcred_mem(struct base_cred *b_cred)
{
	b_cred->pac[SEC_PAC_BASE_IDX] 		= hm_pacga(b_cred->idx, (__u64)&b_cred->idx);
	b_cred->pac[SEC_PAC_BASE_SHIDX]		= hm_pacga(b_cred->sh_idx, b_cred->idx);
	b_cred->pac[SEC_PAC_BASE_DAC]		= hm_pacga_mem((void *)&b_cred->cred_dac,
							       sizeof(struct sec_cred_dac),
							       b_cred->idx);
	b_cred->pac[SEC_PAC_BASE_SEHARMONY]	= hm_pacga_mem((void *)&b_cred->cred_sel,
							       sizeof(struct sec_cred_seharmony),
							       b_cred->idx);
	b_cred->pac[SEC_PAC_BASE_CAPS]		= hm_pacga_mem((void *)&b_cred->cred_caps,
							       sizeof(struct sec_cred_caps),
							       b_cred->idx);
	b_cred->pac[SEC_PAC_BASE_KEYRING]	= hm_pacga_mem((void *)&b_cred->cred_key,
							       sizeof(struct sec_cred_keyring),
							       b_cred->idx);
	b_cred->pac[SEC_PAC_BASE_NSID_GROUP]	= hm_pacga_mem((void *)&b_cred->nsid_group,
							       sizeof(struct nsid_group),
							       b_cred->idx);
	b_cred->pac[SEC_PAC_BASE_ENCAPS]	= hm_pacga_encaps(b_cred);
}

static inline __always_inline void hm_pacdb_bcred_mem(struct base_cred *b_cred)
{
	b_cred->pac[SEC_PAC_BASE_SHIDX]		= hm_pacdb(b_cred->sh_idx, b_cred->idx);
	b_cred->pac[SEC_PAC_BASE_DAC]		= hm_pacdb_mem((void *)&b_cred->cred_dac,
							       sizeof(struct sec_cred_dac),
							       b_cred->idx);
	b_cred->pac[SEC_PAC_BASE_SEHARMONY]	= hm_pacdb_mem((void *)&b_cred->cred_sel,
							       sizeof(struct sec_cred_seharmony),
							       b_cred->idx);
	b_cred->pac[SEC_PAC_BASE_CAPS]		= hm_pacdb_mem((void *)&b_cred->cred_caps,
							       sizeof(struct sec_cred_caps),
							       b_cred->idx);
	b_cred->pac[SEC_PAC_BASE_KEYRING]	= hm_pacdb_mem((void *)&b_cred->cred_key,
							       sizeof(struct sec_cred_keyring),
							       b_cred->idx);
	b_cred->pac[SEC_PAC_BASE_NSID_GROUP]	= hm_pacdb_mem((void *)&b_cred->nsid_group,
							       sizeof(struct nsid_group),
							       b_cred->idx);
	b_cred->pac[SEC_PAC_BASE_ENCAPS]	= hm_pacdb_encaps(b_cred);
}

static inline __always_inline int hm_pacdb_bcred_mem_check(const struct base_cred *b_cred,
							   unsigned long long modifier, bool retry)
{
	int ret = E_HM_OK;
	ret = hm_pacdb_check(b_cred->sh_idx, modifier, b_cred->pac[SEC_PAC_BASE_SHIDX], retry);
	ret = hm_pacdb_mem_check((void *)&b_cred->cred_dac, sizeof(struct sec_cred_dac),
		modifier, b_cred->pac[SEC_PAC_BASE_DAC], SEC_PAC_BASE_DAC, retry);
	ret = hm_pacdb_mem_check((void *)&b_cred->cred_sel, sizeof(struct sec_cred_seharmony),
		modifier, b_cred->pac[SEC_PAC_BASE_SEHARMONY], SEC_PAC_BASE_SEHARMONY, retry);
	ret = hm_pacdb_mem_check((void *)&b_cred->cred_caps, sizeof(struct sec_cred_caps),
		modifier, b_cred->pac[SEC_PAC_BASE_CAPS], SEC_PAC_BASE_CAPS, retry);
	ret = hm_pacdb_mem_check((void *)&b_cred->cred_key, sizeof(struct sec_cred_keyring),
		modifier, b_cred->pac[SEC_PAC_BASE_KEYRING], SEC_PAC_BASE_KEYRING, retry);
	ret = hm_pacdb_mem_check((void *)&b_cred->nsid_group, sizeof(struct nsid_group),
		modifier, b_cred->pac[SEC_PAC_BASE_NSID_GROUP], SEC_PAC_BASE_NSID_GROUP, retry);
	ret = hm_pacdb_encaps_check(b_cred, retry);
	return ret;
}

static inline __always_inline void hm_pacga_shcred_mem(struct shared_cred *sh_cred)
{
	sh_cred->pac[SEC_PAC_SHARED_MAC]	= hm_pacga_mem((void *)&sh_cred->cred_mac,
							       sizeof(struct sec_cred_mac),
							       sh_cred->idx);
	sh_cred->pac[SEC_PAC_SHARED_BASIC]	= hm_pacga_mem((void *)&sh_cred->cred_basic,
							       sizeof(struct sec_cred_basic),
							       sh_cred->idx);
	sh_cred->pac[SEC_PAC_SHARED_PROPERTY]	= hm_pacga_mem((void *)&sh_cred->prop,
							       sizeof(struct sec_cred_property),
							       sh_cred->idx);
	sh_cred->pac[SEC_PAC_SHARED_AUDIT]	= hm_pacga_mem((void *)&sh_cred->cred_audit,
							       sizeof(struct sec_cred_audit),
							       sh_cred->idx);
}

static inline __always_inline void hm_pacdb_shcred_mem(struct shared_cred *sh_cred)
{
	sh_cred->pac[SEC_PAC_SHARED_MAC]	= hm_pacdb_mem((void *)&sh_cred->cred_mac,
							       sizeof(struct sec_cred_mac),
							       sh_cred->idx);
	sh_cred->pac[SEC_PAC_SHARED_BASIC]	= hm_pacdb_mem((void *)&sh_cred->cred_basic,
							       sizeof(struct sec_cred_basic),
							       sh_cred->idx);
	sh_cred->pac[SEC_PAC_SHARED_PROPERTY]	= hm_pacdb_mem((void *)&sh_cred->prop,
							       sizeof(struct sec_cred_property),
							       sh_cred->idx);
	sh_cred->pac[SEC_PAC_SHARED_AUDIT]	= hm_pacdb_mem((void *)&sh_cred->cred_audit,
							       sizeof(struct sec_cred_audit),
							       sh_cred->idx);
}

static inline __always_inline int hm_pacdb_shcred_mem_check(struct shared_cred *sh_cred,
							    unsigned long long modifier, bool retry)
{
	int ret = E_HM_OK;
	ret = hm_pacdb_mem_check((void *)&sh_cred->cred_mac, sizeof(struct sec_cred_mac),
		modifier, sh_cred->pac[SEC_PAC_SHARED_MAC], SEC_PAC_SHARED_MAC, retry);
	ret = hm_pacdb_mem_check((void *)&sh_cred->cred_basic, sizeof(struct sec_cred_basic),
		modifier, sh_cred->pac[SEC_PAC_SHARED_BASIC], SEC_PAC_SHARED_BASIC, retry);
	ret = hm_pacdb_mem_check((void *)&sh_cred->prop, sizeof(struct sec_cred_property),
		modifier, sh_cred->pac[SEC_PAC_SHARED_PROPERTY], SEC_PAC_SHARED_PROPERTY, retry);
	ret = hm_pacdb_mem_check((void *)&sh_cred->cred_audit, sizeof(struct sec_cred_audit),
		modifier, sh_cred->pac[SEC_PAC_SHARED_AUDIT], SEC_PAC_SHARED_AUDIT, retry);
	return ret;
}

static inline __always_inline void hm_pacga_bcred(struct base_cred *b_cred, enum sec_pac_base_id BID)
{
	switch (BID) {
	case SEC_PAC_BASE_IDX:
		b_cred->pac[SEC_PAC_BASE_IDX] = hm_pacga(b_cred->idx, (__u64)&b_cred->idx);
		break;
	case SEC_PAC_BASE_DAC:
		b_cred->pac[SEC_PAC_BASE_DAC] = hm_pacga_mem((void *)&b_cred->cred_dac,
							     sizeof(struct sec_cred_dac),
							     b_cred->idx);
		break;
	case SEC_PAC_BASE_SEHARMONY:
		b_cred->pac[SEC_PAC_BASE_SEHARMONY] = hm_pacga_mem((void *)&b_cred->cred_sel,
								   sizeof(struct sec_cred_seharmony),
								   b_cred->idx);
		break;
	case SEC_PAC_BASE_CAPS:
		b_cred->pac[SEC_PAC_BASE_CAPS] = hm_pacga_mem((void *)&b_cred->cred_caps,
							      sizeof(struct sec_cred_caps),
							      b_cred->idx);
		break;
	case SEC_PAC_BASE_KEYRING:
		b_cred->pac[SEC_PAC_BASE_KEYRING] = hm_pacga_mem((void *)&b_cred->cred_key,
								 sizeof(struct sec_cred_keyring),
								 b_cred->idx);
		break;
	case SEC_PAC_BASE_NSID_GROUP:
		b_cred->pac[SEC_PAC_BASE_NSID_GROUP] = hm_pacga_mem((void *)&b_cred->nsid_group,
								    sizeof(struct nsid_group),
								    b_cred->idx);
		break;
	case SEC_PAC_BASE_ENCAPS:
		b_cred->pac[SEC_PAC_BASE_ENCAPS] = hm_pacga_encaps(b_cred);
		break;
	default:
		break;
	}
}

static inline __always_inline int hm_pacga_bcred_check(const struct base_cred *b_cred,
						       enum sec_pac_base_id BID, bool retry)
{
	/* check the modifier */
	int ret = hm_pacga_check(b_cred->idx, (__u64)&b_cred->idx, b_cred->pac[SEC_PAC_BASE_IDX], retry);

	switch (BID) {
	case SEC_PAC_BASE_DAC:
		ret |= hm_pacga_mem_check((void *)&b_cred->cred_dac, sizeof(struct sec_cred_dac),
					  b_cred->idx, b_cred->pac[SEC_PAC_BASE_DAC], BID, retry);
		break;
	case SEC_PAC_BASE_SEHARMONY:
		ret |= hm_pacga_mem_check((void *)&b_cred->cred_sel, sizeof(struct sec_cred_seharmony),
					  b_cred->idx, b_cred->pac[SEC_PAC_BASE_SEHARMONY], BID, retry);
		break;
	case SEC_PAC_BASE_CAPS:
		ret |= hm_pacga_mem_check((void *)&b_cred->cred_caps, sizeof(struct sec_cred_caps),
					  b_cred->idx, b_cred->pac[SEC_PAC_BASE_CAPS], BID, retry);
		break;
	case SEC_PAC_BASE_KEYRING:
		ret |= hm_pacga_mem_check((void *)&b_cred->cred_key, sizeof(struct sec_cred_keyring),
					  b_cred->idx, b_cred->pac[SEC_PAC_BASE_KEYRING], BID, retry);
		break;
	case SEC_PAC_BASE_NSID_GROUP:
		ret |= hm_pacga_mem_check((void *)&b_cred->nsid_group, sizeof(struct nsid_group),
					  b_cred->idx, b_cred->pac[SEC_PAC_BASE_NSID_GROUP], BID, retry);
		break;
	case SEC_PAC_BASE_ENCAPS:
		ret |= hm_pacga_encaps_check(b_cred, retry);
		break;
	default:
		break;
	}
	return ret;
}

static inline __always_inline void hm_pacdb_bcred(struct base_cred *b_cred, enum sec_pac_base_id BID)
{
	switch (BID) {
	case SEC_PAC_BASE_SHIDX:
		b_cred->pac[SEC_PAC_BASE_SHIDX] = hm_pacdb(b_cred->sh_idx, b_cred->idx);
		break;
	case SEC_PAC_BASE_DAC:
		b_cred->pac[SEC_PAC_BASE_DAC] = hm_pacdb_mem((void *)&b_cred->cred_dac,
							     sizeof(struct sec_cred_dac), b_cred->idx);
		break;
	case SEC_PAC_BASE_SEHARMONY:
		b_cred->pac[SEC_PAC_BASE_SEHARMONY] = hm_pacdb_mem((void *)&b_cred->cred_sel,
								   sizeof(struct sec_cred_seharmony), b_cred->idx);
		break;
	case SEC_PAC_BASE_CAPS:
		b_cred->pac[SEC_PAC_BASE_CAPS] = hm_pacdb_mem((void *)&b_cred->cred_caps,
							      sizeof(struct sec_cred_caps), b_cred->idx);
		break;
	case SEC_PAC_BASE_KEYRING:
		b_cred->pac[SEC_PAC_BASE_KEYRING] = hm_pacdb_mem((void *)&b_cred->cred_key,
								 sizeof(struct sec_cred_keyring), b_cred->idx);
		break;
	case SEC_PAC_BASE_NSID_GROUP:
		b_cred->pac[SEC_PAC_BASE_NSID_GROUP] = hm_pacdb_mem((void *)&b_cred->nsid_group,
								    sizeof(struct nsid_group), b_cred->idx);
		break;
	case SEC_PAC_BASE_ENCAPS:
		b_cred->pac[SEC_PAC_BASE_ENCAPS] = hm_pacdb_encaps(b_cred);
		break;
	default:
		break;
	}
}

static inline __always_inline int hm_pacdb_bcred_check(const struct base_cred *b_cred,
	enum sec_pac_base_id BID, unsigned long long modifier, bool retry)
{
	int ret = E_HM_OK;
	switch (BID) {
	case SEC_PAC_BASE_SHIDX:
		ret = hm_pacdb_check(b_cred->sh_idx, modifier, b_cred->pac[SEC_PAC_BASE_SHIDX], retry);
		break;
	case SEC_PAC_BASE_DAC:
		ret = hm_pacdb_mem_check((void *)&b_cred->cred_dac, sizeof(struct sec_cred_dac),
					 modifier, b_cred->pac[SEC_PAC_BASE_DAC], BID, retry);
		break;
	case SEC_PAC_BASE_SEHARMONY:
		ret = hm_pacdb_mem_check((void *)&b_cred->cred_sel, sizeof(struct sec_cred_seharmony),
					 modifier, b_cred->pac[SEC_PAC_BASE_SEHARMONY], BID, retry);
		break;
	case SEC_PAC_BASE_CAPS:
		ret = hm_pacdb_mem_check((void *)&b_cred->cred_caps, sizeof(struct sec_cred_caps),
					 modifier, b_cred->pac[SEC_PAC_BASE_CAPS], BID, retry);
		break;
	case SEC_PAC_BASE_KEYRING:
		ret = hm_pacdb_mem_check((void *)&b_cred->cred_key, sizeof(struct sec_cred_keyring),
					 modifier, b_cred->pac[SEC_PAC_BASE_KEYRING], BID, retry);
		break;
	case SEC_PAC_BASE_NSID_GROUP:
		ret = hm_pacdb_mem_check((void *)&b_cred->nsid_group, sizeof(struct nsid_group),
					 modifier, b_cred->pac[SEC_PAC_BASE_NSID_GROUP], BID, retry);
		break;
	case SEC_PAC_BASE_ENCAPS:
		ret = hm_pacdb_encaps_check(b_cred, retry);
		break;
	default:
		break;
	}
	return ret;
}

static inline __always_inline void hm_pac_bcred_check_nolock(const struct base_cred *b_cred,
	enum sec_pac_base_id BID)
{
	hm_pacga_bcred_check(b_cred, SEC_PAC_BASE_IDX, PAC_CHECK_NO_RETRY);
	hm_pacdb_bcred_check(b_cred, BID, (b_cred)->idx, PAC_CHECK_NO_RETRY);
}

static inline __always_inline void hm_pacga_shcred(struct shared_cred *sh_cred, enum sec_pac_shared_id BID)
{
	switch (BID) {
	case SEC_PAC_SHARED_IDX:
		sh_cred->pac[SEC_PAC_SHARED_IDX] = hm_pacga(sh_cred->idx, (__u64)&sh_cred->idx);
		break;
	case SEC_PAC_SHARED_MAC:
		sh_cred->pac[SEC_PAC_SHARED_MAC] = hm_pacga_mem((void *)&sh_cred->cred_mac,
								sizeof(struct sec_cred_mac), sh_cred->idx);
		break;
	case SEC_PAC_SHARED_BASIC:
		sh_cred->pac[SEC_PAC_SHARED_BASIC] = hm_pacga_mem((void *)&sh_cred->cred_basic,
								  sizeof(struct sec_cred_basic), sh_cred->idx);
		break;
	case SEC_PAC_SHARED_PROPERTY:
		sh_cred->pac[SEC_PAC_SHARED_PROPERTY] = hm_pacga_mem((void *)&sh_cred->prop,
								     sizeof(struct sec_cred_property), sh_cred->idx);
		break;
	case SEC_PAC_SHARED_AUDIT:
		sh_cred->pac[SEC_PAC_SHARED_AUDIT] = hm_pacga_mem((void *)&sh_cred->cred_audit,
								  sizeof(struct sec_cred_audit), sh_cred->idx);
		break;
	default:
		break;
	}
}

static inline __always_inline int hm_pacga_shcred_check(const struct shared_cred *sh_cred,
							enum sec_pac_shared_id BID, bool retry)
{
	int ret = E_HM_OK;
	switch (BID) {
	case SEC_PAC_SHARED_IDX:
		ret |= hm_pacga_check(sh_cred->idx, (__u64)&sh_cred->idx, sh_cred->pac[SEC_PAC_SHARED_IDX], retry);
		break;
	case SEC_PAC_SHARED_MAC:
		ret |= hm_pacga_mem_check((void *)&sh_cred->cred_mac, sizeof(struct sec_cred_mac),
					  sh_cred->idx, sh_cred->pac[SEC_PAC_SHARED_MAC], (BID | 0xF0), retry);
		break;
	case SEC_PAC_SHARED_BASIC:
		ret |= hm_pacga_mem_check((void *)&sh_cred->cred_basic, sizeof(struct sec_cred_basic),
					  sh_cred->idx, sh_cred->pac[SEC_PAC_SHARED_BASIC], (BID | 0xF0), retry);
		break;
	case SEC_PAC_SHARED_PROPERTY:
		ret |= hm_pacga_mem_check((void *)&sh_cred->prop, sizeof(struct sec_cred_property),
					  sh_cred->idx, sh_cred->pac[SEC_PAC_SHARED_PROPERTY], (BID | 0xF0), retry);
		break;
	case SEC_PAC_SHARED_AUDIT:
		ret |= hm_pacga_mem_check((void *)&sh_cred->cred_audit, sizeof(struct sec_cred_audit),
					  sh_cred->idx, sh_cred->pac[SEC_PAC_SHARED_AUDIT], (BID | 0xF0), retry);
		break;
	default:
		break;
	}
	return ret;
}

static inline __always_inline void hm_pacdb_shcred(struct shared_cred *sh_cred, enum sec_pac_shared_id BID)
{
	switch (BID) {
	case SEC_PAC_SHARED_MAC:
		sh_cred->pac[SEC_PAC_SHARED_MAC] = hm_pacdb_mem((void *)&sh_cred->cred_mac,
								sizeof(struct sec_cred_mac), sh_cred->idx);
		break;
	case SEC_PAC_SHARED_BASIC:
		sh_cred->pac[SEC_PAC_SHARED_BASIC] = hm_pacdb_mem((void *)&sh_cred->cred_basic,
								  sizeof(struct sec_cred_basic), sh_cred->idx);
		break;
	case SEC_PAC_SHARED_PROPERTY:
		sh_cred->pac[SEC_PAC_SHARED_PROPERTY] = hm_pacdb_mem((void *)&sh_cred->prop,
								     sizeof(struct sec_cred_property), sh_cred->idx);
		break;
	case SEC_PAC_SHARED_AUDIT:
		sh_cred->pac[SEC_PAC_SHARED_AUDIT] = hm_pacdb_mem((void *)&sh_cred->cred_audit,
								  sizeof(struct sec_cred_audit), sh_cred->idx);
		break;
	default:
		break;
	}
}

static inline __always_inline int hm_pacdb_shcred_check(const struct shared_cred *sh_cred,
	enum sec_pac_shared_id BID, unsigned long long modifier, bool retry)
{
	int ret = E_HM_OK;
	switch (BID) {
	case SEC_PAC_SHARED_MAC:
		ret = hm_pacdb_mem_check((void *)&sh_cred->cred_mac, sizeof(struct sec_cred_mac),
					 modifier, sh_cred->pac[SEC_PAC_SHARED_MAC], BID, retry);
		break;
	case SEC_PAC_SHARED_BASIC:
		ret = hm_pacdb_mem_check((void *)&sh_cred->cred_basic, sizeof(struct sec_cred_basic),
					 modifier, sh_cred->pac[SEC_PAC_SHARED_BASIC], BID, retry);
		break;
	case SEC_PAC_SHARED_PROPERTY:
		ret = hm_pacdb_mem_check((void *)&sh_cred->prop, sizeof(struct sec_cred_property),
					 modifier, sh_cred->pac[SEC_PAC_SHARED_PROPERTY], BID, retry);
		break;
	case SEC_PAC_SHARED_AUDIT:
		ret = hm_pacdb_mem_check((void *)&sh_cred->cred_audit, sizeof(struct sec_cred_audit),
					 modifier, sh_cred->pac[SEC_PAC_SHARED_AUDIT], BID, retry);
		break;
	default:
		break;
	}
	return ret;
}

static inline __always_inline int hm_pac_shared_bcred_check(const struct sec_chk_cred *cred,
							    enum sec_pac_base_id BID, bool retry)
{
	int ret = E_HM_OK;
	struct base_cred *b_cred = (struct base_cred *)(cred->base_cred);
	if (cred->local_mem_flag == LOCAL_CRED_FLAG) {
		// local
		ret = hm_pacga_bcred_check(b_cred, BID, retry);
	} else {
		// shared_mem
		ret = hm_pacdb_bcred_check(b_cred, BID, cred->cred_idx, retry);
	}
	return ret;
}

static inline __always_inline int hm_pac_shcred_check(const struct sec_chk_cred *cred,
						      enum sec_pac_shared_id BID, bool retry)
{
	int ret = E_HM_OK;
	struct shared_cred *sh_cred = (struct shared_cred *)(cred->cred);
	struct base_cred *b_cred = (struct base_cred *)(cred->base_cred);
	if (cred->local_mem_flag == LOCAL_CRED_FLAG) {
		// local
		ret = hm_pacga_shcred_check(sh_cred, BID, retry);
	} else {
		// shared_mem
		ret = hm_pacdb_shcred_check(sh_cred, BID, b_cred->sh_idx, retry);
	}
	return ret;
}
#else
static inline __always_inline void hm_pacga_bcred_mem(struct base_cred *b_cred)
{
	UNUSED(b_cred);
}

static inline __always_inline void hm_pacdb_bcred_mem(struct base_cred *b_cred)
{
	UNUSED(b_cred);
}

static inline __always_inline int hm_pacdb_bcred_mem_check(const struct base_cred *b_cred,
							   unsigned long long modifier, bool retry)
{
	UNUSED(b_cred, modifier, retry);
	return E_HM_OK;
}

static inline __always_inline void hm_pacga_shcred_mem(struct shared_cred *sh_cred)
{
	UNUSED(sh_cred);
}

static inline __always_inline void hm_pacdb_shcred_mem(struct shared_cred *sh_cred)
{
	UNUSED(sh_cred);
}

static inline __always_inline int hm_pacdb_shcred_mem_check(struct shared_cred *sh_cred,
							    unsigned long long modifier, bool retry)
{
	UNUSED(sh_cred, modifier, retry);
	return E_HM_OK;
}

static inline __always_inline void hm_pac_bcred_check_nolock(struct base_cred *b_cred,
	enum sec_pac_base_id BID)
{
	UNUSED(b_cred, BID);
}

static inline __always_inline void hm_pacga_bcred(struct base_cred *b_cred, enum sec_pac_base_id BID)
{
	UNUSED(b_cred, BID);
}

static inline __always_inline int hm_pacga_bcred_check(const struct base_cred *b_cred,
						       enum sec_pac_base_id BID, bool retry)
{
	UNUSED(b_cred, BID, retry);
	return E_HM_OK;
}

static inline __always_inline void hm_pacdb_bcred(struct base_cred *b_cred, enum sec_pac_base_id BID)
{
	UNUSED(b_cred, BID);
}

static inline __always_inline int hm_pacdb_bcred_check(const struct base_cred *b_cred,
	enum sec_pac_base_id BID, unsigned long long modifier, bool retry)
{
	UNUSED(b_cred, BID, modifier, retry);
	return E_HM_OK;
}

static inline __always_inline void hm_pacga_shcred(struct shared_cred *sh_cred, enum sec_pac_shared_id BID)
{
	UNUSED(sh_cred, BID);
}

static inline __always_inline int hm_pacga_shcred_check(const struct shared_cred *sh_cred,
							enum sec_pac_shared_id BID, bool retry)
{
	UNUSED(sh_cred, BID, retry);
	return E_HM_OK;
}

static inline __always_inline void hm_pacdb_shcred(struct shared_cred *sh_cred, enum sec_pac_shared_id BID)
{
	UNUSED(sh_cred, BID);
}

static inline __always_inline int hm_pacdb_shcred_check(const struct shared_cred *sh_cred,
	enum sec_pac_shared_id BID, unsigned long long modifier, bool retry)
{
	UNUSED(sh_cred, BID, modifier, retry);
	return E_HM_OK;
}

static inline __always_inline int hm_pac_shared_bcred_check(const struct sec_chk_cred *cred,
							    enum sec_pac_base_id BID, bool retry)
{
	UNUSED(cred, BID, retry);
	return E_HM_OK;
}

static inline __always_inline int hm_pac_shcred_check(const struct sec_chk_cred *cred,
						      enum sec_pac_shared_id BID, bool retry)
{
	UNUSED(cred, BID, retry);
	return E_HM_OK;
}
#endif

#if defined(ENABLE_PAC_HARD_CODE) && defined(PAC_DFI_SHARED_WITH_LOCK)
static inline __always_inline void base_pac_seq_lock_init(struct base_cred *b_cred)
{
	b_cred->pac_seq_lock[SEC_PAC_BASE_DAC] = 0;
	b_cred->pac_seq_lock[SEC_PAC_BASE_SEHARMONY] = 0;
	b_cred->pac_seq_lock[SEC_PAC_BASE_CAPS] = 0;
	b_cred->pac_seq_lock[SEC_PAC_BASE_KEYRING] = 0;
	b_cred->pac_seq_lock[SEC_PAC_BASE_NSID_GROUP] = 0;
	b_cred->pac_seq_lock[SEC_PAC_BASE_ENCAPS] = 0;
}

static inline __always_inline void shared_pac_seq_lock_init(struct shared_cred *shcred)
{
	UNUSED(shcred);
}

static inline __always_inline void str_pac_seq_lock_init(struct str_cred *str_cred)
{
	UNUSED(str_cred);
}

static inline __always_inline void hm_pac_bcred_seq_write_start(struct base_cred *b_cred,
	enum sec_pac_base_id BID)
{
	seq_write_start(&b_cred->pac_seq_lock[BID]);
}

static inline __always_inline void hm_pac_bcred_seq_write_end(struct base_cred *b_cred,
	enum sec_pac_base_id BID)
{
	seq_write_end(&b_cred->pac_seq_lock[BID]);
}

static inline __always_inline seq_t hm_pac_bcred_seq_try(struct base_cred *b_cred,
	enum sec_pac_base_id BID)
{
	return seq_try(&b_cred->pac_seq_lock[BID]);
}

static inline __always_inline bool hm_pac_bcred_seq_need_retry(struct base_cred *b_cred,
	enum sec_pac_base_id BID, seq_t ctx_seq)
{
	return seq_need_retry(&b_cred->pac_seq_lock[BID], ctx_seq);
}

static inline __always_inline void hm_pac_bcred_check_with_lock(struct base_cred *b_cred,
        enum sec_pac_base_id BID)
{
	int rc = E_HM_OK;
	seq_t ctx_seq;
	do {
		ctx_seq = hm_pac_bcred_seq_try(b_cred, BID);
		// Allow retry here if failed and check seq
		rc = hm_pacga_bcred_check(b_cred, SEC_PAC_BASE_IDX, PAC_CHECK_ALLOW_RETRY);
		rc = hm_pacdb_bcred_check(b_cred, BID, (b_cred)->idx, PAC_CHECK_ALLOW_RETRY);
	} while (hm_pac_bcred_seq_need_retry(b_cred, BID, ctx_seq));

	// cred check failed
	if (rc != E_HM_OK) {
		_hm_pac_fault();
	}
}

static inline __always_inline void hm_pac_shared_bcred_check_with_lock(const struct sec_chk_cred *cred,
	enum sec_pac_base_id BID)
{
	int rc = E_HM_OK;
	seq_t ctx_seq;
	do {
		ctx_seq = hm_pac_bcred_seq_try((struct base_cred *)(cred->base_cred), BID);
		// Allow retry here if failed and check seq
		hm_pac_shared_bcred_check(cred, BID, PAC_CHECK_ALLOW_RETRY);
	} while (hm_pac_bcred_seq_need_retry((struct base_cred *)(cred->base_cred), BID, ctx_seq));

	// cred check failed
	if (rc != E_HM_OK) {
		_hm_pac_fault();
	}
}
#else

static inline __always_inline void base_pac_seq_lock_init(struct base_cred *b_cred)
{
	UNUSED(b_cred);
}

static inline __always_inline void shared_pac_seq_lock_init(struct shared_cred *shcred)
{
	UNUSED(shcred);
}

static inline __always_inline void str_pac_seq_lock_init(struct str_cred *str_cred)
{
	UNUSED(str_cred);
}

static inline __always_inline void hm_pac_bcred_seq_write_start(struct base_cred *b_cred,
	enum sec_pac_base_id BID)
{
	UNUSED(b_cred, BID);
}

static inline __always_inline void hm_pac_bcred_seq_write_end(struct base_cred *b_cred,
        enum sec_pac_base_id BID)
{
	UNUSED(b_cred, BID);
}

#endif

static inline void sec_cred_idx_to_cred(cred_idx_t idx, void **b_cred,
					void **sh_cred)
{
	struct base_cred *tmp_b_cred = sec_cred_idx_to_ptr(idx);
#ifdef ENABLE_PAC_HARD_CODE
	hm_pacdb_bcred_check(tmp_b_cred, SEC_PAC_BASE_SHIDX, idx, PAC_CHECK_NO_RETRY);
#endif
	struct shared_cred *tmp_sh_cred = sec_cred_idx_to_ptr(tmp_b_cred->sh_idx);
	*b_cred = tmp_b_cred;
	*sh_cred = tmp_sh_cred;
}
/* api using cred cache */
static inline void sec_cred_idx_to_src_cred(cred_idx_t idx, void **b_cred, void **sh_cred)
{
	if (sysif_callinfo.cred_flags.src_base_cred_valid == SEC_CRED_VALID) {
		*b_cred = sysif_cred_cache.src_base_cred;
		*sh_cred = sysif_cred_cache.src_shared_cred;
		return;
	}
	sec_cred_idx_to_cred(idx, b_cred, sh_cred);
	update_src_cred_cache(*b_cred, *sh_cred);
	sysif_callinfo.cred_flags.src_base_cred_valid = SEC_CRED_VALID;
}

/* api using cred cache */
static inline void sec_cred_idx_to_fwd_cred(cred_idx_t idx, void **b_cred, void **sh_cred)
{
	if (sysif_callinfo.cred_flags.fwd_base_cred_valid == SEC_CRED_VALID) {
		*b_cred = sysif_cred_cache.fwd_base_cred;
		*sh_cred = sysif_cred_cache.fwd_shared_cred;
		return;
	}
	sec_cred_idx_to_cred(idx, b_cred, sh_cred);
	update_fwd_cred_cache(*b_cred, *sh_cred);
	sysif_callinfo.cred_flags.fwd_base_cred_valid = SEC_CRED_VALID;
}

static inline void sec_cred_idx_to_fwd_cred_simple(cred_idx_t idx, void **b_cred)
{
	sec_cred_idx_to_cred_simple(idx, b_cred);
}

static inline void sec_cred_idx_to_src_cred_simple(cred_idx_t idx, void **b_cred, void **sh_cred)
{
	sec_cred_idx_to_cred(idx, b_cred, sh_cred);
}
#endif
