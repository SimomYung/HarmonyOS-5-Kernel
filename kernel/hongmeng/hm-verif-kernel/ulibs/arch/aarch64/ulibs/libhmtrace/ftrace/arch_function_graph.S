/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Define for function graph return handler trampoline
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 16 9:53:00 2020
 */

.global return_handler_trampoline
.type return_handler_trampoline, %function
return_handler_trampoline:
	sub sp, sp, #16 * 4
	stp x0, x1, [sp, #16 * 0]
	stp x2, x3, [sp, #16 * 1]
	stp x4, x5, [sp, #16 * 2]
	stp x6, x7, [sp, #16 * 3]

	bl return_handler_hook
	mov x30, x0

	ldp x0, x1, [sp, #16 * 0]
	ldp x2, x3, [sp, #16 * 1]
	ldp x4, x5, [sp, #16 * 2]
	ldp x6, x7, [sp, #16 * 3]
	add sp, sp, #16 * 4
	ret
.size return_handler_trampoline, . - return_handler_trampoline
