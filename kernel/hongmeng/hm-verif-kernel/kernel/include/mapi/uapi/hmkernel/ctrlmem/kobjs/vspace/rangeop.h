/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Rangeop is kobj_vspace method with va-range as argument - uapi
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 15 23:00:23 2023
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_VSPACE_RANGEOP_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_VSPACE_RANGEOP_H

#include <hmasm/page.h>
#include <hmasm/memory.h>
#include <hmkernel/types.h>
#include <hmkernel/strict.h>
#include <hmkernel/mm/kstack.h>

struct __ctrlmem_kobj_vspace_rangeop_op_s {
	__u32 op;
};

/* when write to rangeop_op, 'value' is the operation to be done. */
#define __KOBJVS_RANGEOP_OP_MADVFREE		0x00000000
#define __KOBJVS_RANGEOP_OP_MUNMAP		0x00000001
#define __KOBJVS_RANGEOP_OP_MPROTECT		0x00000002
#define __KOBJVS_RANGEOP_OP_VSCPY		0x00000003
#define __KOBJVS_RANGEOP_OP_CLEAR_REFS		0x00000004

#define __KOBJVS_RANGEOP_VAL_OP_MASK		0xe0000000
#define __KOBJVS_RANGEOP_VAL_OP_SHIFT		29

#define __KOBJVS_RANGEOP_VAL_FLAGS_ANON		0x4
#define __KOBJVS_RANGEOP_VAL_FLAGS_PMPROTECT	0x2
#define __KOBJVS_RANGEOP_VAL_FLAGS_CHANNELPG	0x1
#define __KOBJVS_RANGEOP_VAL_FLAGS_MASK		0x1f000000
#define __KOBJVS_RANGEOP_VAL_FLAGS_SHIFT	24

#define __KOBJVS_RANGEOP_VAL_MODELID_MASK	0x00ff0000
#define __KOBJVS_RANGEOP_VAL_MODELID_SHIFT	16

#define __KOBJVS_RANGEOP_VAL_NRPTES_MASK	0x0000ffff
#define __KOBJVS_RANGEOP_VAL_NRPTES_SHIFT	0

#define __kobjvs_rangeop_mkval(x, r, m, n)	(((__U(__KOBJVS_RANGEOP_OP_##x) << __KOBJVS_RANGEOP_VAL_OP_SHIFT) & \
										   __KOBJVS_RANGEOP_VAL_OP_MASK) | \
						 (((unsigned int)(r) << __KOBJVS_RANGEOP_VAL_FLAGS_SHIFT) & \
									__KOBJVS_RANGEOP_VAL_FLAGS_MASK) | \
						 (((unsigned int)(m) << __KOBJVS_RANGEOP_VAL_MODELID_SHIFT) & \
									__KOBJVS_RANGEOP_VAL_MODELID_MASK) | \
						 (((unsigned int)(n) << __KOBJVS_RANGEOP_VAL_NRPTES_SHIFT) & \
									__KOBJVS_RANGEOP_VAL_NRPTES_MASK))

struct __ctrlmem_kobj_vspace_rangeop_unmap_urecord_s {
	__u64 va;
	__u64 parec;
};

#define __CTRLMEM_KOBJ_VSPACE_RANGEOP_UNMAP_URECORD_PAREC_MASK	(~(__u64)(3UL))
#define __CTRLMEM_KOBJ_VSPACE_RANGEOP_UNMAP_URECORD_PAREC_PRESENT	((__u64)(1UL))
#define __CTRLMEM_KOBJ_VSPACE_RANGEOP_UNMAP_URECORD_PAREC_FRAGILE	((__u64)(2UL))

#define __ctrlmem_kobj_vspace_rangeop_unmap_urecord_pa_from_parec(v)	\
	(((v)) & __CTRLMEM_KOBJ_VSPACE_RANGEOP_UNMAP_URECORD_PAREC_MASK)
#define __ctrlmem_kobj_vspace_rangeop_unmap_urecord_is_pa_present(v)	\
	____IS((v) & __CTRLMEM_KOBJ_VSPACE_RANGEOP_UNMAP_URECORD_PAREC_PRESENT)
#define __ctrlmem_kobj_vspace_rangeop_unmap_urecord_is_pa_fragile(v)	\
	____IS((v) & __CTRLMEM_KOBJ_VSPACE_RANGEOP_UNMAP_URECORD_PAREC_FRAGILE)

/*
 * If result goes to kstack channel page lockable area, records[0].va should be this.
 */
#define __CTRLMEM_KOBJ_VSPACE_RANGEOP_UNMAP_VA_INVAL	(~(__u64)(0ULL))

struct __ctrlmem_kobj_vspace_rangeop_unmap_urecords_page_s {
	struct __ctrlmem_kobj_vspace_rangeop_unmap_urecord_s urecords[
		__PAGE_SIZE / sizeof(struct __ctrlmem_kobj_vspace_rangeop_unmap_urecord_s)];
};

/* Rangeop for vspace copy */
#define __CTRLMEM_KOBJ_VSPACE_RANGEOP_VSCPY_RARG_NR	1

#define __CTRLMEM_KOBJ_VSPACE_RANGEOP_VSCPY_VA_INVAL	(~(__u64)(0ULL))

struct __ctrlmem_kobj_vspace_rangeop_vscpy_urecord_s {
	__u64 va;
	__u64 pa;
};

/* PT_NR + 1 for recording va_next */
#define __CTRLMEM_KOBJ_VSPACE_RANGEOP_VSCPY_USED_RECORDS	\
		(ALIGN_UP((__CTRLMEM_KOBJ_VSPACE_RANGEOP_VSCPY_RARG_NR + 1) * sizeof(void *), \
			  sizeof(struct __ctrlmem_kobj_vspace_rangeop_vscpy_urecord_s)) / \
		sizeof(struct __ctrlmem_kobj_vspace_rangeop_vscpy_urecord_s))

#define __CTRLMEM_KOBJ_VSPACE_RANGEOP_VSCPY_USED_VANEXT_P(records)	\
		(&((__vaddr_t *)(records))[__CTRLMEM_KOBJ_VSPACE_RANGEOP_VSCPY_RARG_NR])

#endif
