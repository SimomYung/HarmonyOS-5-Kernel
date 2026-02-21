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

static inline __always_inline void hm_pacga_cred_encaps(struct cred_encaps *encaps)
{
	UNUSED(encaps);
}

static inline __always_inline void hm_pacga_cred_encaps_check(struct cred_encaps *encaps)
{
	UNUSED(encaps);
}

static inline __always_inline void hm_pacga_encaps_cred(struct sec_cred_encaps *encaps_cred)
{
	UNUSED(encaps_cred);
}

static inline __always_inline void hm_pacga_encaps_cred_check(struct sec_cred_encaps *encaps_cred)
{
	UNUSED(encaps_cred);
}

static inline __always_inline void hm_pacga_bcred_mem(struct base_cred *b_cred)
{
	b_cred->pac[SEC_PAC_BASE_IDX] = hm_pacga(b_cred->idx, &b_cred->idx);
	b_cred->pac[SEC_PAC_BASE_SHIDX] = hm_pacga(b_cred->sh_idx, b_cred->idx);
	b_cred->pac[SEC_PAC_BASE_DAC] = hm_pacga_mem(&b_cred->cred_dac, sizeof(struct sec_cred_dac), b_cred->idx);
	b_cred->pac[SEC_PAC_BASE_SEHARMONY] = hm_pacga_mem(&b_cred->cred_sel, sizeof(struct sec_cred_seharmony), b_cred->idx);
	b_cred->pac[SEC_PAC_BASE_CAPS] = hm_pacga_mem(&b_cred->cred_caps, sizeof(struct sec_cred_caps), b_cred->idx);
	b_cred->pac[SEC_PAC_BASE_KEYRING] = hm_pacga_mem(&b_cred->cred_key, sizeof(struct sec_cred_keyring), b_cred->idx);
	b_cred->pac[SEC_PAC_BASE_NSID_GROUP] = hm_pacga_mem(&b_cred->nsid_group, sizeof(struct nsid_group), b_cred->idx);
}

static inline __always_inline void hm_pacdb_bcred_mem(struct base_cred *b_cred)
{
	b_cred->pac[SEC_PAC_BASE_SHIDX] = hm_pacdb(b_cred->sh_idx, b_cred->idx);
	b_cred->pac[SEC_PAC_BASE_DAC] = hm_pacdb_mem(&b_cred->cred_dac, sizeof(struct sec_cred_dac), b_cred->idx);
	b_cred->pac[SEC_PAC_BASE_SEHARMONY] = hm_pacdb_mem(&b_cred->cred_sel, sizeof(struct sec_cred_seharmony), b_cred->idx);
	b_cred->pac[SEC_PAC_BASE_CAPS] = hm_pacdb_mem(&b_cred->cred_caps, sizeof(struct sec_cred_caps), b_cred->idx);
	b_cred->pac[SEC_PAC_BASE_KEYRING] = hm_pacdb_mem(&b_cred->cred_key, sizeof(struct sec_cred_keyring), b_cred->idx);
	b_cred->pac[SEC_PAC_BASE_NSID_GROUP] = hm_pacdb_mem(&b_cred->nsid_group, sizeof(struct nsid_group), b_cred->idx);
}

static inline __always_inline void hm_pacdb_bcred_mem_check(const struct base_cred *b_cred, unsigned long long modifier)
{
	hm_pacdb_check(b_cred->sh_idx, modifier, b_cred->pac[SEC_PAC_BASE_SHIDX]);
	hm_pacdb_mem_check(&b_cred->cred_dac, sizeof(struct sec_cred_dac),
			   modifier, b_cred->pac[SEC_PAC_BASE_DAC], SEC_PAC_BASE_DAC);
	hm_pacdb_mem_check(&b_cred->cred_sel, sizeof(struct sec_cred_seharmony),
			   modifier, b_cred->pac[SEC_PAC_BASE_SEHARMONY], SEC_PAC_BASE_SEHARMONY);
	hm_pacdb_mem_check(&b_cred->cred_caps, sizeof(struct sec_cred_caps),
			   modifier, b_cred->pac[SEC_PAC_BASE_CAPS], SEC_PAC_BASE_CAPS);
	hm_pacdb_mem_check(&b_cred->cred_key, sizeof(struct sec_cred_keyring),
			   modifier, b_cred->pac[SEC_PAC_BASE_KEYRING], SEC_PAC_BASE_KEYRING);
	hm_pacdb_mem_check(&b_cred->nsid_group, sizeof(struct nsid_group), modifier,
			   b_cred->pac[SEC_PAC_BASE_NSID_GROUP], SEC_PAC_BASE_NSID_GROUP);
}

