/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: ARM64 use tpidrro to deliver "current-xxx" stuffs
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 20 17:18:25 2024
 */

#ifndef A64_MAPI_UAPI_HMASM_CURRENT_H
#define A64_MAPI_UAPI_HMASM_CURRENT_H

/*
 * top 8 bits used for CPUID
 * NOTE that only high 32 bits can be encoded for current.
 * low 32 bits is used for arm compat mode.
 */
#include <hmasm/types.h>

#define __A64_TPIDRRO_CPUID_SHIFT 56
#define __A64_TPIDRRO_CPUID_MASK  (((1ULL << 8) - 1ULL) << __A64_TPIDRRO_CPUID_SHIFT)

/*
 * See store_regs_compat in arch/aarch64/kernel/entry.S, keep
 * consistency when changing __A64_TPIDRRO_COMPAT_TLS_MASK
 */
#define __A64_TPIDRRO_COMPAT_TLS_SHIFT 0
#define __A64_TPIDRRO_COMPAT_TLS_MASK  0xffffffffUL

#define __define_a64_tpidrro_op(F, f, t)						\
static inline __attribute__((always_inline)) t						\
__a64_tpidrrov_extract_##f(__u64 tpidrrov)						\
{											\
	return (t)((tpidrrov & __A64_TPIDRRO_##F##_MASK) >> __A64_TPIDRRO_##F##_SHIFT);	\
}											\
static inline __attribute__((always_inline)) void					\
__a64_tpidrrov_set_##f(__u64 *tpidrrov, t fv)						\
{											\
	*tpidrrov = (*tpidrrov & ~(__u64)(__A64_TPIDRRO_##F##_MASK)) |			\
	((__u64)(fv) << __A64_TPIDRRO_##F##_SHIFT);					\
}

__define_a64_tpidrro_op(CPUID, cpuid, unsigned int)
__define_a64_tpidrro_op(COMPAT_TLS, compat_tls, unsigned int)

#ifndef __arch_current_cpuid

#include <hmasm/sysreg_utils.h>

#define __a64_current_cpuid() __a64_tpidrrov_extract_cpuid(__aarch64_read_sysreg("tpidrro_el0"))

#ifdef __HOST_UT__
#define __arch_current_cpuid() mock__arch_current_cpuid()
#else
#define __arch_current_cpuid() __a64_current_cpuid()
#endif /* __HOST_UT__ */

#endif

#include <hmasm/current_compat.h>

#endif
