/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Definition of cpu features for aarch64
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 24 11:23:36 2020
 */
#ifndef AARCH64_ASM_CPU_CPUFEATURES_H
#define AARCH64_ASM_CPU_CPUFEATURES_H

#include <hmkernel/types.h>
#include <hmkernel/compiler.h>

#ifndef HMKERNEL_CPUFEATURES_H
#error "can only be referenced by hmkernel/cpufeatures.h"
#endif

#ifdef CONFIG_AARCH64_ICACHE_ALIASING_NON_UNIFORM
JAM_INLINE bool __aarch64_is_icache_aliasing(void);
__cpu_feature(is_icache_aliasing, __aarch64_is_icache_aliasing())
#else
#ifdef CONFIG_AARCH64_ICACHE_ALIASING_ON
cpu_feature_on(is_icache_aliasing)
#else
cpu_feature_off(is_icache_aliasing)
#endif
#endif

#ifdef CONFIG_AARCH64_CACHE_HAS_IDC
cpu_feature_on(has_cache_idc)
#elif defined(CONFIG_AARCH64_CACHE_NO_IDC)
cpu_feature_off(has_cache_idc)
#else
JAM_INLINE bool __aarch64_has_cache_idc(void);
__cpu_feature(has_cache_idc, __aarch64_has_cache_idc())
#endif

#ifdef CONFIG_AARCH64_CACHE_HAS_DIC
cpu_feature_on(has_cache_dic)
#elif defined(CONFIG_AARCH64_CACHE_NO_DIC)
cpu_feature_off(has_cache_dic)
#else
JAM_INLINE bool __aarch64_has_cache_dic(void);
__cpu_feature(has_cache_dic, __aarch64_has_cache_dic())
#endif

#ifdef CONFIG_COMPAT
int arch_cpu_number_support_aarch32(void);
#endif

void arch_cpu_features_init(void);
void arch_cpu_features_init_percpu(unsigned int cpu);
unsigned long arch_cpu_get_elf_hwcap(void);

struct arch_regs;
int arch_do_emulate_mrs(struct arch_regs *regs, u32 sys_reg, u32 rt);

JAM_INLINE bool arch_cpu_features_has_a64ttl(void);

u32 hide_cpuid_id(u32 midr);

bool this_cpu_has_cap(unsigned int cap);

#ifndef __HOST_LLT__
static inline u32 arch_cpu_features_pe_mainid(void)
{
	u32 main_id;
	asm volatile (
		"mrs %0, midr_el1\n"
		: "=r"(main_id)
		:
		: "memory");
	return main_id;
}
#else
static inline u32 arch_cpu_features_pe_mainid(void)
{
	return 0;
}
#endif /* __HOST_LLT__ */
#endif
