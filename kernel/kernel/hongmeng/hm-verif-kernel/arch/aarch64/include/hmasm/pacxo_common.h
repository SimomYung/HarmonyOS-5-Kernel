
/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Common code shared by PACXO parts
 * Author: Helsinki System Security Lab
 * Create: Tue Oct 22 17:17:02 2024
 */
#ifndef A64_ASM_PACXO_COMMON_H
#define A64_ASM_PACXO_COMMON_H

#include <hmkernel/mm/kstack.h> // for KSTACK_PAGES_SHIFT

#ifdef __ASSEMBLY__

.macro pacxo_random label reg1 reg2
.ifc \label, ia
	movk \reg1, #0xfeea, lsl #48
	movk \reg2, #0xfeea, lsl #48
	movk \reg1, #0xfeea, lsl #32
	movk \reg2, #0xfeea, lsl #32
	movk \reg1, #0xfeea, lsl #16
	movk \reg2, #0xfeea, lsl #16
	movk \reg1, #0xfeea
	movk \reg2, #0xfeea
.endif

.ifc \label, ib
	movk \reg1, #0xfeeb, lsl #48
	movk \reg2, #0xfeeb, lsl #48
	movk \reg1, #0xfeeb, lsl #32
	movk \reg2, #0xfeeb, lsl #32
	movk \reg1, #0xfeeb, lsl #16
	movk \reg2, #0xfeeb, lsl #16
	movk \reg1, #0xfeeb
	movk \reg2, #0xfeeb
.endif

.ifc \label, da
	movk \reg1, #0xfeda, lsl #48
	movk \reg2, #0xfeda, lsl #48
	movk \reg1, #0xfeda, lsl #32
	movk \reg2, #0xfeda, lsl #32
	movk \reg1, #0xfeda, lsl #16
	movk \reg2, #0xfeda, lsl #16
	movk \reg1, #0xfeda
	movk \reg2, #0xfeda
.endif

.ifc \label, db
	movk \reg1, #0xfedb, lsl #48
	movk \reg2, #0xfedb, lsl #48
	movk \reg1, #0xfedb, lsl #32
	movk \reg2, #0xfedb, lsl #32
	movk \reg1, #0xfedb, lsl #16
	movk \reg2, #0xfedb, lsl #16
	movk \reg1, #0xfedb
	movk \reg2, #0xfedb
.endif

.ifc \label, ga
	movk \reg1, #0xfeee, lsl #48
	movk \reg2, #0xfeee, lsl #48
	movk \reg1, #0xfeee, lsl #32
	movk \reg2, #0xfeee, lsl #32
	movk \reg1, #0xfeee, lsl #16
	movk \reg2, #0xfeee, lsl #16
	movk \reg1, #0xfeee
	movk \reg2, #0xfeee
.endif
.endm

// pacxo loader injects imm16 to replace #feed
// Key setters for kernel/sysmgr
.macro pacxo_set_key label
	pacxo_random \label x3 x4

.ifc \label, ia
	msr APIAKeyHi_EL1, x3
	msr APIAKeyLo_EL1, x4
.endif

.ifc \label, ib
	msr APIBKeyHi_EL1, x3
	msr APIBKeyLo_EL1, x4
.endif

.ifc \label, da
	msr APDAKeyHi_EL1, x3
	msr APDAKeyLo_EL1, x4
.endif

.ifc \label, db
	msr APDBKeyHi_EL1, x3
	msr APDBKeyLo_EL1, x4
.endif

.ifc \label, ga
	msr APGAKeyHi_EL1, x3
	msr APGAKeyLo_EL1, x4
.endif

.endm

#endif
#endif
