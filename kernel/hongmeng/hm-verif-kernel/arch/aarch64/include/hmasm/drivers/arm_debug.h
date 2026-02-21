/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 27 16:19:10 2019
 */
#ifndef _AARCH64_ARM_DEBUG_H
#define _AARCH64_ARM_DEBUG_H

#include <hmasm/types.h>
#include <hmasm/processor.h>
#include <hmasm/barrier.h>
#include <hmasm/drivers/pmu.h>
#include <hmkernel/panic.h>
#ifdef CONFIG_KDP
#include <hmasm/kdp.h>
#endif

/* spsr single step */
#define DBG_SPSR_SS		(1UL << 21)

/* id_aa64dfr0 */
#define DRF_DebugVer_extract(x)		(((x) >> 0) & 0xfULL)
#define DRF_TraceVer_extract(x)		(((x) >> 4) & 0xfULL)
#define DRF_PMUVer_extract(x)		(((x) >> 8) & 0xfULL)
#define DFR_BRPs_extract(x)		(((x) >> 12) & 0xfULL)
#define DFR_WRPs_extract(x)		(((x) >> 20) & 0xfULL)
#define DFR_CTX_CMPs_extract(x)		(((x) >> 28) & 0xfULL)

/* breakpoint registers */
#define BVR(n)	"dbgbvr" #n "_el1"	/* RW Breakpoint Value Register */
#define BCR(n)	"dbgbcr" #n "_el1"	/* RW Breakpoint Control Register */
/* watchpoint registers */
#define WVR(n)	"dbgwvr" #n "_el1"	/* RW Watchpoint Value Register */
#define WCR(n)	"dbgwcr" #n "_el1"	/* RW Watchpoint Control Register */

/* watchpoint registers */
#define WCR_E				0x1ULL		/* bit[0] */
#define WCR_PAC				(0x3ULL << 1)	/* bit[2:1] */
#define WCR_LSC_LOAD			(0x1ULL << 3)	/* bit[3] */
#define WCR_LSC_STORE			(0x1ULL << 4)	/* bit[4] */
#define WCR_BAS				(0xffULL << 5)	/* bit[12:5] */
#define WCR_HMC				(0x1ULL << 13)	/* bit[13] */
#define WCR_SSC				(0x3ULL << 14)	/* bit[15:14] */
#define WCR_LBN				(0xfULL << 16)	/* bit[19:16] */
#define WCR_WT				(0x1ULL << 20)	/* bit[20] */
/* reserved */						/* bit[23:21] */
#define WCR_MASK			(0x1fULL << 24)	/* bit[28:24] */
/* reserved */						/* bit[63:29] */

#define WCR_PAC_USER			(0x2ULL << 1)	/* bit[2:1] is 10 */
#define WCR_HMC_NO			0x0ULL		/* bit[13] is 0 */
#define WCR_SSC_BOTH			0x0ULL		/* bit[15:14] is 00 */
#define WCR_MASK_NOMASK			0x0ULL		/* bit[28:24] is 00000 */

/* breakpoint registers */
#define BCR_E 				0x1ULL /* bit[0] */
#define BCR_PMC 			(0x3ULL << 1) /* bit[2:1] */
/* reserved */                            /* bit[4:3] */
#define BCR_BAS 			(0xfULL << 5) /* bit[8:5] */
/* reserved */                            /* bit[12:9] */
#define BCR_HMC 			(0x1ULL << 13) /* bit[13] */
#define BCR_SSC 			(0x3ULL << 14) /* bit[15:14] */
#define BCR_LBN 			(0xfULL << 16) /* bit[19:16] */
#define BCR_BT 				(0xfULL << 20) /* bit[23:20] */
/* reserved */                             /* bit[63:24] */

#define BCR_PMC_USER 		(0x2ULL << 1) /* bit[2:1] is 10 */
#define BCR_HMC_NO 			0x0ULL /* bit[13] is 0 */
#define BCR_SSC_BOTH 		0x0ULL /* bit[14] is 0 */

#define DEBUG_REG_READ_CASE(DBGREG, idx, rlt)	\
	case idx:				\
	asm volatile("mrs %0," DBGREG(idx) "\n" : "=r" (rlt)); \
	break
#define DEBUG_REG_WRITE_CASE(DBGREG, idx, val)	\
	case idx:				\
	asm volatile("msr " DBGREG(idx) ", %0\n" :: "r" (val)); \
	break

#ifdef CONFIG_KDP
#define DEBUG_REG_CASE_NOP(DBGREG, idx, val)	\
	case idx:				\
	break

#define DEBUG_REG_WRITE_CASE_WVR_KDP(idx)	\
	case idx:				\
	kdp_setwvr##idx();			\
	break

#define DEBUG_REG_READ_CASE_WCR(DBGREG, idx, val)	DEBUG_REG_CASE_NOP(DBGREG, idx, val)
#define DEBUG_REG_READ_CASE_WVR(DBGREG, idx, val)	DEBUG_REG_CASE_NOP(DBGREG, idx, val)
#define DEBUG_REG_READ_CASE_BCR(DBGREG, idx, val)	DEBUG_REG_READ_CASE(DBGREG, idx, val)
#define DEBUG_REG_READ_CASE_BVR(DBGREG, idx, val)	DEBUG_REG_READ_CASE(DBGREG, idx, val)