static inline __always_inline void hm_pacga_shcred_mem(struct shared_cred *sh_cred)
{
	sh_cred->pac[SEC_PAC_SHARED_MAC] = hm_pacga_mem(&sh_cred->cred_mac, sizeof(struct sec_cred_mac), sh_cred->idx);
	sh_cred->pac[SEC_PAC_SHARED_BASIC] = hm_pacga_mem(&sh_cred->cred_basic, sizeof(struct sec_cred_basic), sh_cred->idx);
	sh_cred->pac[SEC_PAC_SHARED_PROPERTY] = hm_pacga_mem(&sh_cred->prop, sizeof(struct sec_cred_property), sh_cred->idx);
	sh_cred->pac[SEC_PAC_SHARED_AUDIT] = hm_pacga_mem(&sh_cred->cred_audit, sizeof(struct sec_cred_audit), sh_cred->idx);
}

static inline __always_inline void hm_pacdb_shcred_mem(struct shared_cred *sh_cred)
{
	sh_cred->pac[SEC_PAC_SHARED_MAC] = hm_pacdb_mem(&sh_cred->cred_mac, sizeof(struct sec_cred_mac), sh_cred->idx);
	sh_cred->pac[SEC_PAC_SHARED_BASIC] = hm_pacdb_mem(&sh_cred->cred_basic, sizeof(struct sec_cred_basic), sh_cred->idx);
	sh_cred->pac[SEC_PAC_SHARED_PROPERTY] = hm_pacdb_mem(&sh_cred->prop, sizeof(struct sec_cred_property), sh_cred->idx);
	sh_cred->pac[SEC_PAC_SHARED_AUDIT] = hm_pacdb_mem(&sh_cred->cred_audit, sizeof(struct sec_cred_audit), sh_cred->idx);
}

static inline __always_inline void hm_pacdb_shcred_mem_check(struct shared_cred *sh_cred, unsigned long long modifier)
{
	hm_pacdb_mem_check(&sh_cred->cred_mac, sizeof(struct sec_cred_mac),
		modifier, sh_cred->pac[SEC_PAC_SHARED_MAC], SEC_PAC_SHARED_MAC);
	hm_pacdb_mem_check(&sh_cred->cred_basic, sizeof(struct sec_cred_basic),
		modifier, sh_cred->pac[SEC_PAC_SHARED_BASIC], SEC_PAC_SHARED_BASIC);
	hm_pacdb_mem_check(&sh_cred->prop, sizeof(struct sec_cred_property), modifier,
		sh_cred->pac[SEC_PAC_SHARED_PROPERTY], SEC_PAC_SHARED_PROPERTY);
	hm_pacdb_mem_check(&sh_cred->cred_audit, sizeof(struct sec_cred_audit),
		modifier, sh_cred->pac[SEC_PAC_SHARED_AUDIT], SEC_PAC_SHARED_AUDIT);
}

static inline __always_inline void hm_pacga_bcred(struct base_cred *b_cred, enum sec_pac_base_id BID)
{
	switch (BID) {
	case SEC_PAC_BASE_IDX:
		b_cred->pac[SEC_PAC_BASE_IDX] = hm_pacga(b_cred->idx, &b_cred->idx);
		break;
	case SEC_PAC_BASE_DAC:
		b_cred->pac[SEC_PAC_BASE_DAC] = hm_pacga_mem(&b_cred->cred_dac, sizeof(struct sec_cred_dac), b_cred->idx);
		break;
	case SEC_PAC_BASE_SEHARMONY:
		b_cred->pac[SEC_PAC_BASE_SEHARMONY] = hm_pacga_mem(&b_cred->cred_sel, sizeof(struct sec_cred_seharmony), b_cred->idx);
		break;
	case SEC_PAC_BASE_CAPS:
		b_cred->pac[SEC_PAC_BASE_CAPS] = hm_pacga_mem(&b_cred->cred_caps, sizeof(struct sec_cred_caps), b_cred->idx);
		break;
	case SEC_PAC_BASE_KEYRING:
		b_cred->pac[SEC_PAC_BASE_KEYRING] = hm_pacga_mem(&b_cred->cred_key, sizeof(struct sec_cred_keyring), b_cred->idx);
		break;
	case SEC_PAC_BASE_NSID_GROUP:
		b_cred->pac[SEC_PAC_BASE_NSID_GROUP] = hm_pacga_mem(&b_cred->nsid_group, sizeof(struct nsid_group), b_cred->idx);
		break;
	}
}

