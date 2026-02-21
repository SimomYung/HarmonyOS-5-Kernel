/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Alternative instruction
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 7 09:58:12 2020
 */

#ifndef AARCH64_ASM_ALTERNATIVE_H
#define AARCH64_ASM_ALTERNATIVE_H

#if !defined(CONFIG_AARCH64_UAO) || defined(ASM_ALTER_LDR_DISABLE)
#define __ASM_DO_NOT_ALTER_LDR
#endif

#if !defined(CONFIG_AARCH64_UAO) || defined(ASM_ALTER_STR_DISABLE)
#define __ASM_DO_NOT_ALTER_STR
#endif

/*
 * Notice:
 * alternative load/store instructions:
 * the macro name is a64_alter_ldr##num or str##num, and the num is parameter's number
 *
 * load/store 8 byte
 */
.macro a64_alter_ldr3 dest_reg, src_reg, post_inc
#ifdef __ASM_DO_NOT_ALTER_LDR
	ldr \dest_reg, [\src_reg], \post_inc
#else
	ldtr \dest_reg, [\src_reg]
	add \src_reg, \src_reg, \post_inc
#endif /* CONFIG_AARCH64_UAO */
.endm

.macro a64_alter_str3 src_reg, dest_reg, post_inc
#ifdef __ASM_DO_NOT_ALTER_STR
	.ifnc \post_inc, #0
	str \src_reg, [\dest_reg], \post_inc
	.else
	str \src_reg, [\dest_reg]
	.endif
#else
	sttr \src_reg, [\dest_reg]
	.ifnc \post_inc, #0
	add \dest_reg, \dest_reg, \post_inc
	.endif
#endif /* CONFIG_AARCH64_UAO */
.endm

// load/store halfword
.macro a64_alter_ldrh3 dest_reg, src_reg, post_inc
#ifdef __ASM_DO_NOT_ALTER_LDR
	ldrh \dest_reg, [\src_reg], \post_inc
#else
	ldtrh \dest_reg, [\src_reg]
	add \src_reg, \src_reg, \post_inc
#endif /* CONFIG_AARCH64_UAO */
.endm

.macro a64_alter_strh3 src_reg, dest_reg, post_inc
#ifdef __ASM_DO_NOT_ALTER_STR
	strh \src_reg, [\dest_reg], \post_inc
#else
	sttrh \src_reg, [\dest_reg]
	add \dest_reg, \dest_reg, \post_inc
#endif /* CONFIG_AARCH64_UAO */
.endm

.macro a64_alter_ldrb3 dest_reg, src_reg, offset
#ifdef __ASM_DO_NOT_ALTER_LDR
	.ifnc \offset, #0
	ldrb \dest_reg, [\src_reg], \offset
	.else
	ldrb \dest_reg, [\src_reg]
	.endif
#else
	ldtrb \dest_reg, [\src_reg]
	.ifnc \offset, #0
	add \src_reg, \src_reg, \offset
	.endif
#endif /* CONFIG_AARCH64_UAO */
.endm

.macro a64_alter_strb3 src_reg, dest_reg, offset
#ifdef __ASM_DO_NOT_ALTER_STR
	.ifnc \offset, #0
	strb \src_reg, [\dest_reg], \offset
	.else
	strb \src_reg, [\dest_reg]
	.endif
#else
	sttrb \src_reg, [\dest_reg]
	.ifnc \offset, #0
	add \dest_reg, \dest_reg, \offset
	.endif
#endif /* CONFIG_AARCH64_UAO */
.endm

/*
 * stp and ldp with post-index, the post-index
 * will be added after reading/writing
 */
.macro a64_alter_ldp4_post reg1, reg2, src_reg, post_inc
#ifdef __ASM_DO_NOT_ALTER_LDR
	.ifnc \post_inc, #0
	ldp \reg1, \reg2, [\src_reg], \post_inc
	.else
	ldp \reg1, \reg2, [\src_reg]
	.endif
#else
	ldtr \reg1, [\src_reg]
	ldtr \reg2, [\src_reg, #8]
	.ifnc \post_inc, #0
	add  \src_reg, \src_reg, \post_inc
	.endif
#endif /* CONFIG_AARCH64_UAO */
.endm

.macro a64_alter_stp4_post reg1, reg2, src_reg, post_inc
#ifdef __ASM_DO_NOT_ALTER_STR
	.ifnc \post_inc, #0
	stp \reg1, \reg2, [\src_reg], \post_inc
	.else
	stp \reg1, \reg2, [\src_reg]
	.endif
#else
	sttr \reg1, [\src_reg]
	sttr \reg2, [\src_reg, #8]
	.ifnc \post_inc, #0
	add  \src_reg, \src_reg, \post_inc
	.endif
#endif /* CONFIG_AARCH64_UAO */
.endm

/*
 * stp and ldp with pre-index, the pre-index
 * will be added before reading/writing
 */
.macro a64_alter_ldp4_pre reg1, reg2, src_reg, pre_inc
#ifdef __ASM_DO_NOT_ALTER_LDR
	ldp \reg1, \reg2, [\src_reg, \pre_inc]!
#else
	add  \src_reg, \src_reg, \pre_inc
	ldtr \reg1, [\src_reg]
	ldtr \reg2, [\src_reg, #8]
#endif /* CONFIG_AARCH64_UAO */
.endm

.macro a64_alter_stp4_pre reg1, reg2, src_reg, pre_inc
#ifdef __ASM_DO_NOT_ALTER_STR
	stp \reg1, \reg2, [\src_reg, \pre_inc]!
#else
	add  \src_reg, \src_reg, \pre_inc
	sttr \reg1, [\src_reg]
	sttr \reg2, [\src_reg, #8]
#endif /* CONFIG_AARCH64_UAO */
.endm

/*
 * stp and ldp with offset, the offset will not be added
 */
.macro a64_alter_ldp4_offset reg1, reg2, src_reg, offset
#ifdef __ASM_DO_NOT_ALTER_LDR
	ldp \reg1, \reg2, [\src_reg, \offset]
#else
	add  \src_reg, \src_reg, \offset
	ldtr \reg1, [\src_reg]
	ldtr \reg2, [\src_reg, #8]
	sub  \src_reg, \src_reg, \offset
#endif /* CONFIG_AARCH64_UAO */
.endm

.macro a64_alter_stp4_offset reg1, reg2, src_reg, offset
#ifdef __ASM_DO_NOT_ALTER_STR
	stp \reg1, \reg2, [\src_reg, \offset]
#else
	add  \src_reg, \src_reg, \offset
	sttr \reg1, [\src_reg]
	sttr \reg2, [\src_reg, #8]
	sub  \src_reg, \src_reg, \offset
#endif /* CONFIG_AARCH64_UAO */
.endm

.macro a64_alter_ldr2 dest_reg, src_reg
#ifdef __ASM_DO_NOT_ALTER_LDR
	ldr \dest_reg, [\src_reg]
#else
	ldtr \dest_reg, [\src_reg]
#endif /* CONFIG_AARCH64_UAO */
.endm

.macro a64_alter_str2 src_reg, dest_reg
#ifdef __ASM_DO_NOT_ALTER_STR
	str \src_reg, [\dest_reg]
#else
	sttr \src_reg, [\dest_reg]
#endif /* CONFIG_AARCH64_UAO */
.endm

#endif
