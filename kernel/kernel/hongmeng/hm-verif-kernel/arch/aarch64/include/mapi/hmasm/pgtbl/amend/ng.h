/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Amend arm64 page table NG bits - mapi
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 10 19:41:40 2022
 */

#ifndef A64_MAPI_ASM_PGTBL_AMEND_NG_H
#define A64_MAPI_ASM_PGTBL_AMEND_NG_H

#ifdef __PGTBLHDLR_CODE
#include <mapi/hmasm/pgtbl/pgtbldesc.h>
#endif
#include <hmasm/pgtbl/pgtblattr.h>
#include <mapi/hmasm/types.h>

#ifndef AARCH64_INCLUDE_MAPI_ASM_POPULATE_H
# error "Should be included in mapi/hmasm/pgtbl/populate.h"
#endif

#define DEFINE_pgtblhdlr_rawcfg_amend_a64_ng_nop(m, t)	\
static __pgtblhdlr_inline void		\
__pgtblhdlr_rawcfg_amend_##m##_ng(const struct __pgtblhdlr_rawcfg_a64_s *cfg, \
				  const struct __pgtblhdlr_hlattr_a64_s *attr,	\
				  const struct __pgtblhdlr_rawfmt_##t##_s *tmpl) \
{ \
	UNUSED(cfg, attr, tmpl); \
}

/*
 * No NG bits in ttbr and table.
 */
DEFINE_pgtblhdlr_rawcfg_amend_a64_ng_nop(a64ttbrreg,	a64ttbrreg)
DEFINE_pgtblhdlr_rawcfg_amend_a64_ng_nop(a64tbl512g,	a64pgentry)
DEFINE_pgtblhdlr_rawcfg_amend_a64_ng_nop(a64tbl1g,	a64pgentry)
DEFINE_pgtblhdlr_rawcfg_amend_a64_ng_nop(a64tbl2m,	a64pgentry)

/*
 * Different from other bits, NG bits reside in A64LOWERBLKATTR and which is common (in manual)
 * for blk1g, 2m ad page4k, so no need to define __PGTBLATTR_##M##_NG like other bits.
 */

static __pgtblhdlr_inline void
__pgtblhdlr_rawcfg_amend_a64lowerblkattr_ng(__u64 *attrbits, const struct __pgtblhdlr_hlattr_a64_s *attr)
{
	switch (attr->setting) {
	case __PGTBLHDLR_HLATTR_A64_SETTING_KERNEL:
	case __PGTBLHDLR_HLATTR_A64_SETTING_ELFLOADER:
#ifdef CONFIG_PRIVILEGED_SERVICE
	case __PGTBLHDLR_HLATTR_A64_SETTING_SERVICE:
#endif
		*attrbits &= ~(__PGTBLATTR_A64LOWERBLKATTR_NG(1));
		break;
	default:
	case __PGTBLHDLR_HLATTR_A64_SETTING_USER:
#ifndef CONFIG_PRIVILEGED_SERVICE
	case __PGTBLHDLR_HLATTR_A64_SETTING_SERVICE:
#endif
		*attrbits |= __PGTBLATTR_A64LOWERBLKATTR_NG(1);
		break;
	}
}

#define DEFINE_pgtblhdlr_rawcfg_amend_a64_ng_reset(m, t)				\
static __pgtblhdlr_inline void								\
__pgtblhdlr_rawcfg_amend_##m##_ng(struct __pgtblhdlr_rawcfg_a64_s *cfg,			\
				  const struct __pgtblhdlr_hlattr_a64_s *attr,		\
				  const struct __pgtblhdlr_rawfmt_##t##_s *tmpl)	\
{											\
	UNUSED(tmpl);									\
	__pgtblhdlr_rawcfg_amend_a64lowerblkattr_ng(&cfg->attrbits.m,			\
						    attr);				\
}

DEFINE_pgtblhdlr_rawcfg_amend_a64_ng_reset(a64blk1g,	a64pgentry)
DEFINE_pgtblhdlr_rawcfg_amend_a64_ng_reset(a64blk2m,	a64pgentry)
DEFINE_pgtblhdlr_rawcfg_amend_a64_ng_reset(a64page4k,	a64pgentry)

#endif
