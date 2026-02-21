/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: AArch64 arch populate barrier
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 25 20:31:50 2021
 */

#ifndef AARCH64_INCLUDE_MAPI_ASM_POPULATE_H
#define AARCH64_INCLUDE_MAPI_ASM_POPULATE_H

#include <hmkernel/pgtblhdlr/types.h>
#include <hmasm/memory.h>

#ifndef __HOST_LLT__
#define pgtblhdlr_barrier()	asm volatile("dsb ishst; isb" : : : "memory")
#else
#define pgtblhdlr_barrier()	do { } while(0)
#endif

#include <hmasm/pgtbl/amend/shareable.h>
#include <hmasm/pgtbl/amend/cnp.h>
#include <hmasm/pgtbl/amend/ng.h>
#include <hmasm/pgtbl/amend/ap21.h>
#include <hmasm/pgtbl/amend/fwb.h>

static __pgtblhdlr_inline enum __pgtblhdlr_hlattr_a64_setting_e
__pgtblhdlr_hlattr_a64_setting_of_va(vaddr_t va)
{
	/* __PGTBLHDLR_HLATTR_A64_SETTING_KERNEL is 0 */
	enum __pgtblhdlr_hlattr_a64_setting_e setting = __PGTBLHDLR_HLATTR_A64_SETTING_KERNEL;

#ifdef __ELFLOADER_NG__
	setting = __PGTBLHDLR_HLATTR_A64_SETTING_ELFLOADER;
#else
	/*
	 * See elf_layout.c. It reside in elfloader_ng and will be moved to kernel, which
	 * decides layout of sysproc vspace. Virtual address of privileged services are defined
	 * in privilege_layout.h.
	 */
	if (va < VA_START) {
		setting = __PGTBLHDLR_HLATTR_A64_SETTING_USER;
	} else if (va < PRIVILEGE_VA_LIMIT) {
		setting = __PGTBLHDLR_HLATTR_A64_SETTING_SERVICE;
	} else {
		setting = __PGTBLHDLR_HLATTR_A64_SETTING_KERNEL;
	}
#endif
	return setting;
}

static __pgtblhdlr_inline void
__pgtblhdlr_a64_init_cfg_hl(struct __pgtblhdlr_rawcfg_a64_s *cfg,
			    const struct __pgtblhdlr_hlattr_a64_s *hlattr)
{
	cfg->hlattr.setting = hlattr->setting;
	/*
	 * Don't use `*cfg = cfg_zero`: gcc may generate
	 * memset or memcpy if we use it. Directly set all
	 * fields to 0 so it has more chance to merge these
	 * assignments with following amendation.
	 */
	cfg->ucfg.va_bits = CONFIG_AARCH64_VA_BITS;
}

/*
 * FIXME: This function should be deleted after all mapping switched
 * to scenario based mapping. Currently we have this function because
 * we are still using hints, which doesn't provide detail attributes.
 * We need to deduce attr->setting from VA.
 */
static __pgtblhdlr_inline void
__pgtblhdlr_a64_init_hlattr_by_va(struct __pgtblhdlr_hlattr_a64_s *attr, vaddr_t va)
{
	attr->setting = __pgtblhdlr_hlattr_a64_setting_of_va(va);
}

#define DEFINE_pgtblhdlr_init_amend_cfg(m, f)	\
static __pgtblhdlr_inline void \
__pgtblhdlr_##m##_init_cfg_partial(struct __pgtblhdlr_rawcfg_a64_s *cfg, \
				   const struct __pgtblhdlr_rawfmt_##f##_s *tmpl) \
{ \
	cfg->attrbits.m	= tmpl->v; \
} \
static __pgtblhdlr_inline void		\
__pgtblhdlr_##m##_amend_cfg_preva(struct __pgtblhdlr_rawcfg_a64_s *cfg, \
				  const struct __pgtblhdlr_hlattr_a64_s *attr, \
				  const struct __pgtblhdlr_rawfmt_##f##_s *tmpl) \
{ \
	__pgtblhdlr_rawcfg_amend_##m##_shareable(cfg, attr, tmpl); \
	__pgtblhdlr_rawcfg_amend_##m##_cnp(cfg, attr, tmpl); \
	__pgtblhdlr_rawcfg_amend_##m##_ng(cfg, attr, tmpl); \
	__pgtblhdlr_rawcfg_amend_##m##_ap21(cfg, attr, tmpl); \
} \
static __pgtblhdlr_inline void		\
__pgtblhdlr_##m##_amend_cfg_postva(struct __pgtblhdlr_rawcfg_a64_s *cfg, vaddr_t va) \
{ \
	__pgtblhdlr_rawcfg_amend_##m##_fwb(cfg, va); \
} \
static __pgtblhdlr_inline void		\
__pgtblhdlr_##m##_amend_cfg_entval(struct __pgtblhdlr_rawcfg_a64_s *cfg, \
				   __pgtblhdlr_rawfmt_base_t entval) \
{ }

DEFINE_pgtblhdlr_init_amend_cfg(a64ttbrreg,	a64ttbrreg)
DEFINE_pgtblhdlr_init_amend_cfg(a64tbl512g,	a64pgentry)
DEFINE_pgtblhdlr_init_amend_cfg(a64blk1g,	a64pgentry)
DEFINE_pgtblhdlr_init_amend_cfg(a64tbl1g,	a64pgentry)
DEFINE_pgtblhdlr_init_amend_cfg(a64blk2m,	a64pgentry)
DEFINE_pgtblhdlr_init_amend_cfg(a64tbl2m,	a64pgentry)
DEFINE_pgtblhdlr_init_amend_cfg(a64page4k,	a64pgentry)

#endif
