/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Attributes in arm64 page table - uapi
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 07 11:38:16 2021
 */

#ifndef AARCH64_MAPI_UAPI_ASM_PGTBLATTR_H
#define AARCH64_MAPI_UAPI_ASM_PGTBLATTR_H

#include <hmkernel/const.h>

#define TTBR_ASID_extract(v)	(((v) >> 48U) & 0xffUL)
#define TTBR_BADDR_extract(v)	((v) & 0x0000fffffffffffeUL)
#define TTBR_CnP_extract(v)	((v) & 0x1U)

#define TTBR_ASID(x)	((((u64)(x)) & 0xffUL) << 48U)
#define TTBR_BADDR(x)	((x) & 0x0000fffffffffffeUL)
#define TTBR_CnP(x)	(((x) != 0) ? 1UL : 0UL)

#define __PGTBLATTR_A64TTBRREG_MASK_ASID	0xffff000000000000
#define __PGTBLATTR_A64TTBRREG_MASK_BADDR	0x0000fffffffffffe
#define __PGTBLATTR_A64TTBRREG_MASK_CNP		0x1

#define __PGTBLATTR_A64TTBRREG_ASID(n)		(((unsigned long)(n) & __UL(0xffff)) << __U(48))
#define __PGTBLATTR_A64TTBRREG_BADDR(n)		((unsigned long)(n) & __UL(__PGTBLATTR_A64TTBRREG_MASK_BADDR))
#define __PGTBLATTR_A64TTBRREG_CNP(n)		((unsigned long)(n) & __UL(1))

#define __PGTBLATTR_A64LV012TBL_MASK_NSTABLE	0x8000000000000000	/* 0x1 << 63 */
#define __PGTBLATTR_A64LV012TBL_MASK_APTABLE	0x6000000000000000	/* 0x3 << 61 */
#define __PGTBLATTR_A64LV012TBL_MASK_XNTABLE	0x1000000000000000	/* 0x1 << 60 */
#define __PGTBLATTR_A64LV012TBL_MASK_PXNTABLE	0x0800000000000000	/* 0x1 << 59 */
#define __PGTBLATTR_A64LV012TBL_MASK_NLTBLADDR	0x0000fffffffff000
#define __PGTBLATTR_A64LV012TBL_MARK_MASK	0x3

#define __PGTBLATTR_A64LV012TBL_NSTABLE(n)	(((unsigned long)(n) & __UL(0x1)) << __U(63))
#define __PGTBLATTR_A64LV012TBL_APTABLE(n)	(((unsigned long)(n) & __UL(0x3)) << __U(61))
#define __PGTBLATTR_A64LV012TBL_XNTABLE(n)	(((unsigned long)(n) & __UL(0x1)) << __U(60))
#define __PGTBLATTR_A64LV012TBL_PXNTABLE(n)	(((unsigned long)(n) & __UL(0x1)) << __U(59))
#define __PGTBLATTR_A64LV012TBL_NLTBLADDR(n)	(((unsigned long)(n) & __UL(__PGTBLATTR_A64LV012TBL_MASK_NLTBLADDR)))
#define __PGTBLATTR_A64LV012TBL_MARK		0x3

#define __PGTBLATTR_A64LV012BLK_MARK_MASK	0x3
#define __PGTBLATTR_A64LV012BLK_UPPERATTR_MASK	0xfff0000000000000	/* 0xfff << 52 */
#define __PGTBLATTR_A64LV012BLK_GP_MASK		0x0004000000000000	/* 0x1 << 50 */
#define __PGTBLATTR_A64LV012BLK_LOWERATTR_MASK	0x0000000000000ffc
#define __PGTBLATTR_A64LV012BLK_MARK		0x1

#define __PGTBLATTR_A64LV012NMP_MARK		0x2

#define __PGTBLATTR_A64UPPERBLKATTR_PBHA_MASK	0x7800000000000000	/* 0xf << 59 */
#define __PGTBLATTR_A64UPPERBLKATTR_UXN_MASK	0x0040000000000000	/* 1 << 54 */
/* UXN and XN use one bit */
#define __PGTBLATTR_A64UPPERBLKATTR_XN_MASK	0x0040000000000000	/* 1 << 54 */
#define __PGTBLATTR_A64UPPERBLKATTR_S2_XN_MASK	0x0060000000000000	/* 3 << 53 */
#define __PGTBLATTR_A64UPPERBLKATTR_PXN_MASK	0x0020000000000000	/* 1 << 53 */
#define __PGTBLATTR_A64UPPERBLKATTR_CONT_MASK	0x0010000000000000	/* 1 << 52 */
#define __PGTBLATTR_A64UPPERBLKATTR_DBM_MASK	0x0008000000000000	/* 1 << 51 */

