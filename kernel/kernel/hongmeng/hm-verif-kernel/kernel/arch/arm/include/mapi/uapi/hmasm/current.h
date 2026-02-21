/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: ARM use tpidrurw to deliver "current-xxx" stuffs
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 20 16:29:11 2024
 */

#ifndef ARM_MAPI_UAPI_HMASM_CURRENT_H
#define ARM_MAPI_UAPI_HMASM_CURRENT_H

#include <hmkernel/types.h>

#define __ARM_TPIDRURW_CPUID_MASK	0xff
#define __ARM_TPIDRURW_CPUID_SHIFT	0

#define __define_arm_tpidrurwv_op(F, f, t) \
static inline __attribute__((always_inline)) t \
__arm_tpidrurwv_extract_##f(__u32 tpidrurwv) \
{ \
    return (t)((tpidrurwv & __ARM_TPIDRURW_##F##_MASK) >> __ARM_TPIDRURW_##F##_SHIFT); \
} \
static inline __attribute__((always_inline)) void \
__arm_tpidrurwv_set_##f(__u32 *tpidrurwv, t fv) \
{ \
    *tpidrurwv = (*tpidrurwv & ~(__u32)(__ARM_TPIDRURW_##F##_MASK)) | \
	((__u32)(fv) << __ARM_TPIDRURW_##F##_SHIFT); \
}

__define_arm_tpidrurwv_op(CPUID, cpuid, unsigned int)

#ifndef __arch_current_cpuid

# include <hmasm/sysreg.h>
# define __arm_arch_current_cpuid()	__arm_tpidrurwv_extract_cpuid(__sysreg_read_tpidrurw())
# ifdef __HOST_UT__
#  define __arch_current_cpuid()	mock__arch_current_cpuid()
# else
#  define __arch_current_cpuid()	__arm_arch_current_cpuid()
#endif /* __HOST_UT__ */

#endif

#endif
