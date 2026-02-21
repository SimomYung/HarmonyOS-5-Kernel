/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: header about kdp trampolines
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 25 06:35:02 2024
 */

#ifndef A64_ASM_KDP_H
#define A64_ASM_KDP_H

#include <hmasm/types.h>
#include <hmasm/sysreg.h>
#include <hmasm/memory.h>
#include <hmkernel/strict.h>
#include <mapi/uapi/hmasm/kdp.h>
#include <mapi/uapi/hmasm/debug.h>
#include <hmasm/processor.h>

#define KDP_IMM(x)		__KDP_IMM(x)

#define MDSCR_KDE_MASK		__MDSCR_KDE_MASK
#define MDSCR_KDE_CLEAR_MASK	__MDSCR_KDE_CLEAR_MASK
#define MDSCR_MDE_MASK		__MDSCR_MDE_MASK
#define MDSCR_MDE_CLEAR_MASK	__MDSCR_MDE_CLEAR_MASK
#define MDSCR_KDP_MASK		__MDSCR_KDP_MASK
#define MDSCR_KDP_CLEAR_MASK	__MDSCR_KDP_CLEAR_MASK
#define DAIF_D_MASK		__DAIF_D_MASK
#define DAIF_D_CLEAR_MASK	__DAIF_D_CLEAR_MASK
#define DAIF_D_INDEX		__DAIF_D_INDEX
#define DBGWVR_MASK		__DBGWVR_MASK
#define KDP_TMP_REGISTER	__KDP_TMP_REGISTER
#ifdef CONFIG_VHE
/* To enable watchpoints in EL1 and EL2, HMC need to be 1. */
#define KDP_WCR_HMC_VALUE	(1 << 13)
#else
#define KDP_WCR_HMC_VALUE	(0)
#endif
/* SSC   BAS               LSC  PAC  E */
/* 01    11111111          10   01   1 */
#define KDP_DBGWCR0_VALUE	(0x1f005ff3 | KDP_WCR_HMC_VALUE)
/*    MASK    RES0-WT-LBN    SSC   BAS               LSC  PAC  E */
/*    01100   000000000      01    11111111          11   01   1 */
#define KDP_DBGWCR1_VALUE	(0x0c005ffb | KDP_WCR_HMC_VALUE)

#ifdef __ASSEMBLY__

.macro mov32, reg, value
	movz	\reg, # \value & 0xffff
	movk	\reg, # \value >> 16, lsl #16
.endm

.macro kdp_daif_set, reg, check_mdscr
	__kdp_daif_set \reg, \check_mdscr
.endm

.macro kdp_mdscr_enable reg
	mrs	\reg, mdscr_el1
	orr	\reg, \reg, #MDSCR_KDE_MASK
	orr	\reg, \reg, #MDSCR_MDE_MASK
	msr	mdscr_el1, \reg
#ifdef CONFIG_KDP_STRICT_CHECK
	tst	\reg, #MDSCR_KDE_MASK
	b.eq	1f
	tst	\reg, #MDSCR_MDE_MASK
	b.eq	1f
	b	2f
1: /* check fail */
	brk     #BRK_KDP_CHECKFAIL
2: /* check pass */
#endif /* CONFIG_KDP_STRICT_CHECK */
.endm

.macro kdp_mdscr_disable reg, daif_check_reg
	mrs	\reg, mdscr_el1
	and	\reg, \reg, #MDSCR_KDE_CLEAR_MASK
	/* only disable KDE, keep MDE enabled */
	msr	mdscr_el1, \reg
#ifdef CONFIG_KDP_STRICT_CHECK
.ifc \daif_check_reg, check_daif
	/* D should be masked */
	mrs	\reg, daif
	tst	\reg, DAIF_D_MASK
	b.ne	2f
1: /* check fail */
	brk	#BRK_KDP_CHECKFAIL
2: /* check pass */
.endif
#endif /* CONFIG_KDP_STRICT_CHECK */
.endm

#ifdef CONFIG_KDP
.macro _kdp_wcr_enable, name, value, reg0, reg1
	mov32	\reg0, \value
	msr	\name, \reg0
# ifdef CONFIG_KDP_STRICT_CHECK
	mov32	\reg1, \value
	sub	\reg0, \reg0, \reg1
	cbz	\reg0, 1f
	mrs	\reg0, \name
	brk	#BRK_KDP_CHECKFAIL
