	.text
	.file	"blk_scsi_kern.c"
	.globl	blk_scsi_kern_ioctl             // -- Begin function blk_scsi_kern_ioctl
	.p2align	2
	.type	blk_scsi_kern_ioctl,@function
blk_scsi_kern_ioctl:                    // @blk_scsi_kern_ioctl
// %bb.0:
	sub	sp, sp, #304
	adrp	x8, :got:__stack_chk_guard
	stp	x29, x30, [sp, #208]            // 16-byte Folded Spill
	stp	x28, x27, [sp, #224]            // 16-byte Folded Spill
	add	x29, sp, #208
	stp	x26, x25, [sp, #240]            // 16-byte Folded Spill
	mov	x28, x1
	stp	x24, x23, [sp, #256]            // 16-byte Folded Spill
	mov	x24, x0
	stp	x22, x21, [sp, #272]            // 16-byte Folded Spill
	stp	x20, x19, [sp, #288]            // 16-byte Folded Spill
	mov	x19, x1
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	mov	x27, x19
	ldr	x8, [x8]
	stur	x8, [x29, #-16]
	ldr	x8, [x1, #32]
	ldr	x9, [x28, #24]!
	ldr	x11, [x27, #144]!
	mov	x20, x27
	ldr	x26, [x24]
	ldp	w10, w0, [x1, #40]
	stp	x9, x8, [x29, #-40]
	ldr	x8, [x27, #8]
	ldr	x12, [x20, #-48]!
	ldp	x6, x5, [x19, #72]
	stur	w10, [x29, #-24]
	ldp	w22, w21, [x1]
	ldp	w15, w16, [x1, #8]
	ldp	w2, w3, [x19, #56]
	ldp	w25, w7, [x19, #64]
	ldp	w23, w17, [x19, #88]
	stp	x8, x12, [x29, #-64]
	ldr	x14, [x1, #16]
	ldp	w10, w12, [x27, #-8]
	ldr	x1, [x1, #48]
	stur	x11, [x29, #-72]
	ldur	w13, [x27, #-40]
	ldur	w4, [x27, #-24]
	ldur	w30, [x27, #-16]
	ldr	x9, [x26, #1360]
	stur	w13, [x29, #-48]
	cbz	x9, .LBB0_6
// %bb.1:
	stp	w25, w17, [x29, #-80]           // 8-byte Folded Spill
	mov	x25, #-22
	cmp	w22, #81
	b.ne	.LBB0_7
// %bb.2:
	cbnz	w21, .LBB0_7
// %bb.3:
	and	w8, w15, #0xfffffffd
	cbnz	w8, .LBB0_7
// %bb.4:
	cmp	w3, #0
	mov	w8, #32
	str	w2, [sp, #60]                   // 4-byte Folded Spill
	mov	w2, wzr
	str	x1, [sp, #72]                   // 8-byte Folded Spill
	cinc	w1, w8, ne
	str	w0, [sp, #68]                   // 4-byte Folded Spill
	mov	x0, x26
	stp	w12, w10, [sp, #44]             // 8-byte Folded Spill
	stp	w30, w4, [sp, #52]              // 8-byte Folded Spill
	str	w7, [sp, #84]                   // 4-byte Folded Spill
	stp	x6, x5, [sp, #88]               // 16-byte Folded Spill
	stur	x16, [x29, #-88]                // 8-byte Folded Spill
	str	w15, [sp, #64]                  // 4-byte Folded Spill
	stur	x14, [x29, #-96]                // 8-byte Folded Spill
	stur	w3, [x29, #-100]                // 4-byte Folded Spill
	bl	blk_get_request
	cmn	x0, #1, lsl #12                 // =4096
	b.ls	.LBB0_9
// %bb.5:
	mov	x25, x0
	b	.LBB0_7
.LBB0_6:
	mov	x25, #-6
.LBB0_7:
	adrp	x8, :got:__stack_chk_guard
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldur	x9, [x29, #-16]
	ldr	x8, [x8]
	cmp	x8, x9
	b.ne	.LBB0_35
// %bb.8:
	mov	x0, x25
	ldp	x20, x19, [sp, #288]            // 16-byte Folded Reload
	ldp	x22, x21, [sp, #272]            // 16-byte Folded Reload
	ldp	x24, x23, [sp, #256]            // 16-byte Folded Reload
	ldp	x26, x25, [sp, #240]            // 16-byte Folded Reload
	ldp	x28, x27, [sp, #224]            // 16-byte Folded Reload
	ldp	x29, x30, [sp, #208]            // 16-byte Folded Reload
	add	sp, sp, #304
	ret
.LBB0_9:
	ldur	x8, [x29, #-88]                 // 8-byte Folded Reload
	mov	x25, x0
	str	w23, [sp, #40]                  // 4-byte Folded Spill
	cmp	w8, #16
	b.hi	.LBB0_21
// %bb.10:
	ldr	x0, [x25, #312]
.LBB0_11:
	ldur	x1, [x29, #-96]                 // 8-byte Folded Reload
	mov	x2, x8
	mov	x23, x8
	bl	memcpy
	ldr	w0, [sp, #40]                   // 4-byte Folded Reload
	strh	w23, [x25, #320]
	bl	__msecs_to_jiffies
	str	w0, [x25, #232]
	cbnz	w0, .LBB0_19
// %bb.12:
	ldr	w0, [x26, #1108]
	str	w0, [x25, #232]
	cbnz	w0, .LBB0_19
// %bb.13:
	ldp	x2, x10, [sp, #88]              // 16-byte Folded Reload
	mov	w8, #6000
	ldur	w9, [x29, #-100]                // 4-byte Folded Reload
	ldr	w11, [sp, #84]                  // 4-byte Folded Reload
.LBB0_14:
	str	w8, [x25, #232]
.LBB0_15:
	mov	w3, w9
	cbnz	w9, .LBB0_17
// %bb.16:
	mov	w3, w11
	mov	x2, x10
	cbz	w11, .LBB0_18
.LBB0_17:
	mov	x0, x26
	mov	x1, x25
	mov	w4, #3264
	bl	blk_rq_map_kern
	cbnz	w0, .LBB0_23
.LBB0_18:
	mov	x26, x25
	b	.LBB0_27
.LBB0_19:
	ldp	x2, x10, [sp, #88]              // 16-byte Folded Reload
	cmp	w0, #699
	ldur	w9, [x29, #-100]                // 4-byte Folded Reload
	ldr	w11, [sp, #84]                  // 4-byte Folded Reload
	b.hi	.LBB0_15
// %bb.20:
	mov	w8, #700
	b	.LBB0_14
.LBB0_21:
	mov	x0, x8
	mov	w1, #3520
	bl	__kmalloc
	ldur	x8, [x29, #-88]                 // 8-byte Folded Reload
	str	x0, [x25, #312]
	cbnz	x0, .LBB0_11
// %bb.22:
	mov	x26, #-12
	b	.LBB0_24
.LBB0_23:
	mov	w8, w0
	ldr	x0, [x25, #312]
	sxtw	x26, w8
.LBB0_24:
	add	x8, x25, #296
	cmp	x0, x8
	b.eq	.LBB0_26
// %bb.25:
	bl	kfree
.LBB0_26:
	mov	x0, x25
	bl	blk_put_request
.LBB0_27:
	ldur	w23, [x29, #-76]                // 4-byte Folded Reload
	cmn	x26, #4095
	b.lo	.LBB0_29
// %bb.28:
	mov	x25, x26
	b	.LBB0_7
.LBB0_29:
	mov	w8, #1
	ldr	x0, [x24]
	bic	w3, w8, w23, lsr #4
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
	cbz	w8, .LBB0_32
// %bb.30:
	ldr	x9, [sp, #72]                   // 8-byte Folded Reload
	cbz	x9, .LBB0_32
// %bb.31:
	ldr	w9, [sp, #68]                   // 4-byte Folded Reload
	ldr	x1, [x26, #344]
	ldr	x0, [sp, #72]                   // 8-byte Folded Reload
	cmp	w9, w8
	csel	w2, w9, w8, lo
	str	x2, [sp, #32]                   // 8-byte Folded Spill
	bl	memcpy
	ldr	w24, [x26, #324]
.LBB0_32:
	ldr	w8, [x26, #24]
	ldr	w9, [x26, #332]
	ldr	x0, [x26, #312]
	tst	w8, #0x1
	ldr	w8, [sp, #48]                   // 4-byte Folded Reload
	csel	w8, w9, w8, eq
	str	w8, [sp, #12]                   // 4-byte Folded Spill
	ldr	w8, [sp, #44]                   // 4-byte Folded Reload
	csel	w8, w8, w9, eq
	cmp	x0, x25
	str	w8, [sp, #48]                   // 4-byte Folded Spill
	b.eq	.LBB0_34
// %bb.33:
	bl	kfree
.LBB0_34:
	mov	x0, x26
	cmp	w24, #0
	csel	w24, w24, wzr, lt
	bl	blk_put_request
	ldr	w10, [sp, #64]                  // 4-byte Folded Reload
	stp	w22, w21, [x19]
	ldur	x9, [x29, #-88]                 // 8-byte Folded Reload
	sxtw	x25, w24
	ldr	x11, [sp, #32]                  // 8-byte Folded Reload
	stp	w10, w9, [x19, #8]
	ldur	x10, [x29, #-96]                // 8-byte Folded Reload
	ldp	x8, x9, [x29, #-40]
	str	x10, [x19, #16]
	ldur	w10, [x29, #-24]
	stp	x8, x9, [x28]
	ldr	w8, [sp, #68]                   // 4-byte Folded Reload
	str	w10, [x28, #16]
	ldr	w10, [sp, #60]                  // 4-byte Folded Reload
	ldur	x9, [x29, #-56]
	str	w8, [x19, #44]
	ldr	x8, [sp, #72]                   // 8-byte Folded Reload
	str	x8, [x19, #48]
	ldur	w8, [x29, #-100]                // 4-byte Folded Reload
	stp	w10, w8, [x19, #56]
	ldur	w10, [x29, #-80]                // 4-byte Folded Reload
	ldr	w8, [sp, #84]                   // 4-byte Folded Reload
	stp	w10, w8, [x19, #64]
	ldr	x10, [sp, #88]                  // 8-byte Folded Reload
	ldr	x8, [sp, #96]                   // 8-byte Folded Reload
	stp	x10, x8, [x19, #72]
	ldur	w8, [x29, #-48]
	ldr	w10, [sp, #40]                  // 4-byte Folded Reload
	stp	w10, w23, [x19, #88]
	str	w8, [x20, #8]
	ldr	w8, [sp, #24]                   // 4-byte Folded Reload
	str	x9, [x20]
	ldr	w9, [sp, #20]                   // 4-byte Folded Reload
	stp	w9, w8, [x19, #108]
	ldr	w9, [sp, #28]                   // 4-byte Folded Reload
	ldr	w8, [sp, #56]                   // 4-byte Folded Reload
	stp	w9, w8, [x19, #116]
	ldr	w9, [sp, #16]                   // 4-byte Folded Reload
	ldp	w10, w8, [sp, #48]              // 8-byte Folded Reload
	stp	w9, w8, [x19, #124]
	ldr	w9, [sp, #12]                   // 4-byte Folded Reload
	str	w10, [x19, #140]
	stp	w11, w9, [x19, #132]
	ldp	x8, x9, [x29, #-72]
	stp	x8, x9, [x27]
	b	.LBB0_7
.LBB0_35:
	bl	__stack_chk_fail
.Lfunc_end0:
	.size	blk_scsi_kern_ioctl, .Lfunc_end0-blk_scsi_kern_ioctl
                                        // -- End function
	.ident	"OHOS (BiSheng Mobile STD 203.0.0.2.B001) clang version 15.0.4 (llvm-project db54e0a809bd)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