static inline __always_inline void hm_pacga_bcred_check(const struct base_cred *b_cred, enum sec_pac_base_id BID)
{
	/* check the modifier */
	hm_pacga_check(b_cred->idx, &b_cred->idx, b_cred->pac[SEC_PAC_BASE_IDX]);

	switch (BID) {
	case SEC_PAC_BASE_DAC:
		hm_pacga_mem_check(&b_cred->cred_dac, sizeof(struct sec_cred_dac), \
						b_cred->idx, b_cred->pac[SEC_PAC_BASE_DAC], BID);
		break;
	case SEC_PAC_BASE_SEHARMONY:
		hm_pacga_mem_check(&b_cred->cred_sel, sizeof(struct sec_cred_seharmony), \
						b_cred->idx, b_cred->pac[SEC_PAC_BASE_SEHARMONY], BID);
		break;
	case SEC_PAC_BASE_CAPS:
		hm_pacga_mem_check(&b_cred->cred_caps, sizeof(struct sec_cred_caps), \
						b_cred->idx, b_cred->pac[SEC_PAC_BASE_CAPS], BID);
		break;
	case SEC_PAC_BASE_KEYRING:
		hm_pacga_mem_check(&b_cred->cred_key, sizeof(struct sec_cred_keyring), \
						b_cred->idx, b_cred->pac[SEC_PAC_BASE_KEYRING], BID);
		break;
	case SEC_PAC_BASE_NSID_GROUP:
		hm_pacga_mem_check(&b_cred->nsid_group, sizeof(struct nsid_group), \
						b_cred->idx, b_cred->pac[SEC_PAC_BASE_NSID_GROUP], BID);
		break;
	}
}

static inline __always_inline void hm_pacdb_bcred(struct base_cred *b_cred, enum sec_pac_base_id BID)
{
	switch (BID) {
	case SEC_PAC_BASE_SHIDX:
		b_cred->pac[SEC_PAC_BASE_SHIDX] = hm_pacdb(b_cred->sh_idx, b_cred->idx);
		break;
	case SEC_PAC_BASE_DAC:
		b_cred->pac[SEC_PAC_BASE_DAC] = hm_pacdb_mem(&b_cred->cred_dac, sizeof(struct sec_cred_dac), b_cred->idx);
		break;
	case SEC_PAC_BASE_SEHARMONY:
		b_cred->pac[SEC_PAC_BASE_SEHARMONY] = hm_pacdb_mem(&b_cred->cred_sel, sizeof(struct sec_cred_seharmony), b_cred->idx);
		break;
	case SEC_PAC_BASE_CAPS:
		b_cred->pac[SEC_PAC_BASE_CAPS] = hm_pacdb_mem(&b_cred->cred_caps, sizeof(struct sec_cred_caps), b_cred->idx);
		break;
	case SEC_PAC_BASE_KEYRING:
		b_cred->pac[SEC_PAC_BASE_KEYRING] = hm_pacdb_mem(&b_cred->cred_key, sizeof(struct sec_cred_keyring), b_cred->idx);
		break;
	case SEC_PAC_BASE_NSID_GROUP:
		b_cred->pac[SEC_PAC_BASE_NSID_GROUP] = hm_pacdb_mem(&b_cred->nsid_group, sizeof(struct nsid_group), b_cred->idx);
		break;
	}
}

static inline __always_inline void hm_pacdb_bcred_check(const struct base_cred *b_cred, enum sec_pac_base_id BID,
	unsigned long long modifier)
{
	switch (BID) {
	case SEC_PAC_BASE_SHIDX:
		hm_pacdb_check(b_cred->sh_idx, modifier, b_cred->pac[SEC_PAC_BASE_SHIDX]);
		break;
	case SEC_PAC_BASE_DAC:
		hm_pacdb_mem_check(&b_cred->cred_dac, sizeof(struct sec_cred_dac), \
						modifier, b_cred->pac[SEC_PAC_BASE_DAC], BID);
		break;
	case SEC_PAC_BASE_SEHARMONY:
		hm_pacdb_mem_check(&b_cred->cred_sel, sizeof(struct sec_cred_seharmony), \
						modifier, b_cred->pac[SEC_PAC_BASE_SEHARMONY], BID);
		break;
	case SEC_PAC_BASE_CAPS:
		hm_pacdb_mem_check(&b_cred->cred_caps, sizeof(struct sec_cred_caps), \
						modifier, b_cred->pac[SEC_PAC_BASE_CAPS], BID);
		break;
	case SEC_PAC_BASE_KEYRING:
		hm_pacdb_mem_check(&b_cred->cred_key, sizeof(struct sec_cred_keyring), \
						modifier, b_cred->pac[SEC_PAC_BASE_KEYRING], BID);
		break;
	case SEC_PAC_BASE_NSID_GROUP:
		hm_pacdb_mem_check(&b_cred->nsid_group, sizeof(struct nsid_group), \
						modifier, b_cred->pac[SEC_PAC_BASE_NSID_GROUP], BID);
		break;
	}
}