#define __PGTBLATTR_A64UPPERATTR_PBHA(n)	(((unsigned long)(n) & __UL(0xf)) << __U(59))
#define __PGTBLATTR_A64UPPERATTR_SW_ATTR(n)	(((unsigned long)(n) & __UL(0xf)) << __U(55))
#define __PGTBLATTR_A64UPPERATTR_UXN(n)		(((unsigned long)(n) & __UL(0x1)) << __U(54))

/*
 * Assignment of __PGTBLATTR_A64UPPERATTR_SW_ATTR
 */
#define __PGTBLATTR_A64UPPERATTR_SW_FRAGILE_MAPPING	__PGTBLATTR_A64UPPERATTR_SW_ATTR(0x1)

/*
 * Use this macro to notice the existance of fragile mapping for uapi
 * to avoid introducing CONFIG_XXX into uapi.
 */
#define __PGTBLATTR_ARCH_SUPPORT_SW_FRAGILE_MAPPING

/* XN and UXN are same */
#define __PGTBLATTR_A64UPPERATTR_XN(n)		(((unsigned long)(n) & __UL(0x1)) << __U(54))
#define __PGTBLATTR_A64UPPERATTR_S2_XN(n)	(((unsigned long)(n) & __UL(0x3)) << __U(53))
#define __PGTBLATTR_A64UPPERATTR_PXN(n)		(((unsigned long)(n) & __UL(0x1)) << __U(53))
#define __PGTBLATTR_A64UPPERATTR_CONT(n)	(((unsigned long)(n) & __UL(0x1)) << __U(52))
#define __PGTBLATTR_A64UPPERATTR_DBM(n)		(((unsigned long)(n) & __UL(0x1)) << __U(51))
#define __PGTBLATTR_A64UPPERATTR_GP(n)		(((unsigned long)(n) & __UL(0x1)) << __U(50))

#define __PGTBLATTR_A64UPPERATTR_SW_STAGE2(n)		(((unsigned long)(n) & __UL(0x1)) << __U(50))

#define __PGTBLATTR_A64LOWERBLKATTR_NT_MASK		0x10000		/* 1 << 16 */
#define __PGTBLATTR_A64LOWERBLKATTR_NG_MASK		0x800		/* 1 << 11 */
#define __PGTBLATTR_A64LOWERBLKATTR_SH_MASK		0x300		/* 0x3 << 8 */
#define __PGTBLATTR_A64LOWERBLKATTR_AP21_MASK		0xc0		/* 0x3 << 6 */
#define __PGTBLATTR_A64LOWERBLKATTR_S2AP_MASK		0xc0		/* 0x3 << 6 */
#define __PGTBLATTR_A64LOWERBLKATTR_NS_MASK		0x20		/* 0x1 << 5 */
#define __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_MASK	0x1c		/* 0x7 << 2 */
#define __PGTBLATTR_A64LOWERBLKATTR_MEMATTR_MASK	0x3c		/* 0xf << 2 */
#define __PGTBLATTR_A64LOWERBLKATTR_AF_MASK		0x400 		/* 1 << 10 */

#define __PGTBLATTR_A64LOWERBLKATTR_NT(n)	(((unsigned long)(n) & __UL(1)) << __U(16))
#define __PGTBLATTR_A64LOWERBLKATTR_NG(n)	(((unsigned long)(n) & __UL(1)) << __U(11))
#define __PGTBLATTR_A64LOWERBLKATTR_AF(n)	(((unsigned long)(n) & __UL(1)) << __U(10))
#define __PGTBLATTR_A64LOWERBLKATTR_SH(n)	(((unsigned long)(n) & __UL(3)) << __U(8))
#define __PGTBLATTR_A64LOWERBLKATTR_AP21(n)	(((unsigned long)(n) & __UL(3)) << __U(6))
#define __PGTBLATTR_A64LOWERBLKATTR_S2AP(n)	(((unsigned long)(n) & __UL(3)) << __U(6))
#define __PGTBLATTR_A64LOWERBLKATTR_NS(n)	(((unsigned long)(n) & __UL(1)) << __U(5))
#define __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX(n)	(((unsigned long)(n) & __UL(7)) << __U(2))
#define __PGTBLATTR_A64LOWERBLKATTR_MEMATTR(n)	(((unsigned long)(n) & __UL(0xf)) << __U(2))

