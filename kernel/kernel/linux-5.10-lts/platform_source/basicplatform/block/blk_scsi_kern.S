	.text
	.file	"blk_scsi_kern.c"
	.globl	blk_scsi_kern_ioctl             // -- Begin function blk_scsi_kern_ioctl
	.p2align	2
	.type	blk_scsi_kern_ioctl,@function
blk_scsi_kern_ioctl:                    // @blk_scsi_kern_ioctl
// %bb.0:
	sub	sp, sp, #288
	adrp	x8, :got:__stack_chk_guard
	stp	x29, x30, [sp, #192]            // 16-byte Folded Spill
	stp	x28, x27, [sp, #208]            // 16-byte Folded Spill
	add	x29, sp, #192
	stp	x26, x25, [sp, #224]            // 16-byte Folded Spill
	mov	x28, x1
	stp	x24, x23, [sp, #240]            // 16-byte Folded Spill
	mov	x27, x1
	stp	x22, x21, [sp, #256]            // 16-byte Folded Spill
	mov	x24, x0
	stp	x20, x19, [sp, #272]            // 16-byte Folded Spill
	mov	x19, x1
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldr	x8, [x8]
	stur	x8, [x29, #-16]
	ldr	x10, [x1, #32]
	ldr	x11, [x28, #24]!
	ldr	x13, [x27, #144]!
	mov	x23, x27
	ldr	x26, [x24]
	ldp	w12, w17, [x1, #40]
	stp	x11, x10, [x29, #-40]
	ldr	x10, [x27, #8]
	ldr	x14, [x23, #-48]!
	ldp	x6, x5, [x1, #72]
	stur	w12, [x29, #-24]
	ldp	w9, w8, [x1]
	ldp	w20, w21, [x1, #8]
	ldp	w3, w22, [x1, #56]
	ldp	w25, w7, [x1, #64]
	stp	x10, x14, [x29, #-64]
	ldr	x16, [x1, #16]
	ldp	w12, w14, [x27, #-8]
	ldr	x2, [x1, #48]
	stur	x13, [x29, #-72]
	ldr	w1, [x1, #88]
	ldr	w0, [x19, #92]
	ldur	w15, [x27, #-40]
	ldur	w4, [x27, #-24]
	ldur	w30, [x27, #-16]
	ldr	x11, [x26, #1360]
	stur	w15, [x29, #-48]
	cbz	x11, .LBB0_15
// %bb.1:
	stp	w25, w0, [x29, #-80]            // 8-byte Folded Spill
	mov	x25, #-22
	cmp	w9, #81
	b.ne	.LBB0_16
// %bb.2:
	cbnz	w8, .LBB0_16
// %bb.3:
	and	w8, w20, #0xfffffffd
	cbnz	w8, .LBB0_16
// %bb.4:
	cmp	w22, #0
	mov	w8, #32
	str	x2, [sp, #64]                   // 8-byte Folded Spill
	mov	x0, x26
	stur	w1, [x29, #-92]                 // 4-byte Folded Spill
	cinc	w1, w8, ne
	mov	w2, wzr
	stp	w14, w12, [sp, #40]             // 8-byte Folded Spill
	stp	w30, w4, [sp, #48]              // 8-byte Folded Spill
	str	w7, [sp, #76]                   // 4-byte Folded Spill
	stp	x6, x5, [sp, #80]               // 16-byte Folded Spill
	str	w3, [sp, #56]                   // 4-byte Folded Spill
	str	w17, [sp, #60]                  // 4-byte Folded Spill
	stur	x16, [x29, #-88]                // 8-byte Folded Spill
	bl	blk_get_request
	mov	x25, x0
	cmn	x0, #1, lsl #12                 // =4096
	b.hi	.LBB0_16
// %bb.5:
	cmp	w21, #16
	b.hi	.LBB0_20
// %bb.6:
	ldr	x0, [x25, #312]
.LBB0_7:
	ldur	x1, [x29, #-88]                 // 8-byte Folded Reload
	mov	x2, x21
	bl	memcpy
	ldur	w0, [x29, #-92]                 // 4-byte Folded Reload
	strh	w21, [x25, #320]
	bl	__msecs_to_jiffies
	str	w0, [x25, #232]
	cbnz	w0, .LBB0_18
// %bb.8:
	ldr	w0, [x26, #1108]
	str	w0, [x25, #232]
	cbnz	w0, .LBB0_18
// %bb.9:
	ldp	x2, x9, [sp, #80]               // 16-byte Folded Reload
	mov	w8, #6000
	ldr	w10, [sp, #76]                  // 4-byte Folded Reload
.LBB0_10:
	str	w8, [x25, #232]
.LBB0_11:
	mov	w3, w22
	cbnz	w22, .LBB0_13
// %bb.12:
	mov	w3, w10
	mov	x2, x9
	cbz	w10, .LBB0_14
.LBB0_13:
	mov	x0, x26
	mov	x1, x25
	mov	w4, #3264
	bl	blk_rq_map_kern
	cbnz	w0, .LBB0_22
.LBB0_14:
	mov	x26, x25
	b	.LBB0_26
.LBB0_15:
	mov	x25, #-6
.LBB0_16:
	adrp	x8, :got:__stack_chk_guard
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldur	x9, [x29, #-16]
	ldr	x8, [x8]
	cmp	x8, x9
	b.ne	.LBB0_34
// %bb.17:
	mov	x0, x25
	ldp	x20, x19, [sp, #272]            // 16-byte Folded Reload
	ldp	x22, x21, [sp, #256]            // 16-byte Folded Reload
	ldp	x24, x23, [sp, #240]            // 16-byte Folded Reload
	ldp	x26, x25, [sp, #224]            // 16-byte Folded Reload
	ldp	x28, x27, [sp, #208]            // 16-byte Folded Reload
	ldp	x29, x30, [sp, #192]            // 16-byte Folded Reload
	add	sp, sp, #288
	ret
.LBB0_18:
	ldp	x2, x9, [sp, #80]               // 16-byte Folded Reload
	cmp	w0, #699
	ldr	w10, [sp, #76]                  // 4-byte Folded Reload
	b.hi	.LBB0_11
// %bb.19:
	mov	w8, #700
	b	.LBB0_10
.LBB0_20:
	mov	x0, x21
	mov	w1, #3520
	bl	__kmalloc
	str	x0, [x25, #312]
	cbnz	x0, .LBB0_7
// %bb.21:
	mov	x26, #-12
	b	.LBB0_23
.LBB0_22:
	mov	w8, w0
	ldr	x0, [x25, #312]
	sxtw	x26, w8
.LBB0_23:
	add	x8, x25, #296
	cmp	x0, x8
	b.eq	.LBB0_25
// %bb.24:
	bl	kfree
.LBB0_25:
	mov	x0, x25
	bl	blk_put_request
.LBB0_26:
	ldur	w9, [x29, #-76]                 // 4-byte Folded Reload
	cmn	x26, #4095
	b.lo	.LBB0_28
// %bb.27:
	mov	x25, x26
	b	.LBB0_16
.LBB0_28:
	mov	w8, #1
	ldr	x0, [x24]
	bic	w3, w8, w9, lsr #4
	mov	x1, xzr
	mov	x2, x26
	bl	blk_execute_rq
	ldr	w24, [x26, #324]
	add	x25, x26, #296
	str	xzr, [sp, #32]                  // 8-byte Folded Spill
	and	w8, w24, #0xff
	ubfx	w9, w24, #16, #8
	lsr	w10, w24, #24
	stp	w9, w8, [sp, #24]               // 8-byte Folded Spill
	orr	w8, w8, w9
	orr	w8, w8, w10
	cmp	w8, #0
	ldr	w8, [x26, #328]
	cset	w9, ne
	stp	w9, w10, [sp, #16]              // 8-byte Folded Spill
	cbz	w8, .LBB0_31
// %bb.29:
	ldr	x0, [sp, #64]                   // 8-byte Folded Reload
	cbz	x0, .LBB0_31
// %bb.30:
	ldr	w9, [sp, #60]                   // 4-byte Folded Reload
	ldr	x1, [x26, #344]
	cmp	w9, w8
	csel	w2, w9, w8, lo
	str	x2, [sp, #32]                   // 8-byte Folded Spill
	bl	memcpy
	ldr	w24, [x26, #324]
.LBB0_31:
	ldr	w8, [x26, #24]
	ldr	w9, [x26, #332]
	ldr	x0, [x26, #312]
	tst	w8, #0x1
	ldr	w8, [sp, #44]                   // 4-byte Folded Reload
	csel	w8, w9, w8, eq
	str	w8, [sp, #12]                   // 4-byte Folded Spill
	ldr	w8, [sp, #40]                   // 4-byte Folded Reload
	csel	w8, w8, w9, eq
	cmp	x0, x25
	str	w8, [sp, #44]                   // 4-byte Folded Spill
	b.eq	.LBB0_33
// %bb.32:
	bl	kfree
.LBB0_33:
	mov	x0, x26
	cmp	w24, #0
	csel	w24, w24, wzr, lt
	bl	blk_put_request
	ldur	x10, [x29, #-88]                // 8-byte Folded Reload
	mov	w8, #81
	ldur	w11, [x29, #-24]
	stp	w20, w21, [x19, #8]
	sxtw	x25, w24
	str	x10, [x19, #16]
	ldp	x9, x10, [x29, #-40]
	str	x8, [x19]
	ldr	w8, [sp, #60]                   // 4-byte Folded Reload
	str	w11, [x28, #16]
	ldur	w11, [x29, #-80]                // 4-byte Folded Reload
	stp	x9, x10, [x28]
	ldur	x9, [x29, #-56]
	str	w8, [x19, #44]
	ldr	x8, [sp, #64]                   // 8-byte Folded Reload
	ldur	w10, [x29, #-76]                // 4-byte Folded Reload
	str	x8, [x19, #48]
	ldr	w8, [sp, #56]                   // 4-byte Folded Reload
	stp	w8, w22, [x19, #56]
	ldr	w8, [sp, #76]                   // 4-byte Folded Reload
	stp	w11, w8, [x19, #64]
	ldr	x11, [sp, #80]                  // 8-byte Folded Reload
	ldr	x8, [sp, #88]                   // 8-byte Folded Reload
	stp	x11, x8, [x19, #72]
	ldur	w8, [x29, #-48]
	ldur	w11, [x29, #-92]                // 4-byte Folded Reload
	stp	w11, w10, [x19, #88]
	ldr	x11, [sp, #32]                  // 8-byte Folded Reload
	str	w8, [x23, #8]
	ldr	w8, [sp, #24]                   // 4-byte Folded Reload
	str	x9, [x23]
	ldr	w9, [sp, #20]                   // 4-byte Folded Reload
	stp	w9, w8, [x19, #108]
	ldr	w9, [sp, #28]                   // 4-byte Folded Reload
	ldr	w8, [sp, #52]                   // 4-byte Folded Reload
	stp	w9, w8, [x19, #116]
	ldr	w9, [sp, #16]                   // 4-byte Folded Reload
	ldp	w10, w8, [sp, #44]              // 8-byte Folded Reload
	stp	w9, w8, [x19, #124]
	ldr	w9, [sp, #12]                   // 4-byte Folded Reload
	str	w10, [x19, #140]
	stp	w11, w9, [x19, #132]
	ldp	x8, x9, [x29, #-72]
	stp	x8, x9, [x27]
	b	.LBB0_16
.LBB0_34:
	bl	__stack_chk_fail
.Lfunc_end0:
	.size	blk_scsi_kern_ioctl, .Lfunc_end0-blk_scsi_kern_ioctl
                                        // -- End function
	.ident	"OHOS (BiSheng Mobile CPU STD 202.1.0.B030) clang version 15.0.4 (llvm-project 7123208073d2)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
