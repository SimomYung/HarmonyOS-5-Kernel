/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Amend arm64 stage2 memattr according to fwb - mapi
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 14 09:59:42 2024
 */

#ifndef A64_MAPI_ASM_PGTBL_AMEND_FWB_H
#define A64_MAPI_ASM_PGTBL_AMEND_FWB_H

#include <hmasm/pgtbl/pgtblattr.h>

#ifndef AARCH64_INCLUDE_MAPI_ASM_POPULATE_H
# error "Should be included in mapi/hmasm/pgtbl/populate.h"
#endif

#define DEFINE_pgtblhdlr_rawcfg_amend_a64_fwb_nop(m, t)	\
static __pgtblhdlr_inline void		\
__pgtblhdlr_rawcfg_amend_##m##_fwb(const struct __pgtblhdlr_rawcfg_a64_s *cfg, \
					 vaddr_t va) \
{			\
	UNUSED(cfg, va); \
}

DEFINE_pgtblhdlr_rawcfg_amend_a64_fwb_nop(a64ttbrreg,	a64ttbrreg)
DEFINE_pgtblhdlr_rawcfg_amend_a64_fwb_nop(a64tbl512g,	a64pgentry)
DEFINE_pgtblhdlr_rawcfg_amend_a64_fwb_nop(a64tbl1g,	a64pgentry)
DEFINE_pgtblhdlr_rawcfg_amend_a64_fwb_nop(a64tbl2m,	a64pgentry)

#ifndef CONFIG_HYPERVISOR

DEFINE_pgtblhdlr_rawcfg_amend_a64_fwb_nop(a64blk1g,	a64pgentry)
DEFINE_pgtblhdlr_rawcfg_amend_a64_fwb_nop(a64blk2m,	a64pgentry)
DEFINE_pgtblhdlr_rawcfg_amend_a64_fwb_nop(a64page4k,	a64pgentry)

#else

#ifdef CONFIG_A64_HYP_FWB
#define __PGTBLATTR_A64LOWERBLKATTR_S2_NORMAL		0x6
#define __PGTBLATTR_A64LOWERBLKATTR_S2_DEV_nGnRE		0x1
#else
#define __PGTBLATTR_A64LOWERBLKATTR_S2_NORMAL		0xf
#define __PGTBLATTR_A64LOWERBLKATTR_S2_DEV_nGnRE		0x1
#endif

static __pgtblhdlr_inline void
__pgtblhdlr_rawcfg_amend_a64lowerblkattr_fwb(__u64 *attrbits, const struct __pgtblhdlr_hlattr_a64_s *attr)
{
	switch (attr->setting) {
	case __PGTBLHDLR_HLATTR_A64_SETTING_S2_NORMAL:
		*attrbits &= (~__PGTBLATTR_A64LOWERBLKATTR_MEMATTR_MASK);
		*attrbits |= __PGTBLATTR_A64LOWERBLKATTR_MEMATTR(__PGTBLATTR_A64LOWERBLKATTR_S2_NORMAL);
		break;
	case __PGTBLHDLR_HLATTR_A64_SETTING_S2_DEV:
		*attrbits &= (~__PGTBLATTR_A64LOWERBLKATTR_MEMATTR_MASK);
		*attrbits |= __PGTBLATTR_A64LOWERBLKATTR_MEMATTR(__PGTBLATTR_A64LOWERBLKATTR_S2_DEV_nGnRE);
		break;
	default:
		break;
	}
}

#define DEFINE_pgtblhdlr_rawcfg_amend_a64_fwb_reset(m, t)		\
static __pgtblhdlr_inline void								\
__pgtblhdlr_rawcfg_amend_##m##_fwb(struct __pgtblhdlr_rawcfg_a64_s *cfg,		\
					 vaddr_t va) \
{											\
	UNUSED(va);									\
	__pgtblhdlr_rawcfg_amend_a64lowerblkattr_fwb(&cfg->attrbits.m,			\
						    &cfg->hlattr);				\
}

DEFINE_pgtblhdlr_rawcfg_amend_a64_fwb_reset(a64blk1g, 	a64pgentry)
DEFINE_pgtblhdlr_rawcfg_amend_a64_fwb_reset(a64blk2m, 	a64pgentry)
DEFINE_pgtblhdlr_rawcfg_amend_a64_fwb_reset(a64page4k,	a64pgentry)

#endif

#endif
