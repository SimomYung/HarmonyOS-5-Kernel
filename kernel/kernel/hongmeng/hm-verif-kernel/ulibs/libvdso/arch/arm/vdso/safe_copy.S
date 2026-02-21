/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 14 19:24:38 2019
 */
.syntax unified
.section ".text", "ax"

.global safe_copy
.global _except_tag_start
.global _except_tag_end
.global _except_fixup
.type safe_copy, %function
safe_copy:
#ifndef CONFIG_U_HW_UNALIGN_ACCESS
	orr	r3, r0, r1
	ands	r3, r3, #0x3
	beq	safe_copy_word
_except_tag_start:
	subs	r2, r2, #1
	ldrbpl	r3, [r1], #1
	strbpl	r3, [r0], #1
	bpl	_except_tag_start
	mov	r0, #0 //return the normal value 0
	bx	lr
safe_copy_word:
	cmp	r2, #4
	mov	r3, r1
	bpl	1f
	cmp	r2, #2
	ldrhpl	r1, [r3], #2
	subpl	r2, r2, #2
	strhpl	r1, [r0], #2
	cmp	r2, #1
	ldrbeq	r3, [r3]
	strbeq	r3, [r0]
	mov	r0, #0 //return the normal value 0
	bx	lr
1:	ldr	r3, [r1], #4
	sub	r2, r2, #4
	str	r3, [r0], #4
	b	safe_copy_word
#else
_except_tag_start:
	#include "copy_template.S"
	mov	r0, #0
	bx	lr
#endif
_except_tag_end:
	nop
_except_fixup:
#ifdef CONFIG_U_HW_UNALIGN_ACCESS
	cmp	ip, #4
	popeq	{ip}
	beq	1f
	cmp	ip, #64
	popls	{ip, lr}
	bls	1f
	add	sp, sp, #48
	ldmia	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, ip, lr}
1:
#endif
	mov	r0, #-14 //return the value of -EFAULT
	bx	lr
.size safe_copy, . - safe_copy

.global safe_copy_align
.set safe_copy_align, safe_copy

/* Local alias */
.global __safe_copy
.set __safe_copy, safe_copy
