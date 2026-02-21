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
	ldr	x8, [x20, #6488]
	mov	x0, x20
	mov	x1, x21
	ldr	x8, [x8]
	blr	x8
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
	ldr	x8, [x20, #6488]
	mov	x0, x20
	ldr	x8, [x8, #8]
	blr	x8
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
	ldr	x27, [x25, #8840]
	mov	w8, #8760
	mov	w9, #8840
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
	ldr	x28, [x25, #8824]
	mov	w8, #8824
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
	str	x8, [x25, #8808]
.LBB2_30:
	adrp	x8, ufs_bkops_ops
	add	x8, x8, :lo12:ufs_bkops_ops
	mov	w20, #1
	mov	w9, #3600
	ldr	x10, [sp]                       // 8-byte Folded Reload
	str	x19, [x25, #8816]
	mov	x0, x19
	str	w20, [x19, #156]
	stp	x8, x10, [x19, #8]
	str	x9, [x19, #160]
	ldr	x1, [x25, #7080]
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
	stp	x28, x27, [sp, #48]             // 16-byte Folded Spill
	add	x29, sp, #32
	stp	x26, x25, [sp, #64]             // 16-byte Folded Spill
	stp	x24, x23, [sp, #80]             // 16-byte Folded Spill
	stp	x22, x21, [sp, #96]             // 16-byte Folded Spill
	stp	x20, x19, [sp, #112]            // 16-byte Folded Spill
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldr	x8, [x8]
	stur	x8, [x29, #-8]
	stp	xzr, xzr, [sp, #8]
	cbz	x0, .LBB6_56
// %bb.1:
	adrp	x25, :got:liblinux_thread_offset
	adrp	x23, :got:liblinux_thread_key
	//APP
	mrs	x24, TPIDR_EL0

	//NO_APP
	ldr	x25, [x25, :got_lo12:liblinux_thread_offset]
	ldr	w8, [x25]
	ldr	x23, [x23, :got_lo12:liblinux_thread_key]
	sub	x8, x24, x8
	ldr	w9, [x23]
	ldr	x8, [x8]
	ldr	x26, [x8, x9, lsl #3]
	ldr	x8, [x0, #88]
	ldrb	w9, [x26, #84]
	ldr	x22, [x8, #56]
	cbnz	w9, .LBB6_5
// %bb.2:
	ldr	w27, [x26, #36]
	cbnz	w27, .LBB6_6
// %bb.3:
	mov	x19, x2
	mov	w21, w1
	mov	x20, x0
	ldr	w8, [x26, #32]
	cbz	w8, .LBB6_51
.LBB6_4:
	ldr	w8, [x26, #32]
	mov	w9, #1
	add	w8, w8, #1
	str	w8, [x26, #32]
	//APP
	//NO_APP
	str	w9, [x26, #36]
	ldr	w8, [x25]
	ldr	w9, [x23]
	sub	x8, x24, x8
	ldr	x8, [x8]
	ldr	x26, [x8, x9, lsl #3]
	b	.LBB6_7
.LBB6_5:
	mov	x19, x2
	mov	w21, w1
	mov	x20, x0
	mov	x27, xzr
	ldr	w8, [x26, #32]
	cbz	w8, .LBB6_8
	b	.LBB6_9
.LBB6_6:
	mov	x19, x2
	mov	w21, w1
	mov	x20, x0
.LBB6_7:
	cmp	w27, #0
	cset	w27, ne
	ldr	w8, [x26, #32]
	cbnz	w8, .LBB6_9
.LBB6_8:
	ldrb	w8, [x26, #84]
	cbz	w8, .LBB6_38
.LBB6_9:
	ldr	w9, [x26, #32]
	mov	x8, xzr
	mov	w10, #1
	add	w9, w9, #1
	str	w9, [x26, #32]
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
	cbnz	w1, .LBB6_57
.LBB6_10:
	ldr	x9, [x20, #88]
	mov	w8, wzr
	ldr	w10, [x20, #1240]
	ldr	x9, [x9, #56]
	//APP
	stlrb	w8, [x9]
	//NO_APP
	ldr	w8, [x25]
	cmp	w10, #2
	ldr	w9, [x23]
	sub	x8, x24, x8
	ldr	x8, [x8]
	ldr	x22, [x8, x9, lsl #3]
	ldrb	w8, [x22, #84]
	b.ne	.LBB6_24
// %bb.11:
	cbnz	w8, .LBB6_14
// %bb.12:
	ldr	w8, [x22, #36]
	cbz	w8, .LBB6_58
// %bb.13:
	cbz	x27, .LBB6_41
.LBB6_14:
	//APP
	//NO_APP
	ldr	w8, [x25]
	ldr	w9, [x23]
	sub	x8, x24, x8
	ldr	x8, [x8]
	ldr	x22, [x8, x9, lsl #3]
	ldr	w8, [x22, #32]
	sub	w8, w8, #1
	str	w8, [x22, #32]
	ldr	w8, [x22, #32]
	cbnz	w8, .LBB6_17
// %bb.15:
	ldrb	w8, [x22, #84]
	cbnz	w8, .LBB6_17
// %bb.16:
	ldr	x23, [x22, #24]
	adrp	x9, :got:__preempt_disabled
	mov	w11, #1
	ldr	w8, [x23, #8]
	ldr	x9, [x9, :got_lo12:__preempt_disabled]
	lsl	x8, x8, #2
	ldr	w10, [x9, x8]
	sub	w10, w10, #1
	str	w10, [x9, x8]
	str	w11, [x23, #4]
	ldr	w8, [x23, #12]
	cbnz	w8, .LBB6_61
.LBB6_17:
	ldr	w8, [x22, #32]
	tbnz	w8, #31, .LBB6_62
.LBB6_18:
	mov	w8, #6720
	add	x22, x20, x8
	mov	x0, x22
	bl	mutex_lock
	add	x23, x20, #3888
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
	b.hs	.LBB6_34
// %bb.19:
	mov	w9, #129
	strb	w9, [x8]
.LBB6_20:
	mov	x0, x20
	mov	w1, #1
	mov	w2, #1300
	bl	ufshcd_exec_dev_cmd
	cbnz	w0, .LBB6_54
// %bb.21:
	cbz	x19, .LBB6_37
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
	b	.LBB6_32
.LBB6_24:
	cbnz	w8, .LBB6_27
// %bb.25:
	ldr	w8, [x22, #36]
	cbz	w8, .LBB6_58
// %bb.26:
	cbz	x27, .LBB6_46
.LBB6_27:
	//APP
	//NO_APP
	ldr	w8, [x25]
	ldr	w9, [x23]
	sub	x8, x24, x8
	ldr	x8, [x8]
	ldr	x19, [x8, x9, lsl #3]
	ldr	w8, [x19, #32]
	sub	w8, w8, #1
	str	w8, [x19, #32]
	ldr	w8, [x19, #32]
	cbnz	w8, .LBB6_30
// %bb.28:
	ldrb	w8, [x19, #84]
	cbnz	w8, .LBB6_30
// %bb.29:
	ldr	x20, [x19, #24]
	adrp	x9, :got:__preempt_disabled
	mov	w11, #1
	ldr	w8, [x20, #8]
	ldr	x9, [x9, :got_lo12:__preempt_disabled]
	lsl	x8, x8, #2
	ldr	w10, [x9, x8]
	sub	w10, w10, #1
	str	w10, [x9, x8]
	str	w11, [x20, #4]
	ldr	w8, [x20, #12]
	cbnz	w8, .LBB6_63
.LBB6_30:
	ldr	w8, [x19, #32]
	tbnz	w8, #31, .LBB6_67
.LBB6_31:
	mov	w21, #-16
.LBB6_32:
	adrp	x8, :got:__stack_chk_guard
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldur	x9, [x29, #-8]
	ldr	x8, [x8]
	cmp	x8, x9
	b.ne	.LBB6_70
// %bb.33:
	mov	w0, w21
	ldp	x20, x19, [sp, #112]            // 16-byte Folded Reload
	ldp	x22, x21, [sp, #96]             // 16-byte Folded Reload
	ldp	x24, x23, [sp, #80]             // 16-byte Folded Reload
	ldp	x26, x25, [sp, #64]             // 16-byte Folded Reload
	ldp	x28, x27, [sp, #48]             // 16-byte Folded Reload
	ldp	x29, x30, [sp, #32]             // 16-byte Folded Reload
	add	sp, sp, #128
	ret
.LBB6_34:
	cmp	w21, #242
	b.ne	.LBB6_55
// %bb.35:
	mov	w9, #1
	strb	w9, [x8]
	cbnz	x19, .LBB6_20
// %bb.36:
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.9
	add	x1, x1, :lo12:.L.str.9
	adrp	x2, .L__func__.mas_bkops_upiu_query_opcode
	add	x2, x2, :lo12:.L__func__.mas_bkops_upiu_query_opcode
	bl	_dev_err
	mov	w21, #-22
	b	.LBB6_23
.LBB6_37:
	mov	w21, wzr
	b	.LBB6_23
.LBB6_38:
	ldr	x28, [x26, #24]
	mov	w8, #2
	adrp	x11, :got:__preempt_disabled
	str	w8, [x28, #4]
	//APP
	//NO_APP
	ldr	w8, [x28, #8]
	ldr	x11, [x11, :got_lo12:__preempt_disabled]
	ldr	w9, [x11, x8, lsl #2]
	cmp	w9, #0
	b.gt	.LBB6_59
// %bb.39:
	ldr	w10, [x28, #4]
	cmp	w10, #3
	b.eq	.LBB6_59
.LBB6_40:
	add	w9, w9, #1
	str	w9, [x11, x8, lsl #2]
	b	.LBB6_9
.LBB6_41:
	//APP
	//NO_APP
	ldr	w8, [x25]
	ldr	w9, [x23]
	sub	x8, x24, x8
	ldr	x8, [x8]
	ldr	x26, [x8, x9, lsl #3]
	ldr	w8, [x26, #32]
	sub	w8, w8, #1
	str	w8, [x26, #32]
	ldr	w8, [x26, #32]
	cbnz	w8, .LBB6_44
// %bb.42:
	ldrb	w8, [x26, #84]
	cbnz	w8, .LBB6_44
// %bb.43:
	ldr	x27, [x26, #24]
	adrp	x9, :got:__preempt_disabled
	mov	w11, #1
	ldr	w8, [x27, #8]
	ldr	x9, [x9, :got_lo12:__preempt_disabled]
	lsl	x8, x8, #2
	ldr	w10, [x9, x8]
	sub	w10, w10, #1
	str	w10, [x9, x8]
	str	w11, [x27, #4]
	ldr	w8, [x27, #12]
	cbnz	w8, .LBB6_64
.LBB6_44:
	ldr	w8, [x26, #32]
	tbnz	w8, #31, .LBB6_65
.LBB6_45:
	str	wzr, [x22, #36]
	b	.LBB6_14
.LBB6_46:
	//APP
	//NO_APP
	ldr	w8, [x25]
	ldr	w9, [x23]
	sub	x8, x24, x8
	ldr	x8, [x8]
	ldr	x19, [x8, x9, lsl #3]
	ldr	w8, [x19, #32]
	sub	w8, w8, #1
	str	w8, [x19, #32]
	ldr	w8, [x19, #32]
	cbnz	w8, .LBB6_49
// %bb.47:
	ldrb	w8, [x19, #84]
	cbnz	w8, .LBB6_49
// %bb.48:
	ldr	x20, [x19, #24]
	adrp	x9, :got:__preempt_disabled
	mov	w11, #1
	ldr	w8, [x20, #8]
	ldr	x9, [x9, :got_lo12:__preempt_disabled]
	lsl	x8, x8, #2
	ldr	w10, [x9, x8]
	sub	w10, w10, #1
	str	w10, [x9, x8]
	str	w11, [x20, #4]
	ldr	w8, [x20, #12]
	cbnz	w8, .LBB6_66
.LBB6_49:
	ldr	w8, [x19, #32]
	tbnz	w8, #31, .LBB6_67
.LBB6_50:
	str	wzr, [x22, #36]
	b	.LBB6_27
.LBB6_51:
	ldr	x28, [x26, #24]
	mov	w8, #2
	str	w8, [x28, #4]
	adrp	x8, :got:__preempt_disabled
	//APP
	//NO_APP
	ldr	w9, [x28, #8]
	ldr	x8, [x8, :got_lo12:__preempt_disabled]
	ldr	w10, [x8, x9, lsl #2]
	cmp	w10, #0
	b.gt	.LBB6_60
// %bb.52:
	ldr	w11, [x28, #4]
	cmp	w11, #3
	b.eq	.LBB6_60
.LBB6_53:
	add	w10, w10, #1
	str	w10, [x8, x9, lsl #2]
	b	.LBB6_4
.LBB6_54:
	mov	w21, w0
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.8
	add	x1, x1, :lo12:.L.str.8
	adrp	x2, .L__func__.ufshcd_query_hi1861_bkops
	add	x2, x2, :lo12:.L__func__.ufshcd_query_hi1861_bkops
	mov	w3, w21
	bl	_dev_err
	b	.LBB6_23
.LBB6_55:
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.10
	add	x1, x1, :lo12:.L.str.10
	adrp	x2, .L__func__.mas_bkops_upiu_query_opcode
	add	x2, x2, :lo12:.L__func__.mas_bkops_upiu_query_opcode
	mov	w3, w21
	bl	_dev_err
	mov	w21, #-22
	b	.LBB6_23
.LBB6_56:
	adrp	x0, .L.str.5
	add	x0, x0, :lo12:.L.str.5
	adrp	x1, .L.str.6
	add	x1, x1, :lo12:.L.str.6
	adrp	x3, .L__func__.ufshcd_query_hi1861_bkops
	add	x3, x3, :lo12:.L__func__.ufshcd_query_hi1861_bkops
	mov	w2, #110
	b	.LBB6_69
.LBB6_57:
	mov	x0, x22
	bl	queued_spin_lock_slowpath
	b	.LBB6_10
.LBB6_58:
	adrp	x0, .L.str.5
	add	x0, x0, :lo12:.L.str.5
	adrp	x1, .L.str.6
	add	x1, x1, :lo12:.L.str.6
	adrp	x3, .L__func__.arch_local_irq_restore
	add	x3, x3, :lo12:.L__func__.arch_local_irq_restore
	mov	w2, #77
	b	.LBB6_69
.LBB6_59:
	bl	kernel_thread_preempt_disable
	adrp	x11, :got:__preempt_disabled
	ldr	x11, [x11, :got_lo12:__preempt_disabled]
	ldr	w8, [x28, #8]
	ldr	w9, [x11, x8, lsl #2]
	b	.LBB6_40
.LBB6_60:
	bl	kernel_thread_preempt_disable
	adrp	x8, :got:__preempt_disabled
	ldr	x8, [x8, :got_lo12:__preempt_disabled]
	ldr	w9, [x28, #8]
	ldr	w10, [x8, x9, lsl #2]
	b	.LBB6_53
.LBB6_61:
	bl	kernel_thread_preempt_enable
	str	wzr, [x23, #12]
	ldr	w8, [x22, #32]
	tbz	w8, #31, .LBB6_18
.LBB6_62:
	str	wzr, [x22, #32]
	b	.LBB6_68
.LBB6_63:
	bl	kernel_thread_preempt_enable
	str	wzr, [x20, #12]
	ldr	w8, [x19, #32]
	tbz	w8, #31, .LBB6_31
	b	.LBB6_67
.LBB6_64:
	bl	kernel_thread_preempt_enable
	str	wzr, [x27, #12]
	ldr	w8, [x26, #32]
	tbz	w8, #31, .LBB6_45
.LBB6_65:
	str	wzr, [x26, #32]
	b	.LBB6_68
.LBB6_66:
	bl	kernel_thread_preempt_enable
	str	wzr, [x20, #12]
	ldr	w8, [x19, #32]
	tbz	w8, #31, .LBB6_50
.LBB6_67:
	str	wzr, [x19, #32]
.LBB6_68:
	adrp	x0, .L.str.5
	add	x0, x0, :lo12:.L.str.5
	adrp	x1, .L.str.6
	add	x1, x1, :lo12:.L.str.6
	adrp	x3, .L__func__.__preempt_count_sub
	add	x3, x3, :lo12:.L__func__.__preempt_count_sub
	mov	w2, #112
.LBB6_69:
	bl	printk
	adrp	x0, .L.str.7
	add	x0, x0, :lo12:.L.str.7
	bl	panic
.LBB6_70:
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

	.ident	"OHOS (BiSheng Mobile CPU STD 202.1.0.B030) clang version 15.0.4 (llvm-project 7123208073d2)"
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
	.addrsig_sym ufs_bkops_ops
	.addrsig_sym hi1861_bkops_ops
	.addrsig_sym hynix_bkops_ops
