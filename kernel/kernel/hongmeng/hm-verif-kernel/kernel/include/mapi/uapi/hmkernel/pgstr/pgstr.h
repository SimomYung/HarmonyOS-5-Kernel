/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: define pgstr_s in uapi
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 03 04:35:57 2022
 */

#ifndef MAPI_UAPI_HMKERNEL_PGSTR_PGSTR_H
#define MAPI_UAPI_HMKERNEL_PGSTR_PGSTR_H

#include <hmkernel/types.h>

#ifndef __KERNEL__

#include <vsync/atomic.h>

struct pgstr_s {
	vatomic32_t v;
};

#endif

/* there is a assert.c in kernel to make sure the number of macro type and enum type are same. */
#define __PGSTR_TYPE_NOMEM		0
#define __PGSTR_TYPE_NORMALPG		1
#define __PGSTR_TYPE_PGSTRTBL		2
#define __PGSTR_TYPE_KERNEL		3
#define __PGSTR_TYPE_CTRLMEM		4
#define __PGSTR_TYPE_IOMEM		5
#define __PGSTR_TYPE_RESERVED		6
#define __PGSTR_TYPE_BOOTDATA		7
#define __PGSTR_TYPE_CTRLMEM_VSPACE	8
#define __PGSTR_TYPE_CONTIG		9
#define __PGSTR_TYPE_KSHARE_UDATA	10
#define __PGSTR_TYPE_PGTBL_FL		11
#define __PGSTR_TYPE_PGTBL_ML1		12
#define __PGSTR_TYPE_PGTBL_ML2		13
#define __PGSTR_TYPE_PGTBL_ULL		14
#define __PGSTR_TYPE_PGTBL_KLL		15
#define __PGSTR_TYPE_IOMEM_RDONLY	16
#define __PGSTR_TYPE_IOMEM_NOMAP	17
#define __PGSTR_TYPE_IOMEM_VIRT		18
#define __PGSTR_TYPE_KOBJ_CONTAINER	19
#define __PGSTR_TYPE_SYSCONF		20
#define __PGSTR_TYPE_SYSMGRPG		21
#define __PGSTR_TYPE_SYSPGARRPG		22
#define __PGSTR_TYPE_SYSPCACHE		23
#define __PGSTR_TYPE_GLOBAL_ZERO	24
#define __PGSTR_TYPE_OPLOG		25
#define __PGSTR_TYPE_VM			26
#define __PGSTR_TYPE_ITERATOR		27
#define __PGSTR_TYPE_TLBGATHER		28
#define __PGSTR_TYPE_COSCHED		29
#define __PGSTR_TYPE_SUPERVS		30
#define __PGSTR_TYPE_CHANNELPG		31
#define __PGSTR_TYPE_REVENTSPG		32
#define __PGSTR_TYPE_HMPSF		33
#define __PGSTR_TYPE_KERNELBIN		34
#define __PGSTR_TYPE_HMPSFSH		35
#define __PGSTR_TYPE_ULWT		36
#define __PGSTR_TYPE_PCPINFO		37
#define __PGSTR_TYPE_TRACKINFO		38
#define __PGSTR_TYPE_LASTKLOG		39
#define __PGSTR_TYPE_KBOX		40
#define __PGSTR_TYPE_COREDUMP		41
#define __PGSTR_TYPE_PUBRESV		42
#define __PGSTR_TYPE_SNAPSHOT_NML	43
#define __PGSTR_TYPE_SNAPSHOT_CTG	44
#define __PGSTR_TYPE_IOFAST		45
#define __PGSTR_TYPE_IOFAST_HIGH	46
#define __PGSTR_TYPE_IOFAST_CTG		47
#define __PGSTR_TYPE_SMMU_PT		48
#define __PGSTR_TYPE_SMMU_PGD		49
#define __PGSTR_TYPE_VERITY_HASH	50
#define __PGSTR_TYPE_HIGHMEM		51
#define __PGSTR_TYPE_SIGNED_CODE	52
#define __PGSTR_TYPE_DM_CODE		53
#define __PGSTR_TYPE_POOL_PAGE		54
#define __PGSTR_TYPE_PROBE_PAGE		55
#define __PGSTR_TYPE_KLOADBIN		56
#define __PGSTR_TYPE_INVALID		57
#define __PGSTR_TYPE_NR			57

