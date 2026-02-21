/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: CPU features for uapi
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 13 15:43:49 2020
 */
#ifndef AARCH64_UAPI_ASM_CPUFEATURES_H
#define AARCH64_UAPI_ASM_CPUFEATURES_H

#include <hmasm/types.h>
#include <hmkernel/types.h>

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define ARCH_PLATFORM           ("aarch64_be")
#else
#define ARCH_PLATFORM           ("aarch64")
#endif

#define __PE_MAINID_IMPLEMENTER(v)		(((__u32)(v) >> 24u) & ((1u << 8u) - 1u))
#define __PE_MAINID_VARIANT(v)			(((__u32)(v) >> 20u) & ((1u << 4u) - 1u))
#define __PE_MAINID_ARCHITECTURE(v)		(((__u32)(v) >> 16u) & ((1u << 4u) - 1u))
#define __PE_MAINID_PARTNUM(v)			(((__u32)(v) >> 4u) & ((1u << 12u) - 1u))
#define __PE_MAINID_REVISION(v)			((__u32)(v) & ((1u << 4u) - 1u))

#ifdef CONFIG_ARM64_BTI_USER
#define ID_AA64PFR1_EL1_BT_MASK 0xf
#define ID_AA64PFR1_EL1_BT 0x1
#define cpu_read_sysreg(name)                              \
({                                                         \
	__u64 __v;                                         \
	asm volatile ("mrs %0, " name "\n" : "=r" (__v));  \
	__v;                                               \
})
static inline bool system_supports_bti(void)
{
	__u32 value_aa64pfr1_el1 = cpu_read_sysreg("ID_AA64PFR1_EL1");
	__u32 val_bp = value_aa64pfr1_el1 & ID_AA64PFR1_EL1_BT_MASK;
	return (val_bp == ID_AA64PFR1_EL1_BT);
}
#endif
#endif
