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
#define __KOBJVS_RANGEOP_OP_REAPING		0x00000005

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
	__u64 varec;
	__u64 parec;
};

#define __CTRLMEM_KOBJ_VSPACE_RANGEOP_UNMAP_URECORD_PAREC_MASK		(~(__u64)(3UL))
#define __CTRLMEM_KOBJ_VSPACE_RANGEOP_UNMAP_URECORD_PAREC_PRESENT	((__u64)(1UL))
#define __CTRLMEM_KOBJ_VSPACE_RANGEOP_UNMAP_URECORD_PAREC_FRAGILE	((__u64)(2UL))

/* When present, use 6 bits to encode page order (1-64) */
#define __CTRLMEM_KOBJ_VSPACE_RANGEOP_UNMAP_URECORD_PAREC_MASK_P	(~(__u64)(0xfffUL))
#define __CTRLMEM_KOBJ_VSPACE_RANGEOP_UNMAP_URECORD_PAREC_SHIFT_P	(2)
#define __CTRLMEM_KOBJ_VSPACE_RANGEOP_UNMAP_URECORD_PAREC_ORDER_P	((__u64)(0x0f3UL))

#define __ctrlmem_kobj_vspace_rangeop_unmap_urecord_pa_from_parec(v)	\
	(((v)) & __CTRLMEM_KOBJ_VSPACE_RANGEOP_UNMAP_URECORD_PAREC_MASK_P)
#define __ctrlmem_kobj_vspace_rangeop_unmap_urecord_is_pa_present(v)	\
	____IS((v) & __CTRLMEM_KOBJ_VSPACE_RANGEOP_UNMAP_URECORD_PAREC_PRESENT)
#define __ctrlmem_kobj_vspace_rangeop_unmap_urecord_is_pa_fragile(v)	\
	____IS((v) & __CTRLMEM_KOBJ_VSPACE_RANGEOP_UNMAP_URECORD_PAREC_FRAGILE)
#define __ctrlmem_kobj_vspace_rangeop_unmap_urecord_order_from_present_parec(v)	\
	((((v)) & __CTRLMEM_KOBJ_VSPACE_RANGEOP_UNMAP_URECORD_PAREC_ORDER_P) >> \
	 __CTRLMEM_KOBJ_VSPACE_RANGEOP_UNMAP_URECORD_PAREC_SHIFT_P)

#define __ctrlmem_kobj_vspace_rangeop_unmap_urecord_size_from_present_parec(v)	\
	(1ULL << __ctrlmem_kobj_vspace_rangeop_unmap_urecord_order_from_present_parec(v))

#define __ctrlmem_kobj_vspace_rangeop_unmap_urecord_size_from_parec(v)	\
	(__ctrlmem_kobj_vspace_rangeop_unmap_urecord_is_pa_present(v) ? \
	 __ctrlmem_kobj_vspace_rangeop_unmap_urecord_size_from_present_parec(v) : __PAGE_SIZE)

/*
 * One urecord at most encode 1024 pages so use 10 bits is enough.
 * Since VA_BITS is 39 or 48, MSB of VAREC and PAREC are also useable if required.
 */
#define __CTRLMEM_KOBJ_VSPACE_RANGEOP_UNMAP_URECORD_VAREC_MASK	(~(__u64)(0xfffUL))
#define __CTRLMEM_KOBJ_VSPACE_RANGEOP_UNMAP_URECORD_VAREC_N	((__u64)(0x7ffUL))

#define __ctrlmem_kobj_vspace_rangeop_unmap_urecord_va_from_varec(v)	\
	((v) & __CTRLMEM_KOBJ_VSPACE_RANGEOP_UNMAP_URECORD_VAREC_MASK)
#define __ctrlmem_kobj_vspace_rangeop_unmap_urecord_n_from_varec(v)	\
	((v) & __CTRLMEM_KOBJ_VSPACE_RANGEOP_UNMAP_URECORD_VAREC_N)

/*
 * If result goes to kstack channel page lockable area, records[0].va should be this.
 */
#define __CTRLMEM_KOBJ_VSPACE_RANGEOP_UNMAP_URECORD_VAREC_INVAL	(~(__u64)(0ULL))

static inline void __ctrlmem_kobj_vspace_rangeop_unmap_urecord_advance(
		struct __ctrlmem_kobj_vspace_rangeop_unmap_urecord_s *x)
{
	unsigned long sz = __ctrlmem_kobj_vspace_rangeop_unmap_urecord_size_from_parec(x->parec);
	x->varec = (x->varec - 1UL) + sz;
	x->parec += sz;
}

#define __for_each_ctrlmem_kobj_vspace_rangeop_unmap_urecord(_vn, _arr, _n)	\
	for (unsigned int __##_vn##_i = 0U; __##_vn##_i < (_n); __##_vn##_i++)	\
	for (struct __ctrlmem_kobj_vspace_rangeop_unmap_urecord_s _vn = (_arr)[__##_vn##_i]; \
	     __ctrlmem_kobj_vspace_rangeop_unmap_urecord_n_from_varec((_vn).varec) != 0UL; \
	     __ctrlmem_kobj_vspace_rangeop_unmap_urecord_advance(&(_vn))) \

#define __ctrlmem_kobj_vspace_rangeop_unmap_urecord_size_of_urec(_urec)	\
	(__ctrlmem_kobj_vspace_rangeop_unmap_urecord_n_from_varec((_urec)->varec) * \
	 __ctrlmem_kobj_vspace_rangeop_unmap_urecord_size_from_parec((_urec)->parec))

#define __ctrlmem_kobj_vspace_rangeop_unmap_urecord_next_va(_urec)	\
	(__ctrlmem_kobj_vspace_rangeop_unmap_urecord_va_from_varec((_urec)->varec) + \
	 __ctrlmem_kobj_vspace_rangeop_unmap_urecord_size_of_urec(_urec))

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