#define __PGTBLATTR_A64LV3PAGE_MARK_MASK	0x3
#define __PGTBLATTR_A64LV3PAGE_MARK		0x3
#define __PGTBLATTR_A64LV3NONE_MARK		0x2

/* A64 page table sharebility */
#define __PGTBLATTR_A64LOWERBLKATTR_SH_NON_SHAREABLE		0
#define __PGTBLATTR_A64LOWERBLKATTR_SH_OUTER_SHAREABLE		2
#define __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE		3

/* A64 page table mair types */
#define __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_DEVICE_nGnRnE  0
#define __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_DEVICE_nGnRE   1
#define __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_DEVICE_GRE     2
#define __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_NORMAL_NC      3
#define __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_NORMAL         4

#define __PGTBLATTR_A64UPPERATTR(pbha, uxn, xn, pxn, cont, dbm) \
	(__PGTBLATTR_A64UPPERATTR_PBHA(pbha) |	\
	 __PGTBLATTR_A64UPPERATTR_UXN(uxn) |	\
	 __PGTBLATTR_A64UPPERATTR_XN(xn) |	\
	 __PGTBLATTR_A64UPPERATTR_PXN(pxn) |	\
	 __PGTBLATTR_A64UPPERATTR_CONT(cont) |	\
	 __PGTBLATTR_A64UPPERATTR_DBM(dbm))

#define __PGTBLATTR_STAGE2_A64UPPERATTR(pbha, xn, cont, dbm, sw_stage2, sw_attr) \
	(__PGTBLATTR_A64UPPERATTR_PBHA(pbha) |	\
	 __PGTBLATTR_A64UPPERATTR_S2_XN(xn) |	\
	 __PGTBLATTR_A64UPPERATTR_CONT(cont) |	\
	 __PGTBLATTR_A64UPPERATTR_DBM(dbm) | \
	 __PGTBLATTR_A64UPPERATTR_SW_STAGE2(sw_stage2) | \
	 __PGTBLATTR_A64UPPERATTR_SW_ATTR(sw_attr))

#define __PGTBLATTR_A64LOWERBLKATTR(nt, ng, af, sh, ap21, ns, attridx)	\
	(__PGTBLATTR_A64LOWERBLKATTR_NT(nt) | \
	 __PGTBLATTR_A64LOWERBLKATTR_NG(ng) | \
	 __PGTBLATTR_A64LOWERBLKATTR_AF(af) | \
	 __PGTBLATTR_A64LOWERBLKATTR_SH(sh) | \
	 __PGTBLATTR_A64LOWERBLKATTR_AP21(ap21) | \
	 __PGTBLATTR_A64LOWERBLKATTR_NS(ns) | \
	 __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX(attridx))

#define __PGTBLATTR_STAGE2_A64LOWERBLKATTR(nt, af, sh, s2ap, memattr)	\
	(__PGTBLATTR_A64LOWERBLKATTR_NT(nt) | \
	 __PGTBLATTR_A64LOWERBLKATTR_AF(af) | \
	 __PGTBLATTR_A64LOWERBLKATTR_SH(sh) | \
	 __PGTBLATTR_A64LOWERBLKATTR_S2AP(s2ap) | \
	 __PGTBLATTR_A64LOWERBLKATTR_MEMATTR(memattr))

#define __PGTBLATTR_A64TBL512G(nstbl, aptbl, xntbl, pxntbl, nltbladdr)	\
	(__PGTBLATTR_A64LV012TBL_NSTABLE(nstbl) | \
	__PGTBLATTR_A64LV012TBL_APTABLE(aptbl) | \
	__PGTBLATTR_A64LV012TBL_XNTABLE(xntbl) | \
	__PGTBLATTR_A64LV012TBL_PXNTABLE(pxntbl) | \
	__PGTBLATTR_A64LV012TBL_NLTBLADDR(nltbladdr) | \
	__PGTBLATTR_A64LV012TBL_MARK)