static inline __always_inline void hm_pacga_shcred(struct shared_cred *sh_cred, enum sec_pac_shared_id BID)
{
	switch (BID) {
	case SEC_PAC_SHARED_IDX:
		sh_cred->pac[SEC_PAC_SHARED_IDX] = hm_pacga(sh_cred->idx, &sh_cred->idx);
		break;
	case SEC_PAC_SHARED_MAC:
		sh_cred->pac[SEC_PAC_SHARED_MAC] = hm_pacga_mem(&sh_cred->cred_mac, sizeof(struct sec_cred_mac), sh_cred->idx);
		break;
	case SEC_PAC_SHARED_BASIC:
		sh_cred->pac[SEC_PAC_SHARED_BASIC] = hm_pacga_mem(&sh_cred->cred_basic, sizeof(struct sec_cred_basic), sh_cred->idx);
		break;
	case SEC_PAC_SHARED_PROPERTY:
		sh_cred->pac[SEC_PAC_SHARED_PROPERTY] = hm_pacga_mem(&sh_cred->prop, sizeof(struct sec_cred_property), sh_cred->idx);
		break;
	case SEC_PAC_SHARED_AUDIT:
		sh_cred->pac[SEC_PAC_SHARED_AUDIT] = hm_pacga_mem(&sh_cred->cred_audit, sizeof(struct sec_cred_audit), sh_cred->idx);
		break;
	}
}

static inline __always_inline void hm_pacga_shcred_check(const struct shared_cred *sh_cred, enum sec_pac_shared_id BID)
{
	switch (BID) {
	case SEC_PAC_SHARED_IDX:
		hm_pacga_check(sh_cred->idx, &sh_cred->idx, sh_cred->pac[SEC_PAC_SHARED_IDX]);
		break;
	case SEC_PAC_SHARED_MAC:
		hm_pacga_mem_check(&sh_cred->cred_mac, sizeof(struct sec_cred_mac), \
						sh_cred->idx, sh_cred->pac[SEC_PAC_SHARED_MAC], (BID | 0xF0));
		break;
	case SEC_PAC_SHARED_BASIC:
		hm_pacga_mem_check(&sh_cred->cred_basic, sizeof(struct sec_cred_basic), \
						sh_cred->idx, sh_cred->pac[SEC_PAC_SHARED_BASIC], (BID | 0xF0));
		break;
	case SEC_PAC_SHARED_PROPERTY:
		hm_pacga_mem_check(&sh_cred->prop, sizeof(struct sec_cred_property), \
						sh_cred->idx, sh_cred->pac[SEC_PAC_SHARED_PROPERTY], (BID | 0xF0));
		break;
	case SEC_PAC_SHARED_AUDIT:
		hm_pacga_mem_check(&sh_cred->cred_audit, sizeof(struct sec_cred_audit), \
						sh_cred->idx, sh_cred->pac[SEC_PAC_SHARED_AUDIT], (BID | 0xF0));
		break;
	}
}

static inline __always_inline void hm_pacdb_shcred(struct shared_cred *sh_cred, enum sec_pac_shared_id BID)
{
	switch (BID) {
	case SEC_PAC_SHARED_MAC:
		sh_cred->pac[SEC_PAC_SHARED_MAC] = hm_pacdb_mem(&sh_cred->cred_mac, sizeof(struct sec_cred_mac), sh_cred->idx);
		break;
	case SEC_PAC_SHARED_BASIC:
		sh_cred->pac[SEC_PAC_SHARED_BASIC] = hm_pacdb_mem(&sh_cred->cred_basic, sizeof(struct sec_cred_basic), sh_cred->idx);
		break;
	case SEC_PAC_SHARED_PROPERTY:
		sh_cred->pac[SEC_PAC_SHARED_PROPERTY] = hm_pacdb_mem(&sh_cred->prop, sizeof(struct sec_cred_property), sh_cred->idx);
		break;
	case SEC_PAC_SHARED_AUDIT:
		sh_cred->pac[SEC_PAC_SHARED_AUDIT] = hm_pacdb_mem(&sh_cred->cred_audit, sizeof(struct sec_cred_audit), sh_cred->idx);
		break;
	}
}

