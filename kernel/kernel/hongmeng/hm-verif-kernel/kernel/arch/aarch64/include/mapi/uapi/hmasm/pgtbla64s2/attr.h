/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Attribute of arm64 stage2 table
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 23 22:24:45 2023
 */

/*
 * arm manual:
 * https://developer.arm.com/documentation/ddi0487/ia
 * Document number ID081822, Document version I.a
 *
 * smmu manual:
 * https://developer.arm.com/documentation/ihi0070/ea
 * Document number ARM IHI 0070, Document version E.a
 */

#ifndef AARCH64_MAPI_UAPI_HMASM_PGTBLA64S2_ATTR_H
#define AARCH64_MAPI_UAPI_HMASM_PGTBLA64S2_ATTR_H

#include <hmkernel/const.h>

/*
 * Check chapter (D17.2.148 TTBR0_EL2, Translation Table Base Register 0 (EL2)) armv9a manual(DDI0487I.a)
 *
 * In smmuv3, register SMMU_STRTAB_BASE record the stream table base address:
 * Check chapter (3.3 Data structures and translation procedure) smmuv3 manual(ARMIHI0070 E.a)
 */
#define TTBREL2_BADDR_extract(v)	((v) & 0x0000fffffffffffeUL)
#define TTBREL2_CnP_extract(v)		((v) & 0x1U)

#define TTBREL2_BADDR(x)	((x) & 0x0000fffffffffffeUL)
#define TTBREL2_CnP(x)	(((x) != 0) ? 1UL : 0UL)

#define __PGTBLATTR_A64S2TTBRREG_MASK_BADDR	0x0000fffffffffffe
#define __PGTBLATTR_A64S2TTBRREG_MASK_CNP	0x1

#define __PGTBLATTR_A64S2TTBRREG_BADDR(n)	((unsigned long)(n) & __UL(__PGTBLATTR_A64S2TTBRREG_MASK_BADDR))
#define __PGTBLATTR_A64S2TTBRREG_CNP(n)		((unsigned long)(n) & __UL(1))

/*
 * Check chapter (D8.3.2 Block descriptor and Page descriptor formats) armv9a manual(DDI0487I.a)
 * Check Figure  (D8-17  Stage 2 attribute fields in Block and Page descriptors) armv9a manual(DDI0487I.a)
 * Check chapter (D8.5   Memory region attributes) armv9a manual(DDI0487I.a)
 */
#define __PGTBLATTR_A64S2LV012BLK_MARK_MASK		0x3
#define __PGTBLATTR_A64S2LV012BLK_UPPERATTR_MASK	0xfff0000000000000	/* 0xfff << 52 */
#define __PGTBLATTR_A64S2LV012BLK_GP_MASK		0x0004000000000000	/* 0x1 << 50 */
#define __PGTBLATTR_A64S2LV012BLK_LOWERATTR_MASK	0x0000000000000ffc
#define __PGTBLATTR_A64S2LV012BLK_MARK			__UL(0x1)

#define __PGTBLATTR_A64S2UPPERBLKATTR_PBHA_MASK		0x7800000000000000	/* 0xf << 59 */
#define __PGTBLATTR_A64S2UPPERBLKATTR_S2_XN_MASK	0x0060000000000000	/* 3 << 53 */
#define __PGTBLATTR_A64S2UPPERBLKATTR_CONT_MASK		0x0010000000000000	/* 1 << 52 */
#define __PGTBLATTR_A64S2UPPERBLKATTR_DBM_MASK		0x0008000000000000	/* 1 << 51 */

#define __PGTBLATTR_A64S2UPPERATTR_PBHA(n)	(((unsigned long)(n) & __UL(0xf)) << __U(59))
#define __PGTBLATTR_A64S2UPPERATTR_SW_ATTR(n)	(((unsigned long)(n) & __UL(0xf)) << __U(55))
#define __PGTBLATTR_A64S2UPPERATTR_S2_XN(n)	(((unsigned long)(n) & __UL(0x3)) << __U(53))
#define __PGTBLATTR_A64S2UPPERATTR_CONT(n)	(((unsigned long)(n) & __UL(0x1)) << __U(52))
#define __PGTBLATTR_A64S2UPPERATTR_DBM(n)		(((unsigned long)(n) & __UL(0x1)) << __U(51))
#define __PGTBLATTR_A64S2UPPERATTR_SW_STAGE2(n)		(((unsigned long)(n) & __UL(0x1)) << __U(50))

#define __PGTBLATTR_A64S2LOWERBLKATTR_NT_MASK		0x10000			/* 1 << 16 */
#define __PGTBLATTR_A64LOWERBLKATTR_FNXS_MASK		0x800			/* 1 << 11 */
#define __PGTBLATTR_A64S2LOWERBLKATTR_SH_MASK		0x300			/* 0x3 << 8 */
#define __PGTBLATTR_A64S2LOWERBLKATTR_S2AP_MASK		0xc0			/* 0x3 << 6 */
#define __PGTBLATTR_A64S2LOWERBLKATTR_MEMATTR_MASK 	0x3c			/* 0xf << 2 */

#define __PGTBLATTR_A64S2LOWERBLKATTR_NT(n)		(((unsigned long)(n) & __UL(1)) << __U(16))
#define __PGTBLATTR_A64LOWERBLKATTR_FNXS(n)		(((unsigned long)(n) & __UL(1)) << __U(11))
#define __PGTBLATTR_A64S2LOWERBLKATTR_AF(n)		(((unsigned long)(n) & __UL(1)) << __U(10))
#define __PGTBLATTR_A64S2LOWERBLKATTR_SH(n)		(((unsigned long)(n) & __UL(3)) << __U(8))
#define __PGTBLATTR_A64S2LOWERBLKATTR_S2AP(n)		(((unsigned long)(n) & __UL(3)) << __U(6))
#define __PGTBLATTR_A64S2LOWERBLKATTR_MEMATTR(n)	(((unsigned long)(n) & __UL(0xf)) << __U(2))