#define __PGTBLATTR_A64TBL1G(nstbl, aptbl, xntbl, pxntbl, nltbladdr)	\
	(__PGTBLATTR_A64LV012TBL_NSTABLE(nstbl) | \
	__PGTBLATTR_A64LV012TBL_APTABLE(aptbl) | \
	__PGTBLATTR_A64LV012TBL_XNTABLE(xntbl) | \
	__PGTBLATTR_A64LV012TBL_PXNTABLE(pxntbl) | \
	__PGTBLATTR_A64LV012TBL_NLTBLADDR(nltbladdr) | \
	__PGTBLATTR_A64LV012TBL_MARK)

#define __PGTBLATTR_A64TBL2M(nstbl, aptbl, xntbl, pxntbl, nltbladdr)	\
	(__PGTBLATTR_A64LV012TBL_NSTABLE(nstbl) | \
	__PGTBLATTR_A64LV012TBL_APTABLE(aptbl) | \
	__PGTBLATTR_A64LV012TBL_XNTABLE(xntbl) | \
	__PGTBLATTR_A64LV012TBL_PXNTABLE(pxntbl) | \
	__PGTBLATTR_A64LV012TBL_NLTBLADDR(nltbladdr) | \
	__PGTBLATTR_A64LV012TBL_MARK)

#define __PGTBLATTR_A64TTBRREG(asid, baddr, cnp) \
		(__PGTBLATTR_A64TTBRREG_ASID(asid) | \
		 __PGTBLATTR_A64TTBRREG_BADDR(baddr) | \
		 __PGTBLATTR_A64TTBRREG_CNP(cnp))

/*
 * NOTE: UXN and XN are one bit.
 */
#define ____PGTBLATTR_A64BLK1G(pbha, uxn, xn, pxn, cont, dbm, nt, ng, af, sh, ap21, ns, attridx, paddr, mark) \
	(__PGTBLATTR_A64UPPERATTR(pbha, uxn, xn, pxn, cont, dbm) | \
	 __PGTBLATTR_A64LOWERBLKATTR(nt, ng, af, sh, ap21, ns, attridx) | \
	 ((unsigned long)(paddr) & __UL(0x0000ffffc0000000)) | \
	 (mark))

#define __PGTBLATTR_A64BLK1G(pbha, uxn, xn, pxn, cont, dbm, nt, ng, af, sh, ap21, ns, attridx, paddr)	\
	____PGTBLATTR_A64BLK1G(pbha, uxn, xn, pxn, cont, dbm, nt, ng, af, sh, ap21, ns, attridx, paddr, \
			       __PGTBLATTR_A64LV012BLK_MARK)

#define ____PGTBLATTR_A64BLK2M(pbha, uxn, xn, pxn, cont, dbm, nt, ng, af, sh, ap21, ns, attridx, paddr, mark) \
	(__PGTBLATTR_A64UPPERATTR(pbha, uxn, xn, pxn, cont, dbm) | \
	 __PGTBLATTR_A64LOWERBLKATTR(nt, ng, af, sh, ap21, ns, attridx) | \
	 ((unsigned long)(paddr) & __UL(0x0000ffffffe00000)) | \
	 (mark))

#define __PGTBLATTR_A64BLK2M(pbha, uxn, xn, pxn, cont, dbm, nt, ng, af, sh, ap21, ns, attridx, paddr) \
	____PGTBLATTR_A64BLK2M(pbha, uxn, xn, pxn, cont, dbm, nt, ng, af, sh, ap21, ns, attridx, paddr, \
			       __PGTBLATTR_A64LV012BLK_MARK) \


#define ____PGTBLATTR_A64PAGE4K(pbha, uxn, xn, pxn, cont, dbm, ng, af, sh, ap21, ns, attridx, paddr, mark) \
	(__PGTBLATTR_A64UPPERATTR(pbha, uxn, xn, pxn, cont, dbm) | \
	 __PGTBLATTR_A64LOWERBLKATTR(0, ng, af, sh, ap21, ns, attridx) | \
	 ((unsigned long)(paddr) & __UL(0x0000fffffffff000)) | \
	 (mark))

#define __PGTBLATTR_A64PAGE4K(pbha, uxn, xn, pxn, cont, dbm, ng, af, sh, ap21, ns, attridx, paddr) \
	____PGTBLATTR_A64PAGE4K(pbha, uxn, xn, pxn, cont, dbm, ng, af, sh, ap21, ns, attridx, paddr, \
				__PGTBLATTR_A64LV3PAGE_MARK)

