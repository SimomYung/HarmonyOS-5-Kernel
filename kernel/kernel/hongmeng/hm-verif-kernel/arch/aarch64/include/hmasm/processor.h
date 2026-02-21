/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 7 10:56:55 2018
 */
#ifndef AARCH64_ASM_PROCESSOR_H
#define AARCH64_ASM_PROCESSOR_H

#include <hmkernel/types.h>
#include <hmkernel/strict.h>
#include <uapi/hmasm/processor.h>
#include <uapi/hmasm/sysreg_utils.h>
#include <hmasm/sysreg.h>

/* SPSR_ELx bits for exceptions taken from AArch32 */
#define PSR_AA32_MODE_MASK	0x0000001F

#ifdef CONFIG_AARCH64_ADDR_TAGGING
#define TAG_BITS 8u
/*
 * The top bits [63:56] of the target address are set 0 or 1
 * based on the bit [55] of the target address, if address tagging is enabled.
 */
static inline u64 a64_handle_addr_tagging(u64 addr)
{
	return (u64)((s64)(addr << TAG_BITS) >> TAG_BITS);
}

static inline u64 a64_sysreg_read_far(void)
{
	u64 far = read_sysreg("far_el1");
	far = a64_handle_addr_tagging(far);
	return far;
}
#else /* !CONFIG_AARCH64_ADDR_TAGGING */
#define a64_handle_addr_tagging(addr) (addr)
#define a64_sysreg_read_far() read_sysreg("far_el1")
#endif /* CONFIG_AARCH64_ADDR_TAGGING */

#define DEFINE_sysreg_rd(name, el)		__AARCH64_DEFINE_sysreg_rd(sysreg_read, name, el)
#define DEFINE_sysreg_wr(name, el)		__AARCH64_DEFINE_sysreg_wr(sysreg_write, name, el)
#define DEFINE_sysreg_rd32(name, el)		__AARCH64_DEFINE_sysreg_rd32(sysreg_read32, name, el)
#define DEFINE_sysreg_wr32(name, el)		__AARCH64_DEFINE_sysreg_wr32(sysreg_write32, name, el)

#define wfi() asm volatile ("wfi")
#define wfe() asm volatile ("wfe")

/* psr settings */
/* D and E share one bit. for 64 bit exception, D is 'debug' */
#define PSR_SS_BIT	(1UL << 21UL)
#define PSR_64_SSBS_BIT	(1UL << 12UL)
#define PSR_32_SSBS_BIT	(1UL << 23UL)
#define PSR_D_BIT	(1UL << 9UL)
#define PSR_E_BIT	(1UL << 9UL)
#define PSR_A_BIT	(1UL << 8UL)
#define PSR_I_BIT	(1UL << 7UL)
#define PSR_F_BIT	(1UL << 6UL)
#define PSR_V_BIT	(1UL << 28)
#define PSR_C_BIT	(1UL << 29)
#define PSR_Z_BIT	(1UL << 30)
#define PSR_N_BIT	(1UL << 31)
/* control 32 or 64 bit */
#define PSR_M4_BIT	(1UL << 4UL)
#define PSR_M_EL0T	0UL
#define PSR_M_EL1T	0x4UL
#define PSR_M_EL1H	0x5UL
#define PSR_M_EL2T	0x8UL
#define PSR_M_EL2H	0x9UL
#define PSR_M_EL3T	0xcUL
#define PSR_M_EL3H	0xdUL
#define PSR_M_MASK	0x1fUL

#define PSR_DAIF_MASK	(PSR_D_BIT | PSR_A_BIT | PSR_I_BIT | PSR_F_BIT)

#define SCR_NS		(1UL << 0)
#define SCR_HCE		(1UL << 8)
#define SCR_RW		(1UL << 10)

/* user's spsr state that is kept unchanged between enter/leave kernel. */
#define SPSR_KEEP_BITS (~(PSR_SS_BIT))

#define DEFINE_sysreg32(name, el)	\
	DEFINE_sysreg_rd32(name, el)	\
	DEFINE_sysreg_wr32(name, el)

#define DEFINE_sysreg(name, el)		\
	DEFINE_sysreg_rd(name, el)	\
	DEFINE_sysreg_wr(name, el)

#define CURRENTEL_EL0		0
#define CURRENTEL_EL1		1
#define CURRENTEL_EL2		2
#define CURRENTEL_EL3		3

#define currentel_bits(reg)	(((reg) >> 2) & 0x03ULL)
static inline u64 a64_curr_el(void)
{
	u64 elvl;
	elvl = read_sysreg("CurrentEL");
	elvl = currentel_bits(elvl);
	return elvl;
}

#define a64_valid_boot_el(el) ((el) != 0U)

#define DEFINE_sysreg_el123(name) \
	DEFINE_sysreg(name, el1) \
	DEFINE_sysreg(name, el2) \
	DEFINE_sysreg(name, el3) \
\
static inline u64 \
sysreg_read_##name##_el(u64 el) {		\
	u64 v;						\
	switch (el) {					\
	case 1:						\
		v = sysreg_read_##name##_el1();		\
		break;					\
	case 2:						\
		v = sysreg_read_##name##_el2();		\
		break;					\
	case 3:						\
		v = sysreg_read_##name##_el3();		\
		break;					\
	default:					\
		__builtin_unreachable();		\
		break;					\
	}						\
	return v;					\
} \
\
static inline void \
sysreg_write_##name##_el(u64 el, u64 v) {	\
	switch (el) {					\
	case 1:						\
		sysreg_write_##name##_el1(v);		\
		break;					\
	case 2:						\
		sysreg_write_##name##_el2(v);		\
		break;					\
	case 3:						\
		sysreg_write_##name##_el3(v);		\
		break;					\
	default:					\
		__builtin_unreachable();		\
		break;					\
	}						\
	return;						\
} \

#endif