static inline __always_inline void hm_pacdb_shcred_check(const struct shared_cred *sh_cred, enum sec_pac_shared_id BID,
	unsigned long long modifier)
{
	switch (BID) {
	case SEC_PAC_SHARED_MAC:
		hm_pacdb_mem_check(&sh_cred->cred_mac, sizeof(struct sec_cred_mac), \
						modifier, sh_cred->pac[SEC_PAC_SHARED_MAC], BID);
		break;
	case SEC_PAC_SHARED_BASIC:
		hm_pacdb_mem_check(&sh_cred->cred_basic, sizeof(struct sec_cred_basic), \
						modifier, sh_cred->pac[SEC_PAC_SHARED_BASIC], BID);
		break;
	case SEC_PAC_SHARED_PROPERTY:
		hm_pacdb_mem_check(&sh_cred->prop, sizeof(struct sec_cred_property), \
						modifier, sh_cred->pac[SEC_PAC_SHARED_PROPERTY], BID);
		break;
	case SEC_PAC_SHARED_AUDIT:
		hm_pacdb_mem_check(&sh_cred->cred_audit, sizeof(struct sec_cred_audit), \
						modifier, sh_cred->pac[SEC_PAC_SHARED_AUDIT], BID);
		break;
	}
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

static inline __always_inline void hm_pacdb_bcred_mem_check(const struct base_cred *b_cred, unsigned long long modifier)
{
	UNUSED(b_cred, modifier);
}

static inline __always_inline void hm_pacga_shcred_mem(struct shared_cred *sh_cred)
{
	UNUSED(sh_cred);
}

static inline __always_inline void hm_pacdb_shcred_mem(struct shared_cred *sh_cred)
{
	UNUSED(sh_cred);
}

static inline __always_inline void hm_pacdb_shcred_mem_check(struct shared_cred *sh_cred, unsigned long long modifier)
{
	UNUSED(sh_cred, modifier);
}

static inline __always_inline void hm_pacga_bcred(struct base_cred *b_cred, enum sec_pac_base_id BID)
{
	UNUSED(b_cred, BID);
}

static inline __always_inline void hm_pacga_bcred_check(const struct base_cred *b_cred, enum sec_pac_base_id BID)
{
	UNUSED(b_cred, BID);
}

static inline __always_inline void hm_pacdb_bcred(struct base_cred *b_cred, enum sec_pac_base_id BID)
{
	UNUSED(b_cred, BID);
}

static inline __always_inline void hm_pacdb_bcred_check(const struct base_cred *b_cred, enum sec_pac_base_id BID,
	unsigned long long modifier)
{
	UNUSED(b_cred, BID, modifier);
}

static inline __always_inline void hm_pacga_shcred(struct shared_cred *sh_cred, enum sec_pac_shared_id BID)
{
	UNUSED(sh_cred, BID);
}

static inline __always_inline void hm_pacga_shcred_check(const struct shared_cred *sh_cred, enum sec_pac_shared_id BID)
{
	UNUSED(sh_cred, BID);
}

static inline __always_inline void hm_pacdb_shcred(struct shared_cred *sh_cred, enum sec_pac_shared_id BID)
{
	UNUSED(sh_cred, BID);
}

static inline __always_inline void hm_pacdb_shcred_check(const struct shared_cred *sh_cred, enum sec_pac_shared_id BID,
	unsigned long long modifier)
{
	UNUSED(sh_cred, BID, modifier);
}

static inline __always_inline void hm_pacga_cred_encaps(struct cred_encaps *encaps)
{
	UNUSED(encaps);
}

static inline __always_inline void hm_pacga_cred_encaps_check(struct cred_encaps *encaps)
{
	UNUSED(encaps);
}
#endif

static inline void sec_cred_idx_to_cred(cred_idx_t idx, void **b_cred,
					void **sh_cred)
{
	struct base_cred *tmp_b_cred = sec_cred_idx_to_ptr(idx);
#ifdef ENABLE_PAC_HARD_CODE
	hm_pacdb_bcred_check(tmp_b_cred, SEC_PAC_BASE_SHIDX, tmp_b_cred->idx);
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