1:
# endif /* CONFIG_KDP_STRICT_CHECK */
.endm

.macro kdp_wcr_enable, idx, reg0, reg1
.ifc \idx, 0
	_kdp_wcr_enable dbgwcr0_el1, KDP_DBGWCR0_VALUE, \reg0, \reg1
.endif
#ifdef CONFIG_KDP_DATA_INVISIBLE
.ifc \idx, 1
	_kdp_wcr_enable dbgwcr1_el1, KDP_DBGWCR1_VALUE, \reg0, \reg1
.endif
#endif
.endm
#else /* CONFIG_KDP */
.macro kdp_wcr_enable, idx, reg0, reg1
.endm
#endif /* CONFIG_KDP */

.macro kdp_spsr_el1_debug_check reg
#ifdef CONFIG_KDP_STRICT_CHECK
	tbnz	\reg, #(PSTATE_D_SHIFT), 1f
	/*
	 * In the kernel, the D bit is always 1.
	 * When sysmgr enters the kernel via spincall, the D bit is also 1.
	 * If sysmgr enters illegally, the D bit may be 0, posing a security
	 * risk that must be intercepted.
	 */
	brk	#BRK_KDP_CHECKFAIL
1:
#endif
.endm

.macro kdp_spsr_el1_set reg, reg1
	msr	spsr_el1, \reg
	msr	elr_el1, \reg1
#ifdef CONFIG_KDP_STRICT_CHECK
	and	\reg, \reg, #(1 << PSTATE_D_SHIFT)
	cbz	\reg, 2f
	/* Check D bit to be 1 in daif, making sure that we are in
	 * kernel now.
	 */
	mrs	\reg, daif
	and	\reg, \reg, #(1 << PSTATE_D_SHIFT)
	cbz	\reg, 1f
	/* Check if elr is greater than kernel _stext, making sure
	 * that we are returning to kernel, not sysmgr
	 */
	adrp	\reg, _stext
	add	\reg, \reg, #:lo12:_stext
	cmp	\reg1, \reg
	b.ge	2f
1:
	brk	#BRK_KDP_CHECKFAIL
2:
#endif
.endm

.macro kdp_spsr_el0_set reg
	msr	spsr_el1, \reg
#ifdef CONFIG_KDP_STRICT_CHECK
	/* Execution state in SPSR must be El0t */
	tst	\reg, #PSR_MODE_MASK
	b.eq	1f
	brk	#BRK_KDP_CHECKFAIL
1:
#endif
.endm

#else /* __ASSEMBLY__ */

#ifdef CONFIG_KDP_STRICT_CHECK
#define kdp_spsr_el0_set(reg)				\
	"msr 	spsr_el1, "#reg"\n"			\
	/* Execution state in SPSR must be El0t */	\
	"tst	" #reg ", " KDP_IMM(PSR_MODE_MASK) "\n"	\
	"b.eq	1f\n"					\
	"brk	" KDP_IMM(BRK_KDP_CHECKFAIL) "\n"	\
	"1:\n"
#else
#define kdp_spsr_el0_set(reg)				\
	"msr	spsr_el1, " #reg "\n"
#endif

#define mov32(dst, val)						\
	"movz	" dst ", " KDP_IMM(val) "& 0xffff\n"		\
	"movk	" dst ", " KDP_IMM(val) ">> 16, lsl #16\n"

