/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: arch page table handler in MAPI
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 06 17:40:30 2021
 */

#ifndef AARCH64_INCLUDE_MAPI_ASM_PGTBLHDLR_H
#define AARCH64_INCLUDE_MAPI_ASM_PGTBLHDLR_H

#include <uapi/hmasm/pgtbl/pgtblhdlr.h>
#ifdef __PGTBLHDLR_CODE
#include <mapi/hmasm/pgtbl/pgtbldesc.h>
#endif
#include <hmasm/ctrlmem/supervs.h>
#include <mapi/hmasm/memory.h>
#include <hmasm/processor.h>
#include <hmkernel/compiler.h>

#define A64_PGTBLHDLR_CFG_INIT_MSK { \
	.a64ttbrreg	= (__u64)0ul, \
	.a64tbl512g	= (__u64)0ul, \
	.a64tbl1g	= (__u64)0ul, \
	.a64tbl2m	= (__u64)0ul, \
	.a64blk1g	= (__u64)0ul, \
	.a64blk2m	= (__u64)0ul, \
	.a64page4k	= (__u64)0ul, \
}

#define PGTBLHDLR_CFG_ZERO { \
	.ucfg.va_bits	= CONFIG_AARCH64_VA_BITS, \
	.attrbits	= A64_PGTBLHDLR_CFG_INIT_MSK, \
}

/* cfg should be determined based on context in the future */
static inline __always_inline struct pgcfg_s
pgtblhdlr_pgcfg_of_va(vaddr_t va)
{
	UNUSED(va);
	struct pgcfg_s cfg = {
		.c.cfg.ucfg.va_bits	= CONFIG_AARCH64_VA_BITS,
		.c.cfg.attrbits		= A64_PGTBLHDLR_CFG_INIT_MSK,
	};
	return cfg;
}

struct pgentry_vpgdreg {
	struct __pgtblhdlr_rawfmt_a64ttbrreg_s vreg;
};

#define PGENTRY_VPGDREG_INIT { .vreg.v = (__u64)(0), }

static inline __always_inline struct pgentry_vpgdreg
pgentry_vpgdreg_of_value(u64 value)
{
	struct pgentry_vpgdreg vpgdreg;
	vpgdreg.vreg.v = value;
	return vpgdreg;
}

static inline __always_inline void
pgentry_vpgdreg_zero(struct pgentry_vpgdreg *pgd)
{
	pgd->vreg.v = U(0);
}

static inline u64 a64_ttbr0_to_vpgdreg(u64 ttbr0v)
{
#ifdef CONFIG_CTRLMEM_SUPERVS
	return arch_ctrlmem_vspace_supervs_fix_ttbr0(ttbr0v);
#else
	return ttbr0v;
#endif
}

static inline __always_inline struct pgentry_vpgdreg
pgentry_vpgdreg_of(vaddr_t va)
{
	struct pgentry_vpgdreg vpgdreg;

	if (va < VA_START) {
		vpgdreg.vreg.v = a64_ttbr0_to_vpgdreg(read_sysreg("ttbr0_el1"));
	} else {
		vpgdreg.vreg.v = read_sysreg("ttbr1_el1");
	}
	return vpgdreg;
}

/*
 * Vpgdreg is a little bit different. It is usually virtual allocated so we can
 * create an entry with struct pgentry_s on-the-fly with struct pgentry_vpgdreg
 * as a common type equipped with a concrete value. For other pgentry type,
 * usually we have a pointer and we need to convert it with pgentry type.
 * In this case we will not have the struct pgentry_vpgdreg. Instead, they
 * accept a void *ptr and return struct pgentry_s with its .e.u.raw (it is a
 * pointer) equal to that ptr.
 */
static inline __always_inline struct pgentry_s
pgentry_make_vpgdreg(struct pgentry_vpgdreg *s)
{
	struct pgentry_s e = {
		.e.u.a64ttbrreg = &s->vreg,
	};
	return e;
}

static inline __always_inline u64
pgentry_vpgdreg_val(struct pgentry_s e)
{
	return e.e.u.a64ttbrreg->v;
}

static inline __always_inline struct pgentry_s
pgentry_make_smallest_blk(void *ptr)
{
	struct pgentry_s e = {
		.e.u.a64pgentry = (struct __pgtblhdlr_rawfmt_a64pgentry_s *)(ptr),
	};
	return e;
}

static inline __always_inline size_t
pgentry_size_of_smallest_blk(void)
{
	struct pgentry_s e;
	return sizeof(*(e.e.u.a64pgentry));
}

#endif
