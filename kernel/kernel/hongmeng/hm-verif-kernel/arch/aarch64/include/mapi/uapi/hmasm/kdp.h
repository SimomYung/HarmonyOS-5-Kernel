/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: header about kdp trampolines
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 25 06:35:02 2024
 */

#ifndef A64_UAPI_ASM_KDP_H
#define A64_UAPI_ASM_KDP_H

#include <hmasm/types.h>
#include <hmasm/debug.h>

#define __KDP_STR(x)		#x
#define __KDP_IMM(x)		"#" __KDP_STR(x)

#define __MDSCR_KDE_MASK	(1 << 13)
#define __MDSCR_KDE_CLEAR_MASK	(~__MDSCR_KDE_MASK)
#define __MDSCR_MDE_MASK	(1 << 15)
#define __MDSCR_MDE_CLEAR_MASK	(~__MDSCR_MDE_MASK)
#define __MDSCR_KDP_MASK	(__MDSCR_KDE_MASK | __MDSCR_MDE_MASK)
#define __MDSCR_KDP_CLEAR_MASK	(~__MDSCR_KDP_MASK)
#define __DAIF_D_MASK		(1 << 9)
#define __DAIF_D_CLEAR_MASK	(~__DAIF_D_MASK)
#define __DAIF_D_INDEX		(1 << 3)
#define __DBGWVR_MASK		0xfffffffffffff000
#ifdef __hmfeature_ffixed_x18__
#define __KDP_TMP_REGISTER asm("x18")
#else
#define __KDP_TMP_REGISTER
#endif
#define USED(...) UNUSED(__VA_ARGS__)

#ifdef __ASSEMBLY__

.macro __kdp_daif_set, reg, check_mdscr
	msr	daif, \reg
#ifdef CONFIG_KDP_STRICT_CHECK
	tst	\reg, #__DAIF_D_MASK
	b.eq	1f
.ifc \check_mdscr, check_mdscr
	/* D Masked, mdscr.KDE should be disabled */
	mrs	\reg, mdscr_el1
	tst	\reg, #__MDSCR_KDE_MASK
	b.eq	1f
	/* D Masked, and mdscr.KDE enabled, BRK! */
.endif
	brk	#BRK_KDP_CHECKFAIL
1:
#endif /* CONFIG_KDP_STRICT_CHECK */
.endm

#else /* __ASSEMBLY__ */

#ifndef __HOST_LLT__
inline void __kdp_daif_set(unsigned long daif)
{
	register __u64 tmp __KDP_TMP_REGISTER = 0;
	USED(tmp);
	asm volatile (
		"msr	daif, %[daif]\n"
#ifdef CONFIG_KDP_STRICT_CHECK
		"tst	%[daif], " __KDP_IMM(__DAIF_D_MASK) "\n"
		"b.eq	1f\n"
		"mrs	%[tmp], mdscr_el1\n"
		"tst	%[tmp], " __KDP_IMM(__MDSCR_KDE_MASK) "\n"
		"b.eq	1f\n"
		"brk	"__KDP_IMM(BRK_KDP_CHECKFAIL) "\n"
		"1:"
#endif
		: [tmp]"=r"(tmp) : [daif]"r"(daif) : "cc"
	);
}
#else /* __HOST_LLT__ */
inline void __kdp_daif_set(unsigned long daif) {}
#endif

#endif /* __ASSEMBLY__ */
#endif /* A64_UAPI_ASM_KDP_H */
