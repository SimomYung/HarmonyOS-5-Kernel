/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: AARCH64 hint transformer in mapi
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 18 15:42:23 2021
 */

#ifndef AARCH64_INCLUDE_MAPI_ASM_HINTS_H
#define AARCH64_INCLUDE_MAPI_ASM_HINTS_H

#include <uapi/hmasm/pgtbl/hints.h>
#include <hmasm/pgtbl/pgtblattr.h>
#include <hmkernel/pgtblhdlr.h>
#include <hmkernel/mm/pgtable.h>

static __pgtblhdlr_inline void
__pgtblhdlr_a64_parse_ttbr_hint(struct pgattr_s *attr, u64 hints)
{
	attr->a.tmpl_a64ttbrreg.v = __PGTBLATTR_A64TTBRREG(0, 0, ((hints & PGTABLE_ATTR_KMAP) != 0U) ? 1 : 0);
}

static __pgtblhdlr_inline void
__pgtblhdlr_a64_parse_table_hint(struct pgattr_s *attr, u64 hints)
{
	attr->a.tmpl_a64tbl512g.v	= __PGTBLATTR_DEFAULT_A64TBL512G;
	attr->a.tmpl_a64tbl1g.v		= __PGTBLATTR_DEFAULT_A64TBL1G;
	attr->a.tmpl_a64tbl2m.v		= __PGTBLATTR_DEFAULT_A64TBL2M;
}

static __pgtblhdlr_inline void
__pgtblhdlr_a64_parse_block_hint_stage1(struct pgattr_s *attr, u64 hints)
{
	unsigned long blkkmap = hints & (PGTABLE_ATTR_KMAP | PGTABLE_ATTR_PSMAP | PGTABLE_ATTR_HYPMAP);
	bool hyp_map = (blkkmap & PGTABLE_ATTR_HYPMAP) != 0U;
	unsigned int xn;
	unsigned int pxn;
	unsigned int ap21;
	unsigned int attridx;
	if ((hints & PGTABLE_ATTR_DEVICE) != 0U) {
		if ((hints & PGTABLE_ATTR_A64_DEV_GRE) != 0U) {
			attridx = __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_DEVICE_GRE;
		} else if ((hints & PGTABLE_ATTR_A64_DEV_nGnRE) != 0U) {
			attridx = __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_DEVICE_nGnRE;
		} else {
			attridx = __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_DEVICE_nGnRnE;
		}
	} else if ((hints & PGTABLE_ATTR_NOCACHE) != 0U) {
		attridx = __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_NORMAL_NC;
	} else {
		attridx = __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_NORMAL;
	}
	if (blkkmap) {
		xn = ((hyp_map == true) && (hints & PGTABLE_ATTR_KEXEC) != 0U) ? 0 : 1;
		pxn = ((hints & PGTABLE_ATTR_KEXEC) == 0U) ? 1 : 0;
		ap21 = ((hints & PGTABLE_ATTR_KWRITE) != 0U) ? 0 : 2;
		attr->a.tmpl_a64blk1g.v = __PGTBLATTR_A64BLK1G(0, 0, xn, pxn, 0, 0, 0, 0, 1,
					  __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, ap21, 0, attridx, 0);
		attr->a.tmpl_a64blk2m.v = __PGTBLATTR_A64BLK2M(0, 0, xn, pxn, 0, 0, 0, 0, 1,
					  __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, ap21, 0, attridx, 0);
		attr->a.tmpl_a64page4k.v = __PGTBLATTR_A64PAGE4K(0, 0, xn, pxn, 0, 0, 0, 1,
					   __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, ap21, 0, attridx, 0);
	} else {
		xn = ((hints & PGTABLE_ATTR_UEXEC) == 0U) ? 1 : 0;
		if ((hints & PGTABLE_ATTR_UWRITE) != 0U) {
			ap21 = 1;
		} else if ((hints & PGTABLE_ATTR_UREAD) == 0U) {
			ap21 = 2;
		} else {
			ap21 = 3;
		}
		attr->a.tmpl_a64blk1g.v = __PGTBLATTR_A64BLK1G(0, 0, xn, 1, 0, 0, 0, 1, 1,
					  __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, ap21, 0, attridx, 0);
		attr->a.tmpl_a64blk2m.v = __PGTBLATTR_A64BLK2M(0, 0, xn, 1, 0, 0, 0, 1, 1,
					  __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, ap21, 0, attridx, 0);
		attr->a.tmpl_a64page4k.v = __PGTBLATTR_A64PAGE4K(0, 0, xn, 1, 0, 0, 1, 1,
					   __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, ap21, 0, attridx, 0);
	}
}

#define PGTABLE_ATTR_SW_SHIFT 25U
#define PGTABLE_ATTR_SW_BITS 4U

#define PGTABLE_STAGE2_XN_UEXEC_KEXEC 0U
#define PGTABLE_STAGE2_XN_ONLY_UEXEC 1U
#define PGTABLE_STAGE2_XN_NO_EXEC 2U
#define PGTABLE_STAGE2_XN_ONLY_KEXEC 3U

