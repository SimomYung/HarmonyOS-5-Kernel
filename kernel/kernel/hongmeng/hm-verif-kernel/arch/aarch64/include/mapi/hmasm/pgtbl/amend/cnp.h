/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Amend arm64 page table CNP bits - mapi
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 07 20:42:34 2022
 */

#ifndef A64_MAPI_ASM_PGTBL_AMEND_CNP_H
#define A64_MAPI_ASM_PGTBL_AMEND_CNP_H

#ifdef __PGTBLHDLR_CODE
#include <mapi/hmasm/pgtbl/pgtbldesc.h>
#endif
#include <hmasm/pgtbl/pgtblattr.h>
#include <mapi/hmasm/types.h>

#ifndef AARCH64_INCLUDE_MAPI_ASM_POPULATE_H
# error "Should be included in mapi/hmasm/pgtbl/populate.h"
#endif

#define DEFINE_pgtblhdlr_rawcfg_amend_a64_cnp_nop(m, t)	\
static __pgtblhdlr_inline void		\
__pgtblhdlr_rawcfg_amend_##m##_cnp(const struct __pgtblhdlr_rawcfg_a64_s *cfg, \
				   const struct __pgtblhdlr_hlattr_a64_s *attr,	\
				   const struct __pgtblhdlr_rawfmt_##t##_s *tmpl) \
{ \
	UNUSED(cfg, attr, tmpl); \
}

DEFINE_pgtblhdlr_rawcfg_amend_a64_cnp_nop(a64tbl512g,	a64pgentry)
DEFINE_pgtblhdlr_rawcfg_amend_a64_cnp_nop(a64tbl1g,	a64pgentry)
DEFINE_pgtblhdlr_rawcfg_amend_a64_cnp_nop(a64tbl2m,	a64pgentry)
DEFINE_pgtblhdlr_rawcfg_amend_a64_cnp_nop(a64blk1g,	a64pgentry)
DEFINE_pgtblhdlr_rawcfg_amend_a64_cnp_nop(a64blk2m,	a64pgentry)
DEFINE_pgtblhdlr_rawcfg_amend_a64_cnp_nop(a64page4k,	a64pgentry)

#ifndef CONFIG_AARCH64_V8_2

static __pgtblhdlr_inline void
__pgtblhdlr_rawcfg_amend_a64ttbrreg_cnp(struct __pgtblhdlr_rawcfg_a64_s *cfg,
					const struct __pgtblhdlr_hlattr_a64_s *attr,
					const struct __pgtblhdlr_rawfmt_a64ttbrreg_s *tmpl)
{
	UNUSED(attr, tmpl);
	__pgtblhdlr_rawcfg_set_a64ttbrreg(cfg, __PGTBLATTR_A64TTBRREG_CNP(1),
					  (__u64)(0u));
}

#else

static __pgtblhdlr_inline void
__pgtblhdlr_rawcfg_amend_a64ttbrreg_cnp(struct __pgtblhdlr_rawcfg_a64_s *cfg,
					__maybe_unused const struct __pgtblhdlr_hlattr_a64_s *attr,
					__maybe_unused const struct __pgtblhdlr_rawfmt_a64ttbrreg_s *tmpl)
{
#ifndef CONFIG_PRIVILEGED_SERVICE
	if (attr->setting == __PGTBLHDLR_HLATTR_A64_SETTING_KERNEL) {
		__pgtblhdlr_rawcfg_set_a64ttbrreg(cfg, __PGTBLATTR_A64TTBRREG_CNP(1),
						       __PGTBLATTR_A64TTBRREG_CNP(1));
	} else {
		__pgtblhdlr_rawcfg_set_a64ttbrreg(cfg, __PGTBLATTR_A64TTBRREG_CNP(1), (__u64)(0u));
	}
#else
	__pgtblhdlr_rawcfg_set_a64ttbrreg(cfg, __PGTBLATTR_A64TTBRREG_CNP(1), (__u64)(0u));
#endif
}

#endif

#endif