static inline __bool __pgstr_type_is_iomem(unsigned int type)
{
	return ((type == __PGSTR_TYPE_IOMEM) ||
			(type == __PGSTR_TYPE_IOMEM_RDONLY) ||
			(type == __PGSTR_TYPE_IOMEM_NOMAP) ||
			(type == __PGSTR_TYPE_IOMEM_VIRT) ||
			(type == __PGSTR_TYPE_VM));
}

static inline __bool __pgstr_type_is_smmu_pgtbl(unsigned int type)
{
	return ((type == __PGSTR_TYPE_SMMU_PT) ||
			(type == __PGSTR_TYPE_SMMU_PGD));
}
/*
 * Internal structure of pgstr
 * +-------------------------+-------------------+---+-------------------------+---------------------+
 * | 7 bits type (128 types) | 1 bit for locking | G |21 bits refcnt (2097152) | 2 bits for pgstrtbl |
 * +-------------------------+-------------------+---+-------------------------+---------------------+
 *
 * Note In some case(such as hi1383), it will map (2GB) Huge page,
 * so we set the refcnt to 21 bits to avoid overflow.
 */

#define __PGSTRV_MASK_TYPE	0xfe000000u
#define __PGSTRV_MASK_LOCK	0x01000000u
#define __PGSTRV_MASK_GUARD	0x00800000u
#define __PGSTRV_MASK_REFCNT	0x007ffffcu
#define __PGSTRV_MASK_PGSTRTBL	0x00000003u

#define __PGSTRV_SHIFT_TYPE	25u
#define __PGSTRV_SHIFT_LOCK	24u
#define __PGSTRV_SHIFT_GUARD	23u
#define __PGSTRV_SHIFT_REFCNT	2u
#define __PGSTRV_SHIFT_PGSTRTBL	0u

#define __pgstrv_extract_type_u32(v)	(((v) & __PGSTRV_MASK_TYPE) >> __PGSTRV_SHIFT_TYPE)

/*
 * In case when sysmgr and kernel shares vspace, give sysmgr more powerful pgstr
 * operations to allow it directly maintain refcnt.
 */

#define __PGSTRSUPEROP_MASK	0x3U
#define __PGSTRSUPEROP_MARK	0x3U

#define __PGSTRSUPEROP_DEC_LST_REFCNT	(0x10U | __PGSTRSUPEROP_MARK)
#define __PGSTRSUPEROP_INC_FST_REFCNT	(0x20U | __PGSTRSUPEROP_MARK)

/*
 * When switch pgstr type, encode transctrl into refcnt field of
 * pgstrv_new. nml2pgt use this to control 3 different argument
 * type:
 *   1. by default, a vaddress to pte of the last mapping and
 *      a vaddress of normal page are required. (va1, va2).
 *   2. If DIRECT_KLINEAR is selected, vaddress to pte can be omitted (va2)
 *   3. if ZERO_REFCNT is selected, a pa is require to let
 *      kernel check if the pgstr is pointing to the page.
 */
#define __PGSTRV_MASK_TRANSCTRL		__PGSTRV_MASK_REFCNT
#define __PGSTRV_SHIFT_TRANSCTRL	__PGSTRV_SHIFT_REFCNT
#define __PGSTRV_TRANSCTRL_NML2PGT_DIRECT_KLINEAR	1U
#define __PGSTRV_TRANSCTRL_NML2PGT_ZERO_REFCNT		2U

#define __PGSTRV_TRANSCTRL_PGT2NML_ZERO_REFCNT		1U

#endif