static __pgtblhdlr_inline unsigned int
__pgtblhdlr_a64_stage2_xn_from_hint(u64 hints)
{
	bool uexec = ((hints & PGTABLE_ATTR_UEXEC) != 0U);
	bool kexec = ((hints & PGTABLE_ATTR_KEXEC) != 0U);
	unsigned int xn;

	if (uexec && kexec) {
		xn = PGTABLE_STAGE2_XN_UEXEC_KEXEC;
	} else if (uexec && !kexec) {
		xn = PGTABLE_STAGE2_XN_ONLY_UEXEC;
	} else if (!uexec && !kexec) {
		xn = PGTABLE_STAGE2_XN_NO_EXEC;
	} else {
		xn = PGTABLE_STAGE2_XN_ONLY_KEXEC;
	}

	return xn;
}

static __pgtblhdlr_inline void
__pgtblhdlr_a64_parse_block_hint_stage2(struct pgattr_s *attr, u64 hints)
{
	unsigned int xn;
	unsigned int s2ap = 0;
	unsigned int sw_attr = ((unsigned)((hints) >> PGTABLE_ATTR_SW_SHIFT) &
			       ((U(1) << PGTABLE_ATTR_SW_BITS) - U(1)));

	xn = __pgtblhdlr_a64_stage2_xn_from_hint(hints);
	if ((hints & PGTABLE_ATTR_UREAD) != 0ULL) {
		s2ap += 1;
	}
	if ((hints & PGTABLE_ATTR_UWRITE) != 0ULL) {
		s2ap += 2;
	}
	attr->a.tmpl_a64blk1g.v = __PGTBLATTR_STAGE2_A64BLK1G(0, xn, 0, 0, 0, 1,
							      __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE,
							      s2ap, 0xf, 1, sw_attr, 0);
	attr->a.tmpl_a64blk2m.v = __PGTBLATTR_STAGE2_A64BLK2M(0, xn, 0, 0, 0, 1,
							      __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE,
							      s2ap, 0xf, 1, sw_attr, 0);
	attr->a.tmpl_a64page4k.v = __PGTBLATTR_STAGE2_A64PAGE4K(0, xn, 0, 0, 1,
								__PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE,
								s2ap, 0xf, 1, sw_attr, 0);
}

static __pgtblhdlr_inline void
__pgtblhdlr_a64_parse_block_hint(struct pgattr_s *attr, u64 hints)
{
	bool stage2 = (hints & PGTABLE_ATTR_A64_STAGE2) != 0U;
	if (stage2) {
		__pgtblhdlr_a64_parse_block_hint_stage2(attr, hints);
	} else {
		__pgtblhdlr_a64_parse_block_hint_stage1(attr, hints);
	}
}

static __pgtblhdlr_inline void
pgtblhdlr_parse_hint(struct pgattr_s *attr, u64 hints)
{
	__pgtblhdlr_a64_parse_ttbr_hint(attr, hints);
	__pgtblhdlr_a64_parse_table_hint(attr, hints);
	__pgtblhdlr_a64_parse_block_hint(attr, hints);
}

static __pgtblhdlr_inline u64
__pgtblhdlr_a64_extract_table_key_hint(u64 e)
{
	UNUSED(e);
	return 0;
}

static __pgtblhdlr_inline u64
__pgtblhdlr_a64_extract_block_key_hint_stage1(u64 ap21, u64 attridx)
{
	u64 attr = 0;
	switch (ap21) {
	case 1:
		attr |= PGTABLE_ATTR_UREAD;
		attr |= PGTABLE_ATTR_UWRITE;
		attr |= PGTABLE_ATTR_KREAD;
		attr |= PGTABLE_ATTR_KWRITE;
		break;
	case 2:
		attr |= PGTABLE_ATTR_KREAD;
		break;
	case 3:
		attr |= PGTABLE_ATTR_KREAD;
		attr |= PGTABLE_ATTR_UREAD;
		break;
	case 0:
		attr |= PGTABLE_ATTR_KREAD;
		attr |= PGTABLE_ATTR_KWRITE;
		break;
	default:
		break;
	}
	switch (attridx) {
	default:
	case A64_PAGE_NORMAL:
		break;
	case A64_PAGE_NORMAL_NC:
		attr |= PGTABLE_ATTR_NOCACHE;
		break;
	case __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_DEVICE_nGnRnE:
		attr |= PGTABLE_ATTR_A64_DEV_nGnRnE;
		attr |= PGTABLE_ATTR_DEVICE;
		break;
	case __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_DEVICE_nGnRE:
		attr |= PGTABLE_ATTR_A64_DEV_nGnRE;
		attr |= PGTABLE_ATTR_DEVICE;
		break;
	case __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_DEVICE_GRE:
		attr |= PGTABLE_ATTR_A64_DEV_GRE;
		attr |= PGTABLE_ATTR_DEVICE;
		break;
	}
	return attr;
}

