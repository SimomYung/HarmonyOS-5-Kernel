/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Amend arm64 page table shareable bits - mapi
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jan 29 09:59:42 2022
 */

#ifndef A64_MAPI_ASM_PGTBL_AMEND_SHAREABLE_H
#define A64_MAPI_ASM_PGTBL_AMEND_SHAREABLE_H

#include <hmasm/pgtbl/pgtblattr.h>

#ifndef AARCH64_INCLUDE_MAPI_ASM_POPULATE_H
# error "Should be included in mapi/hmasm/pgtbl/populate.h"
#endif

#define DEFINE_pgtblhdlr_rawcfg_amend_a64_shareable_nop(m, t)	\
static __pgtblhdlr_inline void		\
__pgtblhdlr_rawcfg_amend_##m##_shareable(const struct __pgtblhdlr_rawcfg_a64_s *cfg, \
					 const struct __pgtblhdlr_hlattr_a64_s *attr,	\
					 const struct __pgtblhdlr_rawfmt_##t##_s *tmpl)	\
{			\
	UNUSED(cfg, attr, tmpl); \
}

/*
 * These 4 entry types does not have shareable settings.
 */
DEFINE_pgtblhdlr_rawcfg_amend_a64_shareable_nop(a64ttbrreg,	a64ttbrreg)
DEFINE_pgtblhdlr_rawcfg_amend_a64_shareable_nop(a64tbl512g,	a64pgentry)
DEFINE_pgtblhdlr_rawcfg_amend_a64_shareable_nop(a64tbl1g,	a64pgentry)
DEFINE_pgtblhdlr_rawcfg_amend_a64_shareable_nop(a64tbl2m,	a64pgentry)

#ifndef CONFIG_PLAT_SD5151T

/*
 * For normal case, don't change user setting.
 */
DEFINE_pgtblhdlr_rawcfg_amend_a64_shareable_nop(a64blk1g,	a64pgentry)
DEFINE_pgtblhdlr_rawcfg_amend_a64_shareable_nop(a64blk2m,	a64pgentry)
DEFINE_pgtblhdlr_rawcfg_amend_a64_shareable_nop(a64page4k,	a64pgentry)

#else

/*
 * For SD5151T, switch all inner shareable to outer shareable because
 * a hardware *bug* which is no software way to fix.
 */
#define DEFINE_pgtblhdlr_rawcfg_amend_a64_shareable_sd5151t_fix(m, t)		\
static __pgtblhdlr_inline void								\
__pgtblhdlr_rawcfg_amend_##m##_shareable(struct __pgtblhdlr_rawcfg_a64_s *cfg,		\
					 const struct __pgtblhdlr_hlattr_a64_s *attr,	\
					 const struct __pgtblhdlr_rawfmt_##t##_s *tmpl)	\
{											\
	__u64 shareable_setbits	= __PGTBLATTR_A64LOWERBLKATTR_SH(3);			\
	__u64 shareable_inner_v	= __PGTBLATTR_A64LOWERBLKATTR_SH(__PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE);	\
	__u64 shareable_outer_v	= __PGTBLATTR_A64LOWERBLKATTR_SH(__PGTBLATTR_A64LOWERBLKATTR_SH_OUTER_SHAREABLE);	\
	if ((tmpl->v & shareable_setbits) == shareable_inner_v) {			\
		__pgtblhdlr_rawcfg_set_##m(cfg, shareable_setbits, shareable_outer_v);	\
	}										\
}

DEFINE_pgtblhdlr_rawcfg_amend_a64_shareable_sd5151t_fix(a64blk1g, 	a64pgentry)
DEFINE_pgtblhdlr_rawcfg_amend_a64_shareable_sd5151t_fix(a64blk2m, 	a64pgentry)
DEFINE_pgtblhdlr_rawcfg_amend_a64_shareable_sd5151t_fix(a64page4k,	a64pgentry)

#endif

#endif
