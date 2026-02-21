/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: memset implementation
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 21 14:19:23 CST 2023
 */

/*
 * __arch_memzero - fast memory zero implementation using aarch64 dcache zero
                    feature and instruction parallelism.
 * @x0: the start address, must be 64 bytes aligned.
 * @x1: the size, must be multiple of 256.
 */
.global __arch_memzero
__arch_memzero:
#ifndef CONFIG_HAVE_DC_ZVA_SIZE_64
	mrs	x2, dczid_el0
	and	x2, x2, 31
	cmp	x2, 4		/* only 64 bytes ZVA size is implemented, usually true. */
	b.ne	.Lno_zva_loop
#endif

	add	x2, x0, 64
	add	x3, x0, 64 * 2
	add	x4, x0, 64 * 3
	.p2align 4
.Lzva_loop:
	/* These can speedup 22% compared to single dc, shown by benchmark on A77. */
	dc	zva, x0
	dc	zva, x2
	dc	zva, x3
	dc	zva, x4
	add	x0, x0, 64 * 4
	add	x2, x2, 64 * 4
	add	x3, x3, 64 * 4
	add	x4, x4, 64 * 4
	subs	x1, x1, 64 * 4
	b.hi	.Lzva_loop
	ret

#ifndef CONFIG_HAVE_DC_ZVA_SIZE_64
	.p2align 4
.Lno_zva_loop:
	stp	xzr, xzr, [x0]
	stp	xzr, xzr, [x0, 16]
	stp	xzr, xzr, [x0, 32]
	stp	xzr, xzr, [x0, 48]
	add	x0, x0, 64
	subs	x1, x1, 64
	b.hi	.Lno_zva_loop
	ret
#endif