#ifdef CONFIG_KDP_STRICT_CHECK
#define GEN_KDP_DBGWVCR_FUNCS(idx, wvr_value)						\
inline void kdp_wcr##idx##_enable(void)							\
{											\
	register u64 wcr KDP_TMP_REGISTER = KDP_DBGWCR##idx##_VALUE;			\
	u64 tmp = 0;									\
	USED(wcr, tmp);									\
	asm volatile (									\
		"msr	dbgwcr" #idx "_el1, %[wcr]\n"					\
/* CONFIG_KDP_STRICT_CHECK START */							\
		mov32("%[tmp]", KDP_DBGWCR##idx##_VALUE)				\
		"cmp	%[tmp], %[wcr]\n"						\
		"b.eq	1f\n"								\
		"brk	" KDP_IMM(BRK_KDP_CHECKFAIL) "\n"				\
		"1:"									\
/* CONFIG_KDP_STRICT_CHECK END */							\
		: [tmp]"=r"(tmp): [wcr]"r"(wcr) : "cc"					\
	);										\
}											\
inline void kdp_wcr##idx##_disable(void)						\
{											\
	register u64 tmp KDP_TMP_REGISTER = 0;						\
	USED(tmp);									\
	asm volatile (									\
		"msr	dbgwcr" #idx "_el1, xzr\n"					\
/* CONFIG_KDP_STRICT_CHECK START */							\
		"mrs	%0, mdscr_el1\n"						\
		"tst	%0, " KDP_IMM(MDSCR_KDE_MASK) "\n"				\
		"b.eq	1f\n"								\
		"brk	" KDP_IMM(BRK_KDP_CHECKFAIL) "\n"				\
		"1:"									\
/* CONFIG_KDP_STRICT_CHECK END */							\
		: "=r"(tmp) :: "cc"							\
	);										\
}											\
inline void kdp_wvr##idx##_enable(void)							\
{											\
	register u64 wvr KDP_TMP_REGISTER = 0;						\
	u64 tmp = 0;									\
	USED(wvr, tmp);									\
	asm volatile (									\
		"adrp	%[wvr], " #wvr_value "\n"					\
		"add	%[wvr], %[wvr], #:lo12:" #wvr_value "\n"			\
		"ldr	%[wvr], [%[wvr]]\n"						\
		"and	%[wvr], %[wvr], " KDP_IMM(DBGWVR_MASK) "\n"			\
		"msr	dbgwvr" #idx "_el1, %[wvr]\n"					\
/* CONFIG_KDP_STRICT_CHECK START */							\
		"adrp	%[tmp], " #wvr_value "\n"					\
		"add	%[tmp], %[tmp], #:lo12:" #wvr_value "\n"			\
		"ldr	%[tmp], [%[tmp]]\n"						\
		"and	%[tmp], %[tmp], " KDP_IMM(DBGWVR_MASK) "\n"			\
		"cmp	%[wvr], %[tmp]\n"						\
		"b.eq	1f\n"								\
		"brk	" KDP_IMM(BRK_KDP_CHECKFAIL) "\n"				\
		"1:\n"									\
/* CONFIG_KDP_STRICT_CHECK END */							\
		: [wvr]"=r"(wvr), [tmp]"=r"(tmp)  :: "cc"				\
		);									\
}											\
inline void kdp_wvr##idx##_disable(void)						\
{											\
	register u64 tmp KDP_TMP_REGISTER = 0;						\
	USED(tmp);									\
	asm volatile (									\
		"msr	dbgwvr" #idx "_el1, xzr\n"					\
/* CONFIG_KDP_STRICT_CHECK START */							\
		"msr	%0, mdscr_el1\n"						\
		"tst	%0, " KDP_IMM(MDSCR_KDE_MASK) "\n"				\
		"b.eq	1f\n"								\
		"brk	" KDP_IMM(BRK_KDP_CHECKFAIL) "\n"				\
		"1:"									\
/* CONFIG_KDP_STRICT_CHECK END */							\
		: "=r"(tmp) :: "cc"							\
	);										\
}
#else /* CONFIG_KDP_STRICT_CHECK */
#define GEN_KDP_DBGWVCR_FUNCS(idx, wvr_value)						\
inline void kdp_wcr##idx##_enable(void)							\
{											\
	register u64 tmp KDP_TMP_REGISTER  = KDP_DBGWCR##idx##_VALUE;			\
	USED(tmp);									\
	asm volatile (									\
		"msr	dbgwcr" #idx "_el1, %0\n"					\
		:: "r"(tmp)								\
	);										\
}											\
inline void kdp_wcr##idx##_disable(void)						\
{											\
	asm volatile (									\
		"msr	dbgwcr" #idx "_el1, xzr\n"					\
	);										\
}											\
inline void kdp_wvr##idx##_enable(void)							\
{											\
	register u64 tmp KDP_TMP_REGISTER = 0;						\
	USED(tmp);									\
	asm volatile (									\
		"adrp	%0, " #wvr_value "\n"						\
		"add	%0, %0, #:lo12:" #wvr_value "\n"				\
		"ldr	%0, [%0]\n"							\
		"and	%0, %0, " KDP_IMM(DBGWVR_MASK) "\n"				\
		"msr	dbgwvr" #idx "_el1, %0\n"					\
		: "=r"(tmp)								\
		);									\
}											\
inline void kdp_wvr##idx##_disable(void)						\
{											\
	asm volatile (									\
		"msr	dbgwvr" #idx "_el1, xzr\n"					\
	);										\
}
#endif /* CONFIG_KDP_STRICT_CHECK */

#define GEN_KDP_DBGWVCR_EMPTY_FUNCS(idx)	\
inline void kdp_wcr##idx##_enable(void) {}	\
inline void kdp_wcr##idx##_disable(void) {}	\
inline void kdp_wvr##idx##_enable(void) {}	\
inline void kdp_wvr##idx##_disable(void) {}

#ifndef __HOST_LLT__
GEN_KDP_DBGWVCR_FUNCS(0, __kaslr_kdp_start)
#else /* __HOST_LLT__ */
GEN_KDP_DBGWVCR_EMPTY_FUNCS(0)
#endif /* __HOST_LLT__ */

#if defined(CONFIG_KDP_DATA_INVISIBLE) && !defined(__HOST_LLT__)
extern uptr_t __data_invisible_kdp_start_real;
GEN_KDP_DBGWVCR_FUNCS(1, __data_invisible_kdp_start_real)
#else /* defined(CONFIG_KDP_DATA_INVISIBLE) && !defined(__HOST_LLT__) */
GEN_KDP_DBGWVCR_EMPTY_FUNCS(1)
#endif /* defined(CONFIG_KDP_DATA_INVISIBLE) && !defined(__HOST_LLT__) */

#ifndef __HOST_LLT__
inline void kdp_mdscr_set(unsigned long mdscr)
{
	register u64 _mdscr KDP_TMP_REGISTER = mdscr;
	u64 daif = 0;
	u64 tmp = 0;
	USED(_mdscr, daif, tmp);

	asm volatile (
		"msr	mdscr_el1, %[mdscr]\n"
#ifdef CONFIG_KDP_STRICT_CHECK
		"mov	%[tmp], " KDP_IMM(MDSCR_KDP_MASK) "\n"
		"mvn	%[mdscr], %[mdscr]\n"
		"tst	%[tmp], %[mdscr]\n"
		"b.eq	1f\n"
		"mrs	%[daif], daif\n"
		"mov	%[tmp], " KDP_IMM(DAIF_D_MASK) "\n"
		"tst	%[tmp], %[daif]\n"
		"b.ne	1f\n"
		"brk	" KDP_IMM(BRK_KDP_CHECKFAIL) "\n"
		"1:"
#endif
		: [tmp]"=r"(tmp), [daif]"=r"(daif), [mdscr]"+r"(_mdscr) :: "cc"
	);
}

inline void kdp_mdscr_enable(void)
{
	register u64 mdscr KDP_TMP_REGISTER;
	u64 tmp = 0;
	USED(mdscr, tmp);

	mdscr = read_sysreg("mdscr_el1") | MDSCR_KDP_MASK;
	asm volatile (
		"msr	mdscr_el1, %[mdscr]\n"
#ifdef CONFIG_KDP_STRICT_CHECK
		"mov	%[tmp], " KDP_IMM(MDSCR_KDP_MASK) "\n"
		"mvn	%[mdscr], %[mdscr]\n"
		"tst	%[tmp], %[mdscr]\n"
		"b.eq	1f\n"
		"brk	" KDP_IMM(BRK_KDP_CHECKFAIL) "\n"
		"1:"
#endif
		: [tmp]"=r"(tmp), [mdscr]"+r"(mdscr) :: "cc"
	);
}

inline void kdp_daif_enable(void)
{
	asm volatile ("msr	daifclr, " KDP_IMM(DAIF_D_INDEX));
}

#else /* __HOST_LLT__ */
inline void kdp_mdscr_set(unsigned long mdscr) {}
inline void kdp_mdscr_enable(void) {}
inline void kdp_daif_enable(void) {}
#endif

inline void kdp_daif_set(unsigned long daif)
{
	__kdp_daif_set(daif);
}

inline void kdp_init_wp(void)
{
	kdp_wvr0_enable();
	kdp_wvr1_enable();
}

#endif /* __ASSEMBLY__ */
#endif /* A64_ASM_KDP_H */
