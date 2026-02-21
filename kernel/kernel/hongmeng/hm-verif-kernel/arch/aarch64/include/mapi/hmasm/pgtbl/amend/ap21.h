/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Amend arm64 page table ap21 bits - mapi
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 20 10:38:42 2023
 */

#ifndef A64_MAPI_ASM_PGTBL_AMEND_AP21_H
#define A64_MAPI_ASM_PGTBL_AMEND_AP21_H

#include <hmasm/pgtbl/pgtblattr.h>

#ifndef AARCH64_INCLUDE_MAPI_ASM_POPULATE_H
# error "Should be included in mapi/hmasm/pgtbl/populate.h"
#endif

#define DEFINE_pgtblhdlr_rawcfg_amend_a64_ap21_nop(m, t)				\
static __pgtblhdlr_inline void								\
__pgtblhdlr_rawcfg_amend_##m##_ap21(const struct __pgtblhdlr_rawcfg_a64_s *cfg,		\
				    const struct __pgtblhdlr_hlattr_a64_s *attr,	\
				    const struct __pgtblhdlr_rawfmt_##t##_s *tmpl)	\
{											\
	UNUSED(cfg, attr, tmpl); 							\
}

/*
 * These 4 entry types does not have ap21 settings.
 */
DEFINE_pgtblhdlr_rawcfg_amend_a64_ap21_nop(a64ttbrreg,	a64ttbrreg)
DEFINE_pgtblhdlr_rawcfg_amend_a64_ap21_nop(a64tbl512g,	a64pgentry)
DEFINE_pgtblhdlr_rawcfg_amend_a64_ap21_nop(a64tbl1g,	a64pgentry)
DEFINE_pgtblhdlr_rawcfg_amend_a64_ap21_nop(a64tbl2m,	a64pgentry)

#ifndef CONFIG_AARCH64_DISABLE_XOM

/*
 * For normal case, don't change user setting.
 */
DEFINE_pgtblhdlr_rawcfg_amend_a64_ap21_nop(a64blk1g,	a64pgentry)
DEFINE_pgtblhdlr_rawcfg_amend_a64_ap21_nop(a64blk2m,	a64pgentry)
DEFINE_pgtblhdlr_rawcfg_amend_a64_ap21_nop(a64page4k,	a64pgentry)

#else

/*
 * For processors prior to ARMv8.7, switch all execute-only memory (XOM) to read + exec
 * at EL0, since a hardware *BUG* makes the Priviledged Access Never (PAN) feature
 * ineffective on such memory, and the bug cannot be fixed in a software way.
 *
 * From the perspective of pgtblattr model, we are changing AP21 of the XOM model from
 * 0b10 into 0b11, making it totaly the same as the UCODE model. Check xom.h and ucode.h
 * under mapi/uapi/hmasm/pgtbl/pgtblattr/ for more about these two models.
 *
 * Note that we want to limit this change to only the XOM model, and not affect other
 * models whose AP21 is 0b10 as well, in order to avoid unexpected problems.
 */
#define DEFINE_pgtblhdlr_rawcfg_amend_a64_ap21_xom_fix(m, t)				\
static __pgtblhdlr_inline void								\
__pgtblhdlr_rawcfg_amend_##m##_ap21(struct __pgtblhdlr_rawcfg_a64_s *cfg,		\
				    __maybe_unused const struct __pgtblhdlr_hlattr_a64_s *attr,	\
				    const struct __pgtblhdlr_rawfmt_##t##_s *tmpl)	\
{											\
	__u64 ap21_setbits = __PGTBLATTR_A64LOWERBLKATTR_AP21_MASK;			\
	__u64 ap21_el0_xom = __PGTBLATTR_A64LOWERBLKATTR_AP21(2);			\
	__u64 ap21_el0_rx  = __PGTBLATTR_A64LOWERBLKATTR_AP21(3);			\
	bool is_xn_not_set = (tmpl->v & __PGTBLATTR_A64UPPERBLKATTR_XN_MASK) == 0ULL;	\
	bool is_pxn_set	   = (tmpl->v & __PGTBLATTR_A64UPPERBLKATTR_PXN_MASK) != 0ULL;	\
	if (is_xn_not_set && is_pxn_set && (tmpl->v & ap21_setbits) == ap21_el0_xom) {	\
		__pgtblhdlr_rawcfg_set_##m(cfg, ap21_setbits, ap21_el0_rx);		\
	}										\
}

DEFINE_pgtblhdlr_rawcfg_amend_a64_ap21_xom_fix(a64blk1g, 	a64pgentry)
DEFINE_pgtblhdlr_rawcfg_amend_a64_ap21_xom_fix(a64blk2m, 	a64pgentry)
DEFINE_pgtblhdlr_rawcfg_amend_a64_ap21_xom_fix(a64page4k,	a64pgentry)

#endif

#endif
