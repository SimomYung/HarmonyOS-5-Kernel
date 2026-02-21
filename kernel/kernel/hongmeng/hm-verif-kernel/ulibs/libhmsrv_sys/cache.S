/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: direct flush cache api
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jun 12 09:40:43 2023
 */

.global flush_dcache_range_el0
.type flush_dcache_range_el0, %function

/*
 *	flush_dcache_range_el0(vstart, vend)
 *
 *	Must confirm that any D-cache lines for [kaddr, kaddr+size)
 *	are cleaned and invalidated to the PoC.
 *
 *	- x0    - vstart
 *	- x1    - vend
 */
flush_dcache_range_el0:
#ifndef __HOST_LLT__
	mrs	x3, ctr_el0				// read CTR
	nop
	ubfm	x3, x3, #16, #19			// cache line size encoding
	mov	x2, #4					// bytes per word
	lsl	x2, x2, x3				// actual cache line size
	sub	x3, x2, #1
	bic	x0, x0, x3
1:
	dc	civac, x0
	add	x0, x0, x2
	cmp	x0, x1
	b.lo	1b
	dsb	sy
	mov	x0, #0
#endif
	ret
.size flush_dcache_range_el0, .-flush_dcache_range_el0


/*
 *	clean_dcache_range_el0(vstart, vend)
 *
 *	Must confirm that any D-cache lines for [kaddr, kaddr+size)
 *	are cleaned to the PoC.
 *
 *	- x0    - vstart
 *	- x1    - vend
 */
.global clean_dcache_range_el0
.type clean_dcache_range_el0, %function
clean_dcache_range_el0:
#ifndef __HOST_LLT__
	mrs	x3, ctr_el0				// read CTR
	nop
	ubfm	x3, x3, #16, #19			// cache line size encoding
	mov	x2, #4					// bytes per word
	lsl	x2, x2, x3				// actual cache line size
	sub	x3, x2, #1
	bic	x0, x0, x3
1:
	dc	cvac, x0
	add	x0, x0, x2
	cmp	x0, x1
	b.lo	1b
	dsb	sy
	mov	x0, #0
#endif
	ret
.size clean_dcache_range_el0, .-clean_dcache_range_el0