#define DEBUG_REG_WRITE_CASE_WCR(DBGREG, idx, val)	DEBUG_REG_CASE_NOP(DBGREG, idx, val)
#define DEBUG_REG_WRITE_CASE_WVR(DBGREG, idx, val)	DEBUG_REG_WRITE_CASE_WVR_KDP(idx)
#define DEBUG_REG_WRITE_CASE_BCR(DBGREG, idx, val)	DEBUG_REG_WRITE_CASE(DBGREG, idx, val)
#define DEBUG_REG_WRITE_CASE_BVR(DBGREG, idx, val)	DEBUG_REG_WRITE_CASE(DBGREG, idx, val)

#else

#define DEBUG_REG_READ_CASE_WCR(DBGREG, idx, val)	DEBUG_REG_READ_CASE(DBGREG, idx, val)
#define DEBUG_REG_READ_CASE_WVR(DBGREG, idx, val)	DEBUG_REG_READ_CASE(DBGREG, idx, val)
#define DEBUG_REG_READ_CASE_BCR(DBGREG, idx, val)	DEBUG_REG_READ_CASE(DBGREG, idx, val)
#define DEBUG_REG_READ_CASE_BVR(DBGREG, idx, val)	DEBUG_REG_READ_CASE(DBGREG, idx, val)

#define DEBUG_REG_WRITE_CASE_WCR(DBGREG, idx, val)	DEBUG_REG_WRITE_CASE(DBGREG, idx, val)
#define DEBUG_REG_WRITE_CASE_WVR(DBGREG, idx, val)	DEBUG_REG_WRITE_CASE(DBGREG, idx, val)
#define DEBUG_REG_WRITE_CASE_BCR(DBGREG, idx, val)	DEBUG_REG_WRITE_CASE(DBGREG, idx, val)
#define DEBUG_REG_WRITE_CASE_BVR(DBGREG, idx, val)	DEBUG_REG_WRITE_CASE(DBGREG, idx, val)
#endif

