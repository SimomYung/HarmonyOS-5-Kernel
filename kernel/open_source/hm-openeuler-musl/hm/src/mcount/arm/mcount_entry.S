/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Thursday Aug 29, 2019 09:09:30 CST
 */

/* ARM profiling entry point is '__gnu_mcount_nc' since gcc 4.4 */
.globl __gnu_mcount_nc
.type __gnu_mcount_nc, %function

__gnu_mcount_nc:
	push	{r0, r1, r2, r3, lr}
	subs	r1, lr, #0		/* callee - current lr */
	ldr	r0, [sp, #20]		/* caller - at the top of the stack */
	bl	mcount_internal
	pop	{r0, r1, r2, r3, ip, lr}
	bx	ip
#ifdef __clang__
.end
#else
.end __gnu_mcount_nc
#endif
