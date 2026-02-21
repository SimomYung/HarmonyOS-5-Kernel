/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Definition of cpu features for aarch64
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 24 11:23:36 2020
 */
#ifndef AARCH64_HMASM_CPU_CPUFEATURES_H
#define AARCH64_HMASM_CPU_CPUFEATURES_H

#include <hmasm/sysreg.h>
#include <hmkernel/types.h>
#include <hmkernel/compiler.h>
#include <hmkernel/bitops/bitops.h>

/*
 * The safe value for the field is dependent by the architecture.
 * So the features are classified into 4 types:
 * - LOWER_SAFE: The lowest value of all the CPUs is chosen.
 * - HIGHER_SAFE: The highest value of all the CPUs is choose.
 * - HIGHER_OR_ZERO_SAFE: The 0 is biggest.
 * - EXACT: All CPUs should have the same value, otherwise the safe
 *          vaule specified in the table is choose.
 */
enum feat_type {
	FEAT_LOWER_SAFE,		/* Smaller value is safe */
	FEAT_HIGHER_SAFE,		/* Bigger value is safe */
	FEAT_HIGHER_OR_ZERO_SAFE,	/* Bigger value is safe, but 0 is biggest */
	FEAT_EXACT			/* Use a predefined safe value if not meet */
};

#define FEAT_STRICT	true	/* mismatch check strict matching required */
#define FEAT_NONSTRICT	false	/* mismatch check ignored */

#define FEAT_SIGNED	true	/* value is signed */
#define FEAT_UNSIGNED	false	/* value is unsigned */

#define FEAT_VISIBLE	true	/* feature is visible to the user */
#define FEAT_HIDDEN	false	/* feature is hidden from the user */

#define FEAT_VISIBLE_IF_IS_ENABLED(config) \
	(IS_ENABLED(config) ? FEAT_VISIBLE : FEAT_HIDDEN)

/* describe the feature bits in the feature register */
struct aarch64_ftr_bits {
	bool sign;
	bool visible; /* is user visible */
	bool strict;  /* strict matching required */
	enum feat_type type;
	u8 shift;
	u8 width;
	s64 safe_val; /* safe value for FEAT_EXACT features */
};

/* describe one feature register */
struct aarch64_ftr_reg {
	const char *name; /* reg name */
	u64 strict_mask;  /* fileds should match across all CPUs */
	u64 user_mask;    /* user visible fileds */
	u64 sys_val;      /* system view of the reg value */
	u64 user_val;     /* user view of the reg value */
	const struct aarch64_ftr_bits *ftr_bits; /* all reg fileds */
};

#define __cpu_feature(f, v)	\
static inline bool arch_cpu_feature_##f(void) { return v; }

#define cpu_feature_on(f) \
	__cpu_feature(f, true)
#define cpu_feature_off(f) \
	__cpu_feature(f, false)

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

struct aarch64_cpuinfo_s;
void arch_cpufeature_init(const struct aarch64_cpuinfo_s *info);
void arch_cpufeature_update(unsigned int cpu, const struct aarch64_cpuinfo_s *info,
			    struct aarch64_cpuinfo_s *boot);

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

static inline int __const
cpufeature_extract_signed_field_width(u64 feature, unsigned int field, unsigned int width)
{
	u64 ret = (feature << (64U - width - field)) >> (64U - width);
	if ((ret & (1ULL << (width - 1U))) != 0) {
		u64 mask = ~((1ULL << width) - 1ULL);
		ret |= mask;
	}
	return (s32)(s64)ret;
}

static inline unsigned int __const
cpufeature_extract_unsigned_field_width(u64 feature, unsigned int field, unsigned int width)
{
	return (u32)((feature << (64U - width - field)) >> (64U - width));
}

static inline int __const
cpufeature_extract_field_width(u64 feature, unsigned int field, unsigned int width, bool sign)
{
	return (sign) ?
		cpufeature_extract_signed_field_width(feature, field, width) :
		(int)cpufeature_extract_unsigned_field_width(feature, field, width);
}

static inline int __const
cpufeature_extract_signed_field(u64 feature, unsigned int field)
{
	return cpufeature_extract_signed_field_width(feature, field, 4U);
}

static inline unsigned int __const
cpufeature_extract_unsigned_field(u64 feature, unsigned int field)
{
	return cpufeature_extract_unsigned_field_width(feature, field, 4U);
}

static inline int __const
cpufeature_extract_field(u64 feature, unsigned int field, bool sign)
{
	return cpufeature_extract_field_width(feature, field, 4U, sign);
}

static inline bool id_aa64pfr0_32bit_el1(u64 pfr0)
{
	u32 val = cpufeature_extract_unsigned_field(pfr0, ID_AA64PFR0_EL1_SHIFT);
	return val == ID_AA64PFR0_ELx_32BIT_64BIT;
}

static inline bool id_aa64pfr0_32bit_el0(u64 pfr0)
{
	u32 val = cpufeature_extract_unsigned_field(pfr0, ID_AA64PFR0_EL0_SHIFT);
	return val == ID_AA64PFR0_ELx_32BIT_64BIT;
}

static inline bool id_aa64pfr0_sve(u64 pfr0)
{
	u32 val = cpufeature_extract_unsigned_field(pfr0, ID_AA64PFR0_SVE_SHIFT);
	return val > 0U;
}

static inline u64 aarch64_ftr_mask_of(const struct aarch64_ftr_bits *ftrp)
{
	return (u64)BIT_RANGE(ftrp->shift, ftrp->shift + ftrp->width - 1);
}

static inline s64 aarch64_ftr_value_of(const struct aarch64_ftr_bits *ftrp, u64 val)
{
	return (s64)cpufeature_extract_field_width(val, ftrp->shift, ftrp->width, ftrp->sign);
}

u64 aarch64_sanitised_ftr_reg_of(u32 sysid);

static inline u64 aarch64_ftr_reg_user_value(const struct aarch64_ftr_reg *reg)
{
	return (reg->user_val | (reg->sys_val & reg->user_mask));
}

#endif