#define __PGTBLATTR_A64BLK1G_GP(pbha, uxn, xn, pxn, cont, dbm, gp, nt, ng, af, sh, ap21, ns, attridx, paddr)	\
	(__PGTBLATTR_A64UPPERATTR(pbha, uxn, xn, pxn, cont, dbm) | \
	 __PGTBLATTR_A64UPPERATTR_GP(gp) | \
	 __PGTBLATTR_A64LOWERBLKATTR(nt, ng, af, sh, ap21, ns, attridx) | \
	 ((unsigned long)(paddr) & __UL(0x0000ffffc0000000)) | \
	 __PGTBLATTR_A64LV012BLK_MARK)

#define __PGTBLATTR_A64BLK2M_GP(pbha, uxn, xn, pxn, cont, dbm, gp, nt, ng, af, sh, ap21, ns, attridx, paddr)	\
	(__PGTBLATTR_A64UPPERATTR(pbha, uxn, xn, pxn, cont, dbm) | \
	__PGTBLATTR_A64UPPERATTR_GP(gp) | \
	 __PGTBLATTR_A64LOWERBLKATTR(nt, ng, af, sh, ap21, ns, attridx) | \
	 ((unsigned long)(paddr) & __UL(0x0000ffffffe00000)) | \
	 __PGTBLATTR_A64LV012BLK_MARK)

#define __PGTBLATTR_A64PAGE4K_GP(pbha, uxn, xn, pxn, cont, dbm, gp, ng, af, sh, ap21, ns, attridx, paddr)	\
	(__PGTBLATTR_A64UPPERATTR(pbha, uxn, xn, pxn, cont, dbm) | \
	 __PGTBLATTR_A64UPPERATTR_GP(gp) | \
	 __PGTBLATTR_A64LOWERBLKATTR(0, ng, af, sh, ap21, ns, attridx) | \
	 ((unsigned long)(paddr) & __UL(0x0000fffffffff000)) | \
	 __PGTBLATTR_A64LV3PAGE_MARK)

#define __PGTBLATTR_STAGE2_A64BLK1G(pbha, xn, cont, dbm, nt, af, sh, s2ap, memattr, sw_stage2, sw_attr, paddr)	\
	(__PGTBLATTR_STAGE2_A64UPPERATTR(pbha, xn, cont, dbm, sw_stage2, sw_attr) | \
	 __PGTBLATTR_STAGE2_A64LOWERBLKATTR(nt, af, sh, s2ap, memattr) | \
	 ((unsigned long)(paddr) & __UL(0x0000ffffc0000000)) | \
	 __PGTBLATTR_A64LV012BLK_MARK)

#define __PGTBLATTR_STAGE2_A64BLK2M(pbha, xn, cont, dbm, nt, af, sh, s2ap, memattr, sw_stage2, sw_attr, paddr)	\
	(__PGTBLATTR_STAGE2_A64UPPERATTR(pbha, xn, cont, dbm, sw_stage2, sw_attr) | \
	 __PGTBLATTR_STAGE2_A64LOWERBLKATTR(nt, af, sh, s2ap, memattr) | \
	 ((unsigned long)(paddr) & __UL(0x0000ffffffe00000)) | \
	 __PGTBLATTR_A64LV012BLK_MARK)

#define __PGTBLATTR_STAGE2_A64PAGE4K(pbha, xn, cont, dbm, af, sh, s2ap, memattr, sw_stage2, sw_attr, paddr)	\
	(__PGTBLATTR_STAGE2_A64UPPERATTR(pbha, xn, cont, dbm, sw_stage2, sw_attr) | \
	 __PGTBLATTR_STAGE2_A64LOWERBLKATTR(0, af, sh, s2ap, memattr) | \
	 ((unsigned long)(paddr) & __UL(0x0000fffffffff000)) | \
	 __PGTBLATTR_A64LV3PAGE_MARK)

/*
 * CnP is controlled by amendation. No need to identify KERNEL and USER TTBR.
 */
#define __PGTBLATTR_DEFAULT_A64TTBRREG	__PGTBLATTR_A64TTBRREG(0, 0, 0)
#define __PGTBLATTR_DEFAULT_A64TBL512G	__PGTBLATTR_A64TBL512G(0, 0, 0, 0, 0)
#define __PGTBLATTR_DEFAULT_A64TBL1G	__PGTBLATTR_A64TBL1G(0, 0, 0, 0, 0)
#define __PGTBLATTR_DEFAULT_A64TBL2M	__PGTBLATTR_A64TBL2M(0, 0, 0, 0, 0)

#endif