#define __PGTBLATTR_A64S2LV3PAGE_MARK_MASK	__UL(0x3)
#define __PGTBLATTR_A64S2LV3PAGE_MARK		__UL(0x3)

/* A64S2 page table sharebility */
#define __PGTBLATTR_A64S2LOWERBLKATTR_SH_NON_SHAREABLE		0
#define __PGTBLATTR_A64S2LOWERBLKATTR_SH_OUTER_SHAREABLE	2
#define __PGTBLATTR_A64S2LOWERBLKATTR_SH_INNER_SHAREABLE	3

#define __PGTBLATTR_STAGE2_A64S2UPPERATTR(pbha, xn, cont, dbm, sw_stage2, sw_attr) \
	(__PGTBLATTR_A64S2UPPERATTR_PBHA(pbha) |	\
	 __PGTBLATTR_A64S2UPPERATTR_S2_XN(xn) |	\
	 __PGTBLATTR_A64S2UPPERATTR_CONT(cont) |	\
	 __PGTBLATTR_A64S2UPPERATTR_DBM(dbm) | \
	 __PGTBLATTR_A64S2UPPERATTR_SW_STAGE2(sw_stage2) | \
	 __PGTBLATTR_A64S2UPPERATTR_SW_ATTR(sw_attr))

#define __PGTBLATTR_STAGE2_A64S2LOWERBLKATTR(nt, fnxs, af, sh, s2ap, memattr)	\
	(__PGTBLATTR_A64S2LOWERBLKATTR_NT(nt) | \
	 __PGTBLATTR_A64LOWERBLKATTR_FNXS(fnxs) | \
	 __PGTBLATTR_A64S2LOWERBLKATTR_AF(af) | \
	 __PGTBLATTR_A64S2LOWERBLKATTR_SH(sh) | \
	 __PGTBLATTR_A64S2LOWERBLKATTR_S2AP(s2ap) | \
	 __PGTBLATTR_A64S2LOWERBLKATTR_MEMATTR(memattr))

#define __PGTBLATTR_A64S2TBL512G	(__PGTBLATTR_A64LV012TBL_MARK)
#define __PGTBLATTR_A64S2TBL1G		(__PGTBLATTR_A64LV012TBL_MARK)
#define __PGTBLATTR_A64S2TBL2M		(__PGTBLATTR_A64LV012TBL_MARK)
#define __PGTBLATTR_A64S2TTBRREG(baddr, cnp) \
		(__PGTBLATTR_A64S2TTBRREG_BADDR(baddr) | \
		 __PGTBLATTR_A64S2TTBRREG_CNP(cnp))

#define __PGTBLATTR_STAGE2_A64S2BLK1G(pbha, xn, cont, dbm, nt, fnxs, af, sh, s2ap, memattr, sw_stage2, sw_attr, paddr) \
	(__PGTBLATTR_STAGE2_A64S2UPPERATTR(pbha, xn, cont, dbm, sw_stage2, sw_attr) | \
	 __PGTBLATTR_STAGE2_A64S2LOWERBLKATTR(nt, fnxs, af, sh, s2ap, memattr) | \
	 ((unsigned long)(paddr) & __UL(0x0000ffffc0000000)) | \
	 __PGTBLATTR_A64S2LV012BLK_MARK)

#define __PGTBLATTR_STAGE2_A64S2BLK2M(pbha, xn, cont, dbm, nt, fnxs, af, sh, s2ap, memattr, sw_stage2, sw_attr, paddr) \
	(__PGTBLATTR_STAGE2_A64S2UPPERATTR(pbha, xn, cont, dbm, sw_stage2, sw_attr) | \
	 __PGTBLATTR_STAGE2_A64S2LOWERBLKATTR(nt, fnxs, af, sh, s2ap, memattr) | \
	 ((unsigned long)(paddr) & __UL(0x0000ffffffe00000)) | \
	 __PGTBLATTR_A64S2LV012BLK_MARK)

#define __PGTBLATTR_STAGE2_A64S2PAGE4K(pbha, xn, cont, dbm, fnxs, af, sh, s2ap, memattr, sw_stage2, sw_attr, paddr) \
	(__PGTBLATTR_STAGE2_A64S2UPPERATTR(pbha, xn, cont, dbm, sw_stage2, sw_attr) | \
	 __PGTBLATTR_STAGE2_A64S2LOWERBLKATTR(0, fnxs, af, sh, s2ap, memattr) | \
	 ((unsigned long)(paddr) & __UL(0x0000fffffffff000)) | \
	 __PGTBLATTR_A64S2LV3PAGE_MARK)

/*
 * CnP is controlled by amendation. No need to identify KERNEL and USER TTBR.
 */
#define __PGTBLATTR_DEFAULT_A64S2TTBRREG	__PGTBLATTR_A64S2TTBRREG(0, 0)
#define __PGTBLATTR_DEFAULT_A64S2TBL512G	__PGTBLATTR_A64S2TBL512G
#define __PGTBLATTR_DEFAULT_A64S2TBL1G		__PGTBLATTR_A64S2TBL1G
#define __PGTBLATTR_DEFAULT_A64S2TBL2M		__PGTBLATTR_A64S2TBL2M

#endif
