/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Descrption:
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 30 22:19:56 2023
 */
#ifndef MMU_PGTBLA64S2_H
#define MMU_PGTBLA64S2_H

#include <hmasm/pgtbla64s2.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include <intapi/mem/linear_map.h>

enum {
	PGTBLA64S2_MAPPING_CFG_RODATA_HISI,
	PGTBLA64S2_MAPPING_CFG_RODATA_NORMAL,
	PGTBLA64S2_MAPPING_CFG_DATAS2_HISI,
	PGTBLA64S2_MAPPING_CFG_DATAS2_NORMAL,
	PGTBLA64S2_MAPPING_CFG_KUWCODES2_HISI,
	PGTBLA64S2_MAPPING_CFG_KUWCODES2_NORMAL,
	MAX_PGTBLA64S2_MAPPING_CFG,
};

int mmu_pgtbla64s2_map(uint64_t pgd,
		       unsigned long ipa, uint64_t pa, size_t size,
		       uint32_t mapping_cfg_id);

int mmu_pgtbla64s2_unmap(uint64_t pgd,
			 unsigned long ipa, size_t size, uint32_t mapping_cfg_id);

uint64_t mmu_pgtbla64s2_ipa_to_pa(uint64_t pgd,
				  unsigned long ipa, uint32_t mapping_cfg_id);

int mmu_pgtbla64s2_shrink(uint64_t pgd, uint32_t mapping_cfg_id, unsigned long *ipa_stop);

uint64_t mmu_pgtbla64s2_create_pgd(uint32_t mapping_cfg_id);

void mmu_pgtbla64s2_dump_pte(uint64_t pgd, unsigned long ipa, uint32_t mapping_cfg_id);

int mmu_pgtbla64s2_map_bestfit(unsigned long pgd, unsigned long ipa, uint64_t pa, size_t size, uint32_t mapping_cfg_id);

#define DUMP_ARGS_LIST(...)	, ##__VA_ARGS__
#define DUMP_PARAMS_LIST(...)	, ##__VA_ARGS__
#define DUMP_PARAMS_LOCAL(...)	__VA_ARGS__
#define DUMP_PARAMS_LOCAL_ALL	DUMP_PARAMS_LOCAL(lparams.t, lparams.cfg, lparams.e, lparams.st, lparams.ed)

#define DEFINE_mmu_pgtbla64s2_bad(lv, func, args, local, params) \
static inline __always_inline int mmu_pgtbla64s2_##func##_bad(enum __pgtblhdlr_pgtbla64s2_pgentry_type_e t, \
							      const struct __pgtblhdlr_pgtbla64s2_pgucfg_s *cfg, \
							      struct __pgtblhdlr_pgtbla64s2_pgentry_s e, \
							      uint64_t st, uint64_t ed args) \
{ \
	panic("impossible\n"); \
	UNUSED(t, cfg, e, st, ed params); \
	return false; \
}

#define DEFINE_mmu_pgtbla64s2_iter(lv, nlv, func, args, local, params) \
static inline __always_inline int mmu_pgtbla64s2_##func##_##lv(enum __pgtblhdlr_pgtbla64s2_pgentry_type_e t, \
							       const struct __pgtblhdlr_pgtbla64s2_pgucfg_s *cfg, \
							       struct __pgtblhdlr_pgtbla64s2_pgentry_s e, \
							       uint64_t st, uint64_t ed args) \
{ \
	__pgtblhdlr_a64s2_rawfmt_base_t *ptr = __pgtblhdlr_pgtbla64s2_pgentry_to_ptr(e); \
	uint64_t ipa = st; \
	int rc = E_HM_OK; \
	__pgtblhdlr_a64s2_rawfmt_base_t *tb = NULL; \
 \
	struct { \
		enum __pgtblhdlr_pgtbla64s2_pgentry_type_e t; \
		const struct __pgtblhdlr_pgtbla64s2_pgucfg_s *cfg; \
		struct __pgtblhdlr_pgtbla64s2_pgentry_s e; \
		uint64_t st; \
		uint64_t ed; \
	} lparams = { \
		.t	= t, \
		.cfg	= cfg, \
		.e	= e, \
		.st	= st, \
		.ed	= ed, \
	}; \
 \
	rc = func(local params); \
	if (__pgtblhdlr_pgtbla64s2_pgentry_is_valid(t, e)) { \
		tb = (__pgtblhdlr_a64s2_rawfmt_base_t *)pa_to_sys_va( \
			__pgtblhdlr_pgtbla64s2_pgentry_decode_val(t, cfg, ptr[0])); \
	} \
	while ((rc == E_HM_OK) && (ipa < ed) && (tb != NULL)) { \
		unsigned long idx = __pgtblhdlr_pgtbla64s2_pgentry_tblidx_from_key(t, cfg, ipa); \
		enum __pgtblhdlr_pgtbla64s2_pgentry_type_e nt; \
		struct __pgtblhdlr_pgtbla64s2_pgentry_s ne; \
		uint64_t _ed; \
		nt = __pgtblhdlr_pgtbla64s2_pgentry_lookup_nextlv(cfg, &ne, t, tb, idx); \
		if (nt == __PGTBLHDLR_PGTBLA64S2_PGENTRY_TYPE_INVALID) { \
			nt = __pgtblhdlr_pgtbla64s2_pgentry_choose_nextlv_tblfit(t, cfg); \
			ne = __pgtblhdlr_pgtbla64s2_pgentry_from_table(nt, tb, idx); \
		} \
		_ed = __pgtblhdlr_pgtbla64s2_key_next(t, cfg, ipa, ed); \
		if (__pgtblhdlr_pgtbla64s2_pgentry_is_lastlv(nt, cfg)) { \
			lparams.t	= nt; \
			lparams.e	= ne; \
			lparams.st	= ipa; \
			lparams.ed	= _ed; \
			rc = func(local params); \
		} else { \
			rc = mmu_pgtbla64s2_##func##_##nlv(nt, cfg, ne, ipa, _ed params); \
		} \
		ipa = _ed; \
	} \
	UNUSED(lparams); \
	return rc; \
}

#endif
