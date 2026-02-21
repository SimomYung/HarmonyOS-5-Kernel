	.text
	.file	"hufs-bkops.c"
	.globl	ufshcd_bkops_status_query       // -- Begin function ufshcd_bkops_status_query
	.p2align	2
	.type	ufshcd_bkops_status_query,@function
ufshcd_bkops_status_query:              // @ufshcd_bkops_status_query
// %bb.0:
	stp	x29, x30, [sp, #-48]!           // 16-byte Folded Spill
	stp	x20, x19, [sp, #32]             // 16-byte Folded Spill
	mov	w19, #-22
	str	x21, [sp, #16]                  // 8-byte Folded Spill
	mov	x29, sp
	cbz	x0, .LBB0_3
// %bb.1:
	cbz	x1, .LBB0_3
// %bb.2:
	mov	x21, x1
	mov	x20, x0
	ldr	x0, [x0, #96]
	mov	w1, #4
	bl	__pm_runtime_resume
	ldr	x8, [x20, #6560]
	mov	w9, #50722
	mov	x0, x20
	mov	x1, x21
	ldr	x8, [x8]
	blraa	x8, x9
	mov	w19, w0
	ldr	x0, [x20, #96]
	mov	w1, #4
	bl	__pm_runtime_idle
	cbnz	w19, .LBB0_4
.LBB0_3:
	mov	w0, w19
	ldr	x21, [sp, #16]                  // 8-byte Folded Reload
	ldp	x20, x19, [sp, #32]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #48             // 16-byte Folded Reload
	ret
.LBB0_4:
	adrp	x0, .L.str
	add	x0, x0, :lo12:.L.str
	bl	printk
	b	.LBB0_3
.Lfunc_end0:
	.size	ufshcd_bkops_status_query, .Lfunc_end0-ufshcd_bkops_status_query
                                        // -- End function
	.globl	ufshcd_bkops_start_stop         // -- Begin function ufshcd_bkops_start_stop
	.p2align	2
	.type	ufshcd_bkops_start_stop,@function
ufshcd_bkops_start_stop:                // @ufshcd_bkops_start_stop
// %bb.0:
	stp	x29, x30, [sp, #-32]!           // 16-byte Folded Spill
	stp	x20, x19, [sp, #16]             // 16-byte Folded Spill
	mov	x29, sp
	cbz	x0, .LBB1_3
// %bb.1:
	cbnz	w1, .LBB1_5
// %bb.2:
	mov	w19, wzr
	b	.LBB1_4
.LBB1_3:
	mov	w19, #-22
.LBB1_4:
	mov	w0, w19
	ldp	x20, x19, [sp, #16]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #32             // 16-byte Folded Reload
	ret
.LBB1_5:
	mov	x20, x0
	ldr	x0, [x0, #96]
	mov	w1, #4
	bl	__pm_runtime_resume
	ldr	x8, [x20, #6560]
	mov	w9, #23545
	mov	x0, x20
	ldr	x8, [x8, #8]
	blraa	x8, x9
	mov	w19, w0
	ldr	x0, [x20, #96]
	mov	w1, #4
	bl	__pm_runtime_idle
	cbz	w19, .LBB1_4
// %bb.6:
	adrp	x0, .L.str.2
	add	x0, x0, :lo12:.L.str.2
	bl	printk
	b	.LBB1_4
.Lfunc_end1:
	.size	ufshcd_bkops_start_stop, .Lfunc_end1-ufshcd_bkops_start_stop
                                        // -- End function
	.globl	hufs_manual_bkops_config        // -- Begin function hufs_manual_bkops_config
	.p2align	2
	.type	hufs_manual_bkops_config,@function
hufs_manual_bkops_config:               // @hufs_manual_bkops_config
// %bb.0:
	sub	sp, sp, #112
	stp	x29, x30, [sp, #16]             // 16-byte Folded Spill
	add	x29, sp, #16
	stp	x28, x27, [sp, #32]             // 16-byte Folded Spill
	stp	x26, x25, [sp, #48]             // 16-byte Folded Spill
	stp	x24, x23, [sp, #64]             // 16-byte Folded Spill
	stp	x22, x21, [sp, #80]             // 16-byte Folded Spill
	stp	x20, x19, [sp, #96]             // 16-byte Folded Spill
	cbz	x0, .LBB2_3
// %bb.1:
	ldr	x25, [x0]
	ldrb	w8, [x25, #584]
	tbnz	w8, #0, .LBB2_5
// %bb.2:
	mov	w0, wzr
	b	.LBB2_4
.LBB2_3:
	mov	w0, #-22
.LBB2_4:
	ldp	x20, x19, [sp, #96]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #80]             // 16-byte Folded Reload
	ldp	x24, x23, [sp, #64]             // 16-byte Folded Reload
	ldp	x26, x25, [sp, #48]             // 16-byte Folded Reload
	ldp	x28, x27, [sp, #32]             // 16-byte Folded Reload
	ldp	x29, x30, [sp, #16]             // 16-byte Folded Reload
	add	sp, sp, #112
	ret
.LBB2_5:
	ldr	x27, [x25, #8912]
	mov	w8, #8832
	mov	w9, #8912
	mov	x19, x0
	add	x8, x25, x8
	add	x28, x25, x9
	cmp	x27, x28
	str	x8, [sp, #8]                    // 8-byte Folded Spill
	b.eq	.LBB2_13
// %bb.6:
	ldr	x8, [sp, #8]                    // 8-byte Folded Reload
	adrp	x21, .L.str.4
	add	x21, x21, :lo12:.L.str.4
	ldr	x20, [x19, #216]
	adrp	x22, .L.str.3
	add	x22, x22, :lo12:.L.str.3
	ldrh	w26, [x8]
	b	.LBB2_8
.LBB2_7:                                //   in Loop: Header=BB2_8 Depth=1
	ldr	x27, [x27]
	cmp	x27, x28
	b.eq	.LBB2_13
.LBB2_8:                                // =>This Inner Loop Header: Depth=1
	ldurh	w8, [x27, #-24]
	cmp	w8, w26
	b.ne	.LBB2_7
// %bb.9:                               //   in Loop: Header=BB2_8 Depth=1
	ldur	x23, [x27, #-16]
	ldr	x24, [x19, #208]
	mov	x0, x23
	bl	strlen
	mov	x2, x0
	mov	x0, x24
	mov	x1, x23
	bl	strncmp
	cbz	w0, .LBB2_11
// %bb.10:                              //   in Loop: Header=BB2_8 Depth=1
	mov	x0, x22
	mov	x1, x23
	bl	strcmp
	cbnz	w0, .LBB2_7
.LBB2_11:                               //   in Loop: Header=BB2_8 Depth=1
	ldur	x23, [x27, #-8]
	mov	x0, x20
	mov	x1, x23
	bl	strcmp
	cbz	w0, .LBB2_27
// %bb.12:                              //   in Loop: Header=BB2_8 Depth=1
	mov	x0, x21
	mov	x1, x23
	bl	strcmp
	cbnz	w0, .LBB2_7
	b	.LBB2_27
.LBB2_13:
	ldr	x28, [x25, #8896]
	mov	w8, #8896
	add	x27, x25, x8
	cmp	x28, x27
	b.eq	.LBB2_27
// %bb.14:
	ldr	x8, [sp, #8]                    // 8-byte Folded Reload
	adrp	x21, .L.str.4
	add	x21, x21, :lo12:.L.str.4
	ldr	x20, [x19, #216]
	adrp	x22, .L.str.3
	add	x22, x22, :lo12:.L.str.3
	ldrh	w26, [x8]
	add	x8, x25, #2320
	str	x8, [sp]                        // 8-byte Folded Spill
	b	.LBB2_16
.LBB2_15:                               //   in Loop: Header=BB2_16 Depth=1
	ldr	x28, [x28]
	cmp	x28, x27
	b.eq	.LBB2_27
.LBB2_16:                               // =>This Inner Loop Header: Depth=1
	ldurh	w8, [x28, #-24]
	cmp	w8, w26
	b.ne	.LBB2_15
// %bb.17:                              //   in Loop: Header=BB2_16 Depth=1
	ldur	x23, [x28, #-16]
	ldr	x24, [x19, #208]
	mov	x0, x23
	bl	strlen
	mov	x2, x0
	mov	x0, x24
	mov	x1, x23
	bl	strncmp
	cbz	w0, .LBB2_19
// %bb.18:                              //   in Loop: Header=BB2_16 Depth=1
	mov	x0, x22
	mov	x1, x23
	bl	strcmp
	cbnz	w0, .LBB2_15
.LBB2_19:                               //   in Loop: Header=BB2_16 Depth=1
	ldur	x23, [x28, #-8]
	mov	x0, x20
	mov	x1, x23
	bl	strcmp
	cbz	w0, .LBB2_21
// %bb.20:                              //   in Loop: Header=BB2_16 Depth=1
	mov	x0, x21
	mov	x1, x23
	bl	strcmp
	cbnz	w0, .LBB2_15
.LBB2_21:
	ldr	x8, [sp, #8]                    // 8-byte Folded Reload
	ldrb	w8, [x8, #40]
	cbnz	w8, .LBB2_27
// %bb.22:
	bl	mas_bkops_alloc
	cbz	x0, .LBB2_26
// %bb.23:
	ldr	x8, [sp, #8]                    // 8-byte Folded Reload
	mov	x19, x0
	ldrh	w8, [x8]
	cmp	w8, #2230
	b.eq	.LBB2_28
// %bb.24:
	cmp	w8, #429
	b.ne	.LBB2_30
// %bb.25:
	mov	w9, #3
	adrp	x8, hynix_bkops_ops
	add	x8, x8, :lo12:hynix_bkops_ops
	b	.LBB2_29
.LBB2_26:
	mov	w0, #-12
	b	.LBB2_4
.LBB2_27:
	adrp	x0, .L.str.1
	add	x0, x0, :lo12:.L.str.1
	bl	printk
	mov	w0, wzr
	b	.LBB2_4
.LBB2_28:
	mov	w9, #2
	adrp	x8, hi1861_bkops_ops
	add	x8, x8, :lo12:hi1861_bkops_ops
.LBB2_29:
	ldr	x10, [x19, #120]
	str	w9, [x19]
	orr	x10, x10, #0x1c
	str	x10, [x19, #120]
	str	x8, [x25, #8880]
.LBB2_30:
	adrp	x8, ufs_bkops_ops
	add	x8, x8, :lo12:ufs_bkops_ops
	mov	w20, #1
	mov	w9, #3600
	ldr	x10, [sp]                       // 8-byte Folded Reload
	str	x19, [x25, #8888]
	mov	x0, x19
	str	w20, [x19, #156]
	stp	x8, x10, [x19, #8]
	str	x9, [x19, #160]
	ldr	x1, [x25, #7152]
	bl	mas_bkops_enable
	cbnz	w0, .LBB2_32
// %bb.31:
	ldr	x8, [sp, #8]                    // 8-byte Folded Reload
	strb	w20, [x8, #40]
	bl	mas_blk_hm_support_bkops_notify
	mov	w0, wzr
	b	.LBB2_4
.LBB2_32:
	mov	x0, x19
	bl	kfree
	mov	w0, #-11
	b	.LBB2_4
.Lfunc_end2:
	.size	hufs_manual_bkops_config, .Lfunc_end2-hufs_manual_bkops_config
                                        // -- End function
	.p2align	2                               // -- Begin function ufshcd_1861_bkops_query
	.type	ufshcd_1861_bkops_query,@function
ufshcd_1861_bkops_query:                // @ufshcd_1861_bkops_query
// %bb.0:
	cbz	x1, .LBB3_2
// %bb.1:
	stp	x29, x30, [sp, #-16]!           // 16-byte Folded Spill
	mov	x2, x1
	mov	w1, #242
	mov	x29, sp
	bl	ufshcd_query_hi1861_bkops
	ldp	x29, x30, [sp], #16             // 16-byte Folded Reload
	ret
.LBB3_2:
	mov	w0, #-22
	ret
.Lfunc_end3:
	.size	ufshcd_1861_bkops_query, .Lfunc_end3-ufshcd_1861_bkops_query
                                        // -- End function
	.p2align	2                               // -- Begin function ufshcd_1861_bkops_start
	.type	ufshcd_1861_bkops_start,@function
ufshcd_1861_bkops_start:                // @ufshcd_1861_bkops_start
// %bb.0:
	stp	x29, x30, [sp, #-16]!           // 16-byte Folded Spill
	mov	w1, #240
	mov	x2, xzr
	mov	x29, sp
	bl	ufshcd_query_hi1861_bkops
	ldp	x29, x30, [sp], #16             // 16-byte Folded Reload
	ret
.Lfunc_end4:
	.size	ufshcd_1861_bkops_start, .Lfunc_end4-ufshcd_1861_bkops_start
                                        // -- End function
	.p2align	2                               // -- Begin function ufshcd_1861_bkops_stop
	.type	ufshcd_1861_bkops_stop,@function
ufshcd_1861_bkops_stop:                 // @ufshcd_1861_bkops_stop
// %bb.0:
	stp	x29, x30, [sp, #-16]!           // 16-byte Folded Spill
	mov	w1, #241
	mov	x2, xzr
	mov	x29, sp
	bl	ufshcd_query_hi1861_bkops
	ldp	x29, x30, [sp], #16             // 16-byte Folded Reload
	ret
.Lfunc_end5:
	.size	ufshcd_1861_bkops_stop, .Lfunc_end5-ufshcd_1861_bkops_stop
                                        // -- End function
	.p2align	2                               // -- Begin function ufshcd_query_hi1861_bkops
	.type	ufshcd_query_hi1861_bkops,@function
ufshcd_query_hi1861_bkops:              // @ufshcd_query_hi1861_bkops
// %bb.0:
	sub	sp, sp, #128
	adrp	x8, :got:__stack_chk_guard
	stp	x29, x30, [sp, #32]             // 16-byte Folded Spill
	str	x27, [sp, #48]                  // 8-byte Folded Spill
	add	x29, sp, #32
	stp	x26, x25, [sp, #64]             // 16-byte Folded Spill
	stp	x24, x23, [sp, #80]             // 16-byte Folded Spill
	stp	x22, x21, [sp, #96]             // 16-byte Folded Spill
	stp	x20, x19, [sp, #112]            // 16-byte Folded Spill
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldr	x8, [x8]
	stur	x8, [x29, #-8]
	stp	xzr, xzr, [sp, #8]
	cbz	x0, .LBB6_63
// %bb.1:
	adrp	x8, :got:liblinux_thread_offset
	adrp	x10, :got:liblinux_thread_key
	//APP
	mrs	x9, TPIDR_EL0

	//NO_APP
	ldr	x8, [x8, :got_lo12:liblinux_thread_offset]
	ldr	x10, [x10, :got_lo12:liblinux_thread_key]
	ldr	w8, [x8]
	ldr	w23, [x10]
	sub	x24, x9, x8
	ldr	x8, [x24]
	ldr	x25, [x8, x23, lsl #3]
	ldr	x8, [x0, #88]
	ldrb	w9, [x25, #92]
	ldr	x22, [x8, #56]
	cbnz	w9, .LBB6_6
// %bb.2:
	ldr	w26, [x25, #44]
	cbnz	w26, .LBB6_7
// %bb.3:
	xpaci	x30
	mov	x19, x2
	str	x30, [x25, #24]
	mov	x20, x0
	ldr	x8, [x24]
	mov	w21, w1
	ldr	x8, [x8, x23, lsl #3]
	ldr	w9, [x8, #40]
	cbnz	w9, .LBB6_5
// %bb.4:
	ldrb	w9, [x8, #92]
	cbz	w9, .LBB6_53
.LBB6_5:
	ldr	w9, [x8, #40]
	mov	w10, #1
	add	w9, w9, #1
	str	w9, [x8, #40]
	//APP
	//NO_APP
	str	w10, [x25, #44]
	ldr	x8, [x24]
	ldr	x25, [x8, x23, lsl #3]
	b	.LBB6_8
.LBB6_6:
	mov	x19, x2
	mov	x20, x0
	mov	w21, w1
	mov	x26, xzr
	ldr	w8, [x25, #40]
	cbz	w8, .LBB6_9
	b	.LBB6_10
.LBB6_7:
	mov	x19, x2
	mov	x20, x0
	mov	w21, w1
.LBB6_8:
	cmp	w26, #0
	cset	w26, ne
	ldr	w8, [x25, #40]
	cbnz	w8, .LBB6_10
.LBB6_9:
	ldrb	w8, [x25, #92]
	cbz	w8, .LBB6_37
.LBB6_10:
	ldr	w9, [x25, #40]
	mov	x8, xzr
	mov	w10, #1
	add	w9, w9, #1
	str	w9, [x25, #40]
	//APP
	//NO_APP
	//APP
	prfm	pstl1strm, [x22]
.Ltmp0:
	ldaxr	w1, [x22]
	eor	w9, w1, w8
	cbnz	w9, .Ltmp1
	stxr	w9, w10, [x22]
	cbnz	w9, .Ltmp0

.Ltmp1:
	//NO_APP
	cbnz	w1, .LBB6_64
.LBB6_11:
	ldr	x9, [x20, #88]
	mov	w8, wzr
	ldr	w10, [x20, #1328]
	ldr	x9, [x9, #56]
	//APP
	stlrb	w8, [x9]
	//NO_APP
	ldr	x8, [x24]
	cmp	w10, #2
	ldr	x22, [x8, x23, lsl #3]
	ldrb	w8, [x22, #92]
	b.ne	.LBB6_24
// %bb.12:
	cbnz	w8, .LBB6_15
// %bb.13:
	ldr	w8, [x22, #44]
	cbz	w8, .LBB6_65
// %bb.14:
	cbz	x26, .LBB6_41
.LBB6_15:
	//APP
	//NO_APP
	ldr	x8, [x24]
	ldr	x22, [x8, x23, lsl #3]
	ldr	w8, [x22, #40]
	sub	w8, w8, #1
	str	w8, [x22, #40]
	ldr	w8, [x22, #40]
	cbnz	w8, .LBB6_17
// %bb.16:
	ldrb	w8, [x22, #92]
	cbz	w8, .LBB6_49
.LBB6_17:
	ldr	w8, [x22, #40]
	tbnz	w8, #31, .LBB6_67
// %bb.18:
	mov	w8, #6792
	add	x22, x20, x8
	mov	x0, x22
	bl	mutex_lock
	add	x23, x20, #3960
	mov	x0, x23
	bl	mutex_lock
	add	x1, sp, #16
	add	x2, sp, #8
	mov	x0, x20
	mov	w3, w21
	mov	w4, wzr
	mov	w5, wzr
	mov	w6, wzr
	bl	ufshcd_init_query
	ldr	x8, [sp, #16]
	sub	w9, w21, #240
	cmp	w9, #2
	b.hs	.LBB6_33
// %bb.19:
	mov	w9, #129
	strb	w9, [x8]
.LBB6_20:
	mov	x0, x20
	mov	w1, #1
	mov	w2, #1300
	bl	ufshcd_exec_dev_cmd
	cbnz	w0, .LBB6_57
// %bb.21:
	cbz	x19, .LBB6_36
// %bb.22:
	ldr	x8, [sp, #8]
	mov	w21, wzr
	ldrb	w8, [x8, #19]
	str	w8, [x19]
.LBB6_23:
	mov	x0, x23
	bl	mutex_unlock
	mov	x0, x22
	bl	mutex_unlock
	b	.LBB6_31
.LBB6_24:
	cbnz	w8, .LBB6_27
// %bb.25:
	ldr	w8, [x22, #44]
	cbz	w8, .LBB6_65
// %bb.26:
	cbz	x26, .LBB6_45
.LBB6_27:
	//APP
	//NO_APP
	ldr	x8, [x24]
	ldr	x19, [x8, x23, lsl #3]
	ldr	w8, [x19, #40]
	sub	w8, w8, #1
	str	w8, [x19, #40]
	ldr	w8, [x19, #40]
	cbnz	w8, .LBB6_29
// %bb.28:
	ldrb	w8, [x19, #92]
	cbz	w8, .LBB6_51
.LBB6_29:
	ldr	w8, [x19, #40]
	tbnz	w8, #31, .LBB6_66
// %bb.30:
	mov	w21, #-16
.LBB6_31:
	adrp	x8, :got:__stack_chk_guard
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldur	x9, [x29, #-8]
	ldr	x8, [x8]
	cmp	x8, x9
	b.ne	.LBB6_73
// %bb.32:
	mov	w0, w21
	ldr	x27, [sp, #48]                  // 8-byte Folded Reload
	ldp	x20, x19, [sp, #112]            // 16-byte Folded Reload
	ldp	x22, x21, [sp, #96]             // 16-byte Folded Reload
	ldp	x24, x23, [sp, #80]             // 16-byte Folded Reload
	ldp	x26, x25, [sp, #64]             // 16-byte Folded Reload
	ldp	x29, x30, [sp, #32]             // 16-byte Folded Reload
	add	sp, sp, #128
	ret
.LBB6_33:
	cmp	w21, #242
	b.ne	.LBB6_62
// %bb.34:
	mov	w9, #1
	strb	w9, [x8]
	cbnz	x19, .LBB6_20
// %bb.35:
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.9
	add	x1, x1, :lo12:.L.str.9
	adrp	x2, .L__func__.mas_bkops_upiu_query_opcode
	add	x2, x2, :lo12:.L__func__.mas_bkops_upiu_query_opcode
	bl	_dev_err
	mov	w21, #-22
	b	.LBB6_23
.LBB6_36:
	mov	w21, wzr
	b	.LBB6_23
.LBB6_37:
	ldr	x9, [x25, #32]
	mov	w8, #2
	adrp	x27, :got:__preempt_disabled
	str	w8, [x9, #4]
	//APP
	//NO_APP
	//APP
	mrs	x10, TPIDRRO_EL0
	lsr	x10, x10, #56

	//NO_APP
	ldr	x27, [x27, :got_lo12:__preempt_disabled]
	ldr	w8, [x27, w10, uxtw #2]
	cmp	w8, #0
	b.gt	.LBB6_70
// %bb.38:
	ldr	w9, [x9, #4]
	cmp	w9, #3
	b.eq	.LBB6_70
// %bb.39:
	and	x9, x10, #0xffffffff
.LBB6_40:
	add	w8, w8, #1
	str	w8, [x27, x9, lsl #2]
	b	.LBB6_10
.LBB6_41:
	str	xzr, [x22, #24]
	//APP
	//NO_APP
	ldr	x8, [x24]
	ldr	x25, [x8, x23, lsl #3]
	ldr	w8, [x25, #40]
	sub	w8, w8, #1
	str	w8, [x25, #40]
	ldr	w8, [x25, #40]
	cbnz	w8, .LBB6_43
// %bb.42:
	ldrb	w8, [x25, #92]
	cbz	w8, .LBB6_58
.LBB6_43:
	ldr	w8, [x25, #40]
	tbnz	w8, #31, .LBB6_71
// %bb.44:
	str	wzr, [x22, #44]
	b	.LBB6_15
.LBB6_45:
	str	xzr, [x22, #24]
	//APP
	//NO_APP
	ldr	x8, [x24]
	ldr	x19, [x8, x23, lsl #3]
	ldr	w8, [x19, #40]
	sub	w8, w8, #1
	str	w8, [x19, #40]
	ldr	w8, [x19, #40]
	cbnz	w8, .LBB6_47
// %bb.46:
	ldrb	w8, [x19, #92]
	cbz	w8, .LBB6_60
.LBB6_47:
	ldr	w8, [x19, #40]
	tbnz	w8, #31, .LBB6_66
// %bb.48:
	str	wzr, [x22, #44]
	b	.LBB6_27
.LBB6_49:
	ldr	x23, [x22, #32]
	//APP
	mrs	x8, TPIDRRO_EL0
	lsr	x8, x8, #56

	//NO_APP
	adrp	x9, :got:__preempt_disabled
	and	x8, x8, #0xffffffff
	lsl	x8, x8, #2
	mov	w11, #1
	ldr	x9, [x9, :got_lo12:__preempt_disabled]
	ldr	w10, [x9, x8]
	sub	w10, w10, #1
	str	w10, [x9, x8]
	str	w11, [x23, #4]
	ldr	w8, [x23, #8]
	cbz	w8, .LBB6_17
// %bb.50:
	bl	kernel_thread_preempt_enable
	str	wzr, [x23, #8]
	b	.LBB6_17
.LBB6_51:
	ldr	x20, [x19, #32]
	//APP
	mrs	x8, TPIDRRO_EL0
	lsr	x8, x8, #56

	//NO_APP
	adrp	x9, :got:__preempt_disabled
	and	x8, x8, #0xffffffff
	lsl	x8, x8, #2
	mov	w11, #1
	ldr	x9, [x9, :got_lo12:__preempt_disabled]
	ldr	w10, [x9, x8]
	sub	w10, w10, #1
	str	w10, [x9, x8]
	str	w11, [x20, #4]
	ldr	w8, [x20, #8]
	cbz	w8, .LBB6_29
// %bb.52:
	bl	kernel_thread_preempt_enable
	str	wzr, [x20, #8]
	b	.LBB6_29
.LBB6_53:
	ldr	x11, [x8, #32]
	mov	w9, #2
	str	w9, [x11, #4]
	adrp	x9, :got:__preempt_disabled
	//APP
	//NO_APP
	//APP
	mrs	x12, TPIDRRO_EL0
	lsr	x12, x12, #56

	//NO_APP
	ldr	x9, [x9, :got_lo12:__preempt_disabled]
	ldr	w10, [x9, w12, uxtw #2]
	cmp	w10, #0
	b.gt	.LBB6_72
// %bb.54:
	ldr	w11, [x11, #4]
	cmp	w11, #3
	b.eq	.LBB6_72
// %bb.55:
	and	x11, x12, #0xffffffff
.LBB6_56:
	add	w10, w10, #1
	str	w10, [x9, x11, lsl #2]
	b	.LBB6_5
.LBB6_57:
	mov	w21, w0
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.8
	add	x1, x1, :lo12:.L.str.8
	adrp	x2, .L__func__.ufshcd_query_hi1861_bkops
	add	x2, x2, :lo12:.L__func__.ufshcd_query_hi1861_bkops
	mov	w3, w21
	bl	_dev_err
	b	.LBB6_23
.LBB6_58:
	ldr	x26, [x25, #32]
	//APP
	mrs	x8, TPIDRRO_EL0
	lsr	x8, x8, #56

	//NO_APP
	adrp	x9, :got:__preempt_disabled
	and	x8, x8, #0xffffffff
	lsl	x8, x8, #2
	mov	w11, #1
	ldr	x9, [x9, :got_lo12:__preempt_disabled]
	ldr	w10, [x9, x8]
	sub	w10, w10, #1
	str	w10, [x9, x8]
	str	w11, [x26, #4]
	ldr	w8, [x26, #8]
	cbz	w8, .LBB6_43
// %bb.59:
	bl	kernel_thread_preempt_enable
	str	wzr, [x26, #8]
	b	.LBB6_43
.LBB6_60:
	ldr	x20, [x19, #32]
	//APP
	mrs	x8, TPIDRRO_EL0
	lsr	x8, x8, #56

	//NO_APP
	adrp	x9, :got:__preempt_disabled
	and	x8, x8, #0xffffffff
	lsl	x8, x8, #2
	mov	w11, #1
	ldr	x9, [x9, :got_lo12:__preempt_disabled]
	ldr	w10, [x9, x8]
	sub	w10, w10, #1
	str	w10, [x9, x8]
	str	w11, [x20, #4]
	ldr	w8, [x20, #8]
	cbz	w8, .LBB6_47
// %bb.61:
	bl	kernel_thread_preempt_enable
	str	wzr, [x20, #8]
	b	.LBB6_47
.LBB6_62:
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.10
	add	x1, x1, :lo12:.L.str.10
	adrp	x2, .L__func__.mas_bkops_upiu_query_opcode
	add	x2, x2, :lo12:.L__func__.mas_bkops_upiu_query_opcode
	mov	w3, w21
	bl	_dev_err
	mov	w21, #-22
	b	.LBB6_23
.LBB6_63:
	adrp	x0, .L.str.5
	add	x0, x0, :lo12:.L.str.5
	adrp	x1, .L.str.6
	add	x1, x1, :lo12:.L.str.6
	adrp	x3, .L__func__.ufshcd_query_hi1861_bkops
	add	x3, x3, :lo12:.L__func__.ufshcd_query_hi1861_bkops
	mov	w2, #110
	b	.LBB6_69
.LBB6_64:
	mov	x0, x22
	bl	queued_spin_lock_slowpath
	b	.LBB6_11
.LBB6_65:
	adrp	x0, .L.str.5
	add	x0, x0, :lo12:.L.str.5
	adrp	x1, .L.str.6
	add	x1, x1, :lo12:.L.str.6
	adrp	x3, .L__func__.arch_local_irq_restore
	add	x3, x3, :lo12:.L__func__.arch_local_irq_restore
	mov	w2, #80
	b	.LBB6_69
.LBB6_66:
	str	wzr, [x19, #40]
	b	.LBB6_68
.LBB6_67:
	str	wzr, [x22, #40]
.LBB6_68:
	adrp	x0, .L.str.5
	add	x0, x0, :lo12:.L.str.5
	adrp	x1, .L.str.6
	add	x1, x1, :lo12:.L.str.6
	adrp	x3, .L__func__.__preempt_count_sub
	add	x3, x3, :lo12:.L__func__.__preempt_count_sub
	mov	w2, #150
.LBB6_69:
	bl	printk
	adrp	x0, .L.str.7
	add	x0, x0, :lo12:.L.str.7
	bl	panic
.LBB6_70:
	bl	kernel_thread_preempt_disable
	//APP
	mrs	x9, TPIDRRO_EL0
	lsr	x9, x9, #56

	//NO_APP
	ldr	w8, [x27, w9, uxtw #2]
	and	x9, x9, #0xffffffff
	b	.LBB6_40
.LBB6_71:
	str	wzr, [x25, #40]
	b	.LBB6_68
.LBB6_72:
	mov	x27, x8
	bl	kernel_thread_preempt_disable
	adrp	x9, :got:__preempt_disabled
	mov	x8, x27
	ldr	x9, [x9, :got_lo12:__preempt_disabled]
	//APP
	mrs	x11, TPIDRRO_EL0
	lsr	x11, x11, #56

	//NO_APP
	ldr	w10, [x9, w11, uxtw #2]
	and	x11, x11, #0xffffffff
	b	.LBB6_56
.LBB6_73:
	bl	__stack_chk_fail
.Lfunc_end6:
	.size	ufshcd_query_hi1861_bkops, .Lfunc_end6-ufshcd_query_hi1861_bkops
                                        // -- End function
	.p2align	2                               // -- Begin function ufshcd_hynix_bkops_query
	.type	ufshcd_hynix_bkops_query,@function
ufshcd_hynix_bkops_query:               // @ufshcd_hynix_bkops_query
// %bb.0:
	cbz	x1, .LBB7_2
// %bb.1:
	stp	x29, x30, [sp, #-16]!           // 16-byte Folded Spill
	mov	x5, x1
	mov	w1, #3
	mov	w2, #19
	mov	w3, wzr
	mov	w4, wzr
	mov	x29, sp
	bl	ufshcd_query_attr_safe
	ldp	x29, x30, [sp], #16             // 16-byte Folded Reload
	ret
.LBB7_2:
	mov	w0, #-22
	ret
.Lfunc_end7:
	.size	ufshcd_hynix_bkops_query, .Lfunc_end7-ufshcd_hynix_bkops_query
                                        // -- End function
	.p2align	2                               // -- Begin function ufshcd_hynix_bkops_start
	.type	ufshcd_hynix_bkops_start,@function
ufshcd_hynix_bkops_start:               // @ufshcd_hynix_bkops_start
// %bb.0:
	sub	sp, sp, #32
	adrp	x8, :got:__stack_chk_guard
	stp	x29, x30, [sp, #16]             // 16-byte Folded Spill
	mov	w9, #1
	add	x5, sp, #4
	mov	w1, #4
	mov	w2, #18
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	mov	w3, wzr
	mov	w4, wzr
	add	x29, sp, #16
	ldr	x8, [x8]
	str	x8, [sp, #8]
	str	w9, [sp, #4]
	bl	ufshcd_query_attr_safe
	adrp	x8, :got:__stack_chk_guard
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldr	x9, [sp, #8]
	ldr	x8, [x8]
	cmp	x8, x9
	b.ne	.LBB8_2
// %bb.1:
	ldp	x29, x30, [sp, #16]             // 16-byte Folded Reload
	add	sp, sp, #32
	ret
.LBB8_2:
	bl	__stack_chk_fail
.Lfunc_end8:
	.size	ufshcd_hynix_bkops_start, .Lfunc_end8-ufshcd_hynix_bkops_start
                                        // -- End function
	.p2align	2                               // -- Begin function ufshcd_hynix_bkops_stop
	.type	ufshcd_hynix_bkops_stop,@function
ufshcd_hynix_bkops_stop:                // @ufshcd_hynix_bkops_stop
// %bb.0:
	sub	sp, sp, #32
	adrp	x8, :got:__stack_chk_guard
	stp	x29, x30, [sp, #16]             // 16-byte Folded Spill
	add	x5, sp, #4
	mov	w1, #4
	mov	w2, #18
	mov	w3, wzr
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	mov	w4, wzr
	add	x29, sp, #16
	ldr	x8, [x8]
	str	x8, [sp, #8]
	str	wzr, [sp, #4]
	bl	ufshcd_query_attr_safe
	adrp	x8, :got:__stack_chk_guard
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldr	x9, [sp, #8]
	ldr	x8, [x8]
	cmp	x8, x9
	b.ne	.LBB9_2
// %bb.1:
	ldp	x29, x30, [sp, #16]             // 16-byte Folded Reload
	add	sp, sp, #32
	ret
.LBB9_2:
	bl	__stack_chk_fail
.Lfunc_end9:
	.size	ufshcd_hynix_bkops_stop, .Lfunc_end9-ufshcd_hynix_bkops_stop
                                        // -- End function
	.section	.pac_init_section,"ax",@progbits
	.p2align	2                               // -- Begin function __pauth_pac_globals
	.type	.L__pauth_pac_globals,@function
.L__pauth_pac_globals:                  // @__pauth_pac_globals
	.cfi_startproc
// %bb.0:
	adrp	x8, ufs_bkops_ops
	add	x8, x8, :lo12:ufs_bkops_ops
	mov	w9, #28009
	mov	w12, #58020
	mov	w13, #50722
	mov	w16, #23545
	ldp	x10, x11, [x8]
	pacia	x10, x9
	adrp	x9, hi1861_bkops_ops
	add	x9, x9, :lo12:hi1861_bkops_ops
	pacia	x11, x12
	stp	x10, x11, [x8]
	adrp	x10, hynix_bkops_ops
	add	x10, x10, :lo12:hynix_bkops_ops
	ldp	x14, x15, [x9]
	ldp	x11, x12, [x10]
	pacia	x14, x13
	ldr	x8, [x9, #16]
	pacia	x15, x16
	stp	x14, x15, [x9]
	ldr	x14, [x10, #16]
	pacia	x8, x16
	pacia	x11, x13
	pacia	x12, x16
	str	x8, [x9, #16]
	pacia	x14, x16
	stp	x11, x12, [x10]
	str	x14, [x10, #16]
	ret
.Lfunc_end10:
	.size	.L__pauth_pac_globals, .Lfunc_end10-.L__pauth_pac_globals
	.cfi_endproc
                                        // -- End function
	.type	.L.str,@object                  // @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"\0013UFS bkops status query failed!\n"
	.size	.L.str, 34

	.type	.L.str.1,@object                // @.str.1
.L.str.1:
	.asciz	"\0013bkops not supported or has been enabled already!\n"
	.size	.L.str.1, 52

	.type	ufs_bkops_ops,@object           // @ufs_bkops_ops
	.data
	.p2align	3
ufs_bkops_ops:
	.xword	__ufshcd_bkops_start_stop
	.xword	__ufshcd_bkops_status_query
	.size	ufs_bkops_ops, 16

	.type	.L.str.2,@object                // @.str.2
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str.2:
	.asciz	"\0013UFS bkops start failed!\n"
	.size	.L.str.2, 27

	.type	.L.str.3,@object                // @.str.3
.L.str.3:
	.asciz	"HUFS_MODEL_ANY"
	.size	.L.str.3, 15

	.type	.L.str.4,@object                // @.str.4
.L.str.4:
	.asciz	"HUFS_REV_ANY"
	.size	.L.str.4, 13

	.type	hi1861_bkops_ops,@object        // @hi1861_bkops_ops
	.data
	.p2align	3
hi1861_bkops_ops:
	.xword	ufshcd_1861_bkops_query
	.xword	ufshcd_1861_bkops_start
	.xword	ufshcd_1861_bkops_stop
	.size	hi1861_bkops_ops, 24

	.type	hynix_bkops_ops,@object         // @hynix_bkops_ops
	.p2align	3
hynix_bkops_ops:
	.xword	ufshcd_hynix_bkops_query
	.xword	ufshcd_hynix_bkops_start
	.xword	ufshcd_hynix_bkops_stop
	.size	hynix_bkops_ops, 24

	.type	.L.str.5,@object                // @.str.5
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str.5:
	.asciz	"BUG: failure at %s:%d/%s()!\n"
	.size	.L.str.5, 29

	.type	.L.str.6,@object                // @.str.6
.L.str.6:
	.asciz	"ldk"
	.size	.L.str.6, 4

	.type	.L__func__.ufshcd_query_hi1861_bkops,@object // @__func__.ufshcd_query_hi1861_bkops
.L__func__.ufshcd_query_hi1861_bkops:
	.asciz	"ufshcd_query_hi1861_bkops"
	.size	.L__func__.ufshcd_query_hi1861_bkops, 26

	.type	.L.str.7,@object                // @.str.7
.L.str.7:
	.asciz	"BUG!"
	.size	.L.str.7, 5

	.type	.L.str.8,@object                // @.str.8
.L.str.8:
	.asciz	"%s: Sending bkops query failed, err = %d\n"
	.size	.L.str.8, 42

	.type	.L__func__.arch_local_irq_restore,@object // @__func__.arch_local_irq_restore
.L__func__.arch_local_irq_restore:
	.asciz	"arch_local_irq_restore"
	.size	.L__func__.arch_local_irq_restore, 23

	.type	.L__func__.__preempt_count_sub,@object // @__func__.__preempt_count_sub
.L__func__.__preempt_count_sub:
	.asciz	"__preempt_count_sub"
	.size	.L__func__.__preempt_count_sub, 20

	.type	.L.str.9,@object                // @.str.9
.L.str.9:
	.asciz	"%s: Invalid argument for read request\n"
	.size	.L.str.9, 39

	.type	.L__func__.mas_bkops_upiu_query_opcode,@object // @__func__.mas_bkops_upiu_query_opcode
.L__func__.mas_bkops_upiu_query_opcode:
	.asciz	"mas_bkops_upiu_query_opcode"
	.size	.L__func__.mas_bkops_upiu_query_opcode, 28

	.type	.L.str.10,@object               // @.str.10
.L.str.10:
	.asciz	"%s: Expected query bkops opcode but got = %d\n"
	.size	.L.str.10, 46

	.type	.Lg__usr1_hmos_for_chipset_src_increment_sourcecode_out_generic_generic_arm_64only_general_7525L_phone_standard_hongmeng_build_tmp_work_shared_sharedrepo_hm_ldk_xianoh_git_platform_source_basicplatform_drivers_scsi_ufs_hufs_bkops__pauth_pac_globals,@object // @g__usr1_hmos_for_chipset_src_increment_sourcecode_out_generic_generic_arm_64only_general_7525L_phone_standard_hongmeng_build_tmp_work_shared_sharedrepo_hm_ldk_xianoh_git_platform_source_basicplatform_drivers_scsi_ufs_hufs_bkops__pauth_pac_globals
	.section	.preinit_array,"awR",@preinit_array,unique,1
	.p2align	3
.Lg__usr1_hmos_for_chipset_src_increment_sourcecode_out_generic_generic_arm_64only_general_7525L_phone_standard_hongmeng_build_tmp_work_shared_sharedrepo_hm_ldk_xianoh_git_platform_source_basicplatform_drivers_scsi_ufs_hufs_bkops__pauth_pac_globals:
	.xword	.L__pauth_pac_globals
	.size	.Lg__usr1_hmos_for_chipset_src_increment_sourcecode_out_generic_generic_arm_64only_general_7525L_phone_standard_hongmeng_build_tmp_work_shared_sharedrepo_hm_ldk_xianoh_git_platform_source_basicplatform_drivers_scsi_ufs_hufs_bkops__pauth_pac_globals, 8

	.ident	"OHOS (BiSheng Mobile STD 203.0.0.2.B001) clang version 15.0.4 (llvm-project db54e0a809bd)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym ufshcd_1861_bkops_query
	.addrsig_sym ufshcd_1861_bkops_start
	.addrsig_sym ufshcd_1861_bkops_stop
	.addrsig_sym ufshcd_hynix_bkops_query
	.addrsig_sym ufshcd_hynix_bkops_start
	.addrsig_sym ufshcd_hynix_bkops_stop
	.addrsig_sym __ufshcd_bkops_start_stop
	.addrsig_sym __ufshcd_bkops_status_query
	.addrsig_sym .L__pauth_pac_globals
	.addrsig_sym ufs_bkops_ops
	.addrsig_sym hi1861_bkops_ops
	.addrsig_sym hynix_bkops_ops
