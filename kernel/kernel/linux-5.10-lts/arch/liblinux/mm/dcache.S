.global __dma_flush_area
.type __dma_flush_area, %function

/*
 *	__dma_flush_area(vstart, size)
 *
 *	Must confirm that any D-cache lines for [kaddr, kaddr+size)
 *	are cleaned and invalidated to the PoC.
 *
 *	- x0    - vstart
 *	- x1    - size
 */
__dma_flush_area:
	add	x1, x0, x1
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
	ret
.size __dma_flush_area, .-__dma_flush_area
