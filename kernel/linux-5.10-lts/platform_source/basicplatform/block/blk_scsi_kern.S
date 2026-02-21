	.text
	.file	"blk_scsi_kern.c"
	.globl	blk_scsi_kern_time_stamp_dump   // -- Begin function blk_scsi_kern_time_stamp_dump
	.p2align	2
	.type	blk_scsi_kern_time_stamp_dump,@function
blk_scsi_kern_time_stamp_dump:          // @blk_scsi_kern_time_stamp_dump
// %bb.0:
	stp	x29, x30, [sp, #-32]!           // 16-byte Folded Spill
	stp	x20, x19, [sp, #16]             // 16-byte Folded Spill
	adrp	x20, rpmb_time_blk_point
	add	x20, x20, :lo12:rpmb_time_blk_point
	adrp	x19, .L.str
	add	x19, x19, :lo12:.L.str
	mov	w1, wzr
	mov	x0, x19
	mov	x29, sp
	ldr	x2, [x20]
	ldr	x3, [x20, #24]
	ldr	x4, [x20, #48]
	bl	printk
	ldr	x2, [x20, #8]
	mov	x0, x19
	ldr	x3, [x20, #32]
	mov	w1, wzr
	ldr	x4, [x20, #56]
	bl	printk
	ldr	x2, [x20, #16]
	mov	x0, x19
	ldr	x3, [x20, #40]
	mov	w1, wzr
	ldr	x4, [x20, #64]
	bl	printk
	ldr	x2, [x20, #72]
	mov	x0, x19
	ldr	x3, [x20, #96]
	mov	w1, #1
	ldr	x4, [x20, #120]
	bl	printk
	ldr	x2, [x20, #80]
	mov	x0, x19
	ldr	x3, [x20, #104]
	mov	w1, #1
	ldr	x4, [x20, #128]
	bl	printk
	ldr	x2, [x20, #88]
	mov	x0, x19
	ldr	x3, [x20, #112]
	mov	w1, #1
	ldr	x4, [x20, #136]
	bl	printk
	ldr	x2, [x20, #144]
	mov	x0, x19
	ldr	x3, [x20, #168]
	mov	w1, #2
	ldr	x4, [x20, #192]
	bl	printk
	ldr	x2, [x20, #152]
	mov	x0, x19
	ldr	x3, [x20, #176]
	mov	w1, #2
	ldr	x4, [x20, #200]
	bl	printk
	ldr	x2, [x20, #160]
	mov	x0, x19
	ldr	x3, [x20, #184]
	mov	w1, #2
	ldr	x4, [x20, #208]
	bl	printk
	ldp	x20, x19, [sp, #16]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #32             // 16-byte Folded Reload
	ret
.Lfunc_end0:
	.size	blk_scsi_kern_time_stamp_dump, .Lfunc_end0-blk_scsi_kern_time_stamp_dump
                                        // -- End function
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
	mov	w26, w3
	stp	x24, x23, [sp, #240]            // 16-byte Folded Spill
	stp	x22, x21, [sp, #256]            // 16-byte Folded Spill
	mov	w21, w4
	stp	x20, x19, [sp, #272]            // 16-byte Folded Spill
	mov	x19, x1
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldr	x8, [x8]
	stur	x8, [x29, #-16]
	bl	fget
	cbz	x0, .LBB1_7
// %bb.1:
	ldp	w13, w27, [x19, #64]
	ldr	x25, [x0, #208]
	mov	x23, x19
	stur	w21, [x29, #-80]                // 4-byte Folded Spill
	mov	x21, x19
	ldp	w12, w16, [x19, #40]
	stur	w13, [x29, #-88]                // 4-byte Folded Spill
	ldr	x28, [x25]
	ldr	x13, [x23, #144]!
	str	x25, [sp, #96]                  // 8-byte Folded Spill
	ldur	w25, [x23, #-16]
	ldr	x10, [x21, #24]!
	ldr	x11, [x19, #32]
	mov	x22, x23
	ldp	x30, x24, [x19, #72]
	mov	x20, x0
	stur	w25, [x29, #-84]                // 4-byte Folded Spill
	ldp	w9, w8, [x19]
	ldp	w7, w6, [x19, #8]
	ldp	w3, w4, [x19, #56]
	ldp	w0, w17, [x19, #88]
	stur	w12, [x29, #-24]
	ldr	x1, [x19, #16]
	ldp	w25, w12, [x23, #-8]
	ldr	x2, [x19, #48]
	stp	x10, x11, [x29, #-40]
	ldr	x14, [x22, #-48]!
	ldur	w15, [x23, #-40]
	stur	x13, [x29, #-72]
	ldur	w5, [x23, #-24]
	ldr	x10, [x23, #8]
	ldr	x11, [x28, #1360]
	stur	w15, [x29, #-48]
	stp	x10, x14, [x29, #-64]
	cbz	x11, .LBB1_10
// %bb.2:
	stp	w27, w17, [sp, #88]             // 8-byte Folded Spill
	mov	x27, #-22
	cmp	w9, #81
	b.ne	.LBB1_8
// %bb.3:
	cbnz	w8, .LBB1_8
// %bb.4:
	and	w8, w7, #0xfffffffd
	cbnz	w8, .LBB1_8
// %bb.5:
	cmp	w4, #0
	mov	w8, #32
	stp	x2, x30, [sp, #48]              // 16-byte Folded Spill
	mov	w2, wzr
	stp	x6, x1, [sp, #72]               // 16-byte Folded Spill
	cinc	w1, w8, ne
	stp	w4, w0, [sp, #64]               // 8-byte Folded Spill
	mov	x0, x28
	stp	w3, w12, [sp, #36]              // 8-byte Folded Spill
	stp	w7, w5, [sp, #28]               // 8-byte Folded Spill
	str	w16, [sp, #44]                  // 4-byte Folded Spill
	bl	blk_get_request
	cmn	x0, #1, lsl #12                 // =4096
	b.ls	.LBB1_11
// %bb.6:
	mov	x27, x0
	b	.LBB1_8
.LBB1_7:
	mov	x27, #-14
.LBB1_8:
	adrp	x8, :got:__stack_chk_guard
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldur	x9, [x29, #-16]
	ldr	x8, [x8]
	cmp	x8, x9
	b.ne	.LBB1_43
// %bb.9:
	mov	x0, x27
	ldp	x20, x19, [sp, #272]            // 16-byte Folded Reload
	ldp	x22, x21, [sp, #256]            // 16-byte Folded Reload
	ldp	x24, x23, [sp, #240]            // 16-byte Folded Reload
	ldp	x26, x25, [sp, #224]            // 16-byte Folded Reload
	ldp	x28, x27, [sp, #208]            // 16-byte Folded Reload
	ldp	x29, x30, [sp, #192]            // 16-byte Folded Reload
	add	sp, sp, #288
	ret
.LBB1_10:
	mov	x27, #-6
	b	.LBB1_8
.LBB1_11:
	str	w25, [sp, #8]                   // 4-byte Folded Spill
	ldur	w25, [x29, #-80]                // 4-byte Folded Reload
	cmp	w26, #2
	mov	x27, x0
	str	w26, [sp, #12]                  // 4-byte Folded Spill
	ccmp	w25, #2, #2, ls
	cset	w8, hi
	cmp	w8, #1
	str	w8, [sp, #4]                    // 4-byte Folded Spill
	b.eq	.LBB1_41
// %bb.12:
	bl	dfx_getcurtime
	mov	w8, #72
	adrp	x9, rpmb_time_blk_point
	add	x9, x9, :lo12:rpmb_time_blk_point
	umaddl	x8, w26, w8, x9
	str	x0, [x8, w25, uxtw #3]
	mov	x25, x24
	mov	x24, x9
.LBB1_13:
	ldp	x2, x1, [sp, #72]               // 16-byte Folded Reload
	str	x25, [sp, #16]                  // 8-byte Folded Spill
	cmp	w2, #16
	b.hi	.LBB1_25
// %bb.14:
	ldr	x0, [x27, #312]
	ldr	w26, [sp, #12]                  // 4-byte Folded Reload
.LBB1_15:
	mov	x25, x2
	bl	memcpy
	ldr	w0, [sp, #68]                   // 4-byte Folded Reload
	strh	w25, [x27, #320]
	bl	__msecs_to_jiffies
	str	w0, [x27, #232]
	cbnz	w0, .LBB1_23
// %bb.16:
	ldr	w0, [x28, #1108]
	str	w0, [x27, #232]
	cbnz	w0, .LBB1_23
// %bb.17:
	ldr	w9, [sp, #64]                   // 4-byte Folded Reload
	mov	w8, #6000
	ldr	x25, [sp, #16]                  // 8-byte Folded Reload
	ldr	x2, [sp, #56]                   // 8-byte Folded Reload
.LBB1_18:
	str	w8, [x27, #232]
.LBB1_19:
	mov	w3, w9
	cbnz	w9, .LBB1_21
// %bb.20:
	ldr	w8, [sp, #88]                   // 4-byte Folded Reload
	mov	x2, x25
	mov	w3, w8
	cbz	w8, .LBB1_22
.LBB1_21:
	mov	x0, x28
	mov	x1, x27
	mov	w4, #3264
	bl	blk_rq_map_kern
	cbnz	w0, .LBB1_27
.LBB1_22:
	mov	x28, x27
	b	.LBB1_31
.LBB1_23:
	ldr	w9, [sp, #64]                   // 4-byte Folded Reload
	cmp	w0, #699
	ldr	x25, [sp, #16]                  // 8-byte Folded Reload
	ldr	x2, [sp, #56]                   // 8-byte Folded Reload
	b.hi	.LBB1_19
// %bb.24:
	mov	w8, #700
	b	.LBB1_18
.LBB1_25:
	mov	x0, x2
	mov	w1, #3520
	bl	__kmalloc
	ldp	x2, x1, [sp, #72]               // 16-byte Folded Reload
	str	x0, [x27, #312]
	ldr	w26, [sp, #12]                  // 4-byte Folded Reload
	cbnz	x0, .LBB1_15
// %bb.26:
	mov	x28, #-12
	b	.LBB1_28
.LBB1_27:
	mov	w8, w0
	ldr	x0, [x27, #312]
	sxtw	x28, w8
.LBB1_28:
	add	x8, x27, #296
	cmp	x0, x8
	b.eq	.LBB1_30
// %bb.29:
	bl	kfree
.LBB1_30:
	mov	x0, x27
	bl	blk_put_request
.LBB1_31:
	ldr	w9, [sp, #92]                   // 4-byte Folded Reload
	cmn	x28, #4095
	b.lo	.LBB1_33
// %bb.32:
	mov	x27, x28
	b	.LBB1_8
.LBB1_33:
	mov	w8, #1
	bic	w27, w8, w9, lsr #4
	ldr	w8, [sp, #4]                    // 4-byte Folded Reload
	cbnz	w8, .LBB1_42
// %bb.34:
	bl	dfx_getcurtime
	mov	w8, #72
	ldur	w9, [x29, #-80]                 // 4-byte Folded Reload
	mov	x1, xzr
	mov	x2, x28
	umaddl	x8, w26, w8, x24
	mov	w3, w27
	add	x25, x8, w9, uxtw #3
	ldr	x8, [sp, #96]                   // 8-byte Folded Reload
	str	x0, [x25, #24]
	ldr	x0, [x8]
	bl	blk_execute_rq
	bl	dfx_getcurtime
	str	x0, [x25, #48]
.LBB1_35:
	ldr	w25, [x28, #324]
	add	x27, x28, #296
	ldr	x0, [sp, #48]                   // 8-byte Folded Reload
	mov	w24, wzr
	and	w8, w25, #0xff
	ubfx	w9, w25, #16, #8
	lsr	w10, w25, #24
	str	w8, [sp, #96]                   // 4-byte Folded Spill
	orr	w8, w8, w9
	orr	w8, w8, w10
	str	w9, [sp, #12]                   // 4-byte Folded Spill
	cmp	w8, #0
	ldr	w8, [x28, #328]
	cset	w9, ne
	stp	w9, w10, [sp]                   // 8-byte Folded Spill
	ldr	w10, [sp, #40]                  // 4-byte Folded Reload
	cbz	w8, .LBB1_38
// %bb.36:
	cbz	x0, .LBB1_38
// %bb.37:
	ldr	w9, [sp, #44]                   // 4-byte Folded Reload
	ldr	x1, [x28, #344]
	cmp	w9, w8
	csel	w24, w9, w8, lo
	mov	x2, x24
	bl	memcpy
	ldr	w10, [sp, #40]                  // 4-byte Folded Reload
	ldr	w25, [x28, #324]
.LBB1_38:
	ldr	w8, [x28, #24]
	stur	x24, [x29, #-80]                // 8-byte Folded Spill
	ldr	w9, [x28, #332]
	ldr	x0, [x28, #312]
	tst	w8, #0x1
	ldr	w8, [sp, #8]                    // 4-byte Folded Reload
	csel	w24, w10, w9, eq
	csel	w26, w9, w8, eq
	cmp	x0, x27
	b.eq	.LBB1_40
// %bb.39:
	bl	kfree
.LBB1_40:
	mov	x0, x28
	cmp	w25, #0
	csel	w25, w25, wzr, lt
	bl	blk_put_request
	ldr	w10, [sp, #28]                  // 4-byte Folded Reload
	mov	w8, #81
	ldr	x9, [sp, #72]                   // 8-byte Folded Reload
	mov	x0, x20
	ldur	w11, [x29, #-24]
	str	x8, [x19]
	ldr	w8, [sp, #44]                   // 4-byte Folded Reload
	stp	w10, w9, [x19, #8]
	ldr	x10, [sp, #80]                  // 8-byte Folded Reload
	str	x10, [x19, #16]
	ldp	x9, x10, [x29, #-40]
	str	w11, [x21, #16]
	ldr	w11, [sp, #36]                  // 4-byte Folded Reload
	stp	x9, x10, [x21]
	ldur	x9, [x29, #-56]
	str	w8, [x19, #44]
	ldr	x8, [sp, #48]                   // 8-byte Folded Reload
	ldr	w10, [sp, #92]                  // 4-byte Folded Reload
	str	x8, [x19, #48]
	ldp	w8, w16, [sp, #64]              // 8-byte Folded Reload
	stp	w11, w8, [x19, #56]
	ldur	w11, [x29, #-88]                // 4-byte Folded Reload
	ldr	w8, [sp, #88]                   // 4-byte Folded Reload
	stp	w16, w10, [x19, #88]
	stp	w11, w8, [x19, #64]
	ldr	x11, [sp, #56]                  // 8-byte Folded Reload
	ldr	x8, [sp, #16]                   // 8-byte Folded Reload
	stp	x11, x8, [x19, #72]
	ldur	w8, [x29, #-48]
	str	x9, [x22]
	ldr	w9, [sp, #4]                    // 4-byte Folded Reload
	str	w8, [x22, #8]
	ldr	w8, [sp, #12]                   // 4-byte Folded Reload
	str	w24, [x19, #140]
	stp	w9, w8, [x19, #108]
	ldr	w9, [sp, #96]                   // 4-byte Folded Reload
	ldr	w8, [sp, #32]                   // 4-byte Folded Reload
	stp	w9, w8, [x19, #116]
	ldr	w9, [sp]                        // 4-byte Folded Reload
	ldur	w8, [x29, #-84]                 // 4-byte Folded Reload
	stp	w9, w8, [x19, #124]
	ldp	x9, x8, [x29, #-80]             // 8-byte Folded Reload
	stp	w9, w26, [x19, #132]
	ldur	x9, [x29, #-64]
	stp	x8, x9, [x23]
	bl	fput
	sxtw	x27, w25
	b	.LBB1_8
.LBB1_41:
	adrp	x0, .L.str.4
	add	x0, x0, :lo12:.L.str.4
	adrp	x1, .L__func__.blk_scsi_kern_time_record
	add	x1, x1, :lo12:.L__func__.blk_scsi_kern_time_record
	bl	printk
	mov	x25, x24
	adrp	x24, rpmb_time_blk_point
	add	x24, x24, :lo12:rpmb_time_blk_point
	b	.LBB1_13
.LBB1_42:
	adrp	x25, .L.str.4
	add	x25, x25, :lo12:.L.str.4
	adrp	x26, .L__func__.blk_scsi_kern_time_record
	add	x26, x26, :lo12:.L__func__.blk_scsi_kern_time_record
	mov	x0, x25
	mov	x1, x26
	bl	printk
	ldr	x8, [sp, #96]                   // 8-byte Folded Reload
	mov	x1, xzr
	mov	x2, x28
	mov	w3, w27
	ldr	x0, [x8]
	bl	blk_execute_rq
	mov	x0, x25
	mov	x1, x26
	bl	printk
	b	.LBB1_35
.LBB1_43:
	bl	__stack_chk_fail
.Lfunc_end1:
	.size	blk_scsi_kern_ioctl, .Lfunc_end1-blk_scsi_kern_ioctl
                                        // -- End function
	.type	.L.str,@object                  // @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"\0013source %d, req_get %llu, start %llu, end %llu\n"
	.size	.L.str, 49

	.type	rpmb_time_blk_point,@object     // @rpmb_time_blk_point
	.local	rpmb_time_blk_point
	.comm	rpmb_time_blk_point,216,8
	.type	.L.str.4,@object                // @.str.4
.L.str.4:
	.asciz	"\0013%s: out of bound\n"
	.size	.L.str.4, 20

	.type	.L__func__.blk_scsi_kern_time_record,@object // @__func__.blk_scsi_kern_time_record
.L__func__.blk_scsi_kern_time_record:
	.asciz	"blk_scsi_kern_time_record"
	.size	.L__func__.blk_scsi_kern_time_record, 26

	.ident	"OHOS (BiSheng Mobile CPU STD 202.1.0.B030) clang version 15.0.4 (llvm-project 7123208073d2)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