/* max support 16 registers */
#define DEFINE_arch_debugrw(name, DBGREG)		\
static inline u64					\
arm_debug_read_##name(u32 idx)				\
{							\
	u64 v = 0;					\
							\
	switch (idx) {					\
	DEBUG_REG_READ_CASE_##DBGREG(DBGREG, 0, v);	\
	DEBUG_REG_READ_CASE_##DBGREG(DBGREG, 1, v);	\
	DEBUG_REG_READ_CASE(DBGREG, 2, v);		\
	DEBUG_REG_READ_CASE(DBGREG, 3, v);		\
	DEBUG_REG_READ_CASE(DBGREG, 4, v);		\
	DEBUG_REG_READ_CASE(DBGREG, 5, v);		\
	DEBUG_REG_READ_CASE(DBGREG, 6, v);		\
	DEBUG_REG_READ_CASE(DBGREG, 7, v);		\
	DEBUG_REG_READ_CASE(DBGREG, 8, v);		\
	DEBUG_REG_READ_CASE(DBGREG, 9, v);		\
	DEBUG_REG_READ_CASE(DBGREG, 10, v);		\
	DEBUG_REG_READ_CASE(DBGREG, 11, v);		\
	DEBUG_REG_READ_CASE(DBGREG, 12, v);		\
	DEBUG_REG_READ_CASE(DBGREG, 13, v);		\
	DEBUG_REG_READ_CASE(DBGREG, 14, v);		\
	DEBUG_REG_READ_CASE(DBGREG, 15, v);		\
	default:					\
		panic("impossible read: "#name"%d\n", idx); \
		break;					\
	}						\
	return v;					\
}							\
static inline void					\
arm_debug_write_##name(u32 idx, u64 _val)		\
{							\
	u64 val = (u64)_val;				\
							\
	switch (idx) {					\
	DEBUG_REG_WRITE_CASE_##DBGREG(DBGREG, 0, val);	\
	DEBUG_REG_WRITE_CASE_##DBGREG(DBGREG, 1, val);	\
	DEBUG_REG_WRITE_CASE(DBGREG, 2, val);		\
	DEBUG_REG_WRITE_CASE(DBGREG, 3, val);		\
	DEBUG_REG_WRITE_CASE(DBGREG, 4, val);		\
	DEBUG_REG_WRITE_CASE(DBGREG, 5, val);		\
	DEBUG_REG_WRITE_CASE(DBGREG, 6, val);		\
	DEBUG_REG_WRITE_CASE(DBGREG, 7, val);		\
	DEBUG_REG_WRITE_CASE(DBGREG, 8, val);		\
	DEBUG_REG_WRITE_CASE(DBGREG, 9, val);		\
	DEBUG_REG_WRITE_CASE(DBGREG, 10, val);		\
	DEBUG_REG_WRITE_CASE(DBGREG, 11, val);		\
	DEBUG_REG_WRITE_CASE(DBGREG, 12, val);		\
	DEBUG_REG_WRITE_CASE(DBGREG, 13, val);		\
	DEBUG_REG_WRITE_CASE(DBGREG, 14, val);		\
	DEBUG_REG_WRITE_CASE(DBGREG, 15, val);		\
	default:					\
		panic("impossible write: "#name"%d\n", idx); \
		break;					\
	}						\
}

/* MDSCR_EL1 enabling bits */
#define DBG_MDSCR_SS		(1u << 0)
#define DBG_MDSCR_KDE		(1u << 13)
#define DBG_MDSCR_MDE		(1u << 15)
#define DBG_MDSCR_MASK		~(DBG_MDSCR_KDE | DBG_MDSCR_MDE)
/* monitor debug system control register */
DEFINE_sysreg(mdscr, el1);
/* os lock access register */
DEFINE_sysreg(oslar, el1);

/* watchpoints and breakpoints registers read write */
DEFINE_arch_debugrw(wcr, WCR)
DEFINE_arch_debugrw(wvr, WVR)
DEFINE_arch_debugrw(bcr, BCR)
DEFINE_arch_debugrw(bvr, BVR)

static inline unsigned int arm_debug_version(void)
{
	u64 value = sysreg_read_id_aa64dfr0_el1();
	return (unsigned int)DRF_DebugVer_extract(value);
}

static inline unsigned int arm_debug_watch_number(void)
{
	u64 value = sysreg_read_id_aa64dfr0_el1();
	return 1u + (unsigned int)DFR_WRPs_extract(value);
}

static inline unsigned int arm_debug_break_number(void)
{
	u64 value = sysreg_read_id_aa64dfr0_el1();
	return 1u + (unsigned int)DFR_BRPs_extract(value);
}

#ifdef __HOST_LLT__
static inline void arm_debug_step_enable(void)
{
	u64 value = sysreg_read_mdscr_el1();
	value = value | DBG_MDSCR_SS;
	sysreg_write_mdscr_el1(value);
	isb();
}
#else
static inline void arm_debug_step_enable(void)
{
	register unsigned long x18 asm("x18");
	asm volatile (
		"mrs     %0, mdscr_el1\n"
		"orr     %0, %0, %1\n"
		"msr     mdscr_el1, %0\n"
#ifdef CONFIG_KDP_STRICT_CHECK
		"and     %0, %0, #1 << 13\n"
		"cbnz    %0, 1f\n"
		"mrs     %0, daif\n"
		"and     %0, %0, #1 << 9\n"
		"cbnz    %0, 1f\n"
		"brk     0xdead\n"
		"1:\n"
#endif
		:
		: "r"(x18), "i"(DBG_MDSCR_SS));
	isb();
}
#endif

#ifdef __HOST_LLT__
static inline void arm_debug_step_disable(void)
{
	u64 value = sysreg_read_mdscr_el1();
	value = value & (~DBG_MDSCR_SS);
	sysreg_write_mdscr_el1(value);
	isb();
}
#else
static inline void arm_debug_step_disable(void)
{
	register unsigned long x18 asm("x18");
	asm volatile (
		"mrs     %0, mdscr_el1\n"
		"and     %0, %0, ~%1\n"
		"msr     mdscr_el1, %0\n"
#ifdef CONFIG_KDP_STRICT_CHECK
		"and     %0, %0, #1 << 13\n"
		"cbnz    %0, 1f\n"
		"mrs     %0, daif\n"
		"and     %0, %0, #1 << 9\n"
		"cbnz    %0, 1f\n"
		"brk     0xdead\n"
		"1:\n"
#endif
		:
		: "r"(x18), "i"(DBG_MDSCR_SS));
	isb();
}
#endif

static inline void arm_debug_unlock_os(void)
{
	sysreg_write_oslar_el1(0ULL);
	isb();
}

static inline void arm_debug_lock_os(void)
{
	sysreg_write_oslar_el1(1ULL);
	isb();
}

#ifdef __HOST_LLT__
static inline void arm_debug_debug_mode_enable(void)
{
	u64 value = sysreg_read_mdscr_el1();
	value = value | DBG_MDSCR_MDE;
	sysreg_write_mdscr_el1(value);
	isb();
}
#else
static inline void arm_debug_debug_mode_enable(void)
{
	register unsigned long x18 asm("x18");
	asm volatile (
		"mrs     %0, mdscr_el1\n"
		"orr     %0, %0, %1\n"
		"msr     mdscr_el1, %0\n"
#ifdef CONFIG_KDP_STRICT_CHECK
		"and     %0, %0, #1 << 13\n"
		"cbnz    %0, 1f\n"
		"mrs     %0, daif\n"
		"and     %0, %0, #1 << 9\n"
		"cbnz    %0, 1f\n"
		"brk     0xdead\n"
		"1:\n"
#endif
		:
		: "r"(x18), "i"(DBG_MDSCR_MDE));
	isb();
}
#endif

#endif
