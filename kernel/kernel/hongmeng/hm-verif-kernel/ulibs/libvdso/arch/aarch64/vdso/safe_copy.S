/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 14 19:24:38 2019
 */
#undef CONFIG_AARCH64_UAO
#include "hmasm/alternative.h"

.section ".text", "ax"

.global safe_copy
.global safe_copy_align
.global _except_tag_start
.global _except_tag_end
.global _except_fixup
.type safe_copy_align, %function
safe_copy_align:
	orr	x3, x0, x1
	ands	x3, x3, #0x7
	beq	safe_copy_word
_except_tag_start:
	0: subs	x2, x2, #1
	b.mi	5f
	ldrb	w3, [x1], #1
	strb	w3, [x0], #1
	b	0b
safe_copy_word:
	mov     x5, x0
	subs    x2, x2, #8
	b.mi    2f
	1:  ldr x3, [x1], #8
	subs    x2, x2, #8
	str x3, [x5], #8
	b.pl    1b
	2:  adds    x2, x2, #4
	b.mi    3f
	ldr w3, [x1], #4
	sub x2, x2, #4
	str w3, [x5], #4
	3:  adds    x2, x2, #2
	b.mi    4f
	ldrh    w3, [x1], #2
	sub x2, x2, #2
	strh    w3, [x5], #2
	4:  adds    x2, x2, #1
	b.mi    5f
	ldrb    w3, [x1]
	strb    w3, [x5]
	5:  mov     x0, #0
	ret

.type safe_copy, %function
safe_copy:
	#include "copy_template.S"
_except_tag_end:
	5:  mov     x0, #0
	ret
_except_fixup:
	mov	x0, #-14  //return the value of -EFAULT
	ret
.size safe_copy, . - safe_copy

/* Local alias */
.global __safe_copy
.set __safe_copy, safe_copy
