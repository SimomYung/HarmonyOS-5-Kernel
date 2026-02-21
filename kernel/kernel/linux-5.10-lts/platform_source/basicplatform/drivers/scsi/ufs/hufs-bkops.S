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
	ldr	x8, [x20, #6816]
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
	ldr	x8, [x20, #6816]
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
	ldr	x27, [x25, #9168]
	mov	w8, #9088
	mov	w9, #9168
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
	ldr	x28, [x25, #9152]
	mov	w8, #9152
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
	str	x8, [x25, #9136]
.LBB2_30:
	adrp	x8, ufs_bkops_ops
	add	x8, x8, :lo12:ufs_bkops_ops
	mov	w20, #1
	mov	w9, #3600
	ldr	x10, [sp]                       // 8-byte Folded Reload
	str	x19, [x25, #9144]
	mov	x0, x19
	str	w20, [x19, #156]
	stp	x8, x10, [x19, #8]
	str	x9, [x19, #160]
	ldr	x1, [x25, #7408]
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
	cbz	x0, .LBB6_57
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
	hint	#7
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
	cbz	w9, .LBB6_52
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
	cbz	w8, .LBB6_39
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
	cbnz	w1, .LBB6_58
.LBB6_11:
	ldr	x9, [x20, #88]
	mov	w8, wzr
	ldr	w10, [x20, #1584]
	ldr	x9, [x9, #56]
	//APP
	stlrb	w8, [x9]
	//NO_APP
	ldr	x8, [x24]
	cmp	w10, #2
	ldr	x22, [x8, x23, lsl #3]
	ldrb	w8, [x22, #92]
	b.ne	.LBB6_25
// %bb.12:
	cbnz	w8, .LBB6_15
// %bb.13:
	ldr	w8, [x22, #44]
	cbz	w8, .LBB6_59
// %bb.14:
	cbz	x26, .LBB6_42
.LBB6_15:
	//APP
	//NO_APP
	ldr	x8, [x24]
	ldr	x22, [x8, x23, lsl #3]
	ldr	w8, [x22, #40]
	sub	w8, w8, #1
	str	w8, [x22, #40]
	ldr	w8, [x22, #40]
	cbnz	w8, .LBB6_18
// %bb.16:
	ldrb	w8, [x22, #92]
	cbnz	w8, .LBB6_18
// %bb.17:
	ldr	x23, [x22, #32]
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
.LBB6_18:
	ldr	w8, [x22, #40]
	tbnz	w8, #31, .LBB6_62
.LBB6_19:
	mov	w8, #7048
	add	x22, x20, x8
	mov	x0, x22
	bl	mutex_lock
	mov	w8, #4216
	add	x23, x20, x8
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
	b.hs	.LBB6_35
// %bb.20:
	mov	w9, #129
	strb	w9, [x8]
.LBB6_21:
	mov	x0, x20
	mov	w1, #1
	mov	w2, #1300
	bl	ufshcd_exec_dev_cmd
	cbnz	w0, .LBB6_55
// %bb.22:
	cbz	x19, .LBB6_38
// %bb.23:
	ldr	x8, [sp, #8]
	mov	w21, wzr
	ldrb	w8, [x8, #19]
	str	w8, [x19]
.LBB6_24:
	mov	x0, x23
	bl	mutex_unlock
	mov	x0, x22
	bl	mutex_unlock
	b	.LBB6_33
.LBB6_25:
	cbnz	w8, .LBB6_28
// %bb.26:
	ldr	w8, [x22, #44]
	cbz	w8, .LBB6_59
// %bb.27:
	cbz	x26, .LBB6_47
.LBB6_28:
	//APP
	//NO_APP
	ldr	x8, [x24]
	ldr	x19, [x8, x23, lsl #3]
	ldr	w8, [x19, #40]
	sub	w8, w8, #1
	str	w8, [x19, #40]
	ldr	w8, [x19, #40]
	cbnz	w8, .LBB6_31
// %bb.29:
	ldrb	w8, [x19, #92]
	cbnz	w8, .LBB6_31
// %bb.30:
	ldr	x20, [x19, #32]
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
.LBB6_31:
	ldr	w8, [x19, #40]
	tbnz	w8, #31, .LBB6_68
.LBB6_32:
	mov	w21, #-16
.LBB6_33:
	adrp	x8, :got:__stack_chk_guard
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldur	x9, [x29, #-8]
	ldr	x8, [x8]
	cmp	x8, x9
	b.ne	.LBB6_71
// %bb.34:
	mov	w0, w21
	ldp	x20, x19, [sp, #112]            // 16-byte Folded Reload
	ldp	x22, x21, [sp, #96]             // 16-byte Folded Reload
	ldp	x24, x23, [sp, #80]             // 16-byte Folded Reload
	ldp	x26, x25, [sp, #64]             // 16-byte Folded Reload
	ldp	x28, x27, [sp, #48]             // 16-byte Folded Reload
	ldp	x29, x30, [sp, #32]             // 16-byte Folded Reload
	add	sp, sp, #128
	ret
.LBB6_35:
	cmp	w21, #242
	b.ne	.LBB6_56
// %bb.36:
	mov	w9, #1
	strb	w9, [x8]
	cbnz	x19, .LBB6_21
// %bb.37:
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.9
	add	x1, x1, :lo12:.L.str.9
	adrp	x2, .L__func__.mas_bkops_upiu_query_opcode
	add	x2, x2, :lo12:.L__func__.mas_bkops_upiu_query_opcode
	bl	_dev_err
	mov	w21, #-22
	b	.LBB6_24
.LBB6_38:
	mov	w21, wzr
	b	.LBB6_24
.LBB6_39:
	ldr	x28, [x25, #32]
	mov	w8, #2
	adrp	x27, :got:__preempt_disabled
	str	w8, [x28, #4]
	//APP
	//NO_APP
	ldr	w8, [x28, #8]
	ldr	x27, [x27, :got_lo12:__preempt_disabled]
	ldr	w9, [x27, x8, lsl #2]
	cmp	w9, #0
	b.gt	.LBB6_60
// %bb.40:
	ldr	w10, [x28, #4]
	cmp	w10, #3
	b.eq	.LBB6_60
.LBB6_41:
	add	w9, w9, #1
	str	w9, [x27, x8, lsl #2]
	b	.LBB6_10
.LBB6_42:
	str	xzr, [x22, #24]
	//APP
	//NO_APP
	ldr	x8, [x24]
	ldr	x25, [x8, x23, lsl #3]
	ldr	w8, [x25, #40]
	sub	w8, w8, #1
	str	w8, [x25, #40]
	ldr	w8, [x25, #40]
	cbnz	w8, .LBB6_45
// %bb.43:
	ldrb	w8, [x25, #92]
	cbnz	w8, .LBB6_45
// %bb.44:
	ldr	x26, [x25, #32]
	adrp	x9, :got:__preempt_disabled
	mov	w11, #1
	ldr	w8, [x26, #8]
	ldr	x9, [x9, :got_lo12:__preempt_disabled]
	lsl	x8, x8, #2
	ldr	w10, [x9, x8]
	sub	w10, w10, #1
	str	w10, [x9, x8]
	str	w11, [x26, #4]
	ldr	w8, [x26, #12]
	cbnz	w8, .LBB6_65
.LBB6_45:
	ldr	w8, [x25, #40]
	tbnz	w8, #31, .LBB6_66
.LBB6_46:
	str	wzr, [x22, #44]
	b	.LBB6_15
.LBB6_47:
	str	xzr, [x22, #24]
	//APP
	//NO_APP
	ldr	x8, [x24]
	ldr	x19, [x8, x23, lsl #3]
	ldr	w8, [x19, #40]
	sub	w8, w8, #1
	str	w8, [x19, #40]
	ldr	w8, [x19, #40]
	cbnz	w8, .LBB6_50
// %bb.48:
	ldrb	w8, [x19, #92]
	cbnz	w8, .LBB6_50
// %bb.49:
	ldr	x20, [x19, #32]
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
	cbnz	w8, .LBB6_67
.LBB6_50:
	ldr	w8, [x19, #40]
	tbnz	w8, #31, .LBB6_68
.LBB6_51:
	str	wzr, [x22, #44]
	b	.LBB6_28
.LBB6_52:
	ldr	x27, [x8, #32]
	mov	w9, #2
	str	w9, [x27, #4]
	adrp	x9, :got:__preempt_disabled
	//APP
	//NO_APP
	ldr	w10, [x27, #8]
	ldr	x9, [x9, :got_lo12:__preempt_disabled]
	ldr	w11, [x9, x10, lsl #2]
	cmp	w11, #0
	b.gt	.LBB6_64
// %bb.53:
	ldr	w12, [x27, #4]
	cmp	w12, #3
	b.eq	.LBB6_64
.LBB6_54:
	add	w11, w11, #1
	str	w11, [x9, x10, lsl #2]
	b	.LBB6_5
.LBB6_55:
	mov	w21, w0
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.8
	add	x1, x1, :lo12:.L.str.8
	adrp	x2, .L__func__.ufshcd_query_hi1861_bkops
	add	x2, x2, :lo12:.L__func__.ufshcd_query_hi1861_bkops
	mov	w3, w21
	bl	_dev_err
	b	.LBB6_24
.LBB6_56:
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.10
	add	x1, x1, :lo12:.L.str.10
	adrp	x2, .L__func__.mas_bkops_upiu_query_opcode
	add	x2, x2, :lo12:.L__func__.mas_bkops_upiu_query_opcode
	mov	w3, w21
	bl	_dev_err
	mov	w21, #-22
	b	.LBB6_24
.LBB6_57:
	adrp	x0, .L.str.5
	add	x0, x0, :lo12:.L.str.5
	adrp	x1, .L.str.6
	add	x1, x1, :lo12:.L.str.6
	adrp	x3, .L__func__.ufshcd_query_hi1861_bkops
	add	x3, x3, :lo12:.L__func__.ufshcd_query_hi1861_bkops
	mov	w2, #110
	b	.LBB6_70
.LBB6_58:
	mov	x0, x22
	bl	queued_spin_lock_slowpath
	b	.LBB6_11
.LBB6_59:
	adrp	x0, .L.str.5
	add	x0, x0, :lo12:.L.str.5
	adrp	x1, .L.str.6
	add	x1, x1, :lo12:.L.str.6
	adrp	x3, .L__func__.arch_local_irq_restore
	add	x3, x3, :lo12:.L__func__.arch_local_irq_restore
	mov	w2, #80
	b	.LBB6_70
.LBB6_60:
	bl	kernel_thread_preempt_disable
	ldr	w8, [x28, #8]
	ldr	w9, [x27, x8, lsl #2]
	b	.LBB6_41
.LBB6_61:
	bl	kernel_thread_preempt_enable
	str	wzr, [x23, #12]
	ldr	w8, [x22, #40]
	tbz	w8, #31, .LBB6_19
.LBB6_62:
	str	wzr, [x22, #40]
	b	.LBB6_69
.LBB6_63:
	bl	kernel_thread_preempt_enable
	str	wzr, [x20, #12]
	ldr	w8, [x19, #40]
	tbz	w8, #31, .LBB6_32
	b	.LBB6_68
.LBB6_64:
	mov	x28, x8
	bl	kernel_thread_preempt_disable
	adrp	x9, :got:__preempt_disabled
	mov	x8, x28
	ldr	x9, [x9, :got_lo12:__preempt_disabled]
	ldr	w10, [x27, #8]
	ldr	w11, [x9, x10, lsl #2]
	b	.LBB6_54
.LBB6_65:
	bl	kernel_thread_preempt_enable
	str	wzr, [x26, #12]
	ldr	w8, [x25, #40]
	tbz	w8, #31, .LBB6_46
.LBB6_66:
	str	wzr, [x25, #40]
	b	.LBB6_69
.LBB6_67:
	bl	kernel_thread_preempt_enable
	str	wzr, [x20, #12]
	ldr	w8, [x19, #40]
	tbz	w8, #31, .LBB6_51
.LBB6_68:
	str	wzr, [x19, #40]
.LBB6_69:
	adrp	x0, .L.str.5
	add	x0, x0, :lo12:.L.str.5
	adrp	x1, .L.str.6
	add	x1, x1, :lo12:.L.str.6
	adrp	x3, .L__func__.__preempt_count_sub
	add	x3, x3, :lo12:.L__func__.__preempt_count_sub
	mov	w2, #127
.LBB6_70:
	bl	printk
	adrp	x0, .L.str.7
	add	x0, x0, :lo12:.L.str.7
	bl	panic
.LBB6_71:
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