static __pgtblhdlr_inline u64
__pgtblhdlr_a64_extract_block_key_hint_stage2(u64 s2ap, u64 xn)
{
	u64 attr = 0;
	attr |= PGTABLE_ATTR_A64_STAGE2;
	switch (s2ap) {
	case 1:
		attr |= PGTABLE_ATTR_UREAD;
		break;
	case 2:
		attr |= PGTABLE_ATTR_UWRITE;
		break;
	case 3:
		attr |= PGTABLE_ATTR_UWRITE;
		attr |= PGTABLE_ATTR_UREAD;
		break;
	case 0:
		/* fall-through */
	default:
		break;
	}
	switch (xn) {
	case 0:
		attr |= PGTABLE_ATTR_UEXEC | PGTABLE_ATTR_KEXEC;
		break;
	case 1:
		attr |= PGTABLE_ATTR_UEXEC;
		break;
	case 3:
		attr |= PGTABLE_ATTR_KEXEC;
		break;
	case 2:
		/* fall-through */
	default:
		break;
	}
	return attr;
}

static __pgtblhdlr_inline u64
__pgtblhdlr_a64_extract_block_key_hint(u64 e)
{
	u64 attr = 0;
	u64 ap21;
	u64 s2ap;
	u64 xn;
	u64 attridx;
	bool kmap = false;
	if ((((u64)(e) >> 11U) & 0x1UL) == 0U) {
		attr |= PGTABLE_ATTR_KMAP;
		kmap = true;
	}
	bool stage2 = ((((u64)(e) >> 50U) & 0x1UL) != 0U);
	if (!kmap && (((u64)(e) >> 54U) & 0x1UL) == 0U) {
		attr |= PGTABLE_ATTR_UEXEC;
	}

	if (stage2) {
		s2ap = (((u64)(e) >> 6U)  & 0x3UL);
		xn = (((u64)(e) >> 53U) & 0x3UL);
		attr |= __pgtblhdlr_a64_extract_block_key_hint_stage2(s2ap, xn);
		attr |= pgtable_attr_sw((((u64)(e) >> 55U) & 0xfUL));
	} else {
		if (kmap && (((u64)(e) >> 53U) & 0x1UL) == 0U) {
			attr |= PGTABLE_ATTR_KEXEC;
		}
		if ((((u64)(e) >> 5U)  & 0x1UL) != 0U) {
			attr |= PGTABLE_ATTR_A64_NONSECURE;
		}
		ap21 = (((u64)(e) >> 6U)  & 0x3UL);
		attridx = (((u64)(e) >> 2U)  & 0x7UL);
		attr |= __pgtblhdlr_a64_extract_block_key_hint_stage1(ap21, attridx);
	}

	return attr;
}

static __pgtblhdlr_inline void
pgtblhdlr_extract_key_hint(u64 *hints, struct pgentry_type_s type, struct pgentry_s entry)
{
	u64 e;
	switch (type.t) {
	case 0:
		*hints = 0;
		break;
	case 1:
#if (VA_BITS == 48)
		e = entry.e.u.a64pgentry->v;
		*hints = (e != 0) ? __pgtblhdlr_a64_extract_table_key_hint(e) : 0;
#elif (VA_BITS == 39)
		*hints = 0;
#endif
		break;
	case 2:
	case 4:
	case 6:
		e = entry.e.u.a64pgentry->v;
		*hints = (e != 0) ? __pgtblhdlr_a64_extract_block_key_hint(e) : 0;
		break;
	case 3:
	case 5:
		e = entry.e.u.a64pgentry->v;
		*hints = (e != 0) ? __pgtblhdlr_a64_extract_table_key_hint(e) : 0;
		break;
	default:
		*hints = 0;
		break;
	}
}


static __pgtblhdlr_inline void
pgtblhdlr_extract_lastlv_keyattr(struct pgentry_type_s type, struct pgentry_s entry,
				 struct __pgtable_key_attr_s *keyattr)
{
	if (type.t == __PGTBLHDLR_PGENTRY_TYPE_A64BLK1G ||
	    type.t == __PGTBLHDLR_PGENTRY_TYPE_A64BLK2M ||
	    type.t == __PGTBLHDLR_PGENTRY_TYPE_A64PAGE4K) {
		__pgtblhdlr_extract_lastlv_keyattr(type.t, entry.e, keyattr);
	} else {
		panic("This function should not be used here!\n");
	}
}

#ifdef CONFIG_HYPERVISOR
#define __PGTABLE_ARCH_USR_WHITELIST \
	(PGTABLE_ATTR_A64_STAGE2 | PGTABLE_ATTR_A64_DEV_nGnRnE | PGTABLE_ATTR_A64_DEV_nGnRE | PGTABLE_ATTR_A64_DEV_GRE)
#else
#define __PGTABLE_ARCH_USR_WHITELIST \
	(PGTABLE_ATTR_A64_DEV_nGnRnE | PGTABLE_ATTR_A64_DEV_nGnRE | PGTABLE_ATTR_A64_DEV_GRE)
#endif

#endif
