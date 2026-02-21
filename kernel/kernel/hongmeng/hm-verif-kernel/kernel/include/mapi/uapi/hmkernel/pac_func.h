/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: ptr auth defines
 * Author: Huawei OS Kernel Lab
 * Create: Sep 26 2024
 */
#ifndef MAPI_UAPI_HM_PACFUNC_H
#define MAPI_UAPI_HM_PACFUNC_H

#ifndef __ASSEMBLY__

#if defined(PAC_DFI_DATA) || defined(PAC_DFI_UCTX)

#define __PAC_MASK(bits) ((1UL << (bits)) - 1UL)
#define __PAC_SHIFT_BITS 32

static inline __u64 __pacga(__u64 context, __u64 data)
{
	__u64 res;
	asm volatile(
			"pacga  %0, %1, %2"
			: "=r"(res)
			: "r" (data), "r"(context)
			:
		    );
	return res >> __PAC_SHIFT_BITS;
}

static inline bool __pac_pacga_check_no_bugon(__u64 pac_cur, __u64 pac_old, __u32 bits)
{
	pac_cur &= __PAC_MASK(bits);
	pac_old &= __PAC_MASK(bits);
	if (pac_cur != pac_old) {
		return false;
	}
	return true;
}

#define __PAC_GEN_PAC_FIELD_BIT_VERIFIER(type, name, mask_bits, pac_get, pac_set, protected_data, protected_bit) \
	static inline void name##_bit_pacga(type *__pac_data) \
	{ \
		pac_set(__pac_data, __pacga((__u64)__pac_data, (__u64)((__pac_data)->protected_data & (protected_bit)))); \
	} \
	static inline bool name##_bit_autga(const type *__pac_data) \
	{ \
		return __pac_pacga_check_no_bugon(__pacga((__u64)__pac_data, \
			(__u64)((__pac_data)->protected_data & (protected_bit))), pac_get(__pac_data), mask_bits); \
	}

#endif /* PAC_DFI_DATA */

#endif  /* __ASSEMBLY__ */
#endif  /* MAPI_UAPI_HM_PACFUNC_H */
