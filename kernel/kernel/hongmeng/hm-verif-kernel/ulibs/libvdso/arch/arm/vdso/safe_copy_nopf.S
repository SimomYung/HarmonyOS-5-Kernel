/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Implemnetion for safe_copy with nopagefault
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 29 11:50:15 2023
 */
.syntax unified
.section ".text", "ax"

.global safe_copy_nopf
.global _except_tag_nopf_start
.global _except_tag_nopf_end

/*
 * safe_copy_nopf will skip the page fault flow when handle data fault
 */
.type safe_copy_nopf, %function
safe_copy_nopf:
_except_tag_nopf_start:
	#include "copy_template.S"
	mov	r0, #0
	bx	lr
_except_tag_nopf_end:
	nop
.size safe_copy_nopf, . - safe_copy_nopf
