/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: ARM page table hints - uapi
 * Author: Huawei OS Kernel Lab
 * Create: Sat Apr 08 19:58:14 2023
 */
#ifndef AARCH64_MAPI_UAPI_ASM_HINTS_H
#define AARCH64_MAPI_UAPI_ASM_HINTS_H

#include <hmasm/pgtbl/pgtblattr.h>
#include <hmkernel/mm/pgtable_layer.h>
#include <hmkernel/mm/pgtable.h>
#include <hmasm/mm/pgtable.h>

static inline __always_inline __u32
__pgtblattr_hint_to_pbha(__u64 hints)
{
	return (__u32)(hints >> PGTABLE_ATTR_A64_PBHA_SHIFT) & 0xFUL;
}

static inline __always_inline void
__pgtblhdlr_a64_extract_lastlv_block_keyattr_stage1(__u64 ap21, __u64 attridx, struct __pgtable_key_attr_s *keyattr)
{
	keyattr->kreadable = __true;
	keyattr->kwritable = ap21 == 0 || ap21 == 1;
	keyattr->ureadable = ap21 == 1 || ap21 == 3;
	keyattr->uwritable = ap21 == 1;

	switch (attridx) {
	default:
	case __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_NORMAL:
		break;
	case __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_NORMAL_NC:
		keyattr->is_nocache = __true;
		break;
	case __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_DEVICE_nGnRnE:
	case __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_DEVICE_nGnRE:
	case __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_DEVICE_GRE:
		keyattr->is_device = __true;
		break;
	}
}

static inline __always_inline void
__pgtblhdlr_a64_extract_lastlv_block_keyattr_stage2(__u64 s2ap, __u64 xn, struct __pgtable_key_attr_s *keyattr)
{
	/*
	 * armv9a manual: https://developer.arm.com/documentation/ddi0487/ia
	 * Check (D8-40 Data access permissions for a stage 2 translation) armv9a manual(DDI0487I.a)
	 */
	keyattr->kreadable = s2ap == 1 || s2ap == 3;
	keyattr->kwritable = s2ap == 2 || s2ap == 3;
	keyattr->ureadable = keyattr->kreadable;
	keyattr->uwritable = keyattr->kwritable;

	keyattr->kexecutable = xn == 0 || xn == 3;
	keyattr->uexecutable = xn == 0 || xn == 1;
}

static inline __always_inline void
__pgtblhdlr_a64_extract_lastlv_block_keyattr(__u64 e, struct __pgtable_key_attr_s *keyattr)
{
	__u64 ap21;
	__u64 s2ap;
	__u64 xn;
	__u64 attridx;
	__bool kmap = __false;
	if ((((__u64)(e) >> 11U) & 0x1UL) == 0U) {
		kmap = __true;
	}
	__bool stage2 = ((((__u64)(e) >> 50U) & 0x1UL) != 0U);
	if (!kmap && (((__u64)(e) >> 54U) & 0x1UL) == 0U) {
		keyattr->uexecutable = __true;
	}

	if (stage2) {
		s2ap = (((__u64)(e) >> 6U)  & 0x3UL);
		xn = (((__u64)(e) >> 53U) & 0x3UL);
		__pgtblhdlr_a64_extract_lastlv_block_keyattr_stage2(s2ap, xn, keyattr);
	} else {
		if (kmap && (((__u64)(e) >> 53U) & 0x1UL) == 0U) {
			keyattr->kexecutable = __true;
		}
		ap21 = (((__u64)(e) >> 6U)  & 0x3UL);
		attridx = (((__u64)(e) >> 2U)  & 0x7UL);
		__pgtblhdlr_a64_extract_lastlv_block_keyattr_stage1(ap21, attridx, keyattr);
	}
}

static inline __always_inline void
__pgtblhdlr_extract_lastlv_keyattr(enum __pgtblhdlr_pgentry_type_e type, struct __pgtblhdlr_pgentry_s entry,
				   struct __pgtable_key_attr_s *keyattr)
{
	(void) type;
	if (type == __PGTBLHDLR_PGENTRY_TYPE_A64BLK1G ||
	    type == __PGTBLHDLR_PGENTRY_TYPE_A64BLK2M ||
	    type == __PGTBLHDLR_PGENTRY_TYPE_A64PAGE4K) {
		__pgtblhdlr_a64_extract_lastlv_block_keyattr(entry.u.a64pgentry->v, keyattr);
	}
}

#endif
