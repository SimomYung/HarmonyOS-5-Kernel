	.text
	.file	"dfa_mcq.c"
	.globl	dfa_set_sdev                    // -- Begin function dfa_set_sdev
	.p2align	2
	.type	dfa_set_sdev,@function
dfa_set_sdev:                           // @dfa_set_sdev
// %bb.0:
	mov	w9, #9320
	ldr	x8, [x0]
	ldrb	w9, [x8, x9]
	cbnz	w9, .LBB0_2
.LBB0_1:
	ret
.LBB0_2:
	ldr	x9, [x0, #120]
	cmp	x9, #3
	b.ne	.LBB0_1
// %bb.3:
	adrp	x9, dfa_sdev
	add	x8, x8, #2320
	adrp	x10, dfa_hba
	str	x0, [x9, :lo12:dfa_sdev]
	str	x8, [x10, :lo12:dfa_hba]
	ret
.Lfunc_end0:
	.size	dfa_set_sdev, .Lfunc_end0-dfa_set_sdev
                                        // -- End function
	.globl	dfa_get_base_addr               // -- Begin function dfa_get_base_addr
	.p2align	2
	.type	dfa_get_base_addr,@function
dfa_get_base_addr:                      // @dfa_get_base_addr
// %bb.0:
	stp	x29, x30, [sp, #-32]!           // 16-byte Folded Spill
	stp	x20, x19, [sp, #16]             // 16-byte Folded Spill
	mov	x20, x1
	mov	x19, x0
	mov	x0, x1
	mov	w1, #5
	mov	x29, sp
	bl	of_iomap
	str	x0, [x19, #7008]
	cbz	x0, .LBB1_4
// %bb.1:
	mov	x0, x20
	mov	w1, #6
	bl	of_iomap
	str	x0, [x19, #7016]
	cbz	x0, .LBB1_5
// %bb.2:
	mov	w0, wzr
.LBB1_3:
	ldp	x20, x19, [sp, #16]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #32             // 16-byte Folded Reload
	ret
.LBB1_4:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str
	add	x1, x1, :lo12:.L.str
	bl	_dev_err
	b	.LBB1_6
.LBB1_5:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.1
	add	x1, x1, :lo12:.L.str.1
	bl	_dev_err
	ldr	x0, [x19, #7008]
	bl	iounmap
.LBB1_6:
	mov	w0, #-12
	b	.LBB1_3
.Lfunc_end1:
	.size	dfa_get_base_addr, .Lfunc_end1-dfa_get_base_addr
                                        // -- End function
	.globl	dfa_link_startup                // -- Begin function dfa_link_startup
	.p2align	2
	.type	dfa_link_startup,@function
dfa_link_startup:                       // @dfa_link_startup
// %bb.0:
	sub	sp, sp, #48
	mov	w8, #4148
	mov	x10, #5
	str	x19, [sp, #32]                  // 8-byte Folded Spill
	mov	x19, x0
	mov	x9, #4294967297
	add	x8, x0, x8
	movk	x10, #5, lsl #32
	ldr	x0, [x0, #96]
	adrp	x7, .L.str.6
	add	x7, x7, :lo12:.L.str.6
	adrp	x1, .L.str.7
	add	x1, x1, :lo12:.L.str.7
	adrp	x2, .L__func__.dfa_link_startup
	add	x2, x2, :lo12:.L__func__.dfa_link_startup
	mov	w3, #1
	mov	w4, #1
	mov	w5, #1
	mov	w6, #1
	stp	x29, x30, [sp, #16]             // 16-byte Folded Spill
	add	x29, sp, #16
	stp	x9, x9, [x8]
	str	x10, [x8, #16]
	str	wzr, [x19, #4172]
	str	wzr, [sp, #8]
	str	x7, [sp]
	bl	_dev_err
	mov	x0, x19
	mov	w1, wzr
	bl	dfa_uie_init
	cbnz	w0, .LBB2_2
// %bb.1:
	ldp	x29, x30, [sp, #16]             // 16-byte Folded Reload
	ldr	x19, [sp, #32]                  // 8-byte Folded Reload
	add	sp, sp, #48
	ret
.LBB2_2:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.8
	add	x1, x1, :lo12:.L.str.8
	bl	_dev_err
	adrp	x0, .L.str.9
	add	x0, x0, :lo12:.L.str.9
	adrp	x1, .L.str.10
	add	x1, x1, :lo12:.L.str.10
	adrp	x3, .L__func__.dfa_link_startup
	add	x3, x3, :lo12:.L__func__.dfa_link_startup
	mov	w2, #136
	bl	printk
	adrp	x0, .L.str.11
	add	x0, x0, :lo12:.L.str.11
	bl	panic
.Lfunc_end2:
	.size	dfa_link_startup, .Lfunc_end2-dfa_link_startup
                                        // -- End function
	.globl	dfa_get_dev_cmd_tag             // -- Begin function dfa_get_dev_cmd_tag
	.p2align	2
	.type	dfa_get_dev_cmd_tag,@function
dfa_get_dev_cmd_tag:                    // @dfa_get_dev_cmd_tag
// %bb.0:
	ldr	x8, [x0, #7040]
	tbz	w8, #15, .LBB3_2
// %bb.1:
	mov	w0, wzr
	ret
.LBB3_2:
	mov	w8, #7040
	add	x8, x0, x8
	//APP
	// atomic64_fetch_or_acquire
	prfm	pstl1strm, [x8]
.Ltmp0:
	ldaxr	x9, [x8]
	orr	x10, x9, #0x8000
	stxr	w11, x10, [x8]
	cbnz	w11, .Ltmp0

	//NO_APP
	tbz	w9, #15, .LBB3_4
// %bb.3:
	mov	w0, wzr
	ret
.LBB3_4:
	mov	w8, #15
	mov	w9, #7072
	add	x9, x0, x9
	mov	w0, #1
	str	w8, [x1]
	//APP
	// atomic_add
	prfm	pstl1strm, [x9]
.Ltmp1:
	ldxr	w8, [x9]
	add	w8, w8, #1
	stxr	w10, w8, [x9]
	cbnz	w10, .Ltmp1

	//NO_APP
	ret
.Lfunc_end3:
	.size	dfa_get_dev_cmd_tag, .Lfunc_end3-dfa_get_dev_cmd_tag
                                        // -- End function
	.globl	dfa_put_dev_cmd_tag             // -- Begin function dfa_put_dev_cmd_tag
	.p2align	2
	.type	dfa_put_dev_cmd_tag,@function
dfa_put_dev_cmd_tag:                    // @dfa_put_dev_cmd_tag
// %bb.0:
	mov	w8, #7040
	mov	w9, #32768
	add	x8, x0, x8
	mov	w10, #7072
	//APP
	// atomic64_fetch_andnot_release
	prfm	pstl1strm, [x8]
.Ltmp2:
	ldxr	x12, [x8]
	bic	x13, x12, x9
	stlxr	w14, x13, [x8]
	cbnz	w14, .Ltmp2

	//NO_APP
	add	x10, x0, x10
	mov	w11, #1
	//APP
	// atomic_sub
	prfm	pstl1strm, [x10]
.Ltmp3:
	ldxr	w8, [x10]
	sub	w8, w8, w11
	stxr	w9, w8, [x10]
	cbnz	w9, .Ltmp3

	//NO_APP
	ret
.Lfunc_end4:
	.size	dfa_put_dev_cmd_tag, .Lfunc_end4-dfa_put_dev_cmd_tag
                                        // -- End function
	.globl	dfa_inflt_req_clear             // -- Begin function dfa_inflt_req_clear
	.p2align	2
	.type	dfa_inflt_req_clear,@function
dfa_inflt_req_clear:                    // @dfa_inflt_req_clear
// %bb.0:
	cmp	w1, #15
	b.eq	.LBB5_4
// %bb.1:
	stp	x29, x30, [sp, #-32]!           // 16-byte Folded Spill
	add	w8, w1, #63
	cmp	w1, #0
	csel	w8, w8, w1, lt
	mov	w11, #7040
	asr	w9, w8, #6
	and	w8, w8, #0xffffffc0
	sub	w8, w1, w8
	str	x19, [sp, #16]                  // 8-byte Folded Spill
	add	x10, x0, w9, sxtw #3
	lsr	w9, w8, #6
	add	x10, x10, x11
	mov	w12, #1
	mov	w2, w1
	mov	x19, x0
	ldr	x11, [x10, w9, uxtw #3]
	lsl	x8, x12, x8
	mov	x29, sp
	tst	x11, x8
	b.eq	.LBB5_5
// %bb.2:
	add	x9, x10, x9, lsl #3
	//APP
	// atomic64_fetch_andnot
	prfm	pstl1strm, [x9]
.Ltmp4:
	ldxr	x10, [x9]
	bic	x11, x10, x8
	stlxr	w12, x11, [x9]
	cbnz	w12, .Ltmp4
	dmb	ish
	//NO_APP
	tst	x10, x8
	b.eq	.LBB5_5
.LBB5_3:
	mov	w8, #7072
	mov	w9, #1
	add	x8, x19, x8
	//APP
	// atomic_sub
	prfm	pstl1strm, [x8]
.Ltmp5:
	ldxr	w10, [x8]
	sub	w10, w10, w9
	stxr	w11, w10, [x8]
	cbnz	w11, .Ltmp5

	//NO_APP
	ldr	x19, [sp, #16]                  // 8-byte Folded Reload
	ldp	x29, x30, [sp], #32             // 16-byte Folded Reload
.LBB5_4:
	ret
.LBB5_5:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.12
	add	x1, x1, :lo12:.L.str.12
	bl	_dev_err
	b	.LBB5_3
.Lfunc_end5:
	.size	dfa_inflt_req_clear, .Lfunc_end5-dfa_inflt_req_clear
                                        // -- End function
	.globl	dfa_mcq_stop_run_sq             // -- Begin function dfa_mcq_stop_run_sq
	.p2align	2
	.type	dfa_mcq_stop_run_sq,@function
dfa_mcq_stop_run_sq:                    // @dfa_mcq_stop_run_sq
// %bb.0:
	stp	x29, x30, [sp, #-64]!           // 16-byte Folded Spill
	ldr	x8, [x0, #48]
	stp	x24, x23, [sp, #16]             // 16-byte Folded Spill
	ldr	w9, [x0, #40]
	mov	w23, #20
	stp	x22, x21, [sp, #32]             // 16-byte Folded Spill
	mov	x29, sp
	stp	x20, x19, [sp, #48]             // 16-byte Folded Spill
	mov	w19, w1
	madd	x8, x9, x23, x8
	mov	w9, #5152
	mov	x21, x0
	and	w20, w1, #0x1
	add	x8, x8, x9
	//APP
.Ltmp6:
	ldr	w8, [x8]
.Ltmp7:
	.section	.altinstructions,"a",@progbits
.Ltmp8:
	.word	.Ltmp6-.Ltmp8
.Ltmp10:
	.word	.Ltmp9-.Ltmp10
	.hword	1
	.byte	.Ltmp7-.Ltmp6
	.byte	.Ltmp11-.Ltmp9
	.text

	.text	1
.Ltmp9:
	ldar	w8, [x8]
.Ltmp11:
.Ltmp12:
.org (.Ltmp12-(.Ltmp11-.Ltmp9))+(.Ltmp7-.Ltmp6), 0
.Ltmp13:
.org (.Ltmp13-(.Ltmp7-.Ltmp6))+(.Ltmp11-.Ltmp9), 0
	.text


	//NO_APP
	mov	w10, w8
	//APP
	dmb	oshld
	//NO_APP
	bfxil	w8, w1, #0, #1
	//APP
	eor	x10, x10, x10
.Ltmp14:
	cbnz	x10, .Ltmp14
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x10, [x0, #48]
	ldr	w11, [x0, #40]
	nop
	madd	x10, x11, x23, x10
	add	x9, x10, x9
	//APP
	str	w8, [x9]
	//NO_APP
	bl	ktime_get
	mov	x22, x0
	bl	ktime_get
	ldr	x8, [x21, #48]
	ldr	w9, [x21, #40]
	nop
	madd	x8, x9, x23, x8
	mov	w9, #5160
	add	x8, x8, x9
	//APP
.Ltmp15:
	ldr	w8, [x8]
.Ltmp16:
	.section	.altinstructions,"a",@progbits
.Ltmp17:
	.word	.Ltmp15-.Ltmp17
.Ltmp19:
	.word	.Ltmp18-.Ltmp19
	.hword	1
	.byte	.Ltmp16-.Ltmp15
	.byte	.Ltmp20-.Ltmp18
	.text

	.text	1
.Ltmp18:
	ldar	w8, [x8]
.Ltmp20:
.Ltmp21:
.org (.Ltmp21-(.Ltmp20-.Ltmp18))+(.Ltmp16-.Ltmp15), 0
.Ltmp22:
.org (.Ltmp22-(.Ltmp16-.Ltmp15))+(.Ltmp20-.Ltmp18), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w8, w8
	//APP
	eor	x9, x8, x8
.Ltmp23:
	cbnz	x9, .Ltmp23
	//NO_APP
	tbnz	w8, #0, .LBB6_4
// %bb.1:
	mov	w8, #51712
	mov	w23, #20
	movk	w8, #15258, lsl #16
	mov	w24, #5160
	add	x22, x22, x8
.LBB6_2:                                // =>This Inner Loop Header: Depth=1
	cmp	x0, x22
	b.gt	.LBB6_9
// %bb.3:                               //   in Loop: Header=BB6_2 Depth=1
	mov	w0, #42950
	bl	__const_udelay
	bl	ktime_get
	ldr	x8, [x21, #48]
	ldr	w9, [x21, #40]
	nop
	madd	x8, x9, x23, x8
	add	x8, x8, x24
	//APP
.Ltmp24:
	ldr	w8, [x8]
.Ltmp25:
	.section	.altinstructions,"a",@progbits
.Ltmp26:
	.word	.Ltmp24-.Ltmp26
.Ltmp28:
	.word	.Ltmp27-.Ltmp28
	.hword	1
	.byte	.Ltmp25-.Ltmp24
	.byte	.Ltmp29-.Ltmp27
	.text

	.text	1
.Ltmp27:
	ldar	w8, [x8]
.Ltmp29:
.Ltmp30:
.org (.Ltmp30-(.Ltmp29-.Ltmp27))+(.Ltmp25-.Ltmp24), 0
.Ltmp31:
.org (.Ltmp31-(.Ltmp25-.Ltmp24))+(.Ltmp29-.Ltmp27), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w8, w8
	//APP
	eor	x9, x8, x8
.Ltmp32:
	cbnz	x9, .Ltmp32
	//NO_APP
	tbz	w8, #0, .LBB6_2
.LBB6_4:
	//APP
	dmb	oshst
	//NO_APP
	mov	w10, #20
	ldr	x8, [x21, #48]
	ldr	w9, [x21, #40]
	nop
	madd	x8, x9, x10, x8
	mov	w9, #5160
	add	x8, x8, x9
	mov	w9, #1
	//APP
	str	w9, [x8]
	//NO_APP
	ldr	x8, [x21, #48]
	ldr	w9, [x21, #40]
	nop
	madd	x8, x9, x10, x8
	mov	w9, #5156
	add	x8, x8, x9
	//APP
.Ltmp33:
	ldr	w3, [x8]
.Ltmp34:
	.section	.altinstructions,"a",@progbits
.Ltmp35:
	.word	.Ltmp33-.Ltmp35
.Ltmp37:
	.word	.Ltmp36-.Ltmp37
	.hword	1
	.byte	.Ltmp34-.Ltmp33
	.byte	.Ltmp38-.Ltmp36
	.text

	.text	1
.Ltmp36:
	ldar	w3, [x8]
.Ltmp38:
.Ltmp39:
.org (.Ltmp39-(.Ltmp38-.Ltmp36))+(.Ltmp34-.Ltmp33), 0
.Ltmp40:
.org (.Ltmp40-(.Ltmp34-.Ltmp33))+(.Ltmp38-.Ltmp36), 0
	.text


	//NO_APP
	mov	w8, w3
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp41:
	cbnz	x8, .Ltmp41
	//NO_APP
	tbz	w19, #0, .LBB6_6
// %bb.5:
	cmp	w3, #1
	b.eq	.LBB6_8
.LBB6_6:
	tbnz	w19, #0, .LBB6_10
// %bb.7:
	cbnz	w3, .LBB6_10
.LBB6_8:
	ldp	x20, x19, [sp, #48]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #32]             // 16-byte Folded Reload
	ldp	x24, x23, [sp, #16]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #64             // 16-byte Folded Reload
	ret
.LBB6_9:
	ldr	w3, [x21, #36]
	adrp	x0, .L.str.13
	add	x0, x0, :lo12:.L.str.13
	adrp	x1, .L__func__.dfa_mcq_stop_run_sq
	add	x1, x1, :lo12:.L__func__.dfa_mcq_stop_run_sq
	b	.LBB6_11
.LBB6_10:
	adrp	x0, .L.str.77
	add	x0, x0, :lo12:.L.str.77
	adrp	x1, .L__func__.dfa_mcq_check_stop_run_status
	add	x1, x1, :lo12:.L__func__.dfa_mcq_check_stop_run_status
.LBB6_11:
	mov	w2, w20
	bl	printk
	b	.LBB6_8
.Lfunc_end6:
	.size	dfa_mcq_stop_run_sq, .Lfunc_end6-dfa_mcq_stop_run_sq
                                        // -- End function
	.globl	dfa_mcq_transfer_req_compl_all  // -- Begin function dfa_mcq_transfer_req_compl_all
	.p2align	2
	.type	dfa_mcq_transfer_req_compl_all,@function
dfa_mcq_transfer_req_compl_all:         // @dfa_mcq_transfer_req_compl_all
// %bb.0:
	sub	sp, sp, #128
	stp	x20, x19, [sp, #112]            // 16-byte Folded Spill
	mov	x19, x0
	stp	x29, x30, [sp, #32]             // 16-byte Folded Spill
	ldr	x0, [x0, #96]
	stp	x28, x27, [sp, #48]             // 16-byte Folded Spill
	adrp	x1, .L.str.14
	add	x1, x1, :lo12:.L.str.14
	stp	x26, x25, [sp, #64]             // 16-byte Folded Spill
	stp	x24, x23, [sp, #80]             // 16-byte Folded Spill
	add	x29, sp, #32
	stp	x22, x21, [sp, #96]             // 16-byte Folded Spill
	ldr	w2, [x19, #7072]
	bl	_dev_err
	mov	w8, #7040
	ldrsw	x1, [x19, #264]
	add	x0, x19, x8
	mov	x2, xzr
	stur	x0, [x29, #-8]                  // 8-byte Folded Spill
	bl	find_next_bit
	ldr	w8, [x19, #264]
	mov	x21, x0
	cmp	w8, w21
	b.hi	.LBB7_2
.LBB7_1:
	ldp	x20, x19, [sp, #112]            // 16-byte Folded Reload
	ldp	x22, x21, [sp, #96]             // 16-byte Folded Reload
	ldp	x24, x23, [sp, #80]             // 16-byte Folded Reload
	ldp	x26, x25, [sp, #64]             // 16-byte Folded Reload
	ldp	x28, x27, [sp, #48]             // 16-byte Folded Reload
	ldp	x29, x30, [sp, #32]             // 16-byte Folded Reload
	add	sp, sp, #128
	ret
.LBB7_2:
	mov	w8, #7040
	adrp	x26, :got:liblinux_thread_offset
	adrp	x27, :got:liblinux_thread_key
	add	x8, x19, x8
	//APP
	mrs	x25, TPIDR_EL0

	//NO_APP
	ldr	x26, [x26, :got_lo12:liblinux_thread_offset]
	ldr	x27, [x27, :got_lo12:liblinux_thread_key]
	str	x8, [sp, #16]                   // 8-byte Folded Spill
	mov	w8, #7072
	add	x8, x19, x8
	str	x8, [sp, #8]                    // 8-byte Folded Spill
.LBB7_3:                                // =>This Inner Loop Header: Depth=1
	ldr	w8, [x26]
	and	x10, x21, #0xffffffff
	ldr	x9, [x19, #224]
	mov	w12, #216
	ldr	w11, [x27]
	mov	x20, x26
	sub	x8, x25, x8
	mov	x22, x27
	madd	x26, x10, x12, x9
	ldr	x24, [x19, #7024]
	ldr	x8, [x8]
	ldr	x28, [x8, x11, lsl #3]
	ldrb	w27, [x26, #193]
	ldr	w8, [x28, #32]
	cbnz	w8, .LBB7_5
// %bb.4:                               //   in Loop: Header=BB7_3 Depth=1
	ldrb	w8, [x28, #84]
	cbz	w8, .LBB7_17
.LBB7_5:                                //   in Loop: Header=BB7_3 Depth=1
	ldr	w9, [x28, #32]
	mov	w10, #80
	mov	x8, xzr
	madd	x27, x27, x10, x24
	add	w9, w9, #1
	add	x24, x27, #68
	str	w9, [x28, #32]
	mov	w28, #1
	//APP
	//NO_APP
	//APP
	prfm	pstl1strm, [x24]
.Ltmp42:
	ldaxr	w1, [x24]
	eor	w9, w1, w8
	cbnz	w9, .Ltmp43
	stxr	w9, w28, [x24]
	cbnz	w9, .Ltmp42

.Ltmp43:
	//NO_APP
	cbnz	w1, .LBB7_22
.LBB7_6:                                //   in Loop: Header=BB7_3 Depth=1
	ldr	x8, [x27, #48]
	ldr	w9, [x27, #36]
	add	x8, x8, x9, lsl #3
	add	x8, x8, #3556
	//APP
.Ltmp44:
	ldr	w8, [x8]
.Ltmp45:
	.section	.altinstructions,"a",@progbits
.Ltmp46:
	.word	.Ltmp44-.Ltmp46
.Ltmp48:
	.word	.Ltmp47-.Ltmp48
	.hword	1
	.byte	.Ltmp45-.Ltmp44
	.byte	.Ltmp49-.Ltmp47
	.text

	.text	1
.Ltmp47:
	ldar	w8, [x8]
.Ltmp49:
.Ltmp50:
.org (.Ltmp50-(.Ltmp49-.Ltmp47))+(.Ltmp45-.Ltmp44), 0
.Ltmp51:
.org (.Ltmp51-(.Ltmp45-.Ltmp44))+(.Ltmp49-.Ltmp47), 0
	.text


	//NO_APP
	mov	w9, w8
	lsr	w8, w8, #5
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x9, x9, x9
.Ltmp52:
	cbnz	x9, .Ltmp52
	//NO_APP
	str	w8, [x27, #72]
	ldrb	w8, [x26, #195]
	cmp	w8, #15
	b.ne	.LBB7_21
.LBB7_7:                                //   in Loop: Header=BB7_3 Depth=1
	mov	x0, x19
	mov	w1, w21
	bl	dfa_compl_one_task
	ldr	w8, [x27, #76]
	cmp	w21, #15
	ldr	w9, [x27, #32]
	add	w8, w8, #1
	udiv	w10, w8, w9
	msub	w8, w10, w9, w8
	str	w8, [x27, #76]
	//APP
	dmb	oshst
	//NO_APP
	ldr	x8, [x27, #48]
	ldr	w9, [x27, #36]
	ldr	w10, [x27, #76]
	add	x8, x8, x9, lsl #3
	lsl	w9, w10, #5
	add	x8, x8, #3552
	//APP
	str	w9, [x8]
	//NO_APP
	b.eq	.LBB7_16
// %bb.8:                               //   in Loop: Header=BB7_3 Depth=1
	add	w8, w21, #63
	cmp	w21, #0
	csel	w8, w8, w21, lt
	mov	w11, #7040
	asr	w9, w8, #6
	and	w8, w8, #0xffffffc0
	sub	w8, w21, w8
	add	x10, x19, w9, sxtw #3
	lsr	w9, w8, #6
	add	x10, x10, x11
	lsl	x8, x28, x8
	ldr	x11, [x10, w9, uxtw #3]
	tst	x11, x8
	b.eq	.LBB7_20
// %bb.9:                               //   in Loop: Header=BB7_3 Depth=1
	add	x9, x10, x9, lsl #3
	//APP
	// atomic64_fetch_andnot
	prfm	pstl1strm, [x9]
.Ltmp53:
	ldxr	x10, [x9]
	bic	x11, x10, x8
	stlxr	w12, x11, [x9]
	cbnz	w12, .Ltmp53
	dmb	ish
	//NO_APP
	tst	x10, x8
	b.eq	.LBB7_20
.LBB7_10:                               //   in Loop: Header=BB7_3 Depth=1
	mov	w8, #7072
	add	x8, x19, x8
	//APP
	// atomic_sub
	prfm	pstl1strm, [x8]
.Ltmp54:
	ldxr	w9, [x8]
	sub	w9, w9, w28
	stxr	w10, w9, [x8]
	cbnz	w10, .Ltmp54

	//NO_APP
.LBB7_11:                               //   in Loop: Header=BB7_3 Depth=1
	mov	w8, wzr
	//APP
	stlrb	w8, [x24]
	//NO_APP
	//APP
	//NO_APP
	mov	x26, x20
	ldr	w8, [x20]
	mov	x27, x22
	ldr	w9, [x22]
	sub	x8, x25, x8
	ldr	x8, [x8]
	ldr	x24, [x8, x9, lsl #3]
	ldr	w8, [x24, #32]
	sub	w8, w8, #1
	str	w8, [x24, #32]
	ldr	w8, [x24, #32]
	cbnz	w8, .LBB7_14
// %bb.12:                              //   in Loop: Header=BB7_3 Depth=1
	ldrb	w8, [x24, #84]
	cbnz	w8, .LBB7_14
// %bb.13:                              //   in Loop: Header=BB7_3 Depth=1
	ldr	x23, [x24, #24]
	adrp	x10, :got:__preempt_disabled
	ldr	w8, [x23, #8]
	ldr	x10, [x10, :got_lo12:__preempt_disabled]
	lsl	x8, x8, #2
	ldr	w9, [x10, x8]
	sub	w9, w9, #1
	str	w9, [x10, x8]
	str	w28, [x23, #4]
	ldr	w8, [x23, #12]
	cbnz	w8, .LBB7_24
.LBB7_14:                               //   in Loop: Header=BB7_3 Depth=1
	ldr	w8, [x24, #32]
	tbnz	w8, #31, .LBB7_25
.LBB7_15:                               //   in Loop: Header=BB7_3 Depth=1
	ldrsw	x1, [x19, #264]
	add	w2, w21, #1
	ldur	x0, [x29, #-8]                  // 8-byte Folded Reload
	bl	find_next_bit
	ldr	w8, [x19, #264]
	mov	x21, x0
	cmp	w8, w21
	b.hi	.LBB7_3
	b	.LBB7_1
.LBB7_16:                               //   in Loop: Header=BB7_3 Depth=1
	ldr	x11, [sp, #16]                  // 8-byte Folded Reload
	mov	w12, #32768
	//APP
	// atomic64_fetch_andnot_release
	prfm	pstl1strm, [x11]
.Ltmp55:
	ldxr	x8, [x11]
	bic	x9, x8, x12
	stlxr	w10, x9, [x11]
	cbnz	w10, .Ltmp55

	//NO_APP
	ldr	x10, [sp, #8]                   // 8-byte Folded Reload
	//APP
	// atomic_sub
	prfm	pstl1strm, [x10]
.Ltmp56:
	ldxr	w8, [x10]
	sub	w8, w8, w28
	stxr	w9, w8, [x10]
	cbnz	w9, .Ltmp56

	//NO_APP
	b	.LBB7_11
.LBB7_17:                               //   in Loop: Header=BB7_3 Depth=1
	ldr	x23, [x28, #24]
	mov	w8, #2
	adrp	x9, :got:__preempt_disabled
	str	w8, [x23, #4]
	//APP
	//NO_APP
	ldr	w8, [x23, #8]
	ldr	x9, [x9, :got_lo12:__preempt_disabled]
	ldr	w9, [x9, x8, lsl #2]
	cmp	w9, #0
	b.gt	.LBB7_23
// %bb.18:                              //   in Loop: Header=BB7_3 Depth=1
	ldr	w10, [x23, #4]
	cmp	w10, #3
	b.eq	.LBB7_23
.LBB7_19:                               //   in Loop: Header=BB7_3 Depth=1
	adrp	x10, :got:__preempt_disabled
	add	w9, w9, #1
	ldr	x10, [x10, :got_lo12:__preempt_disabled]
	str	w9, [x10, x8, lsl #2]
	b	.LBB7_5
.LBB7_20:                               //   in Loop: Header=BB7_3 Depth=1
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.12
	add	x1, x1, :lo12:.L.str.12
	mov	w2, w21
	bl	_dev_err
	b	.LBB7_10
.LBB7_21:                               //   in Loop: Header=BB7_3 Depth=1
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.15
	add	x1, x1, :lo12:.L.str.15
	ldrb	w3, [x26, #128]
	mov	w2, w21
	bl	_dev_err
	mov	w8, #15
	strb	w8, [x26, #195]
	b	.LBB7_7
.LBB7_22:                               //   in Loop: Header=BB7_3 Depth=1
	mov	x0, x24
	bl	queued_spin_lock_slowpath
	b	.LBB7_6
.LBB7_23:                               //   in Loop: Header=BB7_3 Depth=1
	bl	kernel_thread_preempt_disable
	adrp	x9, :got:__preempt_disabled
	ldr	w8, [x23, #8]
	ldr	x9, [x9, :got_lo12:__preempt_disabled]
	ldr	w9, [x9, x8, lsl #2]
	b	.LBB7_19
.LBB7_24:                               //   in Loop: Header=BB7_3 Depth=1
	bl	kernel_thread_preempt_enable
	str	wzr, [x23, #12]
	ldr	w8, [x24, #32]
	tbz	w8, #31, .LBB7_15
.LBB7_25:
	adrp	x0, .L.str.9
	add	x0, x0, :lo12:.L.str.9
	adrp	x1, .L.str.10
	add	x1, x1, :lo12:.L.str.10
	adrp	x3, .L__func__.__preempt_count_sub
	add	x3, x3, :lo12:.L__func__.__preempt_count_sub
	mov	w2, #112
	str	wzr, [x24, #32]
	bl	printk
	adrp	x0, .L.str.11
	add	x0, x0, :lo12:.L.str.11
	bl	panic
.Lfunc_end7:
	.size	dfa_mcq_transfer_req_compl_all, .Lfunc_end7-dfa_mcq_transfer_req_compl_all
                                        // -- End function
	.p2align	2                               // -- Begin function dfa_compl_one_task
	.type	dfa_compl_one_task,@function
dfa_compl_one_task:                     // @dfa_compl_one_task
// %bb.0:
	stp	x29, x30, [sp, #-64]!           // 16-byte Folded Spill
	stp	x24, x23, [sp, #16]             // 16-byte Folded Spill
	mov	x29, sp
	stp	x22, x21, [sp, #32]             // 16-byte Folded Spill
	mov	w23, w1
	stp	x20, x19, [sp, #48]             // 16-byte Folded Spill
	mov	x19, x0
	bl	ktime_get
	ldr	x8, [x19, #224]
	mov	w9, #216
	smaddl	x20, w23, w9, x8
	ldr	x22, [x20, #72]
	ldr	w8, [x20, #100]
	cmp	x22, #0
	and	w9, w8, #0xfffffffe
	ccmp	w9, #2, #4, ne
	b.ne	.LBB8_6
// %bb.1:
	mov	x21, x0
	sub	w8, w8, #1
	cmp	w8, #1
	b.hi	.LBB8_4
// %bb.2:
	ldr	x0, [x19, #3920]
	cbz	x0, .LBB8_4
// %bb.3:
	bl	complete
.LBB8_4:
	mov	w23, wzr
.LBB8_5:
	mov	x0, x19
	bl	ufs_cmd_compl_status_sync
	mov	x0, x19
	mov	x1, x20
	mov	w2, w23
	mov	x3, x21
	bl	ufshcd_complete_time
	ldp	x20, x19, [sp, #48]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #32]             // 16-byte Folded Reload
	ldp	x24, x23, [sp, #16]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #64             // 16-byte Folded Reload
	ret
.LBB8_6:
	mov	x21, x0
	mov	x0, x19
	mov	w1, w23
	bl	dsm_ufs_restore_cmd
	mov	x0, x19
	mov	x1, x20
	mov	w2, #1
	bl	dfa_transfer_rsp_status
	mov	w24, w0
	mov	x0, x22
	bl	scsi_dma_unmap
	str	w24, [x22, #368]
	ldrb	w8, [x20, #177]
	cbz	w8, .LBB8_8
.LBB8_7:
	mov	x0, x19
	mov	x1, x20
	str	xzr, [x20, #72]
	bl	ufstt_unprep_fn
	ldr	x8, [x22, #224]
	mov	x0, x22
	ldrb	w23, [x8]
	ldr	x8, [x22, #288]
	blr	x8
	b	.LBB8_5
.LBB8_8:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.78
	add	x1, x1, :lo12:.L.str.78
	mov	w2, w23
	bl	_dev_err
	b	.LBB8_7
.Lfunc_end8:
	.size	dfa_compl_one_task, .Lfunc_end8-dfa_compl_one_task
                                        // -- End function
	.globl	dfa_enable_ooo_mode             // -- Begin function dfa_enable_ooo_mode
	.p2align	2
	.type	dfa_enable_ooo_mode,@function
dfa_enable_ooo_mode:                    // @dfa_enable_ooo_mode
// %bb.0:
	stp	x29, x30, [sp, #-16]!           // 16-byte Folded Spill
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #13776
	ldr	x8, [x0]
	mov	x29, sp
	add	x8, x8, x9
	//APP
.Ltmp57:
	ldr	w8, [x8]
.Ltmp58:
	.section	.altinstructions,"a",@progbits
.Ltmp59:
	.word	.Ltmp57-.Ltmp59
.Ltmp61:
	.word	.Ltmp60-.Ltmp61
	.hword	1
	.byte	.Ltmp58-.Ltmp57
	.byte	.Ltmp62-.Ltmp60
	.text

	.text	1
.Ltmp60:
	ldar	w8, [x8]
.Ltmp62:
.Ltmp63:
.org (.Ltmp63-(.Ltmp62-.Ltmp60))+(.Ltmp58-.Ltmp57), 0
.Ltmp64:
.org (.Ltmp64-(.Ltmp58-.Ltmp57))+(.Ltmp62-.Ltmp60), 0
	.text


	//NO_APP
	mov	w9, w8
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x9, x9, x9
.Ltmp65:
	cbnz	x9, .Ltmp65
	//NO_APP
	tbz	w1, #0, .LBB9_2
// %bb.1:
	ldr	x9, [x0]
	mov	w10, #13776
	orr	w8, w8, #0x100
	add	x9, x9, x10
	//APP
	str	w8, [x9]
	//NO_APP
	b	.LBB9_3
.LBB9_2:
	ldr	x9, [x0]
	mov	w10, #13776
	and	w8, w8, #0xfffffeff
	add	x9, x9, x10
	//APP
	str	w8, [x9]
	//NO_APP
.LBB9_3:
	ldr	x8, [x0]
	mov	w9, #13776
	ldr	x0, [x0, #96]
	adrp	x1, .L.str.16
	add	x1, x1, :lo12:.L.str.16
	add	x8, x8, x9
	//APP
.Ltmp66:
	ldr	w2, [x8]
.Ltmp67:
	.section	.altinstructions,"a",@progbits
.Ltmp68:
	.word	.Ltmp66-.Ltmp68
.Ltmp70:
	.word	.Ltmp69-.Ltmp70
	.hword	1
	.byte	.Ltmp67-.Ltmp66
	.byte	.Ltmp71-.Ltmp69
	.text

	.text	1
.Ltmp69:
	ldar	w2, [x8]
.Ltmp71:
.Ltmp72:
.org (.Ltmp72-(.Ltmp71-.Ltmp69))+(.Ltmp67-.Ltmp66), 0
.Ltmp73:
.org (.Ltmp73-(.Ltmp67-.Ltmp66))+(.Ltmp71-.Ltmp69), 0
	.text


	//NO_APP
	mov	w8, w2
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp74:
	cbnz	x8, .Ltmp74
	//NO_APP
	bl	_dev_err
	ldp	x29, x30, [sp], #16             // 16-byte Folded Reload
	ret
.Lfunc_end9:
	.size	dfa_enable_ooo_mode, .Lfunc_end9-dfa_enable_ooo_mode
                                        // -- End function
	.globl	dfa_mcq_make_queues_operational // -- Begin function dfa_mcq_make_queues_operational
	.p2align	2
	.type	dfa_mcq_make_queues_operational,@function
dfa_mcq_make_queues_operational:        // @dfa_mcq_make_queues_operational
// %bb.0:
	ldr	w8, [x0, #7032]
	cbz	w8, .LBB10_3
// %bb.1:
	mov	w11, #26215
	mov	w8, wzr
	mov	w9, wzr
	mov	w10, #80
	movk	w11, #26214, lsl #16
	mov	w12, #28
	mov	w13, #4192
	mov	w14, #1
	mov	w15, #4196
.LBB10_2:                               // =>This Inner Loop Header: Depth=1
	smull	x16, w9, w11
	ldr	x17, [x0, #7024]
	lsr	x1, x16, #32
	lsr	x16, x16, #63
	smaddl	x17, w9, w10, x17
	add	w16, w16, w1, asr #1
	add	w16, w16, w16, lsl #2
	sub	w16, w9, w16
	stp	w9, w16, [x17, #36]
	ldr	w16, [x17, #32]
	//APP
	dmb	oshst
	//NO_APP
	ldr	x1, [x0, #7016]
	ldr	w2, [x17, #8]
	nop
	smaddl	x1, w9, w12, x1
	add	x1, x1, #36
	//APP
	str	w2, [x1]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x1, [x0, #7016]
	ldr	w2, [x17, #12]
	nop
	smaddl	x1, w9, w12, x1
	add	x1, x1, #40
	//APP
	str	w2, [x1]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x1, [x0, #7016]
	ldr	w2, [x17, #24]
	nop
	smaddl	x1, w9, w12, x1
	add	x1, x1, #52
	//APP
	str	w2, [x1]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	mov	w2, #-2147483648
	ldr	x1, [x0, #7016]
	ldr	w17, [x17, #28]
	nop
	smaddl	x1, w9, w12, x1
	add	x1, x1, #56
	//APP
	str	w17, [x1]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	sxtw	x17, w9
	ldr	x1, [x0, #7016]
	add	x17, x17, w9, sxtw #1
	lsl	x17, x17, #2
	add	x1, x1, x17
	add	x1, x1, x13
	//APP
	str	w14, [x1]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x1, [x0, #7016]
	add	x17, x1, x17
	lsl	w1, w16, #3
	add	x17, x17, x15
	//APP
	str	w14, [x17]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	sub	w1, w1, #1
	ldr	x17, [x0, #7016]
	lsl	w16, w16, #4
	sub	w16, w16, #1
	bfxil	w2, w1, #0, #16
	and	w16, w16, #0xffff
	smaddl	x17, w9, w12, x17
	orr	w16, w16, w8
	orr	w16, w16, #0x80000000
	add	w8, w8, #16, lsl #12            // =65536
	add	x17, x17, #48
	//APP
	str	w2, [x17]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x17, [x0, #7016]
	nop
	smaddl	x17, w9, w12, x17
	add	w9, w9, #1
	add	x17, x17, #32
	//APP
	str	w16, [x17]
	//NO_APP
	ldr	w16, [x0, #7032]
	cmp	w9, w16
	b.lo	.LBB10_2
.LBB10_3:
	ret
.Lfunc_end10:
	.size	dfa_mcq_make_queues_operational, .Lfunc_end10-dfa_mcq_make_queues_operational
                                        // -- End function
	.globl	dfa_is_need_report_bit_error    // -- Begin function dfa_is_need_report_bit_error
	.p2align	2
	.type	dfa_is_need_report_bit_error,@function
dfa_is_need_report_bit_error:           // @dfa_is_need_report_bit_error
// %bb.0:
	stp	x29, x30, [sp, #-48]!           // 16-byte Folded Spill
	stp	x20, x19, [sp, #32]             // 16-byte Folded Spill
	adrp	x20, .L__func__.dfa_is_need_report_bit_error
	add	x20, x20, :lo12:.L__func__.dfa_is_need_report_bit_error
	mov	x19, x0
	ldr	x0, [x0, #96]
	adrp	x1, .L.str.17
	add	x1, x1, :lo12:.L.str.17
	mov	x2, x20
	stp	x22, x21, [sp, #16]             // 16-byte Folded Spill
	mov	x29, sp
	bl	_dev_err
	mov	w8, #6444
	ldr	x0, [x19, #96]
	add	x21, x19, x8
	adrp	x1, .L.str.18
	add	x1, x1, :lo12:.L.str.18
	mov	x2, x20
	mov	x3, x21
	bl	_dev_err
	adrp	x1, .L.str.19
	add	x1, x1, :lo12:.L.str.19
	mov	x0, x21
	mov	w2, #12
	bl	strncmp
	adrp	x20, .L.str.22
	add	x20, x20, :lo12:.L.str.22
	cbnz	w0, .LBB11_4
// %bb.1:
	adrp	x8, :got:kmalloc_caches
	mov	w1, #3264
	mov	w2, #8192
	ldr	x8, [x8, :got_lo12:kmalloc_caches]
	ldr	x0, [x8, #104]
	bl	kmem_cache_alloc_trace
	cbnz	x0, .LBB11_6
.LBB11_2:
	mov	w21, #1
.LBB11_3:
	mov	w0, w21
	ldp	x20, x19, [sp, #32]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #48             // 16-byte Folded Reload
	ret
.LBB11_4:
	mov	w21, #1
.LBB11_5:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.21
	add	x1, x1, :lo12:.L.str.21
	adrp	x2, .L__func__.dfa_is_need_report_bit_error
	add	x2, x2, :lo12:.L__func__.dfa_is_need_report_bit_error
	mov	x3, x20
	bl	_dev_err
	b	.LBB11_3
.LBB11_6:
	mov	w1, #8192
	mov	w2, wzr
	mov	w3, #8192
	mov	x21, x0
	bl	memset_s
	mov	x0, x19
	mov	x1, x21
	mov	w2, #8192
	bl	ufshcd_read_fsr
	cbnz	w0, .LBB11_8
// %bb.7:
	mov	x0, x21
	ldrb	w22, [x21, #32]
	bl	kfree
	tst	w22, #0xff
	adrp	x8, .L.str.23
	add	x8, x8, :lo12:.L.str.23
	cset	w21, ne
	csel	x20, x8, x20, eq
	b	.LBB11_5
.LBB11_8:
	mov	x0, x21
	bl	kfree
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.20
	add	x1, x1, :lo12:.L.str.20
	adrp	x2, .L__func__.dfa_is_need_report_bit_error
	add	x2, x2, :lo12:.L__func__.dfa_is_need_report_bit_error
	bl	_dev_err
	b	.LBB11_2
.Lfunc_end11:
	.size	dfa_is_need_report_bit_error, .Lfunc_end11-dfa_is_need_report_bit_error
                                        // -- End function
	.globl	dfa_dmd_handle                  // -- Begin function dfa_dmd_handle
	.p2align	2
	.type	dfa_dmd_handle,@function
dfa_dmd_handle:                         // @dfa_dmd_handle
// %bb.0:
	stp	x29, x30, [sp, #-32]!           // 16-byte Folded Spill
	stp	x20, x19, [sp, #16]             // 16-byte Folded Spill
	mov	x19, x0
	ldr	x0, [x0, #96]
	adrp	x1, .L.str.24
	add	x1, x1, :lo12:.L.str.24
	adrp	x2, .L__func__.dfa_dmd_handle
	add	x2, x2, :lo12:.L__func__.dfa_dmd_handle
	ldr	w3, [x19, #7084]
	mov	w20, #18242
	mov	x29, sp
	movk	w20, #14160, lsl #16
	bl	_dev_err
	mov	w8, #7088
	ldr	w3, [x19, #7084]
	ldrb	w8, [x19, x8]
	cbnz	w8, .LBB12_8
.LBB12_1:
	tbnz	w3, #2, .LBB12_14
// %bb.2:
	tbnz	w3, #4, .LBB12_15
.LBB12_3:
	tbnz	w3, #7, .LBB12_16
.LBB12_4:
	tbnz	w3, #11, .LBB12_17
.LBB12_5:
	tbz	w3, #17, .LBB12_7
.LBB12_6:
	mov	x0, x19
	bl	dsm_ufs_updata_ice_info
	ldr	w8, [x19, #7084]
	eor	w3, w8, #0x20000
	str	w3, [x19, #7084]
.LBB12_7:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.25
	add	x1, x1, :lo12:.L.str.25
	adrp	x2, .L__func__.dfa_dmd_handle
	add	x2, x2, :lo12:.L__func__.dfa_dmd_handle
	bl	_dev_err
	ldp	x20, x19, [sp, #16]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #32             // 16-byte Folded Reload
	ret
.LBB12_8:
	tbnz	w3, #3, .LBB12_18
// %bb.9:
	tbnz	w3, #5, .LBB12_19
.LBB12_10:
	tbnz	w3, #6, .LBB12_20
.LBB12_11:
	tbnz	w3, #12, .LBB12_21
.LBB12_12:
	tbz	w3, #16, .LBB12_1
.LBB12_13:
	add	w1, w20, #14
	mov	x0, x19
	bl	dsm_ufs_update_error_info
	ldr	w8, [x19, #7084]
	eor	w3, w8, #0x10000
	str	w3, [x19, #7084]
	b	.LBB12_1
.LBB12_14:
	mov	w1, #18242
	mov	x0, x19
	movk	w1, #14160, lsl #16
	bl	dsm_ufs_update_error_info
	ldr	w8, [x19, #7084]
	eor	w3, w8, #0x4
	str	w3, [x19, #7084]
	tbz	w3, #4, .LBB12_3
.LBB12_15:
	add	w1, w20, #2
	mov	x0, x19
	bl	dsm_ufs_update_error_info
	ldr	w8, [x19, #7084]
	eor	w3, w8, #0x10
	str	w3, [x19, #7084]
	tbz	w3, #7, .LBB12_4
.LBB12_16:
	add	w2, w20, #5
	mov	x0, x19
	mov	w1, #-1
	bl	dsm_ufs_update_upiu_info
	ldr	w8, [x19, #7084]
	eor	w3, w8, #0x80
	str	w3, [x19, #7084]
	tbz	w3, #11, .LBB12_5
.LBB12_17:
	add	w1, w20, #9
	mov	x0, x19
	bl	dsm_ufs_update_error_info
	ldr	w8, [x19, #7084]
	eor	w3, w8, #0x800
	str	w3, [x19, #7084]
	tbnz	w3, #17, .LBB12_6
	b	.LBB12_7
.LBB12_18:
	add	w1, w20, #1
	mov	x0, x19
	bl	dsm_ufs_update_uic_info
	ldr	w8, [x19, #7084]
	eor	w3, w8, #0x8
	str	w3, [x19, #7084]
	tbz	w3, #5, .LBB12_10
.LBB12_19:
	add	w1, w20, #3
	mov	x0, x19
	bl	dsm_ufs_update_error_info
	ldr	w8, [x19, #7084]
	eor	w3, w8, #0x20
	str	w3, [x19, #7084]
	tbz	w3, #6, .LBB12_11
.LBB12_20:
	add	w1, w20, #4
	mov	x0, x19
	bl	dsm_ufs_update_error_info
	ldr	w8, [x19, #7084]
	eor	w3, w8, #0x40
	str	w3, [x19, #7084]
	tbz	w3, #12, .LBB12_12
.LBB12_21:
	add	w1, w20, #10
	mov	x0, x19
	bl	dsm_ufs_update_error_info
	ldr	w8, [x19, #7084]
	eor	w3, w8, #0x1000
	str	w3, [x19, #7084]
	tbz	w3, #16, .LBB12_1
	b	.LBB12_13
.Lfunc_end12:
	.size	dfa_dmd_handle, .Lfunc_end12-dfa_dmd_handle
                                        // -- End function
	.globl	dfa_dsm_handler                 // -- Begin function dfa_dsm_handler
	.p2align	2
	.type	dfa_dsm_handler,@function
dfa_dsm_handler:                        // @dfa_dsm_handler
// %bb.0:
	stp	x29, x30, [sp, #-48]!           // 16-byte Folded Spill
	ldr	x8, [x0, #7008]
	str	x21, [sp, #16]                  // 8-byte Folded Spill
	stp	x20, x19, [sp, #32]             // 16-byte Folded Spill
	ldr	x21, [x0, #296]
	adrp	x20, .L__func__.dfa_dsm_handler
	add	x20, x20, :lo12:.L__func__.dfa_dsm_handler
	add	x8, x8, #540
	mov	x19, x0
	//APP
.Ltmp75:
	ldr	w8, [x8]
.Ltmp76:
	.section	.altinstructions,"a",@progbits
.Ltmp77:
	.word	.Ltmp75-.Ltmp77
.Ltmp79:
	.word	.Ltmp78-.Ltmp79
	.hword	1
	.byte	.Ltmp76-.Ltmp75
	.byte	.Ltmp80-.Ltmp78
	.text

	.text	1
.Ltmp78:
	ldar	w8, [x8]
.Ltmp80:
.Ltmp81:
.org (.Ltmp81-(.Ltmp80-.Ltmp78))+(.Ltmp76-.Ltmp75), 0
.Ltmp82:
.org (.Ltmp82-(.Ltmp76-.Ltmp75))+(.Ltmp80-.Ltmp78), 0
	.text


	//NO_APP
	mov	w9, w8
	//APP
	dmb	oshld
	//NO_APP
	adrp	x1, .L.str.26
	add	x1, x1, :lo12:.L.str.26
	//APP
	eor	x9, x9, x9
.Ltmp83:
	cbnz	x9, .Ltmp83
	//NO_APP
	ldr	w9, [x0, #7084]
	mov	x2, x20
	ldr	x0, [x0, #96]
	mov	x29, sp
	orr	w3, w9, w8
	str	w3, [x19, #7084]
	bl	_dev_err
	mov	w8, #7088
	adrp	x9, .L.str.22
	add	x9, x9, :lo12:.L.str.22
	adrp	x10, .L.str.23
	add	x10, x10, :lo12:.L.str.23
	ldr	x0, [x19, #96]
	ldrb	w8, [x19, x8]
	adrp	x1, .L.str.21
	add	x1, x1, :lo12:.L.str.21
	mov	x2, x20
	cmp	w8, #0
	csel	x3, x10, x9, eq
	bl	_dev_err
	ldr	x8, [x21, #80]
	adrp	x1, .L.str.27
	add	x1, x1, :lo12:.L.str.27
	ldr	x0, [x19, #96]
	add	x8, x8, #476
	//APP
.Ltmp84:
	ldr	w2, [x8]
.Ltmp85:
	.section	.altinstructions,"a",@progbits
.Ltmp86:
	.word	.Ltmp84-.Ltmp86
.Ltmp88:
	.word	.Ltmp87-.Ltmp88
	.hword	1
	.byte	.Ltmp85-.Ltmp84
	.byte	.Ltmp89-.Ltmp87
	.text

	.text	1
.Ltmp87:
	ldar	w2, [x8]
.Ltmp89:
.Ltmp90:
.org (.Ltmp90-(.Ltmp89-.Ltmp87))+(.Ltmp85-.Ltmp84), 0
.Ltmp91:
.org (.Ltmp91-(.Ltmp85-.Ltmp84))+(.Ltmp89-.Ltmp87), 0
	.text


	//NO_APP
	mov	w8, w2
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp92:
	cbnz	x8, .Ltmp92
	//NO_APP
	bl	_dev_err
	ldr	x8, [x19, #7008]
	add	x8, x8, #544
	//APP
.Ltmp93:
	ldr	w2, [x8]
.Ltmp94:
	.section	.altinstructions,"a",@progbits
.Ltmp95:
	.word	.Ltmp93-.Ltmp95
.Ltmp97:
	.word	.Ltmp96-.Ltmp97
	.hword	1
	.byte	.Ltmp94-.Ltmp93
	.byte	.Ltmp98-.Ltmp96
	.text

	.text	1
.Ltmp96:
	ldar	w2, [x8]
.Ltmp98:
.Ltmp99:
.org (.Ltmp99-(.Ltmp98-.Ltmp96))+(.Ltmp94-.Ltmp93), 0
.Ltmp100:
.org (.Ltmp100-(.Ltmp94-.Ltmp93))+(.Ltmp98-.Ltmp96), 0
	.text


	//NO_APP
	mov	w8, w2
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp101:
	cbnz	x8, .Ltmp101
	//NO_APP
	tbz	w2, #31, .LBB13_3
// %bb.1:
	and	w8, w2, #0x1f
	cbz	w8, .LBB13_3
// %bb.2:
	mov	x0, x19
	mov	w1, wzr
	bl	ufshcd_update_evt_hist
.LBB13_3:
	ldr	x8, [x19, #7008]
	add	x8, x8, #548
	//APP
.Ltmp102:
	ldr	w2, [x8]
.Ltmp103:
	.section	.altinstructions,"a",@progbits
.Ltmp104:
	.word	.Ltmp102-.Ltmp104
.Ltmp106:
	.word	.Ltmp105-.Ltmp106
	.hword	1
	.byte	.Ltmp103-.Ltmp102
	.byte	.Ltmp107-.Ltmp105
	.text

	.text	1
.Ltmp105:
	ldar	w2, [x8]
.Ltmp107:
.Ltmp108:
.org (.Ltmp108-(.Ltmp107-.Ltmp105))+(.Ltmp103-.Ltmp102), 0
.Ltmp109:
.org (.Ltmp109-(.Ltmp103-.Ltmp102))+(.Ltmp107-.Ltmp105), 0
	.text


	//NO_APP
	mov	w8, w2
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp110:
	cbnz	x8, .Ltmp110
	//NO_APP
	tbz	w2, #31, .LBB13_6
// %bb.4:
	and	w8, w2, #0xffff
	cbz	w8, .LBB13_6
// %bb.5:
	mov	x0, x19
	mov	w1, #1
	bl	ufshcd_update_evt_hist
.LBB13_6:
	ldr	x8, [x19, #7008]
	add	x8, x8, #552
	//APP
.Ltmp111:
	ldr	w2, [x8]
.Ltmp112:
	.section	.altinstructions,"a",@progbits
.Ltmp113:
	.word	.Ltmp111-.Ltmp113
.Ltmp115:
	.word	.Ltmp114-.Ltmp115
	.hword	1
	.byte	.Ltmp112-.Ltmp111
	.byte	.Ltmp116-.Ltmp114
	.text

	.text	1
.Ltmp114:
	ldar	w2, [x8]
.Ltmp116:
.Ltmp117:
.org (.Ltmp117-(.Ltmp116-.Ltmp114))+(.Ltmp112-.Ltmp111), 0
.Ltmp118:
.org (.Ltmp118-(.Ltmp112-.Ltmp111))+(.Ltmp116-.Ltmp114), 0
	.text


	//NO_APP
	mov	w8, w2
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp119:
	cbnz	x8, .Ltmp119
	//NO_APP
	tbz	w2, #31, .LBB13_9
// %bb.7:
	and	w8, w2, #0x7
	cbz	w8, .LBB13_9
// %bb.8:
	mov	x0, x19
	mov	w1, #2
	bl	ufshcd_update_evt_hist
.LBB13_9:
	ldr	x8, [x19, #7008]
	add	x8, x8, #556
	//APP
.Ltmp120:
	ldr	w2, [x8]
.Ltmp121:
	.section	.altinstructions,"a",@progbits
.Ltmp122:
	.word	.Ltmp120-.Ltmp122
.Ltmp124:
	.word	.Ltmp123-.Ltmp124
	.hword	1
	.byte	.Ltmp121-.Ltmp120
	.byte	.Ltmp125-.Ltmp123
	.text

	.text	1
.Ltmp123:
	ldar	w2, [x8]
.Ltmp125:
.Ltmp126:
.org (.Ltmp126-(.Ltmp125-.Ltmp123))+(.Ltmp121-.Ltmp120), 0
.Ltmp127:
.org (.Ltmp127-(.Ltmp121-.Ltmp120))+(.Ltmp125-.Ltmp123), 0
	.text


	//NO_APP
	mov	w8, w2
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp128:
	cbnz	x8, .Ltmp128
	//NO_APP
	tbz	w2, #31, .LBB13_12
// %bb.10:
	and	w8, w2, #0x7f
	cbz	w8, .LBB13_12
// %bb.11:
	mov	x0, x19
	mov	w1, #3
	bl	ufshcd_update_evt_hist
.LBB13_12:
	ldr	x8, [x19, #7008]
	add	x8, x8, #560
	//APP
.Ltmp129:
	ldr	w2, [x8]
.Ltmp130:
	.section	.altinstructions,"a",@progbits
.Ltmp131:
	.word	.Ltmp129-.Ltmp131
.Ltmp133:
	.word	.Ltmp132-.Ltmp133
	.hword	1
	.byte	.Ltmp130-.Ltmp129
	.byte	.Ltmp134-.Ltmp132
	.text

	.text	1
.Ltmp132:
	ldar	w2, [x8]
.Ltmp134:
.Ltmp135:
.org (.Ltmp135-(.Ltmp134-.Ltmp132))+(.Ltmp130-.Ltmp129), 0
.Ltmp136:
.org (.Ltmp136-(.Ltmp130-.Ltmp129))+(.Ltmp134-.Ltmp132), 0
	.text


	//NO_APP
	mov	w8, w2
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp137:
	cbnz	x8, .Ltmp137
	//NO_APP
	mvn	w8, w2
	tst	w8, #0x80000001
	b.eq	.LBB13_15
.LBB13_13:
	mov	w8, #7084
	ldrb	w8, [x19, x8]
	tbnz	w8, #3, .LBB13_16
.LBB13_14:
	mov	x0, x19
	bl	schedule_ufs_dsm_work
	ldp	x20, x19, [sp, #32]             // 16-byte Folded Reload
	ldr	x21, [sp, #16]                  // 8-byte Folded Reload
	ldp	x29, x30, [sp], #48             // 16-byte Folded Reload
	ret
.LBB13_15:
	mov	x0, x19
	mov	w1, #4
	bl	ufshcd_update_evt_hist
	b	.LBB13_13
.LBB13_16:
	ldr	x8, [x19, #7008]
	adrp	x1, .L.str.28
	add	x1, x1, :lo12:.L.str.28
	adrp	x2, .L__func__.dfa_dsm_handler
	add	x2, x2, :lo12:.L__func__.dfa_dsm_handler
	add	x8, x8, #564
	//APP
.Ltmp138:
	ldr	w8, [x8]
.Ltmp139:
	.section	.altinstructions,"a",@progbits
.Ltmp140:
	.word	.Ltmp138-.Ltmp140
.Ltmp142:
	.word	.Ltmp141-.Ltmp142
	.hword	1
	.byte	.Ltmp139-.Ltmp138
	.byte	.Ltmp143-.Ltmp141
	.text

	.text	1
.Ltmp141:
	ldar	w8, [x8]
.Ltmp143:
.Ltmp144:
.org (.Ltmp144-(.Ltmp143-.Ltmp141))+(.Ltmp139-.Ltmp138), 0
.Ltmp145:
.org (.Ltmp145-(.Ltmp139-.Ltmp138))+(.Ltmp143-.Ltmp141), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w9, w8
	//APP
	eor	x9, x9, x9
.Ltmp146:
	cbnz	x9, .Ltmp146
	//NO_APP
	str	w8, [x19, #7372]
	ldr	x8, [x19, #7008]
	add	x8, x8, #568
	//APP
.Ltmp147:
	ldr	w4, [x8]
.Ltmp148:
	.section	.altinstructions,"a",@progbits
.Ltmp149:
	.word	.Ltmp147-.Ltmp149
.Ltmp151:
	.word	.Ltmp150-.Ltmp151
	.hword	1
	.byte	.Ltmp148-.Ltmp147
	.byte	.Ltmp152-.Ltmp150
	.text

	.text	1
.Ltmp150:
	ldar	w4, [x8]
.Ltmp152:
.Ltmp153:
.org (.Ltmp153-(.Ltmp152-.Ltmp150))+(.Ltmp148-.Ltmp147), 0
.Ltmp154:
.org (.Ltmp154-(.Ltmp148-.Ltmp147))+(.Ltmp152-.Ltmp150), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w8, w4
	//APP
	eor	x8, x8, x8
.Ltmp155:
	cbnz	x8, .Ltmp155
	//NO_APP
	str	w4, [x19, #7376]
	ldr	x0, [x19, #96]
	ldr	w3, [x19, #7372]
	bl	_dev_err
	b	.LBB13_14
.Lfunc_end13:
	.size	dfa_dsm_handler, .Lfunc_end13-dfa_dsm_handler
                                        // -- End function
	.globl	dfa_get_intr_core_irq           // -- Begin function dfa_get_intr_core_irq
	.p2align	2
	.type	dfa_get_intr_core_irq,@function
dfa_get_intr_core_irq:                  // @dfa_get_intr_core_irq
// %bb.0:
	stp	x29, x30, [sp, #-32]!           // 16-byte Folded Spill
	str	x19, [sp, #16]                  // 8-byte Folded Spill
	mov	x19, x1
	mov	w1, #1
	mov	x29, sp
	bl	platform_get_irq
	str	w0, [x19, #7076]
	tbnz	w0, #31, .LBB14_2
.LBB14_1:
	ldr	x19, [sp, #16]                  // 8-byte Folded Reload
	ldp	x29, x30, [sp], #32             // 16-byte Folded Reload
	ret
.LBB14_2:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.29
	add	x1, x1, :lo12:.L.str.29
	bl	_dev_err
	b	.LBB14_1
.Lfunc_end14:
	.size	dfa_get_intr_core_irq, .Lfunc_end14-dfa_get_intr_core_irq
                                        // -- End function
	.globl	dfa_dhq_slot_clear              // -- Begin function dfa_dhq_slot_clear
	.p2align	2
	.type	dfa_dhq_slot_clear,@function
dfa_dhq_slot_clear:                     // @dfa_dhq_slot_clear
// %bb.0:
	ldr	w8, [x0, #7032]
	cbz	w8, .LBB15_3
// %bb.1:
	mov	x8, xzr
	mov	x9, xzr
.LBB15_2:                               // =>This Inner Loop Header: Depth=1
	ldr	x10, [x0, #7024]
	add	x9, x9, #1
	add	x10, x10, x8
	add	x8, x8, #80
	stur	xzr, [x10, #60]
	str	xzr, [x10, #72]
	ldr	w10, [x0, #7032]
	cmp	x9, x10
	b.lo	.LBB15_2
.LBB15_3:
	ret
.Lfunc_end15:
	.size	dfa_dhq_slot_clear, .Lfunc_end15-dfa_dhq_slot_clear
                                        // -- End function
	.globl	dfa_config_mcq                  // -- Begin function dfa_config_mcq
	.p2align	2
	.type	dfa_config_mcq,@function
dfa_config_mcq:                         // @dfa_config_mcq
// %bb.0:
	stp	x29, x30, [sp, #-64]!           // 16-byte Folded Spill
	mov	w9, #5
	stp	x24, x23, [sp, #16]             // 16-byte Folded Spill
	stp	x22, x21, [sp, #32]             // 16-byte Folded Spill
	ldr	x21, [x0, #296]
	stp	x20, x19, [sp, #48]             // 16-byte Folded Spill
	mov	w8, wzr
	str	w9, [x0, #7032]
	mov	w23, #1
	//APP
	dmb	oshst
	//NO_APP
	mov	x29, sp
	ldr	x9, [x0, #7008]
	mov	x19, x0
	mov	w20, w1
	add	x9, x9, #4
	//APP
	str	w8, [x9]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #4
	ldr	x8, [x0, #7008]
	add	x8, x8, #272
	//APP
	str	w23, [x8]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x8, [x0, #7008]
	add	x8, x8, #380
	//APP
	str	w9, [x8]
	//NO_APP
	bl	ufs_disable_auto_h8_status_sync
	//APP
	dmb	oshst
	//NO_APP
	mov	w22, #37855
	ldr	x8, [x19, #7008]
	movk	w22, #4, lsl #16
	add	x8, x8, #12
	//APP
	str	w23, [x8]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x8, [x19, #7008]
	add	x8, x8, #16
	//APP
	str	w23, [x8]
	//NO_APP
.LBB16_1:                               // =>This Inner Loop Header: Depth=1
	ldr	x8, [x19, #7008]
	add	x8, x8, #12
	//APP
.Ltmp156:
	ldr	w8, [x8]
.Ltmp157:
	.section	.altinstructions,"a",@progbits
.Ltmp158:
	.word	.Ltmp156-.Ltmp158
.Ltmp160:
	.word	.Ltmp159-.Ltmp160
	.hword	1
	.byte	.Ltmp157-.Ltmp156
	.byte	.Ltmp161-.Ltmp159
	.text

	.text	1
.Ltmp159:
	ldar	w8, [x8]
.Ltmp161:
.Ltmp162:
.org (.Ltmp162-(.Ltmp161-.Ltmp159))+(.Ltmp157-.Ltmp156), 0
.Ltmp163:
.org (.Ltmp163-(.Ltmp157-.Ltmp156))+(.Ltmp161-.Ltmp159), 0
	.text


	//NO_APP
	mov	w9, w8
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x9, x9, x9
.Ltmp164:
	cbnz	x9, .Ltmp164
	//NO_APP
	tbnz	w8, #0, .LBB16_9
// %bb.2:                               //   in Loop: Header=BB16_1 Depth=1
	mov	w0, #42950
	bl	__const_udelay
	sub	w22, w22, #1
	cmn	w22, #1
	b.ne	.LBB16_1
.LBB16_3:
	ldr	x8, [x21, #32]
	adrp	x1, .L.str.79
	add	x1, x1, :lo12:.L.str.79
	ldr	x0, [x19, #96]
	add	x8, x8, #12
	//APP
.Ltmp165:
	ldr	w2, [x8]
.Ltmp166:
	.section	.altinstructions,"a",@progbits
.Ltmp167:
	.word	.Ltmp165-.Ltmp167
.Ltmp169:
	.word	.Ltmp168-.Ltmp169
	.hword	1
	.byte	.Ltmp166-.Ltmp165
	.byte	.Ltmp170-.Ltmp168
	.text

	.text	1
.Ltmp168:
	ldar	w2, [x8]
.Ltmp170:
.Ltmp171:
.org (.Ltmp171-(.Ltmp170-.Ltmp168))+(.Ltmp166-.Ltmp165), 0
.Ltmp172:
.org (.Ltmp172-(.Ltmp166-.Ltmp165))+(.Ltmp170-.Ltmp168), 0
	.text


	//NO_APP
	mov	w8, w2
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp173:
	cbnz	x8, .Ltmp173
	//NO_APP
	bl	_dev_err
	ldr	x8, [x21, #32]
	adrp	x1, .L.str.80
	add	x1, x1, :lo12:.L.str.80
	ldr	x0, [x19, #96]
	add	x8, x8, #104
	//APP
.Ltmp174:
	ldr	w2, [x8]
.Ltmp175:
	.section	.altinstructions,"a",@progbits
.Ltmp176:
	.word	.Ltmp174-.Ltmp176
.Ltmp178:
	.word	.Ltmp177-.Ltmp178
	.hword	1
	.byte	.Ltmp175-.Ltmp174
	.byte	.Ltmp179-.Ltmp177
	.text

	.text	1
.Ltmp177:
	ldar	w2, [x8]
.Ltmp179:
.Ltmp180:
.org (.Ltmp180-(.Ltmp179-.Ltmp177))+(.Ltmp175-.Ltmp174), 0
.Ltmp181:
.org (.Ltmp181-(.Ltmp175-.Ltmp174))+(.Ltmp179-.Ltmp177), 0
	.text


	//NO_APP
	mov	w8, w2
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp182:
	cbnz	x8, .Ltmp182
	//NO_APP
	bl	_dev_err
	ldr	x8, [x21, #32]
	adrp	x1, .L.str.81
	add	x1, x1, :lo12:.L.str.81
	ldr	x0, [x19, #96]
	add	x8, x8, #1028
	//APP
.Ltmp183:
	ldr	w2, [x8]
.Ltmp184:
	.section	.altinstructions,"a",@progbits
.Ltmp185:
	.word	.Ltmp183-.Ltmp185
.Ltmp187:
	.word	.Ltmp186-.Ltmp187
	.hword	1
	.byte	.Ltmp184-.Ltmp183
	.byte	.Ltmp188-.Ltmp186
	.text

	.text	1
.Ltmp186:
	ldar	w2, [x8]
.Ltmp188:
.Ltmp189:
.org (.Ltmp189-(.Ltmp188-.Ltmp186))+(.Ltmp184-.Ltmp183), 0
.Ltmp190:
.org (.Ltmp190-(.Ltmp184-.Ltmp183))+(.Ltmp188-.Ltmp186), 0
	.text


	//NO_APP
	mov	w8, w2
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp191:
	cbnz	x8, .Ltmp191
	//NO_APP
	bl	_dev_err
	ldr	x8, [x21, #32]
	adrp	x1, .L.str.82
	add	x1, x1, :lo12:.L.str.82
	ldr	x0, [x19, #96]
	add	x8, x8, #1792
	//APP
.Ltmp192:
	ldr	w2, [x8]
.Ltmp193:
	.section	.altinstructions,"a",@progbits
.Ltmp194:
	.word	.Ltmp192-.Ltmp194
.Ltmp196:
	.word	.Ltmp195-.Ltmp196
	.hword	1
	.byte	.Ltmp193-.Ltmp192
	.byte	.Ltmp197-.Ltmp195
	.text

	.text	1
.Ltmp195:
	ldar	w2, [x8]
.Ltmp197:
.Ltmp198:
.org (.Ltmp198-(.Ltmp197-.Ltmp195))+(.Ltmp193-.Ltmp192), 0
.Ltmp199:
.org (.Ltmp199-(.Ltmp193-.Ltmp192))+(.Ltmp197-.Ltmp195), 0
	.text


	//NO_APP
	mov	w8, w2
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp200:
	cbnz	x8, .Ltmp200
	//NO_APP
	bl	_dev_err
	mov	w20, wzr
	mov	w22, #256
	adrp	x21, .L.str.83
	add	x21, x21, :lo12:.L.str.83
.LBB16_4:                               // =>This Inner Loop Header: Depth=1
	ldr	x8, [x19, #7008]
	mov	x1, x21
	ldr	x0, [x19, #96]
	mov	w2, w20
	add	x8, x8, x22
	//APP
.Ltmp201:
	ldr	w3, [x8]
.Ltmp202:
	.section	.altinstructions,"a",@progbits
.Ltmp203:
	.word	.Ltmp201-.Ltmp203
.Ltmp205:
	.word	.Ltmp204-.Ltmp205
	.hword	1
	.byte	.Ltmp202-.Ltmp201
	.byte	.Ltmp206-.Ltmp204
	.text

	.text	1
.Ltmp204:
	ldar	w3, [x8]
.Ltmp206:
.Ltmp207:
.org (.Ltmp207-(.Ltmp206-.Ltmp204))+(.Ltmp202-.Ltmp201), 0
.Ltmp208:
.org (.Ltmp208-(.Ltmp202-.Ltmp201))+(.Ltmp206-.Ltmp204), 0
	.text


	//NO_APP
	mov	w8, w3
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp209:
	cbnz	x8, .Ltmp209
	//NO_APP
	bl	_dev_err
	add	x22, x22, #4
	add	w20, w20, #1
	cmp	x22, #384
	b.ne	.LBB16_4
// %bb.5:
	mov	w20, wzr
	mov	w22, #512
	adrp	x21, .L.str.84
	add	x21, x21, :lo12:.L.str.84
.LBB16_6:                               // =>This Inner Loop Header: Depth=1
	ldr	x8, [x19, #7008]
	mov	x1, x21
	ldr	x0, [x19, #96]
	mov	w2, w20
	add	x8, x8, x22
	//APP
.Ltmp210:
	ldr	w3, [x8]
.Ltmp211:
	.section	.altinstructions,"a",@progbits
.Ltmp212:
	.word	.Ltmp210-.Ltmp212
.Ltmp214:
	.word	.Ltmp213-.Ltmp214
	.hword	1
	.byte	.Ltmp211-.Ltmp210
	.byte	.Ltmp215-.Ltmp213
	.text

	.text	1
.Ltmp213:
	ldar	w3, [x8]
.Ltmp215:
.Ltmp216:
.org (.Ltmp216-(.Ltmp215-.Ltmp213))+(.Ltmp211-.Ltmp210), 0
.Ltmp217:
.org (.Ltmp217-(.Ltmp211-.Ltmp210))+(.Ltmp215-.Ltmp213), 0
	.text


	//NO_APP
	mov	w8, w3
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp218:
	cbnz	x8, .Ltmp218
	//NO_APP
	bl	_dev_err
	add	x22, x22, #4
	add	w20, w20, #1
	cmp	x22, #640
	b.ne	.LBB16_6
// %bb.7:
	mov	w8, #1
	mov	x0, x19
	str	w8, [x19, #7224]
	bl	dfa_early_reset
	mov	w20, #-110
	str	wzr, [x19, #7224]
.LBB16_8:
	mov	w0, w20
	ldp	x20, x19, [sp, #48]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #32]             // 16-byte Folded Reload
	ldp	x24, x23, [sp, #16]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #64             // 16-byte Folded Reload
	ret
.LBB16_9:
	//APP
	dmb	oshst
	//NO_APP
	cmp	w22, #1
	ldr	x8, [x19, #7008]
	mov	w9, #1
	add	x8, x8, #12
	//APP
	str	w9, [x8]
	//NO_APP
	b.lt	.LBB16_3
// %bb.10:
	mov	w8, #48
	mov	w9, #3552
.LBB16_11:                              // =>This Inner Loop Header: Depth=1
	//APP
	dmb	oshst
	//NO_APP
	mov	w10, wzr
	ldr	x11, [x19, #7016]
	add	x11, x11, x8
	//APP
	str	w10, [x11]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x11, [x19, #7016]
	add	x11, x11, x8
	add	x8, x8, #28
	sub	x11, x11, #16
	//APP
	str	w10, [x11]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	cmp	x8, #2288
	ldr	x11, [x19, #7016]
	add	x11, x11, x9
	sub	x11, x11, #1276
	//APP
	str	w10, [x11]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x11, [x19, #7016]
	add	x11, x11, x9
	add	x9, x9, #8
	//APP
	str	w10, [x11]
	//NO_APP
	b.ne	.LBB16_11
// %bb.12:
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #2
	ldr	x8, [x19, #7008]
	mov	x0, x19
	add	x8, x8, #272
	//APP
	str	w9, [x8]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #4
	ldr	x8, [x19, #7008]
	add	x8, x8, #380
	//APP
	str	w9, [x8]
	//NO_APP
	bl	ufs_disable_auto_h8_status_sync
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #1
	ldr	x8, [x19, #7008]
	add	x8, x8, #16
	//APP
	str	w9, [x8]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #1048576
	ldr	x8, [x21, #32]
	add	x8, x8, #1792
	//APP
	str	w9, [x8]
	//NO_APP
	tbz	w20, #0, .LBB16_15
// %bb.13:
	ldr	w8, [x19, #7032]
	mov	w2, #3520
	ldr	x0, [x19, #96]
	add	x8, x8, x8, lsl #2
	lsl	x1, x8, #4
	bl	devm_kmalloc
	str	x0, [x19, #7024]
	cbnz	x0, .LBB16_25
// %bb.14:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.85
	add	x1, x1, :lo12:.L.str.85
	bl	_dev_err
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.30
	add	x1, x1, :lo12:.L.str.30
	mov	w2, #-12
	mov	w20, #-12
	bl	_dev_err
	b	.LBB16_8
.LBB16_15:
	ldr	w8, [x19, #7032]
.LBB16_16:
	cbz	w8, .LBB16_19
// %bb.17:
	mov	x8, xzr
	mov	x9, xzr
.LBB16_18:                              // =>This Inner Loop Header: Depth=1
	ldr	x10, [x19, #7024]
	add	x9, x9, #1
	add	x10, x10, x8
	add	x8, x8, #80
	stur	xzr, [x10, #60]
	str	xzr, [x10, #72]
	ldr	w10, [x19, #7032]
	cmp	x9, x10
	b.lo	.LBB16_18
.LBB16_19:
	ldr	w2, [x19, #7076]
	cmp	w2, #0
	b.gt	.LBB16_21
// %bb.20:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.88
	add	x1, x1, :lo12:.L.str.88
	bl	_dev_err
	mov	w20, #-19
	b	.LBB16_35
.LBB16_21:
	ldr	x24, [x19, #296]
	//APP
	dmb	oshst
	//NO_APP
	ldr	x8, [x19, #7008]
	mov	w9, #1
	adrp	x2, dfa_acore_irq_handler
	add	x2, x2, :lo12:dfa_acore_irq_handler
	adrp	x5, .L.str.89
	add	x5, x5, :lo12:.L.str.89
	add	x8, x8, #12
	//APP
	str	w9, [x8]
	//NO_APP
	ldr	x0, [x19, #96]
	mov	x3, xzr
	ldr	w1, [x19, #7076]
	mov	w4, #128
	mov	x6, x19
	bl	devm_request_threaded_irq
	mov	w20, w0
	ldr	x0, [x19, #96]
	cbnz	w20, .LBB16_33
// %bb.22:
	adrp	x2, dfa_mcq_write_msi_msg
	add	x2, x2, :lo12:dfa_mcq_write_msi_msg
	mov	w1, #325
	bl	platform_msi_domain_alloc_irqs
	mov	w20, w0
	ldr	x0, [x19, #96]
	cbnz	w20, .LBB16_39
// %bb.23:
	ldr	x21, [x0, #600]
	add	x8, x0, #600
	cmp	x21, x8
	b.ne	.LBB16_40
.LBB16_24:
	//APP
	dmb	oshst
	//NO_APP
	mov	w10, #1
	ldr	x8, [x19, #7016]
	mov	w9, #131072
	//APP
	str	w9, [x8]
	//NO_APP
	strb	w10, [x24, #325]
	b	.LBB16_36
.LBB16_25:
	ldr	w8, [x19, #7032]
	cbz	w8, .LBB16_19
// %bb.26:
	mov	w8, wzr
	mov	w9, #80
	mov	w10, #64
.LBB16_27:                              // =>This Inner Loop Header: Depth=1
	ldr	x11, [x19, #7024]
	nop
	smaddl	x11, w8, w9, x11
	add	w8, w8, #1
	str	w10, [x11, #32]
	ldr	x12, [x19, #7016]
	str	wzr, [x11, #56]
	str	wzr, [x11, #68]
	str	x12, [x11, #48]
	ldr	w11, [x19, #7032]
	cmp	w8, w11
	b.lo	.LBB16_27
// %bb.28:
	cbz	w11, .LBB16_19
// %bb.29:
	mov	w21, wzr
	mov	w22, #80
	adrp	x20, .L.str.86
	add	x20, x20, :lo12:.L.str.86
	adrp	x23, .L.str.87
	add	x23, x23, :lo12:.L.str.87
.LBB16_30:                              // =>This Inner Loop Header: Depth=1
	ldr	x8, [x19, #7024]
	mov	w3, #3264
	ldr	x0, [x19, #96]
	mov	x4, xzr
	smaddl	x24, w21, w22, x8
	add	x2, x24, #8
	ldr	w8, [x24, #32]
	lsl	x1, x8, #6
	bl	dmam_alloc_attrs
	str	x0, [x24]
	cbz	x0, .LBB16_46
// %bb.31:                              //   in Loop: Header=BB16_30 Depth=1
	ldr	w8, [x24, #32]
	add	x2, x24, #24
	ldr	x0, [x19, #96]
	mov	w3, #3264
	mov	x4, xzr
	lsl	x1, x8, #5
	bl	dmam_alloc_attrs
	str	x0, [x24, #16]
	cbz	x0, .LBB16_45
// %bb.32:                              //   in Loop: Header=BB16_30 Depth=1
	ldr	w8, [x19, #7032]
	add	w21, w21, #1
	cmp	w21, w8
	b.lo	.LBB16_30
	b	.LBB16_16
.LBB16_33:
	adrp	x1, .L.str.90
	add	x1, x1, :lo12:.L.str.90
.LBB16_34:
	mov	w2, w20
	bl	_dev_err
.LBB16_35:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.31
	add	x1, x1, :lo12:.L.str.31
	mov	w2, w20
	bl	_dev_info
.LBB16_36:
	ldr	x8, [x19, #7016]
	mov	x0, x19
	add	x8, x8, #4
	//APP
.Ltmp219:
	ldr	w8, [x8]
.Ltmp220:
	.section	.altinstructions,"a",@progbits
.Ltmp221:
	.word	.Ltmp219-.Ltmp221
.Ltmp223:
	.word	.Ltmp222-.Ltmp223
	.hword	1
	.byte	.Ltmp220-.Ltmp219
	.byte	.Ltmp224-.Ltmp222
	.text

	.text	1
.Ltmp222:
	ldar	w8, [x8]
.Ltmp224:
.Ltmp225:
.org (.Ltmp225-(.Ltmp224-.Ltmp222))+(.Ltmp220-.Ltmp219), 0
.Ltmp226:
.org (.Ltmp226-(.Ltmp220-.Ltmp219))+(.Ltmp224-.Ltmp222), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w9, w8
	orr	w8, w8, #0x1
	//APP
	eor	x9, x9, x9
.Ltmp227:
	cbnz	x9, .Ltmp227
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x9, [x19, #7016]
	add	x9, x9, #4
	//APP
	str	w8, [x9]
	//NO_APP
	bl	dfa_mcq_make_queues_operational
	mov	w8, #6828
	ldrb	w9, [x19, #259]
	mov	w10, #1
	strb	w10, [x19, x8]
	tbz	w9, #1, .LBB16_38
// %bb.37:
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #13776
	ldr	x8, [x19]
	adrp	x1, .L.str.16
	add	x1, x1, :lo12:.L.str.16
	add	x8, x8, x9
	//APP
.Ltmp228:
	ldr	w8, [x8]
.Ltmp229:
	.section	.altinstructions,"a",@progbits
.Ltmp230:
	.word	.Ltmp228-.Ltmp230
.Ltmp232:
	.word	.Ltmp231-.Ltmp232
	.hword	1
	.byte	.Ltmp229-.Ltmp228
	.byte	.Ltmp233-.Ltmp231
	.text

	.text	1
.Ltmp231:
	ldar	w8, [x8]
.Ltmp233:
.Ltmp234:
.org (.Ltmp234-(.Ltmp233-.Ltmp231))+(.Ltmp229-.Ltmp228), 0
.Ltmp235:
.org (.Ltmp235-(.Ltmp229-.Ltmp228))+(.Ltmp233-.Ltmp231), 0
	.text


	//NO_APP
	mov	w10, w8
	//APP
	dmb	oshld
	//NO_APP
	orr	w8, w8, #0x100
	//APP
	eor	x10, x10, x10
.Ltmp236:
	cbnz	x10, .Ltmp236
	//NO_APP
	ldr	x10, [x19]
	add	x10, x10, x9
	//APP
	str	w8, [x10]
	//NO_APP
	ldr	x8, [x19]
	ldr	x0, [x19, #96]
	add	x8, x8, x9
	//APP
.Ltmp237:
	ldr	w2, [x8]
.Ltmp238:
	.section	.altinstructions,"a",@progbits
.Ltmp239:
	.word	.Ltmp237-.Ltmp239
.Ltmp241:
	.word	.Ltmp240-.Ltmp241
	.hword	1
	.byte	.Ltmp238-.Ltmp237
	.byte	.Ltmp242-.Ltmp240
	.text

	.text	1
.Ltmp240:
	ldar	w2, [x8]
.Ltmp242:
.Ltmp243:
.org (.Ltmp243-(.Ltmp242-.Ltmp240))+(.Ltmp238-.Ltmp237), 0
.Ltmp244:
.org (.Ltmp244-(.Ltmp238-.Ltmp237))+(.Ltmp242-.Ltmp240), 0
	.text


	//NO_APP
	mov	w8, w2
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp245:
	cbnz	x8, .Ltmp245
	//NO_APP
	bl	_dev_err
.LBB16_38:
	ldr	x8, [x19, #7016]
	mov	w9, #4192
	ldr	x0, [x19, #96]
	//APP
.Ltmp246:
	ldr	w2, [x8]
.Ltmp247:
	.section	.altinstructions,"a",@progbits
.Ltmp248:
	.word	.Ltmp246-.Ltmp248
.Ltmp250:
	.word	.Ltmp249-.Ltmp250
	.hword	1
	.byte	.Ltmp247-.Ltmp246
	.byte	.Ltmp251-.Ltmp249
	.text

	.text	1
.Ltmp249:
	ldar	w2, [x8]
.Ltmp251:
.Ltmp252:
.org (.Ltmp252-(.Ltmp251-.Ltmp249))+(.Ltmp247-.Ltmp246), 0
.Ltmp253:
.org (.Ltmp253-(.Ltmp247-.Ltmp246))+(.Ltmp251-.Ltmp249), 0
	.text


	//NO_APP
	mov	w8, w2
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp254:
	cbnz	x8, .Ltmp254
	//NO_APP
	adrp	x1, .L.str.32
	add	x1, x1, :lo12:.L.str.32
	ldr	x8, [x19, #7016]
	add	x8, x8, #16
	//APP
.Ltmp255:
	ldr	w3, [x8]
.Ltmp256:
	.section	.altinstructions,"a",@progbits
.Ltmp257:
	.word	.Ltmp255-.Ltmp257
.Ltmp259:
	.word	.Ltmp258-.Ltmp259
	.hword	1
	.byte	.Ltmp256-.Ltmp255
	.byte	.Ltmp260-.Ltmp258
	.text

	.text	1
.Ltmp258:
	ldar	w3, [x8]
.Ltmp260:
.Ltmp261:
.org (.Ltmp261-(.Ltmp260-.Ltmp258))+(.Ltmp256-.Ltmp255), 0
.Ltmp262:
.org (.Ltmp262-(.Ltmp256-.Ltmp255))+(.Ltmp260-.Ltmp258), 0
	.text


	//NO_APP
	mov	w8, w3
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp263:
	cbnz	x8, .Ltmp263
	//NO_APP
	ldr	x8, [x19, #7016]
	add	x8, x8, #20
	//APP
.Ltmp264:
	ldr	w4, [x8]
.Ltmp265:
	.section	.altinstructions,"a",@progbits
.Ltmp266:
	.word	.Ltmp264-.Ltmp266
.Ltmp268:
	.word	.Ltmp267-.Ltmp268
	.hword	1
	.byte	.Ltmp265-.Ltmp264
	.byte	.Ltmp269-.Ltmp267
	.text

	.text	1
.Ltmp267:
	ldar	w4, [x8]
.Ltmp269:
.Ltmp270:
.org (.Ltmp270-(.Ltmp269-.Ltmp267))+(.Ltmp265-.Ltmp264), 0
.Ltmp271:
.org (.Ltmp271-(.Ltmp265-.Ltmp264))+(.Ltmp269-.Ltmp267), 0
	.text


	//NO_APP
	mov	w8, w4
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp272:
	cbnz	x8, .Ltmp272
	//NO_APP
	ldr	x8, [x19, #7016]
	add	x8, x8, #4
	//APP
.Ltmp273:
	ldr	w5, [x8]
.Ltmp274:
	.section	.altinstructions,"a",@progbits
.Ltmp275:
	.word	.Ltmp273-.Ltmp275
.Ltmp277:
	.word	.Ltmp276-.Ltmp277
	.hword	1
	.byte	.Ltmp274-.Ltmp273
	.byte	.Ltmp278-.Ltmp276
	.text

	.text	1
.Ltmp276:
	ldar	w5, [x8]
.Ltmp278:
.Ltmp279:
.org (.Ltmp279-(.Ltmp278-.Ltmp276))+(.Ltmp274-.Ltmp273), 0
.Ltmp280:
.org (.Ltmp280-(.Ltmp274-.Ltmp273))+(.Ltmp278-.Ltmp276), 0
	.text


	//NO_APP
	mov	w8, w5
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp281:
	cbnz	x8, .Ltmp281
	//NO_APP
	ldr	x8, [x19, #7016]
	add	x8, x8, x9
	//APP
.Ltmp282:
	ldr	w6, [x8]
.Ltmp283:
	.section	.altinstructions,"a",@progbits
.Ltmp284:
	.word	.Ltmp282-.Ltmp284
.Ltmp286:
	.word	.Ltmp285-.Ltmp286
	.hword	1
	.byte	.Ltmp283-.Ltmp282
	.byte	.Ltmp287-.Ltmp285
	.text

	.text	1
.Ltmp285:
	ldar	w6, [x8]
.Ltmp287:
.Ltmp288:
.org (.Ltmp288-(.Ltmp287-.Ltmp285))+(.Ltmp283-.Ltmp282), 0
.Ltmp289:
.org (.Ltmp289-(.Ltmp283-.Ltmp282))+(.Ltmp287-.Ltmp285), 0
	.text


	//NO_APP
	mov	w8, w6
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp290:
	cbnz	x8, .Ltmp290
	//NO_APP
	mov	w9, #4196
	ldr	x8, [x19, #7016]
	add	x8, x8, x9
	//APP
.Ltmp291:
	ldr	w7, [x8]
.Ltmp292:
	.section	.altinstructions,"a",@progbits
.Ltmp293:
	.word	.Ltmp291-.Ltmp293
.Ltmp295:
	.word	.Ltmp294-.Ltmp295
	.hword	1
	.byte	.Ltmp292-.Ltmp291
	.byte	.Ltmp296-.Ltmp294
	.text

	.text	1
.Ltmp294:
	ldar	w7, [x8]
.Ltmp296:
.Ltmp297:
.org (.Ltmp297-(.Ltmp296-.Ltmp294))+(.Ltmp292-.Ltmp291), 0
.Ltmp298:
.org (.Ltmp298-(.Ltmp292-.Ltmp291))+(.Ltmp296-.Ltmp294), 0
	.text


	//NO_APP
	mov	w8, w7
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp299:
	cbnz	x8, .Ltmp299
	//NO_APP
	bl	_dev_err
	mov	w20, wzr
	b	.LBB16_8
.LBB16_39:
	adrp	x1, .L.str.91
	add	x1, x1, :lo12:.L.str.91
	b	.LBB16_34
.LBB16_40:
	adrp	x22, dfa_mcq_esi_handler
	add	x22, x22, :lo12:dfa_mcq_esi_handler
	adrp	x23, .L.str.92
	add	x23, x23, :lo12:.L.str.92
	b	.LBB16_42
.LBB16_41:                              //   in Loop: Header=BB16_42 Depth=1
	ldr	x21, [x21]
	add	x8, x0, #600
	cmp	x21, x8
	b.eq	.LBB16_24
.LBB16_42:                              // =>This Inner Loop Header: Depth=1
	ldrh	w8, [x21, #80]
	sub	w8, w8, #160
	cmp	w8, #79
	b.hi	.LBB16_41
// %bb.43:                              //   in Loop: Header=BB16_42 Depth=1
	ldr	w1, [x21, #16]
	mov	x2, x22
	mov	x3, xzr
	mov	w4, #128
	mov	x5, x23
	mov	x6, x21
	bl	devm_request_threaded_irq
	cbnz	w0, .LBB16_47
// %bb.44:                              //   in Loop: Header=BB16_42 Depth=1
	ldr	x0, [x19, #96]
	b	.LBB16_41
.LBB16_45:
	mov	x20, x23
.LBB16_46:
	ldr	x0, [x19, #96]
	mov	x1, x20
	bl	_dev_err
	mov	w20, #-12
	b	.LBB16_8
.LBB16_47:
	mov	w20, w0
	ldr	x0, [x19, #96]
	ldr	w3, [x21, #16]
	adrp	x1, .L.str.93
	add	x1, x1, :lo12:.L.str.93
	adrp	x2, .L__func__.dfa_mcq_config_esi
	add	x2, x2, :lo12:.L__func__.dfa_mcq_config_esi
	mov	w4, w20
	bl	_dev_err
	ldr	x0, [x19, #96]
	ldr	x22, [x0, #600]
.LBB16_48:                              // =>This Inner Loop Header: Depth=1
	add	x8, x0, #600
	cmp	x22, x8
	b.eq	.LBB16_51
// %bb.49:                              //   in Loop: Header=BB16_48 Depth=1
	cmp	x22, x21
	b.eq	.LBB16_51
// %bb.50:                              //   in Loop: Header=BB16_48 Depth=1
	ldr	w1, [x22, #16]
	mov	x2, x19
	bl	devm_free_irq
	ldr	x0, [x19, #96]
	ldr	x22, [x22]
	b	.LBB16_48
.LBB16_51:
	ldr	w1, [x19, #7076]
	mov	x2, x19
	bl	devm_free_irq
	ldr	x0, [x19, #96]
	bl	platform_msi_domain_free_irqs
	b	.LBB16_35
.Lfunc_end16:
	.size	dfa_config_mcq, .Lfunc_end16-dfa_config_mcq
                                        // -- End function
	.globl	dfa_prepare_req_desc_hdr        // -- Begin function dfa_prepare_req_desc_hdr
	.p2align	2
	.type	dfa_prepare_req_desc_hdr,@function
dfa_prepare_req_desc_hdr:               // @dfa_prepare_req_desc_hdr
// %bb.0:
	ldr	x8, [x0, #184]
	cmp	w2, #2
	b.eq	.LBB17_3
// %bb.1:
	cmp	w2, #1
	b.ne	.LBB17_4
// %bb.2:
	mov	w9, #301989888
	mov	w10, #32
	b	.LBB17_5
.LBB17_3:
	mov	w9, #335544320
	mov	w10, #64
	b	.LBB17_5
.LBB17_4:
	mov	w10, wzr
	mov	w9, #268435456
.LBB17_5:
	strb	w10, [x1]
	orr	w12, w9, #0x1000000
	ldrb	w11, [x0, #109]
	strh	wzr, [x8, #28]
	ldr	w10, [x8, #8]
	cmp	w11, #0
	orr	w10, w10, #0xf
	csel	w9, w9, w12, eq
	stp	w10, wzr, [x8, #8]
	stp	w9, wzr, [x8]
	ret
.Lfunc_end17:
	.size	dfa_prepare_req_desc_hdr, .Lfunc_end17-dfa_prepare_req_desc_hdr
                                        // -- End function
	.globl	dfa_uie_utrd_prepare            // -- Begin function dfa_uie_utrd_prepare
	.p2align	2
	.type	dfa_uie_utrd_prepare,@function
dfa_uie_utrd_prepare:                   // @dfa_uie_utrd_prepare
// %bb.0:
	sub	sp, sp, #144
	adrp	x8, :got:__stack_chk_guard
	stp	x29, x30, [sp, #96]             // 16-byte Folded Spill
	stp	x22, x21, [sp, #112]            // 16-byte Folded Spill
	add	x29, sp, #96
	stp	x20, x19, [sp, #128]            // 16-byte Folded Spill
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldr	x8, [x8]
	stur	x8, [x29, #-8]
	stp	xzr, xzr, [sp, #64]
	stp	xzr, xzr, [sp, #48]
	stp	xzr, xzr, [sp, #32]
	str	wzr, [sp, #28]
	str	xzr, [sp, #16]
	str	wzr, [sp, #12]
	cbz	x1, .LBB18_9
// %bb.1:
	ldr	x8, [x1, #72]
	mov	x20, x1
	mov	x19, x0
	ldr	x21, [x1, #184]
	ldr	x9, [x8, #224]
	ldrb	w9, [x9]
	cmp	w9, #40
	b.eq	.LBB18_4
// %bb.2:
	cmp	w9, #136
	b.eq	.LBB18_4
// %bb.3:
	cmp	w9, #42
	b.ne	.LBB18_7
.LBB18_4:
	mov	w9, #8388608
	ldr	x8, [x8, #272]
	str	w9, [sp, #80]
	cbz	x8, .LBB18_7
// %bb.5:
	ldr	x8, [x8, #192]
	cbz	x8, .LBB18_7
// %bb.6:
	add	x2, sp, #12
	add	x3, sp, #28
	add	x4, sp, #16
	mov	x0, x19
	mov	x1, x20
	bl	hufs_crypto_set_keyindex
	ldr	x8, [x20, #72]
	add	x9, sp, #32
	add	x2, x9, #32
	add	x3, x9, #36
	add	x4, x9, #40
	add	x5, x9, #44
	ldr	x8, [x8, #272]
	mov	x0, x20
	mov	x1, x19
	ldr	x8, [x8, #56]
	ldr	x22, [x8, #128]
	bl	hufs_get_meta_data_factor
	ldr	w8, [sp, #80]
	ldr	w9, [sp, #28]
	ldr	w10, [sp, #12]
	orr	w8, w9, w8
	lsr	x9, x22, #32
	orr	w9, w10, w9
	stp	w8, w22, [sp, #32]
	str	w9, [sp, #44]
	ldr	w9, [x21]
	orr	w8, w9, w8
	str	w8, [x21]
	ldr	w8, [sp, #36]
	str	w8, [x21, #4]
	ldr	w8, [sp, #44]
	str	w8, [x21, #12]
	ldr	w8, [sp, #64]
	str	w8, [x21, #32]
	ldr	w8, [sp, #68]
	str	w8, [x21, #36]
	ldr	w8, [sp, #72]
	str	w8, [x21, #40]
	ldr	w8, [sp, #76]
	str	w8, [x21, #44]
.LBB18_7:
	adrp	x8, :got:__stack_chk_guard
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldur	x9, [x29, #-8]
	ldr	x8, [x8]
	cmp	x8, x9
	b.ne	.LBB18_10
// %bb.8:
	ldp	x20, x19, [sp, #128]            // 16-byte Folded Reload
	ldp	x22, x21, [sp, #112]            // 16-byte Folded Reload
	ldp	x29, x30, [sp, #96]             // 16-byte Folded Reload
	add	sp, sp, #144
	ret
.LBB18_9:
	mov	x19, x0
	ldr	x0, [x0, #96]
	adrp	x1, .L.str.33
	add	x1, x1, :lo12:.L.str.33
	bl	_dev_err
	b	.LBB18_7
.LBB18_10:
	bl	__stack_chk_fail
.Lfunc_end18:
	.size	dfa_uie_utrd_prepare, .Lfunc_end18-dfa_uie_utrd_prepare
                                        // -- End function
	.globl	dfa_memory_alloc                // -- Begin function dfa_memory_alloc
	.p2align	2
	.type	dfa_memory_alloc,@function
dfa_memory_alloc:                       // @dfa_memory_alloc
// %bb.0:
	stp	x29, x30, [sp, #-32]!           // 16-byte Folded Spill
	ldr	w9, [x0, #264]
	mov	w8, #5120
	ldr	x10, [x0, #304]
	stp	x20, x19, [sp, #16]             // 16-byte Folded Spill
	mov	x19, x0
	ldr	x0, [x0, #96]
	sub	w9, w9, #1
	add	x20, x19, #64
	add	x8, x8, x10, lsl #8
	sxtw	x9, w9
	mov	w10, #19456
	mov	x2, x20
	madd	x1, x8, x9, x10
	mov	w3, #3264
	mov	x4, xzr
	mov	x29, sp
	bl	dmam_alloc_attrs
	str	x0, [x19, #32]
	cbz	x0, .LBB19_7
// %bb.1:
	ldrh	w8, [x20]
	tst	w8, #0xfff
	b.ne	.LBB19_6
// %bb.2:
	ldrsw	x8, [x19, #264]
	mov	w9, #216
	umulh	x10, x8, x9
	cmp	xzr, x10
	b.ne	.LBB19_8
// %bb.3:
	mul	x1, x8, x9
	ldr	x0, [x19, #96]
	mov	w2, #3520
	bl	devm_kmalloc
	str	x0, [x19, #224]
	cbz	x0, .LBB19_9
// %bb.4:
	mov	w0, wzr
.LBB19_5:
	ldp	x20, x19, [sp, #16]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #32             // 16-byte Folded Reload
	ret
.LBB19_6:
	adrp	x0, .L.str.10
	add	x0, x0, :lo12:.L.str.10
	mov	w1, #1455
	mov	w2, #9
	mov	x3, xzr
	bl	warn_slowpath_fmt
.LBB19_7:
	adrp	x1, .L.str.34
	add	x1, x1, :lo12:.L.str.34
	b	.LBB19_10
.LBB19_8:
	str	xzr, [x19, #224]
.LBB19_9:
	adrp	x1, .L.str.35
	add	x1, x1, :lo12:.L.str.35
.LBB19_10:
	ldr	x0, [x19, #96]
	bl	_dev_err
	mov	w0, #-12
	b	.LBB19_5
.Lfunc_end19:
	.size	dfa_memory_alloc, .Lfunc_end19-dfa_memory_alloc
                                        // -- End function
	.globl	dfa_lrbp_configure              // -- Begin function dfa_lrbp_configure
	.p2align	2
	.type	dfa_lrbp_configure,@function
dfa_lrbp_configure:                     // @dfa_lrbp_configure
// %bb.0:
	stp	x29, x30, [sp, #-96]!           // 16-byte Folded Spill
	ldr	x8, [x0, #224]
	mov	w9, #216
	stp	x26, x25, [sp, #32]             // 16-byte Folded Spill
	adrp	x26, :got:liblinux_thread_offset
	stp	x22, x21, [sp, #64]             // 16-byte Folded Spill
	adrp	x14, :got:liblinux_thread_key
	umaddl	x22, w1, w9, x8
	stp	x28, x27, [sp, #16]             // 16-byte Folded Spill
	stp	x24, x23, [sp, #48]             // 16-byte Folded Spill
	//APP
	mrs	x25, TPIDR_EL0

	//NO_APP
	stp	x20, x19, [sp, #80]             // 16-byte Folded Spill
	mov	w10, #5120
	ldr	x26, [x26, :got_lo12:liblinux_thread_offset]
	mov	w11, #17408
	strb	wzr, [x22, #193]
	cmp	w1, #15
	mov	w12, #15
	csel	w23, w11, w10, eq
	ldr	w8, [x26]
	mov	x29, sp
	ldr	x14, [x14, :got_lo12:liblinux_thread_key]
	sub	x8, x25, x8
	ldr	w9, [x14]
	ldr	x8, [x8]
	ldr	x8, [x8, x9, lsl #3]
	ldr	x8, [x8, #24]
	ldr	w8, [x8, #8]
	strb	w12, [x22, #195]
	ldr	x9, [x22, #72]
	strb	w8, [x22, #194]
	cbz	x9, .LBB20_5
// %bb.1:
	ldr	x8, [x9, #272]
	cbz	x8, .LBB20_16
// %bb.2:
	ldr	x10, [x9, #224]
	mov	w19, w1
	mov	x20, x0
	ldrb	w11, [x10]
	mov	w10, #4
	cmp	w11, #65
	b.gt	.LBB20_17
// %bb.3:
	cmp	w11, #40
	b.eq	.LBB20_22
// %bb.4:
	mov	w8, wzr
	cmp	w11, #42
	b.eq	.LBB20_23
	b	.LBB20_24
.LBB20_5:
	mov	w19, w1
	mov	x20, x0
	mov	w8, wzr
.LBB20_6:
	ldr	w9, [x26]
	ldr	w10, [x14]
	ldr	x11, [x20, #7024]
	sub	x9, x25, x9
	ldr	x9, [x9]
	ldr	x28, [x9, x10, lsl #3]
	mov	w9, #80
	umaddl	x27, w8, w9, x11
	ldr	w8, [x28, #32]
	add	x21, x27, #56
	cbnz	w8, .LBB20_8
// %bb.7:
	ldrb	w8, [x28, #84]
	cbz	w8, .LBB20_19
.LBB20_8:
	ldr	w9, [x28, #32]
	mov	x8, xzr
	mov	w10, #1
	add	w9, w9, #1
	str	w9, [x28, #32]
	//APP
	//NO_APP
	//APP
	prfm	pstl1strm, [x21]
.Ltmp300:
	ldaxr	w1, [x21]
	eor	w9, w1, w8
	cbnz	w9, .Ltmp301
	stxr	w9, w10, [x21]
	cbnz	w9, .Ltmp300

.Ltmp301:
	//NO_APP
	cbnz	w1, .LBB20_35
.LBB20_9:
	ldr	x8, [x27, #48]
	ldr	w9, [x27, #36]
	add	x8, x8, x9, lsl #3
	add	x8, x8, #2272
	//APP
.Ltmp302:
	ldr	w9, [x8]
.Ltmp303:
	.section	.altinstructions,"a",@progbits
.Ltmp304:
	.word	.Ltmp302-.Ltmp304
.Ltmp306:
	.word	.Ltmp305-.Ltmp306
	.hword	1
	.byte	.Ltmp303-.Ltmp302
	.byte	.Ltmp307-.Ltmp305
	.text

	.text	1
.Ltmp305:
	ldar	w9, [x8]
.Ltmp307:
.Ltmp308:
.org (.Ltmp308-(.Ltmp307-.Ltmp305))+(.Ltmp303-.Ltmp302), 0
.Ltmp309:
.org (.Ltmp309-(.Ltmp303-.Ltmp302))+(.Ltmp307-.Ltmp305), 0
	.text


	//NO_APP
	mov	w8, w9
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp310:
	cbnz	x8, .Ltmp310
	//NO_APP
	lsr	w13, w9, #6
	ldr	w8, [x27, #60]
	ldr	w10, [x27, #32]
	str	w13, [x27, #64]
	add	w11, w8, #1
	udiv	w12, w11, w10
	msub	w9, w12, w10, w11
	cmp	w9, w13
	b.eq	.LBB20_30
// %bb.10:
	str	w9, [x27, #60]
	mov	w10, wzr
	add	x11, x27, #56
	ldr	x21, [x27]
	//APP
	stlrb	w10, [x11]
	//NO_APP
	//APP
	//NO_APP
	ldr	w9, [x26]
	ldr	w10, [x14]
	sub	x9, x25, x9
	ldr	x9, [x9]
	ldr	x24, [x9, x10, lsl #3]
	ldr	w9, [x24, #32]
	sub	w9, w9, #1
	str	w9, [x24, #32]
	ldr	w9, [x24, #32]
	cbnz	w9, .LBB20_13
// %bb.11:
	ldrb	w9, [x24, #84]
	cbnz	w9, .LBB20_13
// %bb.12:
	ldr	x25, [x24, #24]
	adrp	x10, :got:__preempt_disabled
	mov	w12, #1
	ldr	w9, [x25, #8]
	ldr	x10, [x10, :got_lo12:__preempt_disabled]
	lsl	x9, x9, #2
	ldr	w11, [x10, x9]
	sub	w11, w11, #1
	str	w11, [x10, x9]
	str	w12, [x25, #4]
	ldr	w9, [x25, #12]
	cbnz	w9, .LBB20_37
.LBB20_13:
	ldr	w9, [x24, #32]
	tbnz	w9, #31, .LBB20_38
.LBB20_14:
	ldr	x9, [x20, #304]
	mov	w11, #5120
	ldr	x10, [x20, #64]
	lsl	w8, w8, #6
	add	x8, x21, x8
	mov	w12, #16896
	add	x9, x11, x9, lsl #8
	mov	w11, w19
	mov	w13, #512
	mov	w0, wzr
	madd	x9, x9, x11, x10
	mov	w10, #271
	movk	w10, #1023, lsl #16
	str	w10, [x8, #8]
	and	w10, w19, #0xffff
	str	x9, [x8, #16]
	mov	w9, #4608
	ldr	w11, [x20, #708]
	cmp	w10, #15
	csel	w9, w12, w9, eq
	mov	w10, #128
	lsr	w12, w9, #2
	tst	w11, #0x200
	lsr	w11, w23, #2
	csel	w10, w10, w13, eq
	csel	w11, w11, w23, eq
	csel	w9, w12, w9, eq
	strh	w10, [x8, #26]
	strh	w11, [x8, #30]
	strh	w9, [x8, #24]
	str	x8, [x22, #184]
.LBB20_15:
	ldp	x20, x19, [sp, #80]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #64]             // 16-byte Folded Reload
	ldp	x24, x23, [sp, #48]             // 16-byte Folded Reload
	ldp	x26, x25, [sp, #32]             // 16-byte Folded Reload
	ldp	x28, x27, [sp, #16]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #96             // 16-byte Folded Reload
	ret
.LBB20_16:
	mov	w19, w1
	mov	x20, x0
	b	.LBB20_6
.LBB20_17:
	cmp	w11, #66
	b.eq	.LBB20_23
// %bb.18:
	mov	w8, wzr
	cmp	w11, #181
	b.eq	.LBB20_23
	b	.LBB20_24
.LBB20_19:
	ldr	x24, [x28, #24]
	mov	w8, #2
	adrp	x11, :got:__preempt_disabled
	str	w8, [x24, #4]
	//APP
	//NO_APP
	ldr	w8, [x24, #8]
	ldr	x11, [x11, :got_lo12:__preempt_disabled]
	ldr	w9, [x11, x8, lsl #2]
	cmp	w9, #0
	b.gt	.LBB20_36
// %bb.20:
	ldr	w10, [x24, #4]
	cmp	w10, #3
	b.eq	.LBB20_36
.LBB20_21:
	add	w9, w9, #1
	str	w9, [x11, x8, lsl #2]
	b	.LBB20_8
.LBB20_22:
	mov	w10, #3
.LBB20_23:
	mov	w8, w10
	strb	w10, [x22, #193]
.LBB20_24:
	ldrb	w10, [x22, #108]
	cmp	w10, #1
	b.hi	.LBB20_26
// %bb.25:
	mov	w8, wzr
	strb	wzr, [x22, #193]
.LBB20_26:
	ldr	x10, [x9, #224]
	ldrb	w10, [x10]
	sub	w11, w10, #59
	cmp	w11, #1
	b.hi	.LBB20_28
// %bb.27:
	strb	wzr, [x22, #193]
	mov	w8, wzr
	ldr	x9, [x9, #224]
	ldrb	w10, [x9]
.LBB20_28:
	cmp	w10, #27
	b.ne	.LBB20_6
// %bb.29:
	mov	w8, wzr
	strb	wzr, [x22, #193]
	b	.LBB20_6
.LBB20_30:
	mov	w8, wzr
	//APP
	stlrb	w8, [x21]
	//NO_APP
	//APP
	//NO_APP
	ldr	w8, [x26]
	ldr	w9, [x14]
	sub	x8, x25, x8
	ldr	x8, [x8]
	ldr	x21, [x8, x9, lsl #3]
	ldr	w8, [x21, #32]
	sub	w8, w8, #1
	str	w8, [x21, #32]
	ldr	w8, [x21, #32]
	cbnz	w8, .LBB20_33
// %bb.31:
	ldrb	w8, [x21, #84]
	cbnz	w8, .LBB20_33
// %bb.32:
	ldr	x22, [x21, #24]
	adrp	x9, :got:__preempt_disabled
	mov	w11, #1
	ldr	w8, [x22, #8]
	ldr	x9, [x9, :got_lo12:__preempt_disabled]
	lsl	x8, x8, #2
	ldr	w10, [x9, x8]
	sub	w10, w10, #1
	str	w10, [x9, x8]
	str	w11, [x22, #4]
	ldr	w8, [x22, #12]
	cbnz	w8, .LBB20_40
.LBB20_33:
	ldr	w8, [x21, #32]
	tbnz	w8, #31, .LBB20_41
.LBB20_34:
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.36
	add	x1, x1, :lo12:.L.str.36
	mov	w2, w19
	bl	_dev_err
	mov	w0, #-1
	b	.LBB20_15
.LBB20_35:
	mov	x0, x21
	mov	x24, x14
	bl	queued_spin_lock_slowpath
	mov	x14, x24
	b	.LBB20_9
.LBB20_36:
	bl	kernel_thread_preempt_disable
	adrp	x11, :got:__preempt_disabled
	adrp	x14, :got:liblinux_thread_key
	ldr	x11, [x11, :got_lo12:__preempt_disabled]
	ldr	x14, [x14, :got_lo12:liblinux_thread_key]
	ldr	w8, [x24, #8]
	ldr	w9, [x11, x8, lsl #2]
	b	.LBB20_21
.LBB20_37:
	mov	w26, w8
	bl	kernel_thread_preempt_enable
	mov	w8, w26
	str	wzr, [x25, #12]
	ldr	w9, [x24, #32]
	tbz	w9, #31, .LBB20_14
.LBB20_38:
	str	wzr, [x24, #32]
.LBB20_39:
	adrp	x0, .L.str.9
	add	x0, x0, :lo12:.L.str.9
	adrp	x1, .L.str.10
	add	x1, x1, :lo12:.L.str.10
	adrp	x3, .L__func__.__preempt_count_sub
	add	x3, x3, :lo12:.L__func__.__preempt_count_sub
	mov	w2, #112
	bl	printk
	adrp	x0, .L.str.11
	add	x0, x0, :lo12:.L.str.11
	bl	panic
.LBB20_40:
	bl	kernel_thread_preempt_enable
	str	wzr, [x22, #12]
	ldr	w8, [x21, #32]
	tbz	w8, #31, .LBB20_34
.LBB20_41:
	str	wzr, [x21, #32]
	b	.LBB20_39
.Lfunc_end20:
	.size	dfa_lrbp_configure, .Lfunc_end20-dfa_lrbp_configure
                                        // -- End function
	.globl	dfa_host_memory_configure       // -- Begin function dfa_host_memory_configure
	.p2align	2
	.type	dfa_host_memory_configure,@function
dfa_host_memory_configure:              // @dfa_host_memory_configure
// %bb.0:
	ldr	w8, [x0, #264]
	cmp	w8, #1
	b.lt	.LBB21_8
// %bb.1:
	mov	w8, #7000
	mov	x9, xzr
	mov	w11, #192
	ldrb	w10, [x0, x8]
	ldr	x8, [x0, #64]
.LBB21_2:                               // =>This Inner Loop Header: Depth=1
	ldr	x12, [x0, #224]
	add	x9, x9, #1
	strb	w10, [x12, x11]
	add	x11, x11, #216
	ldrsw	x12, [x0, #264]
	cmp	x9, x12
	b.lt	.LBB21_2
// %bb.3:
	cmp	w12, #1
	b.lt	.LBB21_8
// %bb.4:
	mov	w11, #5120
	mov	x9, xzr
	mov	x10, xzr
	mov	w12, #64
	mov	w13, #17408
	mov	w14, w11
	b	.LBB21_6
.LBB21_5:                               //   in Loop: Header=BB21_6 Depth=1
	ldrsw	x15, [x0, #264]
	add	x10, x10, #1
	add	x12, x12, #216
	add	x9, x9, #64
	cmp	x10, x15
	b.ge	.LBB21_8
.LBB21_6:                               // =>This Inner Loop Header: Depth=1
	ldr	x15, [x0, #72]
	and	w2, w10, #0xffff
	ldr	x16, [x0, #224]
	cmp	w2, #15
	ldr	x17, [x0, #304]
	csel	w14, w13, w14, eq
	add	x15, x9, x15
	ldr	x1, [x0, #32]
	add	x16, x16, x12
	add	x17, x11, x17, lsl #8
	stur	x15, [x16, #-24]
	mul	x16, x17, x2
	ldr	x15, [x0, #224]
	add	x17, x16, x8
	add	x15, x15, x12
	stur	x17, [x15, #-16]
	add	x15, x1, x16
	ldr	x3, [x0, #224]
	add	x1, x17, #512
	add	x17, x17, w14, uxth
	add	x16, x3, x12
	stur	x15, [x16, #-48]
	ldr	x16, [x0, #224]
	add	x16, x16, x12
	stur	x1, [x16, #-8]
	add	x16, x15, #512
	ldr	x1, [x0, #224]
	add	x1, x1, x12
	stur	x16, [x1, #-40]
	ldr	x1, [x0, #224]
	str	x17, [x1, x12]
	add	x1, x15, x11
	ldr	x17, [x0, #224]
	add	x17, x17, x12
	stur	x1, [x17, #-32]
	b.ne	.LBB21_5
// %bb.7:                               //   in Loop: Header=BB21_6 Depth=1
	ldr	x17, [x0, #224]
	add	x15, x15, x13
	add	x17, x17, x12
	stur	x16, [x17, #-40]
	ldr	x16, [x0, #224]
	add	x16, x16, x12
	stur	x15, [x16, #-32]
	b	.LBB21_5
.LBB21_8:
	ret
.Lfunc_end21:
	.size	dfa_host_memory_configure, .Lfunc_end21-dfa_host_memory_configure
                                        // -- End function
	.globl	dfa_send_command                // -- Begin function dfa_send_command
	.p2align	2
	.type	dfa_send_command,@function
dfa_send_command:                       // @dfa_send_command
// %bb.0:
	stp	x29, x30, [sp, #-80]!           // 16-byte Folded Spill
	ldr	x8, [x0, #224]
	mov	w9, #216
	stp	x26, x25, [sp, #16]             // 16-byte Folded Spill
	mov	x29, sp
	stp	x24, x23, [sp, #32]             // 16-byte Folded Spill
	stp	x22, x21, [sp, #48]             // 16-byte Folded Spill
	nop
	umaddl	x21, w1, w9, x8
	stp	x20, x19, [sp, #64]             // 16-byte Folded Spill
	mov	w20, w1
	mov	x19, x0
	bl	ktime_get
	mov	w9, #255
	mov	w10, #1
	ldr	x8, [x21, #72]
	strb	wzr, [x21, #128]
	stp	x0, xzr, [x21, #112]
	str	w9, [x21, #132]
	strb	w10, [x21, #177]
	cbz	x8, .LBB22_3
// %bb.1:
	ldr	x9, [x8, #224]
	ldrb	w9, [x9]
	strb	w9, [x21, #128]
	ldr	x8, [x8, #272]
	cbz	x8, .LBB22_3
// %bb.2:
	ldr	w8, [x8, #32]
	str	w8, [x21, #132]
.LBB22_3:
	cmp	w20, #15
	b.eq	.LBB22_7
// %bb.4:
	add	w8, w20, #63
	cmp	w20, #0
	csel	w8, w8, w20, lt
	mov	w11, #7040
	asr	w9, w8, #6
	and	w8, w8, #0xffffffc0
	sub	w8, w20, w8
	mov	w12, #1
	add	x10, x19, w9, sxtw #3
	lsr	w9, w8, #6
	add	x10, x10, x11
	lsl	x8, x12, x8
	ldr	x11, [x10, w9, uxtw #3]
	tst	x11, x8
	b.ne	.LBB22_18
// %bb.5:
	add	x9, x10, x9, lsl #3
	//APP
	// atomic64_fetch_or
	prfm	pstl1strm, [x9]
.Ltmp311:
	ldxr	x10, [x9]
	orr	x11, x10, x8
	stlxr	w12, x11, [x9]
	cbnz	w12, .Ltmp311
	dmb	ish
	//NO_APP
	tst	x10, x8
	b.ne	.LBB22_18
.LBB22_6:
	mov	w8, #7072
	add	x8, x19, x8
	//APP
	// atomic_add
	prfm	pstl1strm, [x8]
.Ltmp312:
	ldxr	w9, [x8]
	add	w9, w9, #1
	stxr	w10, w9, [x8]
	cbnz	w10, .Ltmp312

	//NO_APP
.LBB22_7:
	mov	x0, x19
	bl	ufs_send_cmd_status_sync
	adrp	x22, :got:liblinux_thread_offset
	adrp	x23, :got:liblinux_thread_key
	//APP
	mrs	x20, TPIDR_EL0

	//NO_APP
	ldr	x22, [x22, :got_lo12:liblinux_thread_offset]
	ldr	w8, [x22]
	ldr	x23, [x23, :got_lo12:liblinux_thread_key]
	ldr	x9, [x19, #7024]
	sub	x8, x20, x8
	ldrb	w11, [x21, #193]
	ldr	w10, [x23]
	ldr	x8, [x8]
	ldr	x24, [x8, x10, lsl #3]
	mov	w8, #80
	madd	x21, x11, x8, x9
	ldr	w8, [x24, #32]
	add	x19, x21, #56
	cbnz	w8, .LBB22_9
// %bb.8:
	ldrb	w8, [x24, #84]
	cbz	w8, .LBB22_15
.LBB22_9:
	ldr	w9, [x24, #32]
	mov	x8, xzr
	mov	w10, #1
	add	w9, w9, #1
	str	w9, [x24, #32]
	//APP
	//NO_APP
	//APP
	prfm	pstl1strm, [x19]
.Ltmp313:
	ldaxr	w1, [x19]
	eor	w9, w1, w8
	cbnz	w9, .Ltmp314
	stxr	w9, w10, [x19]
	cbnz	w9, .Ltmp313

.Ltmp314:
	//NO_APP
	cbnz	w1, .LBB22_19
.LBB22_10:
	//APP
	dmb	oshst
	//NO_APP
	mov	w8, wzr
	ldr	x10, [x21, #48]
	ldr	w11, [x21, #36]
	ldr	w9, [x21, #60]
	add	x10, x10, x11, lsl #3
	add	x11, x21, #56
	lsl	w9, w9, #6
	add	x10, x10, #2276
	//APP
	str	w9, [x10]
	//NO_APP
	//APP
	stlrb	w8, [x11]
	//NO_APP
	//APP
	//NO_APP
	ldr	w8, [x22]
	ldr	w9, [x23]
	sub	x8, x20, x8
	ldr	x8, [x8]
	ldr	x19, [x8, x9, lsl #3]
	ldr	w8, [x19, #32]
	sub	w8, w8, #1
	str	w8, [x19, #32]
	ldr	w8, [x19, #32]
	cbnz	w8, .LBB22_13
// %bb.11:
	ldrb	w8, [x19, #84]
	cbnz	w8, .LBB22_13
// %bb.12:
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
	cbnz	w8, .LBB22_21
.LBB22_13:
	ldr	w8, [x19, #32]
	tbnz	w8, #31, .LBB22_22
.LBB22_14:
	//APP
	dsb	st
	//NO_APP
	ldp	x20, x19, [sp, #64]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #48]             // 16-byte Folded Reload
	ldp	x24, x23, [sp, #32]             // 16-byte Folded Reload
	ldp	x26, x25, [sp, #16]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #80             // 16-byte Folded Reload
	ret
.LBB22_15:
	ldr	x26, [x24, #24]
	mov	w8, #2
	adrp	x25, :got:__preempt_disabled
	str	w8, [x26, #4]
	//APP
	//NO_APP
	ldr	w8, [x26, #8]
	ldr	x25, [x25, :got_lo12:__preempt_disabled]
	ldr	w9, [x25, x8, lsl #2]
	cmp	w9, #0
	b.gt	.LBB22_20
// %bb.16:
	ldr	w10, [x26, #4]
	cmp	w10, #3
	b.eq	.LBB22_20
.LBB22_17:
	add	w9, w9, #1
	str	w9, [x25, x8, lsl #2]
	b	.LBB22_9
.LBB22_18:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.106
	add	x1, x1, :lo12:.L.str.106
	mov	w2, w20
	bl	_dev_err
	b	.LBB22_6
.LBB22_19:
	mov	x0, x19
	bl	queued_spin_lock_slowpath
	b	.LBB22_10
.LBB22_20:
	bl	kernel_thread_preempt_disable
	ldr	w8, [x26, #8]
	ldr	w9, [x25, x8, lsl #2]
	b	.LBB22_17
.LBB22_21:
	bl	kernel_thread_preempt_enable
	str	wzr, [x20, #12]
	ldr	w8, [x19, #32]
	tbz	w8, #31, .LBB22_14
.LBB22_22:
	adrp	x0, .L.str.9
	add	x0, x0, :lo12:.L.str.9
	adrp	x1, .L.str.10
	add	x1, x1, :lo12:.L.str.10
	adrp	x3, .L__func__.__preempt_count_sub
	add	x3, x3, :lo12:.L__func__.__preempt_count_sub
	mov	w2, #112
	str	wzr, [x19, #32]
	bl	printk
	adrp	x0, .L.str.11
	add	x0, x0, :lo12:.L.str.11
	bl	panic
.Lfunc_end22:
	.size	dfa_send_command, .Lfunc_end22-dfa_send_command
                                        // -- End function
	.globl	dfa_queuecommand                // -- Begin function dfa_queuecommand
	.p2align	2
	.type	dfa_queuecommand,@function
dfa_queuecommand:                       // @dfa_queuecommand
// %bb.0:
	stp	x29, x30, [sp, #-80]!           // 16-byte Folded Spill
	ldr	x8, [x1, #272]
	str	x25, [sp, #16]                  // 8-byte Folded Spill
	stp	x24, x23, [sp, #32]             // 16-byte Folded Spill
	mov	x29, sp
	stp	x22, x21, [sp, #48]             // 16-byte Folded Spill
	stp	x20, x19, [sp, #64]             // 16-byte Folded Spill
	ldr	w20, [x8, #32]
	ldr	w8, [x0, #9548]
	cbnz	w8, .LBB23_15
// %bb.1:
	add	w8, w20, #63
	cmp	w20, #0
	csel	w8, w8, w20, lt
	add	x19, x0, #2320
	asr	w8, w8, #6
	mov	x22, x0
	add	x8, x19, w8, sxtw #3
	ldr	x8, [x8, #7040]
	lsr	x8, x8, x20
	tbnz	w8, #0, .LBB23_20
// %bb.2:
	ldr	w8, [x0, #2528]
	cbnz	w8, .LBB23_14
// %bb.3:
	mov	w8, #7360
	ldrb	w8, [x22, x8]
	cbnz	w8, .LBB23_14
// %bb.4:
	mov	w8, w20
	ldr	x9, [x22, #2544]
	mov	w10, #216
	sxtw	x8, w8
	mov	x21, x1
	madd	x23, x8, x10, x9
	mov	w8, #96
	mov	w10, #49408
	str	x1, [x23, #72]
	str	w8, [x23, #88]
	ldr	x8, [x1, #280]
	str	wzr, [x23, #92]
	str	w20, [x23, #104]
	str	x8, [x23, #80]
	ldr	x8, [x1, #56]
	ldr	x8, [x8, #120]
	and	x9, x8, #0xff00
	cmp	x9, x10
	cset	w9, eq
	bfi	w8, w9, #7, #1
	mov	w9, #6868
	strb	w8, [x23, #108]
	ldrb	w8, [x22, x9]
	tbnz	w8, #4, .LBB23_9
.LBB23_5:
	mov	w8, #1
.LBB23_6:
	mov	x0, x19
	mov	x1, x21
	strb	w8, [x23, #109]
	strh	wzr, [x23, #176]
	bl	ufshcd_start_delay_work
	mov	w8, #9688
	add	x0, x22, x8
	bl	__raw_spin_lock_irqsave
	mov	x0, x19
	mov	w1, w20
	bl	dfa_lrbp_configure
	cbz	w0, .LBB23_11
// %bb.7:
	mov	w24, w0
	str	xzr, [x23, #72]
.LBB23_8:
	mov	x0, x19
	bl	dfa_irq_sche_unlock
	b	.LBB23_16
.LBB23_9:
	ldrb	w8, [x22, #3028]
	tbnz	w8, #0, .LBB23_5
// %bb.10:
	mov	w8, wzr
	b	.LBB23_6
.LBB23_11:
	mov	x0, x19
	mov	x1, x23
	bl	ufshcd_comp_scsi_upiu
	cbz	w0, .LBB23_17
// %bb.12:
	mov	w24, w0
	str	xzr, [x23, #72]
.LBB23_13:
	mov	x0, x19
	mov	x1, x23
	bl	dfa_pack_fail_rollback
	b	.LBB23_8
.LBB23_14:
	ldr	x0, [x22, #2416]
	adrp	x1, .L.str.38
	add	x1, x1, :lo12:.L.str.38
	bl	_dev_err
.LBB23_15:
	mov	w24, #4181
.LBB23_16:
	mov	w0, w24
	ldr	x25, [sp, #16]                  // 8-byte Folded Reload
	ldp	x20, x19, [sp, #64]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #48]             // 16-byte Folded Reload
	ldp	x24, x23, [sp, #32]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #80             // 16-byte Folded Reload
	ret
.LBB23_17:
	ldr	x25, [x23, #72]
	mov	x0, x25
	bl	scsi_dma_map
	tbnz	w0, #31, .LBB23_21
// %bb.18:
	cbnz	w0, .LBB23_22
// %bb.19:
	ldr	x8, [x23, #184]
	strh	wzr, [x8, #28]
	b	.LBB23_27
.LBB23_20:
	ldr	x8, [x1, #224]
	adrp	x1, .L.str.37
	add	x1, x1, :lo12:.L.str.37
	ldr	x0, [x22, #2416]
	mov	w3, w20
	ldrb	w2, [x8]
	bl	_dev_err
	b	.LBB23_15
.LBB23_21:
	mov	w24, w0
	mov	x0, x19
	mov	x1, x23
	str	xzr, [x23, #72]
	bl	ufstt_unprep_fn
	b	.LBB23_13
.LBB23_22:
	ldrb	w8, [x22, #3029]
	tbnz	w8, #1, .LBB23_24
// %bb.23:
	mov	w24, w0
	mov	w8, w0
	b	.LBB23_25
.LBB23_24:
	ldr	w8, [x22, #2624]
	mov	w24, w0
	mul	w8, w8, w0
.LBB23_25:
	ldr	x9, [x23, #184]
	strh	w8, [x9, #28]
	ldr	x0, [x25, #232]
	ldr	x23, [x23, #32]
.LBB23_26:                              // =>This Inner Loop Header: Depth=1
	ldr	x8, [x0, #16]
	str	w8, [x23]
	ldr	w8, [x0, #20]
	str	w8, [x23, #4]
	ldr	x8, [x22, #2624]
	add	x23, x23, x8
	bl	sg_next
	subs	w24, w24, #1
	b.ne	.LBB23_26
.LBB23_27:
	//APP
	dsb	st
	//NO_APP
	mov	x0, x19
	mov	x1, x21
	bl	ufshcd_check_disable_dev_tmt_cnt
	mov	x0, x19
	mov	w1, w20
	bl	dfa_send_command
	mov	w24, wzr
	b	.LBB23_8
.Lfunc_end23:
	.size	dfa_queuecommand, .Lfunc_end23-dfa_queuecommand
                                        // -- End function
	.p2align	2                               // -- Begin function dfa_pack_fail_rollback
	.type	dfa_pack_fail_rollback,@function
dfa_pack_fail_rollback:                 // @dfa_pack_fail_rollback
// %bb.0:
	stp	x29, x30, [sp, #-80]!           // 16-byte Folded Spill
	stp	x22, x21, [sp, #48]             // 16-byte Folded Spill
	adrp	x21, :got:liblinux_thread_offset
	stp	x26, x25, [sp, #16]             // 16-byte Folded Spill
	adrp	x22, :got:liblinux_thread_key
	stp	x24, x23, [sp, #32]             // 16-byte Folded Spill
	mov	x29, sp
	stp	x20, x19, [sp, #64]             // 16-byte Folded Spill
	//APP
	mrs	x20, TPIDR_EL0

	//NO_APP
	ldr	x21, [x21, :got_lo12:liblinux_thread_offset]
	ldr	w8, [x21]
	ldr	x22, [x22, :got_lo12:liblinux_thread_key]
	ldr	x9, [x0, #7024]
	sub	x8, x20, x8
	ldrb	w11, [x1, #193]
	ldr	w10, [x22]
	ldr	x8, [x8]
	ldr	x24, [x8, x10, lsl #3]
	mov	w8, #80
	madd	x23, x11, x8, x9
	ldr	w8, [x24, #32]
	add	x19, x23, #56
	cbnz	w8, .LBB24_2
// %bb.1:
	ldrb	w8, [x24, #84]
	cbz	w8, .LBB24_8
.LBB24_2:
	ldr	w9, [x24, #32]
	mov	x8, xzr
	mov	w10, #1
	add	w9, w9, #1
	str	w9, [x24, #32]
	//APP
	//NO_APP
	//APP
	prfm	pstl1strm, [x19]
.Ltmp315:
	ldaxr	w1, [x19]
	eor	w9, w1, w8
	cbnz	w9, .Ltmp316
	stxr	w9, w10, [x19]
	cbnz	w9, .Ltmp315

.Ltmp316:
	//NO_APP
	cbnz	w1, .LBB24_11
.LBB24_3:
	ldr	w9, [x23, #60]
	mov	w8, wzr
	ldr	w10, [x23, #32]
	add	w9, w9, w10
	sub	w9, w9, #1
	udiv	w11, w9, w10
	msub	w9, w11, w10, w9
	add	x10, x23, #56
	str	w9, [x23, #60]
	//APP
	stlrb	w8, [x10]
	//NO_APP
	//APP
	//NO_APP
	ldr	w8, [x21]
	ldr	w9, [x22]
	sub	x8, x20, x8
	ldr	x8, [x8]
	ldr	x19, [x8, x9, lsl #3]
	ldr	w8, [x19, #32]
	sub	w8, w8, #1
	str	w8, [x19, #32]
	ldr	w8, [x19, #32]
	cbnz	w8, .LBB24_6
// %bb.4:
	ldrb	w8, [x19, #84]
	cbnz	w8, .LBB24_6
// %bb.5:
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
	cbnz	w8, .LBB24_13
.LBB24_6:
	ldr	w8, [x19, #32]
	tbnz	w8, #31, .LBB24_14
.LBB24_7:
	ldp	x20, x19, [sp, #64]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #48]             // 16-byte Folded Reload
	ldp	x24, x23, [sp, #32]             // 16-byte Folded Reload
	ldp	x26, x25, [sp, #16]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #80             // 16-byte Folded Reload
	ret
.LBB24_8:
	ldr	x26, [x24, #24]
	mov	w8, #2
	adrp	x25, :got:__preempt_disabled
	str	w8, [x26, #4]
	//APP
	//NO_APP
	ldr	w8, [x26, #8]
	ldr	x25, [x25, :got_lo12:__preempt_disabled]
	ldr	w9, [x25, x8, lsl #2]
	cmp	w9, #0
	b.gt	.LBB24_12
// %bb.9:
	ldr	w10, [x26, #4]
	cmp	w10, #3
	b.eq	.LBB24_12
.LBB24_10:
	add	w9, w9, #1
	str	w9, [x25, x8, lsl #2]
	b	.LBB24_2
.LBB24_11:
	mov	x0, x19
	bl	queued_spin_lock_slowpath
	b	.LBB24_3
.LBB24_12:
	bl	kernel_thread_preempt_disable
	ldr	w8, [x26, #8]
	ldr	w9, [x25, x8, lsl #2]
	b	.LBB24_10
.LBB24_13:
	bl	kernel_thread_preempt_enable
	str	wzr, [x20, #12]
	ldr	w8, [x19, #32]
	tbz	w8, #31, .LBB24_7
.LBB24_14:
	adrp	x0, .L.str.9
	add	x0, x0, :lo12:.L.str.9
	adrp	x1, .L.str.10
	add	x1, x1, :lo12:.L.str.10
	adrp	x3, .L__func__.__preempt_count_sub
	add	x3, x3, :lo12:.L__func__.__preempt_count_sub
	mov	w2, #112
	str	wzr, [x19, #32]
	bl	printk
	adrp	x0, .L.str.11
	add	x0, x0, :lo12:.L.str.11
	bl	panic
.Lfunc_end24:
	.size	dfa_pack_fail_rollback, .Lfunc_end24-dfa_pack_fail_rollback
                                        // -- End function
	.p2align	2                               // -- Begin function dfa_irq_sche_unlock
	.type	dfa_irq_sche_unlock,@function
dfa_irq_sche_unlock:                    // @dfa_irq_sche_unlock
// %bb.0:
	sub	sp, sp, #48
	mov	w9, #7368
	stp	x29, x30, [sp, #16]             // 16-byte Folded Spill
	add	x10, x0, x9
	adrp	x9, :got:liblinux_thread_offset
	str	x19, [sp, #32]                  // 8-byte Folded Spill
	mov	w8, wzr
	add	x29, sp, #16
	ldr	x9, [x9, :got_lo12:liblinux_thread_offset]
	//APP
	stlrb	w8, [x10]
	//NO_APP
	adrp	x10, :got:liblinux_thread_key
	//APP
	mrs	x8, TPIDR_EL0

	//NO_APP
	ldr	w11, [x9]
	ldr	x10, [x10, :got_lo12:liblinux_thread_key]
	sub	x11, x8, x11
	ldr	w12, [x10]
	ldr	x11, [x11]
	ldr	x11, [x11, x12, lsl #3]
	ldrb	w12, [x11, #84]
	cbnz	w12, .LBB25_7
// %bb.1:
	ldr	w12, [x11, #36]
	cbz	w12, .LBB25_12
// %bb.2:
	//APP
	//NO_APP
	ldr	w12, [x9]
	ldr	w13, [x10]
	sub	x12, x8, x12
	ldr	x12, [x12]
	ldr	x12, [x12, x13, lsl #3]
	ldr	w13, [x12, #32]
	sub	w13, w13, #1
	str	w13, [x12, #32]
	ldr	w13, [x12, #32]
	cbnz	w13, .LBB25_5
// %bb.3:
	ldrb	w13, [x12, #84]
	cbnz	w13, .LBB25_5
// %bb.4:
	ldr	x13, [x12, #24]
	adrp	x15, :got:__preempt_disabled
	mov	w17, #1
	ldr	w14, [x13, #8]
	ldr	x15, [x15, :got_lo12:__preempt_disabled]
	lsl	x14, x14, #2
	ldr	w16, [x15, x14]
	sub	w16, w16, #1
	str	w16, [x15, x14]
	str	w17, [x13, #4]
	ldr	w14, [x13, #12]
	cbnz	w14, .LBB25_15
.LBB25_5:
	ldr	w13, [x12, #32]
	tbnz	w13, #31, .LBB25_16
.LBB25_6:
	str	wzr, [x11, #36]
.LBB25_7:
	//APP
	//NO_APP
	ldr	w9, [x9]
	sub	x8, x8, x9
	ldr	w9, [x10]
	ldr	x8, [x8]
	ldr	x8, [x8, x9, lsl #3]
	ldr	w9, [x8, #32]
	sub	w9, w9, #1
	str	w9, [x8, #32]
	ldr	w9, [x8, #32]
	cbnz	w9, .LBB25_10
// %bb.8:
	ldrb	w9, [x8, #84]
	cbnz	w9, .LBB25_10
// %bb.9:
	ldr	x9, [x8, #24]
	adrp	x11, :got:__preempt_disabled
	mov	w13, #1
	ldr	w10, [x9, #8]
	ldr	x11, [x11, :got_lo12:__preempt_disabled]
	lsl	x10, x10, #2
	ldr	w12, [x11, x10]
	sub	w12, w12, #1
	str	w12, [x11, x10]
	str	w13, [x9, #4]
	ldr	w10, [x9, #12]
	cbnz	w10, .LBB25_13
.LBB25_10:
	ldr	w9, [x8, #32]
	tbnz	w9, #31, .LBB25_14
.LBB25_11:
	ldp	x29, x30, [sp, #16]             // 16-byte Folded Reload
	ldr	x19, [sp, #32]                  // 8-byte Folded Reload
	add	sp, sp, #48
	ret
.LBB25_12:
	adrp	x0, .L.str.9
	add	x0, x0, :lo12:.L.str.9
	adrp	x1, .L.str.10
	add	x1, x1, :lo12:.L.str.10
	adrp	x3, .L__func__.arch_local_irq_restore
	add	x3, x3, :lo12:.L__func__.arch_local_irq_restore
	mov	w2, #77
	b	.LBB25_18
.LBB25_13:
	str	x8, [x29, #24]                  // 8-byte Folded Spill
	mov	x19, x9
	bl	kernel_thread_preempt_enable
	ldr	x8, [x29, #24]                  // 8-byte Folded Reload
	str	wzr, [x19, #12]
	ldr	w9, [x8, #32]
	tbz	w9, #31, .LBB25_11
.LBB25_14:
	str	wzr, [x8, #32]
	b	.LBB25_17
.LBB25_15:
	str	x8, [x29, #24]                  // 8-byte Folded Spill
	mov	x19, x13
	stp	x11, x12, [sp]                  // 16-byte Folded Spill
	bl	kernel_thread_preempt_enable
	adrp	x10, :got:liblinux_thread_key
	adrp	x9, :got:liblinux_thread_offset
	ldp	x11, x12, [sp]                  // 16-byte Folded Reload
	ldr	x10, [x10, :got_lo12:liblinux_thread_key]
	ldr	x9, [x9, :got_lo12:liblinux_thread_offset]
	ldr	x8, [x29, #24]                  // 8-byte Folded Reload
	str	wzr, [x19, #12]
	ldr	w13, [x12, #32]
	tbz	w13, #31, .LBB25_6
.LBB25_16:
	str	wzr, [x12, #32]
.LBB25_17:
	adrp	x0, .L.str.9
	add	x0, x0, :lo12:.L.str.9
	adrp	x1, .L.str.10
	add	x1, x1, :lo12:.L.str.10
	adrp	x3, .L__func__.__preempt_count_sub
	add	x3, x3, :lo12:.L__func__.__preempt_count_sub
	mov	w2, #112
.LBB25_18:
	bl	printk
	adrp	x0, .L.str.11
	add	x0, x0, :lo12:.L.str.11
	bl	panic
.Lfunc_end25:
	.size	dfa_irq_sche_unlock, .Lfunc_end25-dfa_irq_sche_unlock
                                        // -- End function
	.globl	dfa_reset_dev_cmd_complete      // -- Begin function dfa_reset_dev_cmd_complete
	.p2align	2
	.type	dfa_reset_dev_cmd_complete,@function
dfa_reset_dev_cmd_complete:             // @dfa_reset_dev_cmd_complete
// %bb.0:
	sub	sp, sp, #64
	ldr	x8, [x0, #7024]
	mov	w10, #80
	ldrb	w9, [x2, #193]
	stp	x20, x19, [sp, #48]             // 16-byte Folded Spill
	mov	x19, x0
	stp	x29, x30, [sp, #16]             // 16-byte Folded Spill
	str	x21, [sp, #32]                  // 8-byte Folded Spill
	add	x29, sp, #16
	madd	x8, x9, x10, x8
	add	x20, x8, #68
	mov	x0, x20
	bl	__raw_spin_lock_irqsave
	adrp	x8, :got:liblinux_thread_offset
	mov	w9, wzr
	ldr	x8, [x8, :got_lo12:liblinux_thread_offset]
	str	xzr, [x19, #3920]
	//APP
	stlrb	w9, [x20]
	//NO_APP
	adrp	x20, :got:liblinux_thread_key
	//APP
	mrs	x19, TPIDR_EL0

	//NO_APP
	ldr	w9, [x8]
	ldr	x20, [x20, :got_lo12:liblinux_thread_key]
	sub	x9, x19, x9
	ldr	w10, [x20]
	ldr	x9, [x9]
	ldr	x9, [x9, x10, lsl #3]
	ldrb	w10, [x9, #84]
	cbnz	w10, .LBB26_3
// %bb.1:
	ldr	w10, [x9, #36]
	cbz	w10, .LBB26_13
// %bb.2:
	cmp	x0, #1
	b.ne	.LBB26_8
.LBB26_3:
	//APP
	//NO_APP
	ldr	w8, [x8]
	ldr	w9, [x20]
	sub	x8, x19, x8
	ldr	x8, [x8]
	ldr	x19, [x8, x9, lsl #3]
	ldr	w8, [x19, #32]
	sub	w8, w8, #1
	str	w8, [x19, #32]
	ldr	w8, [x19, #32]
	cbnz	w8, .LBB26_6
// %bb.4:
	ldrb	w8, [x19, #84]
	cbnz	w8, .LBB26_6
// %bb.5:
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
	cbnz	w8, .LBB26_14
.LBB26_6:
	ldr	w8, [x19, #32]
	tbnz	w8, #31, .LBB26_15
.LBB26_7:
	ldp	x20, x19, [sp, #48]             // 16-byte Folded Reload
	ldp	x29, x30, [sp, #16]             // 16-byte Folded Reload
	ldr	x21, [sp, #32]                  // 8-byte Folded Reload
	add	sp, sp, #64
	ret
.LBB26_8:
	//APP
	//NO_APP
	ldr	w10, [x8]
	ldr	w11, [x20]
	sub	x10, x19, x10
	ldr	x10, [x10]
	ldr	x10, [x10, x11, lsl #3]
	ldr	w11, [x10, #32]
	sub	w11, w11, #1
	str	w11, [x10, #32]
	ldr	w11, [x10, #32]
	cbnz	w11, .LBB26_11
// %bb.9:
	ldrb	w11, [x10, #84]
	cbnz	w11, .LBB26_11
// %bb.10:
	ldr	x11, [x10, #24]
	adrp	x13, :got:__preempt_disabled
	mov	w15, #1
	ldr	w12, [x11, #8]
	ldr	x13, [x13, :got_lo12:__preempt_disabled]
	lsl	x12, x12, #2
	ldr	w14, [x13, x12]
	sub	w14, w14, #1
	str	w14, [x13, x12]
	str	w15, [x11, #4]
	ldr	w12, [x11, #12]
	cbnz	w12, .LBB26_18
.LBB26_11:
	ldr	w11, [x10, #32]
	tbnz	w11, #31, .LBB26_19
.LBB26_12:
	str	wzr, [x9, #36]
	b	.LBB26_3
.LBB26_13:
	adrp	x0, .L.str.9
	add	x0, x0, :lo12:.L.str.9
	adrp	x1, .L.str.10
	add	x1, x1, :lo12:.L.str.10
	adrp	x3, .L__func__.arch_local_irq_restore
	add	x3, x3, :lo12:.L__func__.arch_local_irq_restore
	mov	w2, #77
	b	.LBB26_17
.LBB26_14:
	bl	kernel_thread_preempt_enable
	str	wzr, [x20, #12]
	ldr	w8, [x19, #32]
	tbz	w8, #31, .LBB26_7
.LBB26_15:
	str	wzr, [x19, #32]
.LBB26_16:
	adrp	x0, .L.str.9
	add	x0, x0, :lo12:.L.str.9
	adrp	x1, .L.str.10
	add	x1, x1, :lo12:.L.str.10
	adrp	x3, .L__func__.__preempt_count_sub
	add	x3, x3, :lo12:.L__func__.__preempt_count_sub
	mov	w2, #112
.LBB26_17:
	bl	printk
	adrp	x0, .L.str.11
	add	x0, x0, :lo12:.L.str.11
	bl	panic
.LBB26_18:
	str	x9, [sp, #8]                    // 8-byte Folded Spill
	mov	x21, x11
	str	x10, [x29, #24]                 // 8-byte Folded Spill
	bl	kernel_thread_preempt_enable
	adrp	x8, :got:liblinux_thread_offset
	ldr	x10, [x29, #24]                 // 8-byte Folded Reload
	ldr	x9, [sp, #8]                    // 8-byte Folded Reload
	ldr	x8, [x8, :got_lo12:liblinux_thread_offset]
	str	wzr, [x21, #12]
	ldr	w11, [x10, #32]
	tbz	w11, #31, .LBB26_12
.LBB26_19:
	str	wzr, [x10, #32]
	b	.LBB26_16
.Lfunc_end26:
	.size	dfa_reset_dev_cmd_complete, .Lfunc_end26-dfa_reset_dev_cmd_complete
                                        // -- End function
	.p2align	2                               // -- Begin function __raw_spin_lock_irqsave
	.type	__raw_spin_lock_irqsave,@function
__raw_spin_lock_irqsave:                // @__raw_spin_lock_irqsave
// %bb.0:
	stp	x29, x30, [sp, #-64]!           // 16-byte Folded Spill
	adrp	x9, :got:liblinux_thread_offset
	str	x23, [sp, #16]                  // 8-byte Folded Spill
	stp	x22, x21, [sp, #32]             // 16-byte Folded Spill
	adrp	x10, :got:liblinux_thread_key
	stp	x20, x19, [sp, #48]             // 16-byte Folded Spill
	//APP
	mrs	x8, TPIDR_EL0

	//NO_APP
	ldr	x9, [x9, :got_lo12:liblinux_thread_offset]
	mov	x29, sp
	ldr	w11, [x9]
	ldr	x10, [x10, :got_lo12:liblinux_thread_key]
	sub	x13, x8, x11
	ldr	w12, [x10]
	ldr	x11, [x13]
	ldr	x23, [x11, x12, lsl #3]
	ldrb	w14, [x23, #84]
	cbnz	w14, .LBB27_4
// %bb.1:
	ldr	w20, [x23, #36]
	cbnz	w20, .LBB27_5
// %bb.2:
	mov	x19, x0
	ldr	w12, [x23, #32]
	cbz	w12, .LBB27_14
.LBB27_3:
	ldr	w12, [x23, #32]
	mov	w13, #1
	add	w12, w12, #1
	str	w12, [x23, #32]
	//APP
	//NO_APP
	str	w13, [x23, #36]
	ldr	w9, [x9]
	ldr	w12, [x10]
	sub	x13, x8, x9
	b	.LBB27_6
.LBB27_4:
	mov	x19, x0
	mov	x20, xzr
	b	.LBB27_7
.LBB27_5:
	mov	x19, x0
.LBB27_6:
	cmp	w20, #0
	cset	w20, ne
.LBB27_7:
	ldr	x8, [x13]
	ldr	x21, [x8, x12, lsl #3]
	ldr	w8, [x21, #32]
	cbnz	w8, .LBB27_9
// %bb.8:
	ldrb	w8, [x21, #84]
	cbz	w8, .LBB27_11
.LBB27_9:
	ldr	w9, [x21, #32]
	mov	x8, xzr
	mov	w10, #1
	add	w9, w9, #1
	str	w9, [x21, #32]
	//APP
	//NO_APP
	//APP
	prfm	pstl1strm, [x19]
.Ltmp317:
	ldaxr	w1, [x19]
	eor	w9, w1, w8
	cbnz	w9, .Ltmp318
	stxr	w9, w10, [x19]
	cbnz	w9, .Ltmp317

.Ltmp318:
	//NO_APP
	cbnz	w1, .LBB27_17
.LBB27_10:
	mov	x0, x20
	ldr	x23, [sp, #16]                  // 8-byte Folded Reload
	ldp	x20, x19, [sp, #48]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #32]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #64             // 16-byte Folded Reload
	ret
.LBB27_11:
	ldr	x23, [x21, #24]
	mov	w8, #2
	adrp	x22, :got:__preempt_disabled
	str	w8, [x23, #4]
	//APP
	//NO_APP
	ldr	w8, [x23, #8]
	ldr	x22, [x22, :got_lo12:__preempt_disabled]
	ldr	w9, [x22, x8, lsl #2]
	cmp	w9, #0
	b.gt	.LBB27_18
// %bb.12:
	ldr	w10, [x23, #4]
	cmp	w10, #3
	b.eq	.LBB27_18
.LBB27_13:
	add	w9, w9, #1
	str	w9, [x22, x8, lsl #2]
	b	.LBB27_9
.LBB27_14:
	ldr	x22, [x23, #24]
	mov	w12, #2
	adrp	x21, :got:__preempt_disabled
	str	w12, [x22, #4]
	//APP
	//NO_APP
	ldr	w12, [x22, #8]
	ldr	x21, [x21, :got_lo12:__preempt_disabled]
	ldr	w13, [x21, x12, lsl #2]
	cmp	w13, #0
	b.gt	.LBB27_19
// %bb.15:
	ldr	w14, [x22, #4]
	cmp	w14, #3
	b.eq	.LBB27_19
.LBB27_16:
	add	w13, w13, #1
	str	w13, [x21, x12, lsl #2]
	b	.LBB27_3
.LBB27_17:
	mov	x0, x19
	bl	queued_spin_lock_slowpath
	b	.LBB27_10
.LBB27_18:
	bl	kernel_thread_preempt_disable
	ldr	w8, [x23, #8]
	ldr	w9, [x22, x8, lsl #2]
	b	.LBB27_13
.LBB27_19:
	str	x8, [x29, #24]                  // 8-byte Folded Spill
	bl	kernel_thread_preempt_disable
	adrp	x10, :got:liblinux_thread_key
	adrp	x9, :got:liblinux_thread_offset
	ldr	x10, [x10, :got_lo12:liblinux_thread_key]
	ldr	x9, [x9, :got_lo12:liblinux_thread_offset]
	ldr	x8, [x29, #24]                  // 8-byte Folded Reload
	ldr	w12, [x22, #8]
	ldr	w13, [x21, x12, lsl #2]
	b	.LBB27_16
.Lfunc_end27:
	.size	__raw_spin_lock_irqsave, .Lfunc_end27-__raw_spin_lock_irqsave
                                        // -- End function
	.globl	dfa_exec_dev_cmd                // -- Begin function dfa_exec_dev_cmd
	.p2align	2
	.type	dfa_exec_dev_cmd,@function
dfa_exec_dev_cmd:                       // @dfa_exec_dev_cmd
// %bb.0:
	sub	sp, sp, #144
	adrp	x8, :got:__stack_chk_guard
	stp	x29, x30, [sp, #80]             // 16-byte Folded Spill
	stp	x24, x23, [sp, #96]             // 16-byte Folded Spill
	add	x29, sp, #80
	stp	x22, x21, [sp, #112]            // 16-byte Folded Spill
	stp	x20, x19, [sp, #128]            // 16-byte Folded Spill
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldr	x8, [x8]
	stur	x8, [x29, #-8]
	stp	xzr, xzr, [sp, #56]
	stp	xzr, xzr, [sp, #40]
	ldr	w8, [x0, #7224]
	cbnz	w8, .LBB28_14
// %bb.1:
	mov	w20, w2
	mov	x19, x0
	mov	w22, w1
	ldr	x8, [x0, #7040]
	tbnz	w8, #15, .LBB28_3
// %bb.2:
	mov	w8, #7040
	add	x8, x19, x8
	//APP
	// atomic64_fetch_or_acquire
	prfm	pstl1strm, [x8]
.Ltmp319:
	ldaxr	x9, [x8]
	orr	x10, x9, #0x8000
	stxr	w11, x10, [x8]
	cbnz	w11, .Ltmp319

	//NO_APP
	tbz	w9, #15, .LBB28_17
.LBB28_3:
	mov	x0, sp
	mov	w1, wzr
	stp	xzr, xzr, [sp, #24]
	stp	xzr, xzr, [sp, #8]
	str	xzr, [sp]
	bl	init_wait_entry
	mov	w8, #7040
	add	x21, x19, #3928
	add	x23, x19, x8
	b	.LBB28_5
.LBB28_4:                               //   in Loop: Header=BB28_5 Depth=1
	bl	schedule
.LBB28_5:                               // =>This Inner Loop Header: Depth=1
	mov	x1, sp
	mov	x0, x21
	mov	w2, #2
	bl	prepare_to_wait_event
	ldr	x8, [x19, #7040]
	tbnz	w8, #15, .LBB28_4
// %bb.6:                               //   in Loop: Header=BB28_5 Depth=1
	//APP
	// atomic64_fetch_or_acquire
	prfm	pstl1strm, [x23]
.Ltmp320:
	ldaxr	x8, [x23]
	orr	x9, x8, #0x8000
	stxr	w10, x9, [x23]
	cbnz	w10, .Ltmp320

	//NO_APP
	tbnz	w8, #15, .LBB28_4
// %bb.7:
	mov	w8, #7072
	mov	x1, sp
	add	x8, x19, x8
	//APP
	// atomic_add
	prfm	pstl1strm, [x8]
.Ltmp321:
	ldxr	w9, [x8]
	add	w9, w9, #1
	stxr	w10, w9, [x8]
	cbnz	w10, .Ltmp321

	//NO_APP
	mov	x0, x21
	bl	finish_wait
.LBB28_8:
	add	x8, sp, #40
	adrp	x1, .L.str.107
	add	x1, x1, :lo12:.L.str.107
	add	x0, x8, #8
	adrp	x2, __init_completion.__key
	add	x2, x2, :lo12:__init_completion.__key
	str	wzr, [sp, #40]
	bl	__init_swait_queue_head
	ldr	x23, [x19, #224]
	ldr	x8, [x23, #3312]
	add	x21, x23, #3240
	add	x24, x23, #3312
	cbnz	x8, .LBB28_30
.LBB28_9:
	mov	w8, #7368
	add	x0, x19, x8
	bl	__raw_spin_lock_irqsave
	mov	w8, #15
	mov	w9, #256
	mov	x0, x19
	mov	x1, x21
	stp	xzr, xzr, [x24]
	str	xzr, [x24, #16]
	str	w8, [x23, #3344]
	strb	wzr, [x23, #3417]
	strh	w9, [x23, #3348]
	str	w22, [x19, #3880]
	bl	ufshcd_update_dev_cmd_lun
	mov	x0, x19
	mov	w1, #15
	bl	dfa_lrbp_configure
	cbnz	w0, .LBB28_15
// %bb.10:
	mov	w8, #1
	ldr	x9, [x23, #3424]
	ldrb	w10, [x23, #3349]
	mov	w11, #285212672
	str	w8, [x23, #3340]
	mov	w8, #268435456
	cmp	w10, #0
	ldr	w10, [x9, #8]
	csel	w8, w8, w11, eq
	strh	wzr, [x9, #28]
	orr	w10, w10, #0xf
	stp	w8, wzr, [x9]
	stp	w10, wzr, [x9, #8]
	ldr	w8, [x19, #3880]
	cbz	w8, .LBB28_18
// %bb.11:
	cmp	w8, #2
	b.eq	.LBB28_19
// %bb.12:
	mov	x0, x19
	cmp	w8, #1
	b.ne	.LBB28_27
// %bb.13:
	mov	x1, x21
	mov	w2, wzr
	bl	ufshcd_prepare_utp_query_req_upiu
	b	.LBB28_20
.LBB28_14:
	mov	w20, #-16
	b	.LBB28_23
.LBB28_15:
	mov	w20, #-1
.LBB28_16:
	mov	x0, x19
	bl	dfa_irq_sche_unlock
	b	.LBB28_22
.LBB28_17:
	mov	w8, #7072
	add	x8, x19, x8
	//APP
	// atomic_add
	prfm	pstl1strm, [x8]
.Ltmp322:
	ldxr	w9, [x8]
	add	w9, w9, #1
	stxr	w10, w9, [x8]
	cbnz	w10, .Ltmp322

	//NO_APP
	b	.LBB28_8
.LBB28_18:
	mov	x0, x21
	bl	ufshcd_prepare_utp_nop_upiu
	b	.LBB28_20
.LBB28_19:
	mov	x0, x21
	bl	ufshcd_prepare_utp_verify_upiu
.LBB28_20:
	add	x8, sp, #40
	mov	x0, x19
	mov	w1, #15
	str	x8, [x19, #3920]
	//APP
	dsb	st
	//NO_APP
	bl	dfa_send_command
	mov	x0, x19
	bl	dfa_irq_sche_unlock
	mov	w0, w20
	ldr	x22, [x19, #3920]
	bl	__msecs_to_jiffies
	mov	x1, x0
	mov	x0, x22
	bl	wait_for_completion_timeout
	mov	x20, x0
	mov	x0, x19
	mov	x2, x21
	bl	dfa_reset_dev_cmd_complete
	cbz	x20, .LBB28_31
// %bb.21:
	ldrb	w20, [x23, #3435]
	cbz	w20, .LBB28_28
.LBB28_22:
	mov	w8, #7040
	mov	w9, #32768
	add	x8, x19, x8
	mov	w10, #7072
	//APP
	// atomic64_fetch_andnot_release
	prfm	pstl1strm, [x8]
.Ltmp323:
	ldxr	x12, [x8]
	bic	x13, x12, x9
	stlxr	w14, x13, [x8]
	cbnz	w14, .Ltmp323

	//NO_APP
	add	x10, x19, x10
	mov	w11, #1
	//APP
	// atomic_sub
	prfm	pstl1strm, [x10]
.Ltmp324:
	ldxr	w8, [x10]
	sub	w8, w8, w11
	stxr	w9, w8, [x10]
	cbnz	w9, .Ltmp324

	//NO_APP
	add	x0, x19, #3928
	mov	w1, #3
	mov	w2, #1
	mov	x3, xzr
	bl	__wake_up
	cmn	w20, #110
	b.eq	.LBB28_25
.LBB28_23:
	adrp	x8, :got:__stack_chk_guard
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldur	x9, [x29, #-8]
	ldr	x8, [x8]
	cmp	x8, x9
	b.ne	.LBB28_37
// %bb.24:
	mov	w0, w20
	ldp	x20, x19, [sp, #128]            // 16-byte Folded Reload
	ldp	x22, x21, [sp, #112]            // 16-byte Folded Reload
	ldp	x24, x23, [sp, #96]             // 16-byte Folded Reload
	ldp	x29, x30, [sp, #80]             // 16-byte Folded Reload
	add	sp, sp, #144
	ret
.LBB28_25:
	mov	w8, #7128
	mov	w9, #7184
	add	x0, x19, x8
	add	x1, x19, x9
	bl	kthread_queue_work
	tbz	w0, #0, .LBB28_29
.LBB28_26:
	mov	w20, #-110
	b	.LBB28_23
.LBB28_27:
	mov	x1, x21
	bl	dfa_pack_fail_rollback
	mov	w20, #-22
	b	.LBB28_16
.LBB28_28:
	mov	x0, x19
	mov	x1, x21
	bl	ufshcd_dev_cmd_completion
	mov	w20, w0
	b	.LBB28_22
.LBB28_29:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.39
	add	x1, x1, :lo12:.L.str.39
	adrp	x2, .L__func__.dfa_exec_dev_cmd
	add	x2, x2, :lo12:.L__func__.dfa_exec_dev_cmd
	bl	_dev_err
	b	.LBB28_26
.LBB28_30:
	adrp	x0, .L.str.10
	add	x0, x0, :lo12:.L.str.10
	mov	w1, #1977
	mov	w2, #9
	mov	x3, xzr
	bl	warn_slowpath_fmt
	b	.LBB28_9
.LBB28_31:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.108
	add	x1, x1, :lo12:.L.str.108
	ldrb	w3, [x23, #3368]
	ldrb	w4, [x23, #3348]
	adrp	x2, .L__func__.dfa_wait_for_dev_cmd
	add	x2, x2, :lo12:.L__func__.dfa_wait_for_dev_cmd
	bl	_dev_err
	ldr	x8, [x19, #376]
	mov	w10, #80
	ldr	x9, [x19, #7024]
	add	x8, x8, #1
	str	x8, [x19, #376]
	ldrb	w8, [x23, #3433]
	nop
	madd	x8, x8, x10, x9
	ldp	w6, w5, [x8, #72]
	ldr	w4, [x8, #60]
	cmp	w5, w6
	b.ne	.LBB28_36
// %bb.32:
	cmp	w4, w5
	b.ne	.LBB28_36
.LBB28_33:
	mov	w8, #7080
	ldrb	w8, [x19, x8]
	cmp	w8, #1
	b.ne	.LBB28_35
.LBB28_34:
	mov	w20, #-110
	b	.LBB28_22
.LBB28_35:
	ldr	w1, [x23, #3344]
	mov	x0, x19
	bl	dfa_try_to_abort_task
	b	.LBB28_34
.LBB28_36:
	ldr	w2, [x8, #36]
	adrp	x0, .L.str.113
	add	x0, x0, :lo12:.L.str.113
	ldr	w3, [x8, #64]
	adrp	x1, .L__func__.dfa_wait_for_dev_cmd
	add	x1, x1, :lo12:.L__func__.dfa_wait_for_dev_cmd
	bl	printk
	b	.LBB28_33
.LBB28_37:
	bl	__stack_chk_fail
.Lfunc_end28:
	.size	dfa_exec_dev_cmd, .Lfunc_end28-dfa_exec_dev_cmd
                                        // -- End function
	.globl	dfa_exec_spec_cmd               // -- Begin function dfa_exec_spec_cmd
	.p2align	2
	.type	dfa_exec_spec_cmd,@function
dfa_exec_spec_cmd:                      // @dfa_exec_spec_cmd
// %bb.0:
	sub	sp, sp, #176
	adrp	x8, :got:__stack_chk_guard
	stp	x29, x30, [sp, #96]             // 16-byte Folded Spill
	str	x25, [sp, #112]                 // 8-byte Folded Spill
	add	x29, sp, #96
	stp	x24, x23, [sp, #128]            // 16-byte Folded Spill
	sub	x23, x29, #40
	stp	x22, x21, [sp, #144]            // 16-byte Folded Spill
	mov	w21, w2
	stp	x20, x19, [sp, #160]            // 16-byte Folded Spill
	mov	x22, x1
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	mov	x19, x0
	add	x0, x23, #8
	adrp	x1, .L.str.107
	add	x1, x1, :lo12:.L.str.107
	adrp	x2, __init_completion.__key
	add	x2, x2, :lo12:__init_completion.__key
	ldr	x8, [x8]
	stur	x8, [x29, #-8]
	stp	xzr, xzr, [x29, #-24]
	stp	xzr, xzr, [x29, #-40]
	bl	__init_swait_queue_head
	add	x20, x19, #3888
	mov	x0, x20
	bl	mutex_lock
	str	x23, [x19, #3920]
	ldr	x8, [x19, #7040]
	tbnz	w8, #15, .LBB29_2
// %bb.1:
	mov	w8, #7040
	add	x8, x19, x8
	//APP
	// atomic64_fetch_or_acquire
	prfm	pstl1strm, [x8]
.Ltmp325:
	ldaxr	x9, [x8]
	orr	x10, x9, #0x8000
	stxr	w11, x10, [x8]
	cbnz	w11, .Ltmp325

	//NO_APP
	tbz	w9, #15, .LBB29_21
.LBB29_2:
	add	x0, sp, #16
	mov	w1, wzr
	stp	xzr, xzr, [sp, #40]
	stp	xzr, xzr, [sp, #24]
	str	xzr, [sp, #16]
	bl	init_wait_entry
	mov	w8, #7040
	add	x23, x19, #3928
	add	x24, x19, x8
	b	.LBB29_4
.LBB29_3:                               //   in Loop: Header=BB29_4 Depth=1
	bl	schedule
.LBB29_4:                               // =>This Inner Loop Header: Depth=1
	add	x1, sp, #16
	mov	x0, x23
	mov	w2, #2
	bl	prepare_to_wait_event
	ldr	x8, [x19, #7040]
	tbnz	w8, #15, .LBB29_3
// %bb.5:                               //   in Loop: Header=BB29_4 Depth=1
	//APP
	// atomic64_fetch_or_acquire
	prfm	pstl1strm, [x24]
.Ltmp326:
	ldaxr	x8, [x24]
	orr	x9, x8, #0x8000
	stxr	w10, x9, [x24]
	cbnz	w10, .Ltmp326

	//NO_APP
	tbnz	w8, #15, .LBB29_3
// %bb.6:
	mov	w8, #7072
	add	x1, sp, #16
	add	x8, x19, x8
	//APP
	// atomic_add
	prfm	pstl1strm, [x8]
.Ltmp327:
	ldxr	w9, [x8]
	add	w9, w9, #1
	stxr	w10, w9, [x8]
	cbnz	w10, .Ltmp327

	//NO_APP
	mov	x0, x23
	bl	finish_wait
.LBB29_7:
	ldr	x25, [x19, #224]
	ldr	x8, [x25, #3312]
	cbnz	x8, .LBB29_32
.LBB29_8:
	mov	w8, #7368
	add	x0, x19, x8
	bl	__raw_spin_lock_irqsave
	mov	w8, #15
	str	x22, [x25, #3312]
	str	wzr, [x25, #3332]
	mov	w9, #96
	mov	w10, #49408
	str	w8, [x25, #3344]
	ldr	x8, [x22, #280]
	str	w9, [x25, #3328]
	str	x8, [x25, #3320]
	ldr	x8, [x22, #56]
	ldr	x8, [x8, #120]
	and	x9, x8, #0xff00
	cmp	x9, x10
	cset	w9, eq
	bfi	w8, w9, #7, #1
	mov	w9, #4548
	strb	w8, [x25, #3348]
	ldrb	w8, [x19, x9]
	tbz	w8, #4, .LBB29_11
// %bb.9:
	ldrb	w8, [x19, #708]
	tbnz	w8, #0, .LBB29_11
// %bb.10:
	mov	w8, wzr
	b	.LBB29_12
.LBB29_11:
	mov	w8, #1
.LBB29_12:
	mov	x0, x19
	mov	w1, #15
	strb	w8, [x25, #3349]
	str	wzr, [x25, #3340]
	bl	dfa_lrbp_configure
	cbnz	w0, .LBB29_17
// %bb.13:
	add	x23, x25, #3240
	mov	x0, x19
	mov	x1, x23
	bl	ufshcd_compose_upiu
	cbnz	w0, .LBB29_22
// %bb.14:
	ldr	x24, [x25, #3312]
	mov	w8, #2
	mov	x0, x24
	str	w8, [x25, #3340]
	bl	scsi_dma_map
	tbnz	w0, #31, .LBB29_23
// %bb.15:
	cbnz	w0, .LBB29_24
// %bb.16:
	ldr	x8, [x25, #3424]
	strh	wzr, [x8, #28]
	b	.LBB29_29
.LBB29_17:
	mov	w22, #-1
.LBB29_18:
	mov	x0, x19
	bl	dfa_irq_sche_unlock
.LBB29_19:
	mov	w8, #7040
	mov	w9, #32768
	add	x8, x19, x8
	mov	w10, #7072
	//APP
	// atomic64_fetch_andnot_release
	prfm	pstl1strm, [x8]
.Ltmp328:
	ldxr	x12, [x8]
	bic	x13, x12, x9
	stlxr	w14, x13, [x8]
	cbnz	w14, .Ltmp328

	//NO_APP
	add	x10, x19, x10
	mov	w11, #1
	//APP
	// atomic_sub
	prfm	pstl1strm, [x10]
.Ltmp329:
	ldxr	w8, [x10]
	sub	w8, w8, w11
	stxr	w9, w8, [x10]
	cbnz	w9, .Ltmp329

	//NO_APP
	add	x0, x19, #3928
	mov	w1, #3
	mov	w2, #1
	mov	x3, xzr
	str	xzr, [x25, #3312]
	bl	__wake_up
	mov	x0, x20
	bl	mutex_unlock
	adrp	x8, :got:__stack_chk_guard
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldur	x9, [x29, #-8]
	ldr	x8, [x8]
	cmp	x8, x9
	b.ne	.LBB29_37
// %bb.20:
	mov	w0, w22
	ldr	x25, [sp, #112]                 // 8-byte Folded Reload
	ldp	x20, x19, [sp, #160]            // 16-byte Folded Reload
	ldp	x22, x21, [sp, #144]            // 16-byte Folded Reload
	ldp	x24, x23, [sp, #128]            // 16-byte Folded Reload
	ldp	x29, x30, [sp, #96]             // 16-byte Folded Reload
	add	sp, sp, #176
	ret
.LBB29_21:
	mov	w8, #7072
	add	x8, x19, x8
	//APP
	// atomic_add
	prfm	pstl1strm, [x8]
.Ltmp330:
	ldxr	w9, [x8]
	add	w9, w9, #1
	stxr	w10, w9, [x8]
	cbnz	w10, .Ltmp330

	//NO_APP
	b	.LBB29_7
.LBB29_22:
	mov	w22, w0
	mov	x0, x19
	mov	x1, x23
	bl	dfa_pack_fail_rollback
	b	.LBB29_18
.LBB29_23:
	mov	w22, w0
	b	.LBB29_18
.LBB29_24:
	ldrb	w8, [x19, #709]
	tbnz	w8, #1, .LBB29_26
// %bb.25:
	mov	w22, w0
	mov	w8, w0
	b	.LBB29_27
.LBB29_26:
	ldr	w8, [x19, #304]
	mov	w22, w0
	mul	w8, w8, w0
.LBB29_27:
	ldr	x9, [x25, #3424]
	strh	w8, [x9, #28]
	ldr	x0, [x24, #232]
	ldr	x24, [x25, #3272]
.LBB29_28:                              // =>This Inner Loop Header: Depth=1
	ldr	x8, [x0, #16]
	str	w8, [x24]
	ldr	w8, [x0, #20]
	str	w8, [x24, #4]
	ldr	x8, [x19, #304]
	add	x24, x24, x8
	bl	sg_next
	subs	w22, w22, #1
	b.ne	.LBB29_28
.LBB29_29:
	//APP
	dsb	st
	//NO_APP
	mov	x0, x19
	mov	w1, #15
	bl	dfa_send_command
	mov	x0, x19
	bl	dfa_irq_sche_unlock
	mov	w0, w21
	ldr	x22, [x19, #3920]
	bl	__msecs_to_jiffies
	mov	x1, x0
	mov	x0, x22
	bl	wait_for_completion_timeout
	mov	x21, x0
	mov	x0, x19
	mov	x2, x23
	bl	dfa_reset_dev_cmd_complete
	cbz	x21, .LBB29_33
// %bb.30:
	mov	x0, x19
	mov	x1, x23
	mov	w2, wzr
	bl	dfa_transfer_rsp_status
	mov	w22, wzr
	cbz	w0, .LBB29_19
// %bb.31:
	mov	w21, w0
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.40
	add	x1, x1, :lo12:.L.str.40
	mov	w2, w21
	bl	_dev_err
	mov	w22, w21
	b	.LBB29_19
.LBB29_32:
	adrp	x0, .L.str.10
	add	x0, x0, :lo12:.L.str.10
	mov	w1, #2046
	mov	w2, #9
	mov	x3, xzr
	bl	warn_slowpath_fmt
	b	.LBB29_8
.LBB29_33:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.41
	add	x1, x1, :lo12:.L.str.41
	ldrb	w2, [x25, #3368]
	ldrb	w3, [x25, #3348]
	ldrb	w4, [x25, #3433]
	ldrb	w5, [x25, #3434]
	bl	_dev_err
	ldr	x8, [x19, #376]
	mov	w10, #80
	ldr	x9, [x19, #7024]
	adrp	x1, .L.str.42
	add	x1, x1, :lo12:.L.str.42
	ldr	x0, [x19, #96]
	add	x8, x8, #1
	str	x8, [x19, #376]
	ldrb	w8, [x25, #3433]
	nop
	madd	x21, x8, x10, x9
	mov	w10, #12
	ldr	w2, [x21, #36]
	ldr	x8, [x21, #48]
	add	x8, x8, x2, lsl #3
                                        // kill: def $w2 killed $w2 killed $x2
	add	x8, x8, #2272
	//APP
.Ltmp331:
	ldr	w3, [x8]
.Ltmp332:
	.section	.altinstructions,"a",@progbits
.Ltmp333:
	.word	.Ltmp331-.Ltmp333
.Ltmp335:
	.word	.Ltmp334-.Ltmp335
	.hword	1
	.byte	.Ltmp332-.Ltmp331
	.byte	.Ltmp336-.Ltmp334
	.text

	.text	1
.Ltmp334:
	ldar	w3, [x8]
.Ltmp336:
.Ltmp337:
.org (.Ltmp337-(.Ltmp336-.Ltmp334))+(.Ltmp332-.Ltmp331), 0
.Ltmp338:
.org (.Ltmp338-(.Ltmp332-.Ltmp331))+(.Ltmp336-.Ltmp334), 0
	.text


	//NO_APP
	mov	w8, w3
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp339:
	cbnz	x8, .Ltmp339
	//NO_APP
	ldr	x8, [x21, #48]
	ldr	w9, [x21, #36]
	add	x8, x8, x9, lsl #3
	add	x8, x8, #2276
	//APP
.Ltmp340:
	ldr	w4, [x8]
.Ltmp341:
	.section	.altinstructions,"a",@progbits
.Ltmp342:
	.word	.Ltmp340-.Ltmp342
.Ltmp344:
	.word	.Ltmp343-.Ltmp344
	.hword	1
	.byte	.Ltmp341-.Ltmp340
	.byte	.Ltmp345-.Ltmp343
	.text

	.text	1
.Ltmp343:
	ldar	w4, [x8]
.Ltmp345:
.Ltmp346:
.org (.Ltmp346-(.Ltmp345-.Ltmp343))+(.Ltmp341-.Ltmp340), 0
.Ltmp347:
.org (.Ltmp347-(.Ltmp341-.Ltmp340))+(.Ltmp345-.Ltmp343), 0
	.text


	//NO_APP
	mov	w8, w4
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp348:
	cbnz	x8, .Ltmp348
	//NO_APP
	ldr	x8, [x21, #48]
	ldr	w9, [x21, #36]
	add	x8, x8, x9, lsl #3
	add	x8, x8, #3552
	//APP
.Ltmp349:
	ldr	w5, [x8]
.Ltmp350:
	.section	.altinstructions,"a",@progbits
.Ltmp351:
	.word	.Ltmp349-.Ltmp351
.Ltmp353:
	.word	.Ltmp352-.Ltmp353
	.hword	1
	.byte	.Ltmp350-.Ltmp349
	.byte	.Ltmp354-.Ltmp352
	.text

	.text	1
.Ltmp352:
	ldar	w5, [x8]
.Ltmp354:
.Ltmp355:
.org (.Ltmp355-(.Ltmp354-.Ltmp352))+(.Ltmp350-.Ltmp349), 0
.Ltmp356:
.org (.Ltmp356-(.Ltmp350-.Ltmp349))+(.Ltmp354-.Ltmp352), 0
	.text


	//NO_APP
	mov	w8, w5
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp357:
	cbnz	x8, .Ltmp357
	//NO_APP
	ldr	x8, [x21, #48]
	ldr	w9, [x21, #36]
	add	x8, x8, x9, lsl #3
	add	x8, x8, #3556
	//APP
.Ltmp358:
	ldr	w6, [x8]
.Ltmp359:
	.section	.altinstructions,"a",@progbits
.Ltmp360:
	.word	.Ltmp358-.Ltmp360
.Ltmp362:
	.word	.Ltmp361-.Ltmp362
	.hword	1
	.byte	.Ltmp359-.Ltmp358
	.byte	.Ltmp363-.Ltmp361
	.text

	.text	1
.Ltmp361:
	ldar	w6, [x8]
.Ltmp363:
.Ltmp364:
.org (.Ltmp364-(.Ltmp363-.Ltmp361))+(.Ltmp359-.Ltmp358), 0
.Ltmp365:
.org (.Ltmp365-(.Ltmp359-.Ltmp358))+(.Ltmp363-.Ltmp361), 0
	.text


	//NO_APP
	mov	w8, w6
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp366:
	cbnz	x8, .Ltmp366
	//NO_APP
	ldr	x8, [x21, #48]
	ldr	w9, [x21, #36]
	nop
	madd	x8, x9, x10, x8
	mov	w9, #4196
	add	x8, x8, x9
	//APP
.Ltmp367:
	ldr	w7, [x8]
.Ltmp368:
	.section	.altinstructions,"a",@progbits
.Ltmp369:
	.word	.Ltmp367-.Ltmp369
.Ltmp371:
	.word	.Ltmp370-.Ltmp371
	.hword	1
	.byte	.Ltmp368-.Ltmp367
	.byte	.Ltmp372-.Ltmp370
	.text

	.text	1
.Ltmp370:
	ldar	w7, [x8]
.Ltmp372:
.Ltmp373:
.org (.Ltmp373-(.Ltmp372-.Ltmp370))+(.Ltmp368-.Ltmp367), 0
.Ltmp374:
.org (.Ltmp374-(.Ltmp368-.Ltmp367))+(.Ltmp372-.Ltmp370), 0
	.text


	//NO_APP
	mov	w8, w7
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp375:
	cbnz	x8, .Ltmp375
	//NO_APP
	ldr	x8, [x21, #48]
	ldr	w9, [x21, #36]
	nop
	madd	x8, x9, x10, x8
	mov	w9, #4192
	add	x8, x8, x9
	//APP
.Ltmp376:
	ldr	w8, [x8]
.Ltmp377:
	.section	.altinstructions,"a",@progbits
.Ltmp378:
	.word	.Ltmp376-.Ltmp378
.Ltmp380:
	.word	.Ltmp379-.Ltmp380
	.hword	1
	.byte	.Ltmp377-.Ltmp376
	.byte	.Ltmp381-.Ltmp379
	.text

	.text	1
.Ltmp379:
	ldar	w8, [x8]
.Ltmp381:
.Ltmp382:
.org (.Ltmp382-(.Ltmp381-.Ltmp379))+(.Ltmp377-.Ltmp376), 0
.Ltmp383:
.org (.Ltmp383-(.Ltmp377-.Ltmp376))+(.Ltmp381-.Ltmp379), 0
	.text


	//NO_APP
	mov	w9, w8
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x9, x9, x9
.Ltmp384:
	cbnz	x9, .Ltmp384
	//NO_APP
	str	w8, [sp]
	bl	_dev_err
	ldp	w6, w5, [x21, #72]
	ldr	w4, [x21, #60]
	cmp	w5, w6
	b.ne	.LBB29_36
// %bb.34:
	cmp	w4, w5
	b.ne	.LBB29_36
.LBB29_35:
	mov	w22, #-110
	b	.LBB29_19
.LBB29_36:
	ldr	w2, [x21, #36]
	adrp	x0, .L.str.113
	add	x0, x0, :lo12:.L.str.113
	ldr	w3, [x21, #64]
	adrp	x1, .L__func__.dfa_exec_spec_cmd
	add	x1, x1, :lo12:.L__func__.dfa_exec_spec_cmd
	bl	printk
	b	.LBB29_35
.LBB29_37:
	bl	__stack_chk_fail
.Lfunc_end29:
	.size	dfa_exec_spec_cmd, .Lfunc_end29-dfa_exec_spec_cmd
                                        // -- End function
	.p2align	2                               // -- Begin function dfa_transfer_rsp_status
	.type	dfa_transfer_rsp_status,@function
dfa_transfer_rsp_status:                // @dfa_transfer_rsp_status
// %bb.0:
	stp	x29, x30, [sp, #-64]!           // 16-byte Folded Spill
	stp	x20, x19, [sp, #48]             // 16-byte Folded Spill
	ldrb	w20, [x1, #195]
	str	x23, [sp, #16]                  // 8-byte Folded Spill
	mov	x29, sp
	stp	x22, x21, [sp, #32]             // 16-byte Folded Spill
	cbnz	w20, .LBB30_7
// %bb.1:
	ldr	x8, [x1, #24]
	mov	w22, w2
	ldrb	w2, [x8]
	cmp	w2, #33
	b.ne	.LBB30_17
// %bb.2:
	ldr	w8, [x8, #4]
	mov	x21, x1
	mov	x19, x0
	mov	x0, x1
	lsr	w23, w8, #24
	mov	w1, w23
	bl	ufshcd_scsi_cmd_status
	mov	w20, w0
	cbz	w23, .LBB30_4
// %bb.3:
	cmp	w20, #112, lsl #12              // =458752
	b.ne	.LBB30_10
.LBB30_4:
	cbnz	w23, .LBB30_11
// %bb.5:
	tbz	w22, #0, .LBB30_11
// %bb.6:
	mov	x0, x19
	mov	x1, x21
	bl	ufshpb_check_rsp_upiu_device_mode
	b	.LBB30_11
.LBB30_7:
	mov	x21, x1
	mov	x19, x0
	cmp	w20, #15
	b.hi	.LBB30_19
// %bb.8:
	mov	w8, #1
	mov	w9, #36352
	lsl	w8, w8, w20
	tst	w8, w9
	b.eq	.LBB30_19
// %bb.9:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.109
	add	x1, x1, :lo12:.L.str.109
	ldrb	w3, [x21, #128]
	ldr	w4, [x21, #104]
	mov	w2, w20
	bl	_dev_err
	mov	w20, #851968
	b	.LBB30_14
.LBB30_10:
	mov	x0, x19
	mov	x1, x21
	mov	w2, w23
	bl	ufshcd_rsp_sense_data
.LBB30_11:
	ldr	w8, [x19, #208]
	cbnz	w8, .LBB30_14
// %bb.12:
	ldrb	w8, [x19, #1248]
	tbnz	w8, #0, .LBB30_14
// %bb.13:
	mov	w8, #7080
	ldr	x9, [x21, #24]
	mov	w10, #4
	ldrb	w8, [x19, x8]
	cmp	w8, #1
	mov	w8, #8
	csel	x8, x10, x8, eq
	add	x8, x9, x8
	ldrb	w8, [x8, #1]
	tbnz	w8, #0, .LBB30_15
.LBB30_14:
	mov	w0, w20
	ldr	x23, [sp, #16]                  // 8-byte Folded Reload
	ldp	x20, x19, [sp, #48]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #32]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #64             // 16-byte Folded Reload
	ret
.LBB30_15:
	adrp	x8, :got:system_wq
	add	x2, x19, #1400
	mov	w0, #12
	ldr	x8, [x8, :got_lo12:system_wq]
	ldr	x1, [x8]
	bl	queue_work_on
	tbz	w0, #0, .LBB30_14
// %bb.16:
	ldr	x8, [x19, #96]
	add	x8, x8, #472
	//APP
	// atomic_add
	prfm	pstl1strm, [x8]
.Ltmp385:
	ldxr	w9, [x8]
	add	w9, w9, #1
	stxr	w10, w9, [x8]
	cbnz	w10, .Ltmp385

	//NO_APP
	b	.LBB30_14
.LBB30_17:
	mov	x19, x0
	cmp	w2, #63
	b.ne	.LBB30_21
// %bb.18:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.111
	add	x1, x1, :lo12:.L.str.111
	bl	_dev_err
	mov	w20, #458752
	b	.LBB30_14
.LBB30_19:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.110
	add	x1, x1, :lo12:.L.str.110
	ldr	w3, [x21, #104]
	mov	w2, w20
	bl	_dev_err
	mov	w1, #18248
	mov	x0, x19
	movk	w1, #14160, lsl #16
	mov	w2, w20
	bl	dsm_ufs_update_ocs_info
	cbz	w0, .LBB30_23
.LBB30_20:
	mov	x0, x19
	bl	dfa_print_host_state
	mov	w20, wzr
	b	.LBB30_14
.LBB30_21:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.112
	add	x1, x1, :lo12:.L.str.112
	bl	_dev_err
	mov	w1, #18248
	mov	x0, x19
	movk	w1, #14160, lsl #16
	mov	w2, wzr
	bl	dsm_ufs_update_ocs_info
	cbz	w0, .LBB30_24
// %bb.22:
	mov	w20, #458752
	b	.LBB30_14
.LBB30_23:
	mov	x0, x19
	bl	schedule_ufs_dsm_work
	b	.LBB30_20
.LBB30_24:
	mov	x0, x19
	bl	schedule_ufs_dsm_work
	mov	w20, #458752
	b	.LBB30_14
.Lfunc_end30:
	.size	dfa_transfer_rsp_status, .Lfunc_end30-dfa_transfer_rsp_status
                                        // -- End function
	.globl	dfa_queuecommand_directly       // -- Begin function dfa_queuecommand_directly
	.p2align	2
	.type	dfa_queuecommand_directly,@function
dfa_queuecommand_directly:              // @dfa_queuecommand_directly
// %bb.0:
	stp	x29, x30, [sp, #-48]!           // 16-byte Folded Spill
	stp	x22, x21, [sp, #16]             // 16-byte Folded Spill
	mov	w21, w2
	stp	x20, x19, [sp, #32]             // 16-byte Folded Spill
	mov	w20, w3
	mov	x19, x0
	ldr	w8, [x0, #7224]
	mov	x22, x1
	mov	x29, sp
	cbnz	w8, .LBB31_8
// %bb.1:
	ldr	w8, [x19, #7224]
	cbnz	w8, .LBB31_11
.LBB31_2:
	mov	x0, x19
	mov	x1, x22
	mov	w2, w21
	bl	dfa_exec_spec_cmd
	cmn	w0, #110
	b.ne	.LBB31_5
// %bb.3:
	mov	w8, #7080
	ldrb	w8, [x19, x8]
	cmp	w8, #1
	b.eq	.LBB31_9
// %bb.4:
	mov	x0, x19
	mov	w1, #15
	bl	dfa_try_to_abort_task
.LBB31_5:
	cbnz	w0, .LBB31_11
// %bb.6:
	mov	w21, wzr
.LBB31_7:
	mov	w0, w21
	ldp	x20, x19, [sp, #32]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #48             // 16-byte Folded Reload
	ret
.LBB31_8:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.43
	add	x1, x1, :lo12:.L.str.43
	bl	_dev_err
	mov	w8, #7184
	add	x0, x19, x8
	bl	kthread_flush_work
	ldr	w8, [x19, #7224]
	cbz	w8, .LBB31_2
	b	.LBB31_11
.LBB31_9:
	mov	w8, #7128
	mov	w9, #7184
	add	x0, x19, x8
	add	x1, x19, x9
	bl	kthread_queue_work
	tbnz	w0, #0, .LBB31_11
// %bb.10:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.96
	add	x1, x1, :lo12:.L.str.96
	adrp	x2, .L__func__.__dfa_queuecommand_directly
	add	x2, x2, :lo12:.L__func__.__dfa_queuecommand_directly
	bl	_dev_err
.LBB31_11:
	ldr	w8, [x19, #7224]
	cbnz	w8, .LBB31_17
// %bb.12:
	mov	x0, x19
	mov	x1, x22
	mov	w2, w21
	bl	dfa_exec_spec_cmd
	mov	w21, w0
	cmn	w0, #110
	b.ne	.LBB31_18
// %bb.13:
	mov	w8, #7080
	ldrb	w8, [x19, x8]
	cmp	w8, #1
	b.ne	.LBB31_22
// %bb.14:
	mov	w8, #7128
	mov	w9, #7184
	add	x0, x19, x8
	add	x1, x19, x9
	bl	kthread_queue_work
	tbnz	w0, #0, .LBB31_16
// %bb.15:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.96
	add	x1, x1, :lo12:.L.str.96
	adrp	x2, .L__func__.__dfa_queuecommand_directly
	add	x2, x2, :lo12:.L__func__.__dfa_queuecommand_directly
	bl	_dev_err
.LBB31_16:
	mov	w21, #-110
	b	.LBB31_18
.LBB31_17:
	mov	w21, #-16
.LBB31_18:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.44
	add	x1, x1, :lo12:.L.str.44
	mov	w2, w21
	bl	_dev_err
	cbz	w21, .LBB31_7
// %bb.19:
	tbz	w20, #0, .LBB31_7
// %bb.20:
	mov	w8, #7128
	mov	w9, #7184
	add	x0, x19, x8
	add	x1, x19, x9
	bl	kthread_queue_work
	tbnz	w0, #0, .LBB31_7
// %bb.21:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.45
	add	x1, x1, :lo12:.L.str.45
	bl	_dev_err
	b	.LBB31_7
.LBB31_22:
	mov	x0, x19
	mov	w1, #15
	bl	dfa_try_to_abort_task
	mov	w21, w0
	b	.LBB31_18
.Lfunc_end31:
	.size	dfa_queuecommand_directly, .Lfunc_end31-dfa_queuecommand_directly
                                        // -- End function
	.globl	dfa_tt_do_scsi_cmd              // -- Begin function dfa_tt_do_scsi_cmd
	.p2align	2
	.type	dfa_tt_do_scsi_cmd,@function
dfa_tt_do_scsi_cmd:                     // @dfa_tt_do_scsi_cmd
// %bb.0:
	stp	x29, x30, [sp, #-32]!           // 16-byte Folded Spill
	str	x19, [sp, #16]                  // 8-byte Folded Spill
	mov	x29, sp
	ldr	w8, [x0, #7228]
	cbz	w8, .LBB32_3
// %bb.1:
	mov	w0, #-16
.LBB32_2:
	ldr	x19, [sp, #16]                  // 8-byte Folded Reload
	ldp	x29, x30, [sp], #32             // 16-byte Folded Reload
	ret
.LBB32_3:
	mov	x19, x0
	bl	dfa_exec_spec_cmd
	cmn	w0, #110
	b.ne	.LBB32_2
// %bb.4:
	mov	w8, #7080
	mov	x0, x19
	ldrb	w8, [x19, x8]
	cmp	w8, #1
	b.ne	.LBB32_6
// %bb.5:
	bl	dfa_reset_directly
	mov	w0, #-110
	b	.LBB32_2
.LBB32_6:
	mov	w1, #15
	bl	dfa_try_to_abort_task
	b	.LBB32_2
.Lfunc_end32:
	.size	dfa_tt_do_scsi_cmd, .Lfunc_end32-dfa_tt_do_scsi_cmd
                                        // -- End function
	.globl	dfa_clk_init                    // -- Begin function dfa_clk_init
	.p2align	2
	.type	dfa_clk_init,@function
dfa_clk_init:                           // @dfa_clk_init
// %bb.0:
	stp	x29, x30, [sp, #-48]!           // 16-byte Folded Spill
	stp	x22, x21, [sp, #16]             // 16-byte Folded Spill
	ldr	x22, [x0, #296]
	stp	x20, x19, [sp, #32]             // 16-byte Folded Spill
	mov	x19, x0
	mov	x29, sp
	ldr	x8, [x22, #48]
	add	x8, x8, #1220
	//APP
.Ltmp386:
	ldr	w8, [x8]
.Ltmp387:
	.section	.altinstructions,"a",@progbits
.Ltmp388:
	.word	.Ltmp386-.Ltmp388
.Ltmp390:
	.word	.Ltmp389-.Ltmp390
	.hword	1
	.byte	.Ltmp387-.Ltmp386
	.byte	.Ltmp391-.Ltmp389
	.text

	.text	1
.Ltmp389:
	ldar	w8, [x8]
.Ltmp391:
.Ltmp392:
.org (.Ltmp392-(.Ltmp391-.Ltmp389))+(.Ltmp387-.Ltmp386), 0
.Ltmp393:
.org (.Ltmp393-(.Ltmp387-.Ltmp386))+(.Ltmp391-.Ltmp389), 0
	.text


	//NO_APP
	mov	w9, w8
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x9, x9, x9
.Ltmp394:
	cbnz	x9, .Ltmp394
	//NO_APP
	ldr	x0, [x22, #200]
	tbz	w8, #0, .LBB33_8
// %bb.1:
	mov	w1, #11392
	movk	w1, #6378, lsl #16
	bl	clk_set_rate
	cbnz	w0, .LBB33_17
.LBB33_2:
	ldr	x20, [x22, #200]
	mov	x0, x20
	bl	clk_prepare
	cbnz	w0, .LBB33_21
// %bb.3:
	mov	x0, x20
	bl	clk_enable
	cbnz	w0, .LBB33_20
.LBB33_4:
	mov	w1, #30976
	ldr	x0, [x22, #208]
	movk	w1, #14709, lsl #16
	bl	clk_set_rate
	cbnz	w0, .LBB33_15
.LBB33_5:
	ldr	x21, [x22, #208]
	mov	x0, x21
	bl	clk_prepare
	cbnz	w0, .LBB33_16
// %bb.6:
	mov	x0, x21
	bl	clk_enable
	cbnz	w0, .LBB33_18
.LBB33_7:
	ldp	x20, x19, [sp, #32]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #48             // 16-byte Folded Reload
	ret
.LBB33_8:
	mov	w1, #55744
	movk	w1, #3646, lsl #16
	bl	clk_set_rate
	cbz	w0, .LBB33_10
// %bb.9:
	mov	w3, w0
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.48
	add	x1, x1, :lo12:.L.str.48
	adrp	x2, .L__func__.dfa_clk_init
	add	x2, x2, :lo12:.L__func__.dfa_clk_init
	bl	_dev_err
.LBB33_10:
	ldr	x21, [x22, #200]
	mov	x0, x21
	bl	clk_prepare
	mov	w20, w0
	cbnz	w0, .LBB33_13
// %bb.11:
	mov	x0, x21
	bl	clk_enable
	cbz	w0, .LBB33_14
// %bb.12:
	mov	w20, w0
	mov	x0, x21
	bl	clk_unprepare
.LBB33_13:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.49
	add	x1, x1, :lo12:.L.str.49
	adrp	x2, .L__func__.dfa_clk_init
	add	x2, x2, :lo12:.L__func__.dfa_clk_init
	mov	w3, w20
	bl	_dev_err
.LBB33_14:
	adrp	x1, .L.str.50
	add	x1, x1, :lo12:.L.str.50
	b	.LBB33_22
.LBB33_15:
	mov	w3, w0
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.51
	add	x1, x1, :lo12:.L.str.51
	adrp	x2, .L__func__.dfa_clk_init
	add	x2, x2, :lo12:.L__func__.dfa_clk_init
	bl	_dev_err
	b	.LBB33_5
.LBB33_16:
	mov	w20, w0
	b	.LBB33_19
.LBB33_17:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.46
	add	x1, x1, :lo12:.L.str.46
	bl	_dev_err
	b	.LBB33_2
.LBB33_18:
	mov	w20, w0
	mov	x0, x21
	bl	clk_unprepare
.LBB33_19:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.52
	add	x1, x1, :lo12:.L.str.52
	adrp	x2, .L__func__.dfa_clk_init
	add	x2, x2, :lo12:.L__func__.dfa_clk_init
	mov	w3, w20
	bl	_dev_err
	b	.LBB33_7
.LBB33_20:
	mov	x0, x20
	bl	clk_unprepare
.LBB33_21:
	adrp	x1, .L.str.47
	add	x1, x1, :lo12:.L.str.47
.LBB33_22:
	ldr	x0, [x19, #96]
	bl	_dev_err
	b	.LBB33_4
.Lfunc_end33:
	.size	dfa_clk_init, .Lfunc_end33-dfa_clk_init
                                        // -- End function
	.globl	dfa_em_check                    // -- Begin function dfa_em_check
	.p2align	2
	.type	dfa_em_check,@function
dfa_em_check:                           // @dfa_em_check
// %bb.0:
	stp	x29, x30, [sp, #-16]!           // 16-byte Folded Spill
	ldr	x8, [x0, #296]
	mov	w9, #4332
	mov	x29, sp
	ldr	x8, [x8, #24]
	add	x8, x8, x9
	//APP
.Ltmp395:
	ldr	w8, [x8]
.Ltmp396:
	.section	.altinstructions,"a",@progbits
.Ltmp397:
	.word	.Ltmp395-.Ltmp397
.Ltmp399:
	.word	.Ltmp398-.Ltmp399
	.hword	1
	.byte	.Ltmp396-.Ltmp395
	.byte	.Ltmp400-.Ltmp398
	.text

	.text	1
.Ltmp398:
	ldar	w8, [x8]
.Ltmp400:
.Ltmp401:
.org (.Ltmp401-(.Ltmp400-.Ltmp398))+(.Ltmp396-.Ltmp395), 0
.Ltmp402:
.org (.Ltmp402-(.Ltmp396-.Ltmp395))+(.Ltmp400-.Ltmp398), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w8, w8
	//APP
	eor	x9, x8, x8
.Ltmp403:
	cbnz	x9, .Ltmp403
	//NO_APP
	tbnz	w8, #5, .LBB34_2
// %bb.1:
	ldp	x29, x30, [sp], #16             // 16-byte Folded Reload
	ret
.LBB34_2:
	mov	w0, #300
	bl	msleep
	ldp	x29, x30, [sp], #16             // 16-byte Folded Reload
	ret
.Lfunc_end34:
	.size	dfa_em_check, .Lfunc_end34-dfa_em_check
                                        // -- End function
	.globl	dfa_suspend                     // -- Begin function dfa_suspend
	.p2align	2
	.type	dfa_suspend,@function
dfa_suspend:                            // @dfa_suspend
// %bb.0:
	stp	x29, x30, [sp, #-48]!           // 16-byte Folded Spill
	stp	x22, x21, [sp, #16]             // 16-byte Folded Spill
	mov	x29, sp
	stp	x20, x19, [sp, #32]             // 16-byte Folded Spill
	cbnz	w1, .LBB35_2
// %bb.1:
	mov	w20, wzr
	b	.LBB35_25
.LBB35_2:
	ldr	w8, [x0, #7224]
	cbnz	w8, .LBB35_33
// %bb.3:
	mov	w8, #1
	mov	x19, x0
	cmp	w1, #2
	str	w8, [x0, #208]
	strb	w8, [x0, #368]
	b.ne	.LBB35_5
// %bb.4:
	mov	w1, #3
	b	.LBB35_6
.LBB35_5:
	ldr	w8, [x0, #140]
	adrp	x9, :got:ufs_pm_lvl_states
	lsl	x8, x8, #3
	ldr	x9, [x9, :got_lo12:ufs_pm_lvl_states]
	ldr	w1, [x9, x8]
.LBB35_6:
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #48
	ldr	x8, [x19, #7008]
	add	x8, x8, #68
	//APP
	str	w9, [x8]
	//NO_APP
	ldr	w8, [x19, #128]
	cmp	w1, w8
	b.eq	.LBB35_8
// %bb.7:
	mov	x0, x19
	bl	ufshcd_set_dev_pwr_mode
	cbnz	w0, .LBB35_35
.LBB35_8:
	ldr	x21, [x19, #296]
	//APP
	dmb	oshst
	//NO_APP
	ldr	x8, [x19, #7016]
	mov	w20, #4999
	mov	w9, #1
	add	x8, x8, #24
	//APP
	str	w9, [x8]
	//NO_APP
.LBB35_9:                               // =>This Inner Loop Header: Depth=1
	ldr	x8, [x19, #7016]
	add	x8, x8, #28
	//APP
.Ltmp404:
	ldr	w8, [x8]
.Ltmp405:
	.section	.altinstructions,"a",@progbits
.Ltmp406:
	.word	.Ltmp404-.Ltmp406
.Ltmp408:
	.word	.Ltmp407-.Ltmp408
	.hword	1
	.byte	.Ltmp405-.Ltmp404
	.byte	.Ltmp409-.Ltmp407
	.text

	.text	1
.Ltmp407:
	ldar	w8, [x8]
.Ltmp409:
.Ltmp410:
.org (.Ltmp410-(.Ltmp409-.Ltmp407))+(.Ltmp405-.Ltmp404), 0
.Ltmp411:
.org (.Ltmp411-(.Ltmp405-.Ltmp404))+(.Ltmp409-.Ltmp407), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w8, w8
	//APP
	eor	x9, x8, x8
.Ltmp412:
	cbnz	x9, .Ltmp412
	//NO_APP
	tbnz	w8, #0, .LBB35_11
// %bb.10:                              //   in Loop: Header=BB35_9 Depth=1
	mov	w0, #4295
	bl	__const_udelay
	sub	w20, w20, #1
	cmn	w20, #1
	b.ne	.LBB35_9
	b	.LBB35_12
.LBB35_11:
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #1
	ldr	x8, [x19, #7016]
	add	x8, x8, #28
	//APP
	str	w9, [x8]
	//NO_APP
.LBB35_12:
	//APP
	dmb	oshst
	//NO_APP
	cmp	w20, #0
	ldr	x9, [x19, #7016]
	mov	w8, wzr
	add	x9, x9, #24
	//APP
	str	w8, [x9]
	//NO_APP
	b.le	.LBB35_34
// %bb.13:
	//APP
	dmb	oshst
	//NO_APP
	mov	w22, #4999
	ldr	x8, [x19, #7008]
	mov	w9, #17
	add	x8, x8, #64
	//APP
	str	w9, [x8]
	//NO_APP
.LBB35_14:                              // =>This Inner Loop Header: Depth=1
	ldr	x8, [x19, #7008]
	add	x8, x8, #68
	//APP
.Ltmp413:
	ldr	w20, [x8]
.Ltmp414:
	.section	.altinstructions,"a",@progbits
.Ltmp415:
	.word	.Ltmp413-.Ltmp415
.Ltmp417:
	.word	.Ltmp416-.Ltmp417
	.hword	1
	.byte	.Ltmp414-.Ltmp413
	.byte	.Ltmp418-.Ltmp416
	.text

	.text	1
.Ltmp416:
	ldar	w20, [x8]
.Ltmp418:
.Ltmp419:
.org (.Ltmp419-(.Ltmp418-.Ltmp416))+(.Ltmp414-.Ltmp413), 0
.Ltmp420:
.org (.Ltmp420-(.Ltmp414-.Ltmp413))+(.Ltmp418-.Ltmp416), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w8, w20
	//APP
	eor	x9, x8, x8
.Ltmp421:
	cbnz	x9, .Ltmp421
	//NO_APP
	tbnz	w8, #4, .LBB35_16
// %bb.15:                              //   in Loop: Header=BB35_14 Depth=1
	mov	w0, #4295
	bl	__const_udelay
	sub	w22, w22, #1
	cmn	w22, #1
	b.ne	.LBB35_14
	b	.LBB35_27
.LBB35_16:
	//APP
	dmb	oshst
	//NO_APP
	cmp	w22, #0
	ldr	x8, [x19, #7008]
	mov	w9, #48
	add	x8, x8, #68
	//APP
	str	w9, [x8]
	//NO_APP
	b.le	.LBB35_27
// %bb.17:
	mov	w22, #13572
	mov	x1, xzr
	movk	w22, #49920, lsl #16
	mov	x2, xzr
	orr	x0, x22, #0x2
	mov	x3, xzr
	bl	atfd_ufs_smc
	cbnz	w0, .LBB35_37
// %bb.18:
	mov	w0, #13572
	mov	w1, #32
	movk	w0, #49920, lsl #16
	mov	w2, #1
	mov	w3, #5000
	bl	atfd_ufs_smc
	cbnz	w0, .LBB35_38
// %bb.19:
	ldr	x20, [x21, #200]
	mov	x0, x20
	bl	clk_disable
	mov	x0, x20
	bl	clk_unprepare
	ldr	x8, [x19, #296]
	mov	w9, #4332
	ldr	x8, [x8, #24]
	add	x8, x8, x9
	//APP
.Ltmp422:
	ldr	w8, [x8]
.Ltmp423:
	.section	.altinstructions,"a",@progbits
.Ltmp424:
	.word	.Ltmp422-.Ltmp424
.Ltmp426:
	.word	.Ltmp425-.Ltmp426
	.hword	1
	.byte	.Ltmp423-.Ltmp422
	.byte	.Ltmp427-.Ltmp425
	.text

	.text	1
.Ltmp425:
	ldar	w8, [x8]
.Ltmp427:
.Ltmp428:
.org (.Ltmp428-(.Ltmp427-.Ltmp425))+(.Ltmp423-.Ltmp422), 0
.Ltmp429:
.org (.Ltmp429-(.Ltmp423-.Ltmp422))+(.Ltmp427-.Ltmp425), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w8, w8
	//APP
	eor	x9, x8, x8
.Ltmp430:
	cbnz	x9, .Ltmp430
	//NO_APP
	tbnz	w8, #5, .LBB35_26
.LBB35_20:
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #1
	ldr	x8, [x21, #80]
	add	x8, x8, #468
	//APP
	str	w9, [x8]
	//NO_APP
	ldr	x0, [x21, #208]
	bl	__clk_is_enabled
	tbnz	w0, #0, .LBB35_23
// %bb.21:
	ldr	x20, [x21, #208]
	mov	x0, x20
	bl	clk_prepare
	cbnz	w0, .LBB35_48
// %bb.22:
	mov	x0, x20
	bl	clk_enable
	cbnz	w0, .LBB35_47
.LBB35_23:
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #8388608
	ldr	x8, [x21, #32]
	add	x8, x8, #2384
	//APP
	str	w9, [x8]
	//NO_APP
	ldr	x20, [x21, #208]
	mov	x0, x20
	bl	clk_disable
	mov	x0, x20
	bl	clk_unprepare
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #4128
	ldr	x8, [x21, #56]
	movk	w9, #4128, lsl #16
	//APP
	str	w9, [x8]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x8, [x21, #64]
	mov	w9, #4096
	mov	w0, #108
	mov	w1, wzr
	add	x8, x8, #96
	//APP
	str	w9, [x8]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #268435456
	ldr	x8, [x21, #32]
	add	x8, x8, #96
	//APP
	str	w9, [x8]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #524288
	ldr	x8, [x21, #80]
	add	x8, x8, #524
	//APP
	str	w9, [x8]
	//NO_APP
	bl	pmic_write_reg
	mov	x0, x19
	mov	w1, #1
	bl	ufs_vddc_mem_volt_vote
	mov	w20, wzr
.LBB35_24:
	str	wzr, [x19, #208]
.LBB35_25:
	mov	w0, w20
	ldp	x20, x19, [sp, #32]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #48             // 16-byte Folded Reload
	ret
.LBB35_26:
	mov	w0, #300
	bl	msleep
	b	.LBB35_20
.LBB35_27:
	adrp	x0, .L.str.114
	add	x0, x0, :lo12:.L.str.114
	adrp	x1, .L__func__.dfa_power_off
	add	x1, x1, :lo12:.L__func__.dfa_power_off
	mov	w2, w20
	bl	printk
.LBB35_28:
	mov	w20, #-1
.LBB35_29:
	ldr	w8, [x19, #128]
	cmp	w8, #1
	b.ne	.LBB35_31
.LBB35_30:
	strb	wzr, [x19, #368]
	b	.LBB35_24
.LBB35_31:
	mov	x0, x19
	mov	w1, #1
	bl	ufshcd_set_dev_pwr_mode
	cbz	w0, .LBB35_30
// %bb.32:
	adrp	x0, .L.str.56
	add	x0, x0, :lo12:.L.str.56
	b	.LBB35_36
.LBB35_33:
	mov	x19, x0
	ldr	x0, [x0, #96]
	adrp	x1, .L.str.53
	add	x1, x1, :lo12:.L.str.53
	bl	_dev_err
	mov	w20, #-16
	b	.LBB35_25
.LBB35_34:
	adrp	x0, .L.str.117
	add	x0, x0, :lo12:.L.str.117
	adrp	x1, .L__func__.dfa_suspend_prepare
	add	x1, x1, :lo12:.L__func__.dfa_suspend_prepare
	bl	printk
	b	.LBB35_28
.LBB35_35:
	mov	w20, w0
	adrp	x0, .L.str.54
	add	x0, x0, :lo12:.L.str.54
.LBB35_36:
	adrp	x1, .L__func__.dfa_suspend
	add	x1, x1, :lo12:.L__func__.dfa_suspend
	mov	w2, w20
	bl	printk
	b	.LBB35_30
.LBB35_37:
	mov	w20, w0
	adrp	x0, .L.str.115
	add	x0, x0, :lo12:.L.str.115
	b	.LBB35_39
.LBB35_38:
	mov	w20, w0
	adrp	x0, .L.str.116
	add	x0, x0, :lo12:.L.str.116
.LBB35_39:
	adrp	x1, .L__func__.dfa_power_off
	add	x1, x1, :lo12:.L__func__.dfa_power_off
	mov	w2, w20
	bl	printk
	cmn	w20, #1
	b.eq	.LBB35_29
// %bb.40:
	add	x0, x22, #1
	mov	w1, #1
	mov	x2, xzr
	mov	x3, xzr
	bl	atfd_ufs_smc
	mov	w20, w0
	cbnz	w0, .LBB35_49
.LBB35_41:
	//APP
	dmb	oshst
	//NO_APP
	mov	w22, #4999
	ldr	x8, [x19, #7016]
	mov	w9, #2
	add	x8, x8, #24
	//APP
	str	w9, [x8]
	//NO_APP
.LBB35_42:                              // =>This Inner Loop Header: Depth=1
	ldr	x8, [x19, #7008]
	add	x8, x8, #68
	//APP
.Ltmp431:
	ldr	w21, [x8]
.Ltmp432:
	.section	.altinstructions,"a",@progbits
.Ltmp433:
	.word	.Ltmp431-.Ltmp433
.Ltmp435:
	.word	.Ltmp434-.Ltmp435
	.hword	1
	.byte	.Ltmp432-.Ltmp431
	.byte	.Ltmp436-.Ltmp434
	.text

	.text	1
.Ltmp434:
	ldar	w21, [x8]
.Ltmp436:
.Ltmp437:
.org (.Ltmp437-(.Ltmp436-.Ltmp434))+(.Ltmp432-.Ltmp431), 0
.Ltmp438:
.org (.Ltmp438-(.Ltmp432-.Ltmp431))+(.Ltmp436-.Ltmp434), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w8, w21
	//APP
	eor	x9, x8, x8
.Ltmp439:
	cbnz	x9, .Ltmp439
	//NO_APP
	tbnz	w8, #1, .LBB35_44
// %bb.43:                              //   in Loop: Header=BB35_42 Depth=1
	mov	w0, #4295
	bl	__const_udelay
	sub	w22, w22, #1
	cmn	w22, #1
	b.ne	.LBB35_42
	b	.LBB35_45
.LBB35_44:
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #2
	ldr	x8, [x19, #7008]
	add	x8, x8, #68
	//APP
	str	w9, [x8]
	//NO_APP
.LBB35_45:
	//APP
	dmb	oshst
	//NO_APP
	cmp	w22, #0
	ldr	x9, [x19, #7016]
	mov	w8, wzr
	add	x9, x9, #24
	//APP
	str	w8, [x9]
	//NO_APP
	b.gt	.LBB35_29
// %bb.46:
	adrp	x0, .L.str.118
	add	x0, x0, :lo12:.L.str.118
	adrp	x1, .L__func__.dfa_que_proc_reload
	add	x1, x1, :lo12:.L__func__.dfa_que_proc_reload
	mov	w2, w21
	bl	printk
	b	.LBB35_29
.LBB35_47:
	mov	x0, x20
	bl	clk_unprepare
.LBB35_48:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.71
	add	x1, x1, :lo12:.L.str.71
	adrp	x2, .L__func__.dfa_power_off
	add	x2, x2, :lo12:.L__func__.dfa_power_off
	bl	_dev_err
	b	.LBB35_28
.LBB35_49:
	adrp	x0, .L.str.55
	add	x0, x0, :lo12:.L.str.55
	adrp	x1, .L__func__.dfa_suspend
	add	x1, x1, :lo12:.L__func__.dfa_suspend
	mov	w2, w20
	bl	printk
	b	.LBB35_41
.Lfunc_end35:
	.size	dfa_suspend, .Lfunc_end35-dfa_suspend
                                        // -- End function
	.globl	dfa_resume                      // -- Begin function dfa_resume
	.p2align	2
	.type	dfa_resume,@function
dfa_resume:                             // @dfa_resume
// %bb.0:
	stp	x29, x30, [sp, #-48]!           // 16-byte Folded Spill
	stp	x22, x21, [sp, #16]             // 16-byte Folded Spill
	mov	x29, sp
	stp	x20, x19, [sp, #32]             // 16-byte Folded Spill
	cbnz	w1, .LBB36_3
// %bb.1:
	mov	w20, wzr
.LBB36_2:
	mov	w0, w20
	ldp	x20, x19, [sp, #32]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #48             // 16-byte Folded Reload
	ret
.LBB36_3:
	ldr	w8, [x0, #7224]
	cbnz	w8, .LBB36_17
// %bb.4:
	mov	w1, wzr
	mov	x19, x0
	bl	ufs_vddc_mem_volt_vote
	mov	w0, #108
	mov	w1, #1
	ldr	x22, [x19, #296]
	bl	pmic_write_reg
	mov	w0, #55132
	movk	w0, #163, lsl #16
	bl	__const_udelay
	mov	w0, #13573
	mov	w1, #1
	movk	w0, #49920, lsl #16
	mov	x2, xzr
	mov	x3, xzr
	bl	atfd_ufs_smc
	cbnz	w0, .LBB36_18
// %bb.5:
	ldr	x8, [x22, #48]
	add	x8, x8, #1220
	//APP
.Ltmp440:
	ldr	w8, [x8]
.Ltmp441:
	.section	.altinstructions,"a",@progbits
.Ltmp442:
	.word	.Ltmp440-.Ltmp442
.Ltmp444:
	.word	.Ltmp443-.Ltmp444
	.hword	1
	.byte	.Ltmp441-.Ltmp440
	.byte	.Ltmp445-.Ltmp443
	.text

	.text	1
.Ltmp443:
	ldar	w8, [x8]
.Ltmp445:
.Ltmp446:
.org (.Ltmp446-(.Ltmp445-.Ltmp443))+(.Ltmp441-.Ltmp440), 0
.Ltmp447:
.org (.Ltmp447-(.Ltmp441-.Ltmp440))+(.Ltmp445-.Ltmp443), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w9, w8
	tst	w8, #0x1
	//APP
	eor	x8, x9, x9
.Ltmp448:
	cbnz	x8, .Ltmp448
	//NO_APP
	mov	w9, #55744
	mov	w8, #11392
	movk	w9, #3646, lsl #16
	movk	w8, #6378, lsl #16
	ldr	x0, [x22, #200]
	csel	x1, x9, x8, eq
	bl	clk_set_rate
	cbnz	w0, .LBB36_19
// %bb.6:
	//APP
	dmb	oshst
	//NO_APP
	mov	w0, #4295
	ldr	x8, [x22, #80]
	mov	w9, #6
	movk	w9, #30, lsl #16
	add	x8, x8, #612
	//APP
	str	w9, [x8]
	//NO_APP
	bl	__const_udelay
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #524288
	ldr	x8, [x22, #80]
	add	x8, x8, #528
	//APP
	str	w9, [x8]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #4096
	ldr	x8, [x22, #64]
	add	x8, x8, #100
	//APP
	str	w9, [x8]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #536870912
	ldr	x8, [x22, #32]
	add	x8, x8, #100
	//APP
	str	w9, [x8]
	//NO_APP
	ldr	x21, [x22, #200]
	mov	x0, x21
	bl	clk_prepare
	mov	w20, w0
	cbnz	w0, .LBB36_21
// %bb.7:
	mov	x0, x21
	bl	clk_enable
	cbnz	w0, .LBB36_20
// %bb.8:
	//APP
	dmb	oshst
	//NO_APP
	mov	w1, #30976
	ldr	x8, [x22, #80]
	mov	w9, #1
	movk	w1, #14709, lsl #16
	add	x8, x8, #464
	//APP
	str	w9, [x8]
	//NO_APP
	ldr	x0, [x22, #208]
	bl	clk_set_rate
	cbnz	w0, .LBB36_23
// %bb.9:
	ldr	x21, [x22, #208]
	mov	x0, x21
	bl	clk_prepare
	mov	w20, w0
	cbnz	w0, .LBB36_25
// %bb.10:
	mov	x0, x21
	bl	clk_enable
	cbnz	w0, .LBB36_24
// %bb.11:
	mov	w20, #4999
.LBB36_12:                              // =>This Inner Loop Header: Depth=1
	ldr	x8, [x22, #96]
	add	x8, x8, #1064
	//APP
.Ltmp449:
	ldr	w8, [x8]
.Ltmp450:
	.section	.altinstructions,"a",@progbits
.Ltmp451:
	.word	.Ltmp449-.Ltmp451
.Ltmp453:
	.word	.Ltmp452-.Ltmp453
	.hword	1
	.byte	.Ltmp450-.Ltmp449
	.byte	.Ltmp454-.Ltmp452
	.text

	.text	1
.Ltmp452:
	ldar	w8, [x8]
.Ltmp454:
.Ltmp455:
.org (.Ltmp455-(.Ltmp454-.Ltmp452))+(.Ltmp450-.Ltmp449), 0
.Ltmp456:
.org (.Ltmp456-(.Ltmp450-.Ltmp449))+(.Ltmp454-.Ltmp452), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w8, w8
	//APP
	eor	x9, x8, x8
.Ltmp457:
	cbnz	x9, .Ltmp457
	//NO_APP
	tbz	w8, #2, .LBB36_28
// %bb.13:                              //   in Loop: Header=BB36_12 Depth=1
	mov	w0, #4295
	bl	__const_udelay
	sub	w20, w20, #1
	cmn	w20, #1
	b.ne	.LBB36_12
.LBB36_14:
	adrp	x0, .L.str.119
	add	x0, x0, :lo12:.L.str.119
.LBB36_15:
	adrp	x1, .L__func__.dfa_power_on
	add	x1, x1, :lo12:.L__func__.dfa_power_on
	bl	printk
.LBB36_16:
	ldr	x19, [x22, #208]
	mov	x0, x19
	bl	clk_disable
	mov	x0, x19
	bl	clk_unprepare
	b	.LBB36_27
.LBB36_17:
	mov	x19, x0
	ldr	x0, [x0, #96]
	adrp	x1, .L.str.57
	add	x1, x1, :lo12:.L.str.57
	bl	_dev_err
	mov	w20, #-16
	b	.LBB36_2
.LBB36_18:
	mov	w20, w0
	adrp	x0, .L.str.55
	add	x0, x0, :lo12:.L.str.55
	adrp	x1, .L__func__.dfa_power_on
	add	x1, x1, :lo12:.L__func__.dfa_power_on
	mov	w2, w20
	bl	printk
	b	.LBB36_2
.LBB36_19:
	mov	w20, w0
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.48
	add	x1, x1, :lo12:.L.str.48
	b	.LBB36_22
.LBB36_20:
	mov	w20, w0
	mov	x0, x21
	bl	clk_unprepare
.LBB36_21:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.49
	add	x1, x1, :lo12:.L.str.49
.LBB36_22:
	adrp	x2, .L__func__.dfa_power_on
	add	x2, x2, :lo12:.L__func__.dfa_power_on
	mov	w3, w20
	bl	_dev_err
	b	.LBB36_2
.LBB36_23:
	mov	w3, w0
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.51
	add	x1, x1, :lo12:.L.str.51
	adrp	x2, .L__func__.dfa_power_on
	add	x2, x2, :lo12:.L__func__.dfa_power_on
	b	.LBB36_26
.LBB36_24:
	mov	w20, w0
	mov	x0, x21
	bl	clk_unprepare
.LBB36_25:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.52
	add	x1, x1, :lo12:.L.str.52
	adrp	x2, .L__func__.dfa_power_on
	add	x2, x2, :lo12:.L__func__.dfa_power_on
	mov	w3, w20
.LBB36_26:
	bl	_dev_err
.LBB36_27:
	ldr	x19, [x22, #200]
	mov	x0, x19
	bl	clk_disable
	mov	x0, x19
	bl	clk_unprepare
	mov	w20, #-1
	b	.LBB36_2
.LBB36_28:
	cmp	w20, #0
	b.le	.LBB36_14
// %bb.29:
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #7080
	ldr	x10, [x19, #7008]
	mov	w20, #4999
	mov	w8, wzr
	ldrb	w9, [x19, x9]
	add	x10, x10, #256
	//APP
	str	w9, [x10]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	mov	w10, #1
	ldr	x9, [x19, #7008]
	add	x9, x9, #260
	//APP
	str	w10, [x9]
	//NO_APP
	ldr	x9, [x19, #7008]
	add	x9, x9, #36
	//APP
.Ltmp458:
	ldr	w9, [x9]
.Ltmp459:
	.section	.altinstructions,"a",@progbits
.Ltmp460:
	.word	.Ltmp458-.Ltmp460
.Ltmp462:
	.word	.Ltmp461-.Ltmp462
	.hword	1
	.byte	.Ltmp459-.Ltmp458
	.byte	.Ltmp463-.Ltmp461
	.text

	.text	1
.Ltmp461:
	ldar	w9, [x9]
.Ltmp463:
.Ltmp464:
.org (.Ltmp464-(.Ltmp463-.Ltmp461))+(.Ltmp459-.Ltmp458), 0
.Ltmp465:
.org (.Ltmp465-(.Ltmp459-.Ltmp458))+(.Ltmp463-.Ltmp461), 0
	.text


	//NO_APP
	mov	w10, w9
	//APP
	dmb	oshld
	//NO_APP
	and	w9, w9, #0xfffffffe
	//APP
	eor	x10, x10, x10
.Ltmp466:
	cbnz	x10, .Ltmp466
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x10, [x19, #7008]
	add	x10, x10, #36
	//APP
	str	w9, [x10]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	mov	w10, #268435456
	ldr	x9, [x22, #32]
	add	x9, x9, #100
	//APP
	str	w10, [x9]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x9, [x19, #7008]
	add	x9, x9, #100
	//APP
	str	w8, [x9]
	//NO_APP
.LBB36_30:                              // =>This Inner Loop Header: Depth=1
	ldr	x8, [x19, #7008]
	add	x8, x8, #104
	//APP
.Ltmp467:
	ldr	w8, [x8]
.Ltmp468:
	.section	.altinstructions,"a",@progbits
.Ltmp469:
	.word	.Ltmp467-.Ltmp469
.Ltmp471:
	.word	.Ltmp470-.Ltmp471
	.hword	1
	.byte	.Ltmp468-.Ltmp467
	.byte	.Ltmp472-.Ltmp470
	.text

	.text	1
.Ltmp470:
	ldar	w8, [x8]
.Ltmp472:
.Ltmp473:
.org (.Ltmp473-(.Ltmp472-.Ltmp470))+(.Ltmp468-.Ltmp467), 0
.Ltmp474:
.org (.Ltmp474-(.Ltmp468-.Ltmp467))+(.Ltmp472-.Ltmp470), 0
	.text


	//NO_APP
	mov	w9, w8
	//APP
	dmb	oshld
	//NO_APP
	tst	w8, #0x3
	//APP
	eor	x9, x9, x9
.Ltmp475:
	cbnz	x9, .Ltmp475
	//NO_APP
	b.eq	.LBB36_33
// %bb.31:                              //   in Loop: Header=BB36_30 Depth=1
	mov	w0, #4295
	bl	__const_udelay
	sub	w20, w20, #1
	cmn	w20, #1
	b.ne	.LBB36_30
.LBB36_32:
	adrp	x0, .L.str.120
	add	x0, x0, :lo12:.L.str.120
	b	.LBB36_15
.LBB36_33:
	cmp	w20, #0
	b.le	.LBB36_32
// %bb.34:
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #-63869
	ldr	x8, [x22, #8]
	mov	w21, #4999
	add	x8, x8, #8
	//APP
	str	w9, [x8]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #270532608
	ldr	x8, [x22, #56]
	//APP
	str	w9, [x8]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #2
	ldr	x8, [x19, #7016]
	add	x8, x8, #24
	//APP
	str	w9, [x8]
	//NO_APP
.LBB36_35:                              // =>This Inner Loop Header: Depth=1
	ldr	x8, [x19, #7008]
	add	x8, x8, #68
	//APP
.Ltmp476:
	ldr	w20, [x8]
.Ltmp477:
	.section	.altinstructions,"a",@progbits
.Ltmp478:
	.word	.Ltmp476-.Ltmp478
.Ltmp480:
	.word	.Ltmp479-.Ltmp480
	.hword	1
	.byte	.Ltmp477-.Ltmp476
	.byte	.Ltmp481-.Ltmp479
	.text

	.text	1
.Ltmp479:
	ldar	w20, [x8]
.Ltmp481:
.Ltmp482:
.org (.Ltmp482-(.Ltmp481-.Ltmp479))+(.Ltmp477-.Ltmp476), 0
.Ltmp483:
.org (.Ltmp483-(.Ltmp477-.Ltmp476))+(.Ltmp481-.Ltmp479), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w8, w20
	//APP
	eor	x9, x8, x8
.Ltmp484:
	cbnz	x9, .Ltmp484
	//NO_APP
	tbnz	w8, #1, .LBB36_37
// %bb.36:                              //   in Loop: Header=BB36_35 Depth=1
	mov	w0, #4295
	bl	__const_udelay
	sub	w21, w21, #1
	cmn	w21, #1
	b.ne	.LBB36_35
	b	.LBB36_38
.LBB36_37:
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #2
	ldr	x8, [x19, #7008]
	add	x8, x8, #68
	//APP
	str	w9, [x8]
	//NO_APP
.LBB36_38:
	//APP
	dmb	oshst
	//NO_APP
	cmp	w21, #0
	ldr	x9, [x19, #7016]
	mov	w8, wzr
	add	x9, x9, #24
	//APP
	str	w8, [x9]
	//NO_APP
	b.gt	.LBB36_40
// %bb.39:
	adrp	x0, .L.str.118
	add	x0, x0, :lo12:.L.str.118
	adrp	x1, .L__func__.dfa_que_proc_reload
	add	x1, x1, :lo12:.L__func__.dfa_que_proc_reload
	mov	w2, w20
	bl	printk
	b	.LBB36_16
.LBB36_40:
	ldr	x8, [x19, #7008]
	add	x8, x8, #36
	//APP
.Ltmp485:
	ldr	w8, [x8]
.Ltmp486:
	.section	.altinstructions,"a",@progbits
.Ltmp487:
	.word	.Ltmp485-.Ltmp487
.Ltmp489:
	.word	.Ltmp488-.Ltmp489
	.hword	1
	.byte	.Ltmp486-.Ltmp485
	.byte	.Ltmp490-.Ltmp488
	.text

	.text	1
.Ltmp488:
	ldar	w8, [x8]
.Ltmp490:
.Ltmp491:
.org (.Ltmp491-(.Ltmp490-.Ltmp488))+(.Ltmp486-.Ltmp485), 0
.Ltmp492:
.org (.Ltmp492-(.Ltmp486-.Ltmp485))+(.Ltmp490-.Ltmp488), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w9, w8
	orr	w8, w8, #0x1
	//APP
	eor	x9, x9, x9
.Ltmp493:
	cbnz	x9, .Ltmp493
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x9, [x19, #7008]
	add	x9, x9, #36
	//APP
	str	w8, [x9]
	//NO_APP
	mov	w8, #1
	ldr	w9, [x19, #7032]
	str	w8, [x19, #208]
	cbz	w9, .LBB36_43
// %bb.41:
	mov	x8, xzr
	mov	x9, xzr
.LBB36_42:                              // =>This Inner Loop Header: Depth=1
	ldr	x10, [x19, #7024]
	add	x9, x9, #1
	add	x10, x10, x8
	add	x8, x8, #80
	stur	xzr, [x10, #60]
	str	xzr, [x10, #72]
	ldr	w10, [x19, #7032]
	cmp	x9, x10
	b.lo	.LBB36_42
.LBB36_43:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.58
	add	x1, x1, :lo12:.L.str.58
	adrp	x2, .L__func__.dfa_resume
	add	x2, x2, :lo12:.L__func__.dfa_resume
	bl	_dev_info
	ldr	w8, [x19, #128]
	cmp	w8, #1
	b.ne	.LBB36_46
.LBB36_44:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.60
	add	x1, x1, :lo12:.L.str.60
	adrp	x2, .L__func__.dfa_resume
	add	x2, x2, :lo12:.L__func__.dfa_resume
	bl	_dev_info
	mov	x0, x19
	bl	ufshcd_clear_ua_wluns
	mov	w8, #7232
	add	x21, x19, x8
	mov	x0, x21
	bl	mutex_lock
	mov	x0, x19
	bl	dfa_resume_inline_crypt_key
	mov	w20, w0
	mov	x0, x19
	bl	dfa_set_timestamp_attr
	mov	x0, x21
	bl	mutex_unlock
	cbnz	w20, .LBB36_48
.LBB36_45:
	mov	x0, x19
	bl	ufshcd_enable_autodiv
	mov	w20, wzr
	str	wzr, [x19, #208]
	strb	wzr, [x19, #368]
	b	.LBB36_2
.LBB36_46:
	mov	x0, x19
	mov	w1, #1
	bl	ufshcd_set_dev_pwr_mode
	cbz	w0, .LBB36_44
// %bb.47:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.59
	add	x1, x1, :lo12:.L.str.59
	bl	_dev_err
	b	.LBB36_45
.LBB36_48:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.61
	add	x1, x1, :lo12:.L.str.61
	mov	w2, w20
	bl	_dev_err
	b	.LBB36_45
.Lfunc_end36:
	.size	dfa_resume, .Lfunc_end36-dfa_resume
                                        // -- End function
	.globl	dfa_set_timestamp_attr          // -- Begin function dfa_set_timestamp_attr
	.p2align	2
	.type	dfa_set_timestamp_attr,@function
dfa_set_timestamp_attr:                 // @dfa_set_timestamp_attr
// %bb.0:
	sub	sp, sp, #80
	adrp	x8, :got:__stack_chk_guard
	stp	x29, x30, [sp, #32]             // 16-byte Folded Spill
	str	x21, [sp, #48]                  // 8-byte Folded Spill
	add	x29, sp, #32
	stp	x20, x19, [sp, #64]             // 16-byte Folded Spill
	add	x19, x0, #3888
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	mov	x20, x0
	mov	x0, x19
	ldr	x8, [x8]
	stur	x8, [x29, #-8]
	stp	xzr, xzr, [sp, #8]
	bl	mutex_lock
	add	x1, sp, #16
	add	x2, sp, #8
	mov	x0, x20
	mov	w3, #4
	mov	w4, #48
	mov	w5, wzr
	mov	w6, wzr
	bl	ufshcd_init_query
	ldr	x8, [sp, #16]
	mov	w9, #129
	mov	w0, wzr
	strb	w9, [x8]
	ldr	x21, [sp, #16]
	bl	ktime_get_with_offset
	rev	x8, x0
	mov	x0, x20
	mov	w1, #1
	mov	w2, #1500
	str	x8, [x21, #8]
	bl	dfa_exec_dev_cmd
	cbnz	w0, .LBB37_3
.LBB37_1:
	mov	x0, x19
	bl	mutex_unlock
	adrp	x8, :got:__stack_chk_guard
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldur	x9, [x29, #-8]
	ldr	x8, [x8]
	cmp	x8, x9
	b.ne	.LBB37_4
// %bb.2:
	ldp	x20, x19, [sp, #64]             // 16-byte Folded Reload
	ldp	x29, x30, [sp, #32]             // 16-byte Folded Reload
	ldr	x21, [sp, #48]                  // 8-byte Folded Reload
	add	sp, sp, #80
	ret
.LBB37_3:
	mov	w3, w0
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.74
	add	x1, x1, :lo12:.L.str.74
	adrp	x2, .L__func__.dfa_set_timestamp_attr
	add	x2, x2, :lo12:.L__func__.dfa_set_timestamp_attr
	bl	_dev_err
	b	.LBB37_1
.LBB37_4:
	bl	__stack_chk_fail
.Lfunc_end37:
	.size	dfa_set_timestamp_attr, .Lfunc_end37-dfa_set_timestamp_attr
                                        // -- End function
	.globl	dfa_dump_lrbp_status            // -- Begin function dfa_dump_lrbp_status
	.p2align	2
	.type	dfa_dump_lrbp_status,@function
dfa_dump_lrbp_status:                   // @dfa_dump_lrbp_status
// %bb.0:
	sub	sp, sp, #80
	mov	w8, #7040
	stp	x20, x19, [sp, #64]             // 16-byte Folded Spill
	add	x20, x0, x8
	mov	x19, x0
	ldrsw	x1, [x0, #264]
	mov	x0, x20
	mov	x2, xzr
	stp	x29, x30, [sp, #16]             // 16-byte Folded Spill
	stp	x24, x23, [sp, #32]             // 16-byte Folded Spill
	add	x29, sp, #16
	stp	x22, x21, [sp, #48]             // 16-byte Folded Spill
	bl	find_next_bit
	ldr	w8, [x19, #264]
	mov	x22, x0
	cmp	w8, w22
	b.le	.LBB38_3
// %bb.1:
	mov	x24, #63439
	mov	w23, #216
	movk	x24, #58195, lsl #16
	adrp	x21, .L.str.62
	add	x21, x21, :lo12:.L.str.62
	movk	x24, #39845, lsl #32
	movk	x24, #8388, lsl #48
.LBB38_2:                               // =>This Inner Loop Header: Depth=1
	ldr	x8, [x19, #224]
	mov	x1, x21
	ldr	x0, [x19, #96]
	mov	w2, w22
	smaddl	x8, w22, w23, x8
	ldp	x9, x10, [x8, #112]
	ldr	w3, [x8, #100]
	ldrb	w4, [x8, #128]
	smulh	x9, x9, x24
	ldrb	w5, [x8, #194]
	smulh	x10, x10, x24
	ldrb	w6, [x8, #193]
	asr	x11, x9, #7
	add	x7, x11, x9, lsr #63
	asr	x9, x10, #7
	add	x8, x9, x10, lsr #63
	str	x8, [sp]
	bl	_dev_err
	add	w8, w22, #1
	ldrsw	x1, [x19, #264]
	sxtw	x2, w8
	mov	x0, x20
	bl	find_next_bit
	ldr	w8, [x19, #264]
	mov	x22, x0
	cmp	w8, w22
	b.gt	.LBB38_2
.LBB38_3:
	ldr	w8, [x19, #7032]
	cbnz	w8, .LBB38_5
.LBB38_4:
	ldp	x20, x19, [sp, #64]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #48]             // 16-byte Folded Reload
	ldp	x24, x23, [sp, #32]             // 16-byte Folded Reload
	ldp	x29, x30, [sp, #16]             // 16-byte Folded Reload
	add	sp, sp, #80
	ret
.LBB38_5:
	mov	x22, xzr
	mov	x23, xzr
	adrp	x20, .L.str.113
	add	x20, x20, :lo12:.L.str.113
	adrp	x21, .L__func__.dfa_dump_lrbp_status
	add	x21, x21, :lo12:.L__func__.dfa_dump_lrbp_status
	b	.LBB38_7
.LBB38_6:                               //   in Loop: Header=BB38_7 Depth=1
	add	x23, x23, #1
	add	x22, x22, #80
	cmp	x23, w8, uxtw
	b.hs	.LBB38_4
.LBB38_7:                               // =>This Inner Loop Header: Depth=1
	ldr	x9, [x19, #7024]
	add	x9, x9, x22
	ldp	w6, w5, [x9, #72]
	ldr	w4, [x9, #60]
	cmp	w5, w6
	ccmp	w4, w5, #0, eq
	b.eq	.LBB38_6
// %bb.8:                               //   in Loop: Header=BB38_7 Depth=1
	ldr	w2, [x9, #36]
	mov	x0, x20
	ldr	w3, [x9, #64]
	mov	x1, x21
	bl	printk
	ldr	w8, [x19, #7032]
	b	.LBB38_6
.Lfunc_end38:
	.size	dfa_dump_lrbp_status, .Lfunc_end38-dfa_dump_lrbp_status
                                        // -- End function
	.globl	is_dfa_host                     // -- Begin function is_dfa_host
	.p2align	2
	.type	is_dfa_host,@function
is_dfa_host:                            // @is_dfa_host
// %bb.0:
	ldrb	w8, [x0, #584]
	ubfx	w0, w8, #2, #1
	ret
.Lfunc_end39:
	.size	is_dfa_host, .Lfunc_end39-is_dfa_host
                                        // -- End function
	.globl	is_dfa_enabled_backdata         // -- Begin function is_dfa_enabled_backdata
	.p2align	2
	.type	is_dfa_enabled_backdata,@function
is_dfa_enabled_backdata:                // @is_dfa_enabled_backdata
// %bb.0:
	ldr	x8, [x0, #296]
	ldr	x8, [x8, #48]
	add	x8, x8, #1220
	//APP
.Ltmp494:
	ldr	w8, [x8]
.Ltmp495:
	.section	.altinstructions,"a",@progbits
.Ltmp496:
	.word	.Ltmp494-.Ltmp496
.Ltmp498:
	.word	.Ltmp497-.Ltmp498
	.hword	1
	.byte	.Ltmp495-.Ltmp494
	.byte	.Ltmp499-.Ltmp497
	.text

	.text	1
.Ltmp497:
	ldar	w8, [x8]
.Ltmp499:
.Ltmp500:
.org (.Ltmp500-(.Ltmp499-.Ltmp497))+(.Ltmp495-.Ltmp494), 0
.Ltmp501:
.org (.Ltmp501-(.Ltmp495-.Ltmp494))+(.Ltmp499-.Ltmp497), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w8, w8
	tst	x8, #0x18
	//APP
	eor	x8, x8, x8
.Ltmp502:
	cbnz	x8, .Ltmp502
	//NO_APP
	cset	w0, ne
	ret
.Lfunc_end40:
	.size	is_dfa_enabled_backdata, .Lfunc_end40-is_dfa_enabled_backdata
                                        // -- End function
	.globl	dfa_mcq_params_init             // -- Begin function dfa_mcq_params_init
	.p2align	2
	.type	dfa_mcq_params_init,@function
dfa_mcq_params_init:                    // @dfa_mcq_params_init
// %bb.0:
	stp	x29, x30, [sp, #-32]!           // 16-byte Folded Spill
	ldr	x9, [x0, #7008]
	stp	x20, x19, [sp, #16]             // 16-byte Folded Spill
	mov	w8, #7000
	mov	x29, sp
	add	x8, x0, x8
	add	x9, x9, #256
	//APP
.Ltmp503:
	ldr	w9, [x9]
.Ltmp504:
	.section	.altinstructions,"a",@progbits
.Ltmp505:
	.word	.Ltmp503-.Ltmp505
.Ltmp507:
	.word	.Ltmp506-.Ltmp507
	.hword	1
	.byte	.Ltmp504-.Ltmp503
	.byte	.Ltmp508-.Ltmp506
	.text

	.text	1
.Ltmp506:
	ldar	w9, [x9]
.Ltmp508:
.Ltmp509:
.org (.Ltmp509-(.Ltmp508-.Ltmp506))+(.Ltmp504-.Ltmp503), 0
.Ltmp510:
.org (.Ltmp510-(.Ltmp504-.Ltmp503))+(.Ltmp508-.Ltmp506), 0
	.text


	//NO_APP
	mov	w10, w9
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x10, x10, x10
.Ltmp511:
	cbnz	x10, .Ltmp511
	//NO_APP
	cbnz	w9, .LBB41_3
// %bb.1:
	strb	wzr, [x8]
.LBB41_2:
	ldp	x20, x19, [sp, #16]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #32             // 16-byte Folded Reload
	ret
.LBB41_3:
	mov	w9, #1
	str	x0, [x0, #6992]
	mov	x19, x0
	adrp	x1, .L.str.63
	add	x1, x1, :lo12:.L.str.63
	adrp	x2, dfa_mcq_params_init.__key
	add	x2, x2, :lo12:dfa_mcq_params_init.__key
	strb	w9, [x8]
	str	wzr, [x0, #7072]
	ldr	x9, [x0, #88]
	ldr	x10, [x9, #584]
	orr	x10, x10, #0x4
	str	x10, [x9, #584]
	ldr	x9, [x0, #7008]
	add	x9, x9, #256
	//APP
.Ltmp512:
	ldr	w9, [x9]
.Ltmp513:
	.section	.altinstructions,"a",@progbits
.Ltmp514:
	.word	.Ltmp512-.Ltmp514
.Ltmp516:
	.word	.Ltmp515-.Ltmp516
	.hword	1
	.byte	.Ltmp513-.Ltmp512
	.byte	.Ltmp517-.Ltmp515
	.text

	.text	1
.Ltmp515:
	ldar	w9, [x9]
.Ltmp517:
.Ltmp518:
.org (.Ltmp518-(.Ltmp517-.Ltmp515))+(.Ltmp513-.Ltmp512), 0
.Ltmp519:
.org (.Ltmp519-(.Ltmp513-.Ltmp512))+(.Ltmp517-.Ltmp515), 0
	.text


	//NO_APP
	mov	w10, w9
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x10, x10, x10
.Ltmp520:
	cbnz	x10, .Ltmp520
	//NO_APP
	strb	w9, [x8, #80]
	mov	w10, #7128
	add	x20, x0, x10
	mov	x0, x20
	bl	__kthread_init_worker
	adrp	x0, :got:kthread_worker_fn
	adrp	x3, .L.str.64
	add	x3, x3, :lo12:.L.str.64
	mov	x1, x20
	mov	w2, #-1
	ldr	x0, [x0, :got_lo12:kthread_worker_fn]
	bl	kthread_create_on_node
	mov	x20, x0
	cmn	x0, #1, lsl #12                 // =4096
	b.ls	.LBB41_10
.LBB41_4:
	mov	w8, #7184
	adrp	x9, :got:dfa_error_handler
	add	x8, x19, x8
	str	x20, [x19, #7120]
	str	xzr, [x19, #7208]
	adrp	x1, .L.str.66
	add	x1, x1, :lo12:.L.str.66
	str	xzr, [x19, #7216]
	ldr	x9, [x9, :got_lo12:dfa_error_handler]
	str	x8, [x19, #7184]
	str	x8, [x19, #7192]
	mov	w8, #7232
	add	x0, x19, x8
	adrp	x2, dfa_mcq_params_init.__key.65
	add	x2, x2, :lo12:dfa_mcq_params_init.__key.65
	str	x9, [x19, #7200]
	str	wzr, [x19, #7224]
	str	wzr, [x19, #7228]
	str	wzr, [x19, #7364]
	bl	__mutex_init
	mov	w9, #7280
	mov	x8, #68719476704
	add	x9, x19, x9
	adrp	x10, dfa_proc_reset_req_work_fn
	add	x10, x10, :lo12:dfa_proc_reset_req_work_fn
	adrp	x1, :got:delayed_work_timer_fn
	mov	w11, #7304
	str	x8, [x19, #7272]
	str	x9, [x19, #7280]
	add	x0, x19, x11
	str	x9, [x19, #7288]
	mov	w2, #2097152
	str	x10, [x19, #7296]
	mov	x3, xzr
	ldr	x1, [x1, :got_lo12:delayed_work_timer_fn]
	mov	x4, xzr
	bl	init_timer_key
	adrp	x0, .L.str.67
	add	x0, x0, :lo12:.L.str.67
	mov	w1, #2
	mov	w2, wzr
	bl	alloc_workqueue
	str	x0, [x19, #7264]
	str	wzr, [x19, #7360]
	cbz	x0, .LBB41_12
// %bb.5:
	adrp	x8, dfa_mcq_params_init.registered
	str	wzr, [x19, #7368]
	ldrb	w9, [x8, :lo12:dfa_mcq_params_init.registered]
	tbnz	w9, #0, .LBB41_7
// %bb.6:
	mov	w9, #1
	strb	w9, [x8, :lo12:dfa_mcq_params_init.registered]
.LBB41_7:
	mov	x2, #1811
	mov	w0, #818151424
	mov	w1, #17408
	movk	x2, #104, lsl #48
	mov	w3, wzr
	str	xzr, [x19, #7112]
	bl	__ioremap
	str	x0, [x19, #7096]
	cbz	x0, .LBB41_11
.LBB41_8:
	mov	w0, #57344
	mov	w1, #4096
	mov	x2, #1811
	movk	w0, #12262, lsl #16
	movk	w1, #1, lsl #16
	movk	x2, #104, lsl #48
	mov	w3, wzr
	bl	__ioremap
	str	x0, [x19, #7104]
	cbnz	x0, .LBB41_2
// %bb.9:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.70
	add	x1, x1, :lo12:.L.str.70
	bl	_dev_err
	b	.LBB41_2
.LBB41_10:
	mov	x0, x20
	bl	wake_up_process
	b	.LBB41_4
.LBB41_11:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.69
	add	x1, x1, :lo12:.L.str.69
	bl	_dev_err
	b	.LBB41_8
.LBB41_12:
	adrp	x0, .L.str.68
	add	x0, x0, :lo12:.L.str.68
	bl	panic
.Lfunc_end41:
	.size	dfa_mcq_params_init, .Lfunc_end41-dfa_mcq_params_init
                                        // -- End function
	.p2align	2                               // -- Begin function dfa_proc_reset_req_work_fn
	.type	dfa_proc_reset_req_work_fn,@function
dfa_proc_reset_req_work_fn:             // @dfa_proc_reset_req_work_fn
// %bb.0:
	stp	x29, x30, [sp, #-32]!           // 16-byte Folded Spill
	str	x19, [sp, #16]                  // 8-byte Folded Spill
	mov	x19, x0
	mov	w8, #2
	add	x9, x0, #88
	//APP
	prfm	pstl1strm, [x9]
.Ltmp521:
	ldxr	w2, [x9]
	eor	w10, w2, #0x1
	cbnz	w10, .Ltmp522
	stlxr	w10, w8, [x9]
	cbnz	w10, .Ltmp521
	dmb	ish
.Ltmp522:
	//NO_APP
	cmp	w2, #1
	mov	x29, sp
	b.ne	.LBB42_3
// %bb.1:
	mov	x8, #-7272
	add	x0, x19, x8
	bl	dfa_reset_directly
	str	wzr, [x19, #88]
.LBB42_2:
	ldr	x19, [sp, #16]                  // 8-byte Folded Reload
	ldp	x29, x30, [sp], #32             // 16-byte Folded Reload
	ret
.LBB42_3:
	mov	x8, #-7176
	cmp	w2, #2
	b.ne	.LBB42_5
// %bb.4:
	ldr	x0, [x19, x8]
	adrp	x1, .L.str.121
	add	x1, x1, :lo12:.L.str.121
	b	.LBB42_6
.LBB42_5:
	ldr	x0, [x19, x8]
	adrp	x1, .L.str.122
	add	x1, x1, :lo12:.L.str.122
.LBB42_6:
	bl	_dev_err
	b	.LBB42_2
.Lfunc_end42:
	.size	dfa_proc_reset_req_work_fn, .Lfunc_end42-dfa_proc_reset_req_work_fn
                                        // -- End function
	.globl	dfa_cpu_clk_vote                // -- Begin function dfa_cpu_clk_vote
	.p2align	2
	.type	dfa_cpu_clk_vote,@function
dfa_cpu_clk_vote:                       // @dfa_cpu_clk_vote
// %bb.0:
	stp	x29, x30, [sp, #-32]!           // 16-byte Folded Spill
	stp	x20, x19, [sp, #16]             // 16-byte Folded Spill
	ldr	x20, [x0, #296]
	mov	x19, x0
	mov	x29, sp
	ldr	x0, [x20, #208]
	tbz	w1, #0, .LBB43_4
// %bb.1:
	bl	__clk_is_enabled
	tbnz	w0, #0, .LBB43_3
// %bb.2:
	ldr	x0, [x20, #208]
	bl	clk_enable
	cbnz	w0, .LBB43_8
.LBB43_3:
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #8388608
	ldr	x8, [x20, #32]
	add	x8, x8, #2384
	//APP
	str	w9, [x8]
	//NO_APP
	ldr	x0, [x20, #208]
	bl	clk_disable
	b	.LBB43_5
.LBB43_4:
	bl	clk_enable
	cbnz	w0, .LBB43_6
.LBB43_5:
	ldp	x20, x19, [sp, #16]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #32             // 16-byte Folded Reload
	ret
.LBB43_6:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.72
	add	x1, x1, :lo12:.L.str.72
.LBB43_7:
	adrp	x2, .L__func__.dfa_cpu_clk_vote
	add	x2, x2, :lo12:.L__func__.dfa_cpu_clk_vote
	bl	_dev_err
	b	.LBB43_5
.LBB43_8:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.71
	add	x1, x1, :lo12:.L.str.71
	b	.LBB43_7
.Lfunc_end43:
	.size	dfa_cpu_clk_vote, .Lfunc_end43-dfa_cpu_clk_vote
                                        // -- End function
	.globl	dfa_sram_deep_sleep_vote        // -- Begin function dfa_sram_deep_sleep_vote
	.p2align	2
	.type	dfa_sram_deep_sleep_vote,@function
dfa_sram_deep_sleep_vote:               // @dfa_sram_deep_sleep_vote
// %bb.0:
	stp	x29, x30, [sp, #-48]!           // 16-byte Folded Spill
	str	x21, [sp, #16]                  // 8-byte Folded Spill
	mov	x29, sp
	stp	x20, x19, [sp, #32]             // 16-byte Folded Spill
	tbz	w1, #0, .LBB44_7
// %bb.1:
	mov	x19, x0
	ldr	x21, [x0, #296]
	mov	w20, #99
.LBB44_2:                               // =>This Inner Loop Header: Depth=1
	ldr	x8, [x21, #32]
	add	x8, x8, #1028
	//APP
.Ltmp523:
	ldr	w8, [x8]
.Ltmp524:
	.section	.altinstructions,"a",@progbits
.Ltmp525:
	.word	.Ltmp523-.Ltmp525
.Ltmp527:
	.word	.Ltmp526-.Ltmp527
	.hword	1
	.byte	.Ltmp524-.Ltmp523
	.byte	.Ltmp528-.Ltmp526
	.text

	.text	1
.Ltmp526:
	ldar	w8, [x8]
.Ltmp528:
.Ltmp529:
.org (.Ltmp529-(.Ltmp528-.Ltmp526))+(.Ltmp524-.Ltmp523), 0
.Ltmp530:
.org (.Ltmp530-(.Ltmp524-.Ltmp523))+(.Ltmp528-.Ltmp526), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w8, w8
	//APP
	eor	x9, x8, x8
.Ltmp531:
	cbnz	x9, .Ltmp531
	//NO_APP
	tbz	w8, #19, .LBB44_4
// %bb.3:                               //   in Loop: Header=BB44_2 Depth=1
	mov	w0, #4295
	bl	__const_udelay
	sub	w20, w20, #1
	cmn	w20, #1
	b.ne	.LBB44_2
	b	.LBB44_8
.LBB44_4:
	cmp	w20, #0
	b.le	.LBB44_8
// %bb.5:
	ldr	x8, [x19, #7008]
	mov	w0, wzr
	add	x8, x8, #40
	//APP
.Ltmp532:
	ldr	w8, [x8]
.Ltmp533:
	.section	.altinstructions,"a",@progbits
.Ltmp534:
	.word	.Ltmp532-.Ltmp534
.Ltmp536:
	.word	.Ltmp535-.Ltmp536
	.hword	1
	.byte	.Ltmp533-.Ltmp532
	.byte	.Ltmp537-.Ltmp535
	.text

	.text	1
.Ltmp535:
	ldar	w8, [x8]
.Ltmp537:
.Ltmp538:
.org (.Ltmp538-(.Ltmp537-.Ltmp535))+(.Ltmp533-.Ltmp532), 0
.Ltmp539:
.org (.Ltmp539-(.Ltmp533-.Ltmp532))+(.Ltmp537-.Ltmp535), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w9, w8
	orr	w8, w8, #0x2
	//APP
	eor	x9, x9, x9
.Ltmp540:
	cbnz	x9, .Ltmp540
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x9, [x19, #7008]
	add	x9, x9, #40
	//APP
	str	w8, [x9]
	//NO_APP
.LBB44_6:
	ldp	x20, x19, [sp, #32]             // 16-byte Folded Reload
	ldr	x21, [sp, #16]                  // 8-byte Folded Reload
	ldp	x29, x30, [sp], #48             // 16-byte Folded Reload
	ret
.LBB44_7:
	ldr	x8, [x0, #7008]
	add	x8, x8, #40
	//APP
.Ltmp541:
	ldr	w8, [x8]
.Ltmp542:
	.section	.altinstructions,"a",@progbits
.Ltmp543:
	.word	.Ltmp541-.Ltmp543
.Ltmp545:
	.word	.Ltmp544-.Ltmp545
	.hword	1
	.byte	.Ltmp542-.Ltmp541
	.byte	.Ltmp546-.Ltmp544
	.text

	.text	1
.Ltmp544:
	ldar	w8, [x8]
.Ltmp546:
.Ltmp547:
.org (.Ltmp547-(.Ltmp546-.Ltmp544))+(.Ltmp542-.Ltmp541), 0
.Ltmp548:
.org (.Ltmp548-(.Ltmp542-.Ltmp541))+(.Ltmp546-.Ltmp544), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w9, w8
	and	w8, w8, #0xfffffffd
	//APP
	eor	x9, x9, x9
.Ltmp549:
	cbnz	x9, .Ltmp549
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x9, [x0, #7008]
	mov	w0, #42950
	add	x9, x9, #40
	//APP
	str	w8, [x9]
	//NO_APP
	bl	__const_udelay
	mov	w0, wzr
	b	.LBB44_6
.LBB44_8:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.73
	add	x1, x1, :lo12:.L.str.73
	adrp	x2, .L__func__.dfa_sram_deep_sleep_vote
	add	x2, x2, :lo12:.L__func__.dfa_sram_deep_sleep_vote
	bl	_dev_err
	mov	w0, #-110
	b	.LBB44_6
.Lfunc_end44:
	.size	dfa_sram_deep_sleep_vote, .Lfunc_end44-dfa_sram_deep_sleep_vote
                                        // -- End function
	.globl	dfa_lowpower_vote_status_sync   // -- Begin function dfa_lowpower_vote_status_sync
	.p2align	2
	.type	dfa_lowpower_vote_status_sync,@function
dfa_lowpower_vote_status_sync:          // @dfa_lowpower_vote_status_sync
// %bb.0:
	stp	x29, x30, [sp, #-16]!           // 16-byte Folded Spill
	mov	x29, sp
	tbz	w1, #0, .LBB45_2
// %bb.1:
	bl	ufs_auto_h8_entry_status_sync
	ldp	x29, x30, [sp], #16             // 16-byte Folded Reload
	ret
.LBB45_2:
	bl	ufs_disable_auto_h8_status_sync
	ldp	x29, x30, [sp], #16             // 16-byte Folded Reload
	ret
.Lfunc_end45:
	.size	dfa_lowpower_vote_status_sync, .Lfunc_end45-dfa_lowpower_vote_status_sync
                                        // -- End function
	.globl	dfa_direct_flush                // -- Begin function dfa_direct_flush
	.p2align	2
	.type	dfa_direct_flush,@function
dfa_direct_flush:                       // @dfa_direct_flush
// %bb.0:
	stp	x29, x30, [sp, #-96]!           // 16-byte Folded Spill
	stp	x28, x27, [sp, #16]             // 16-byte Folded Spill
	mov	x29, sp
	stp	x26, x25, [sp, #32]             // 16-byte Folded Spill
	stp	x24, x23, [sp, #48]             // 16-byte Folded Spill
	stp	x22, x21, [sp, #64]             // 16-byte Folded Spill
	stp	x20, x19, [sp, #80]             // 16-byte Folded Spill
	sub	sp, sp, #432
	adrp	x8, :got:__stack_chk_guard
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldr	x8, [x8]
	stur	x8, [x29, #-16]
	str	xzr, [sp, #24]
	ldr	w8, [x1, #7228]
	cbnz	w8, .LBB46_24
// %bb.1:
	mov	x20, x1
	mov	x19, x0
	add	x1, sp, #24
	mov	x0, x20
	mov	x21, x2
	bl	ufshcd_trylock_hostlock
	cbz	w0, .LBB46_25
// %bb.2:
	mov	x0, x19
	bl	scsi_device_get
	cbnz	w0, .LBB46_26
// %bb.3:
	ldr	w8, [x19, #2136]
	cmp	w8, #7
	b.hi	.LBB46_5
// %bb.4:
	mov	w9, #1
	lsl	w8, w9, w8
	mov	w9, #208
	tst	w8, w9
	b.ne	.LBB46_22
.LBB46_5:
	ldr	w3, [x20, #1240]
	cmp	w3, #2
	b.ne	.LBB46_43
// %bb.6:
	mov	w22, wzr
.LBB46_7:
	ldr	x8, [x20, #88]
	adrp	x24, :got:liblinux_thread_offset
	mov	w9, wzr
	adrp	x25, :got:liblinux_thread_key
	//APP
	mrs	x23, TPIDR_EL0

	//NO_APP
	ldr	x10, [x8, #56]
	ldr	x24, [x24, :got_lo12:liblinux_thread_offset]
	ldr	x8, [sp, #24]
	//APP
	stlrb	w9, [x10]
	//NO_APP
	ldr	w9, [x24]
	ldr	x25, [x25, :got_lo12:liblinux_thread_key]
	sub	x9, x23, x9
	ldr	w10, [x25]
	ldr	x9, [x9]
	ldr	x26, [x9, x10, lsl #3]
	ldrb	w9, [x26, #84]
	cbnz	w9, .LBB46_10
// %bb.8:
	ldr	w9, [x26, #36]
	cbz	w9, .LBB46_62
// %bb.9:
	cmp	x8, #1
	b.ne	.LBB46_17
.LBB46_10:
	//APP
	//NO_APP
	ldr	w8, [x24]
	ldr	w9, [x25]
	sub	x8, x23, x8
	ldr	x8, [x8]
	ldr	x26, [x8, x9, lsl #3]
	ldr	w8, [x26, #32]
	sub	w8, w8, #1
	str	w8, [x26, #32]
	ldr	w8, [x26, #32]
	cbnz	w8, .LBB46_13
// %bb.11:
	ldrb	w8, [x26, #84]
	cbnz	w8, .LBB46_13
// %bb.12:
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
	cbnz	w8, .LBB46_63
.LBB46_13:
	ldr	w8, [x26, #32]
	tbnz	w8, #31, .LBB46_70
.LBB46_14:
	cbz	w22, .LBB46_27
.LBB46_15:
	adrp	x8, :got:__stack_chk_guard
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldur	x9, [x29, #-16]
	ldr	x8, [x8]
	cmp	x8, x9
	b.ne	.LBB46_66
// %bb.16:
	mov	w0, w22
	add	sp, sp, #432
	ldp	x20, x19, [sp, #80]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #64]             // 16-byte Folded Reload
	ldp	x24, x23, [sp, #48]             // 16-byte Folded Reload
	ldp	x26, x25, [sp, #32]             // 16-byte Folded Reload
	ldp	x28, x27, [sp, #16]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #96             // 16-byte Folded Reload
	ret
.LBB46_17:
	//APP
	//NO_APP
	ldr	w8, [x24]
	ldr	w9, [x25]
	sub	x8, x23, x8
	ldr	x8, [x8]
	ldr	x8, [x8, x9, lsl #3]
	ldr	w9, [x8, #32]
	sub	w9, w9, #1
	str	w9, [x8, #32]
	ldr	w9, [x8, #32]
	cbnz	w9, .LBB46_20
// %bb.18:
	ldrb	w9, [x8, #84]
	cbnz	w9, .LBB46_20
// %bb.19:
	ldr	x27, [x8, #24]
	adrp	x10, :got:__preempt_disabled
	mov	w12, #1
	ldr	w9, [x27, #8]
	ldr	x10, [x10, :got_lo12:__preempt_disabled]
	lsl	x9, x9, #2
	ldr	w11, [x10, x9]
	sub	w11, w11, #1
	str	w11, [x10, x9]
	str	w12, [x27, #4]
	ldr	w9, [x27, #12]
	cbnz	w9, .LBB46_64
.LBB46_20:
	ldr	w9, [x8, #32]
	tbnz	w9, #31, .LBB46_65
.LBB46_21:
	str	wzr, [x26, #36]
	b	.LBB46_10
.LBB46_22:
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.126
	add	x1, x1, :lo12:.L.str.126
	adrp	x2, .L__func__.dfa_direct_flush_get_device_and_check
	add	x2, x2, :lo12:.L__func__.dfa_direct_flush_get_device_and_check
	mov	w3, wzr
	bl	_dev_err
	mov	w22, #-19
.LBB46_23:
	mov	x0, x19
	bl	scsi_device_put
	b	.LBB46_7
.LBB46_24:
	ldr	x0, [x1, #96]
	adrp	x1, .L.str.123
	add	x1, x1, :lo12:.L.str.123
	bl	_dev_err
	mov	w22, #4181
	b	.LBB46_15
.LBB46_25:
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.124
	add	x1, x1, :lo12:.L.str.124
	adrp	x2, .L__func__.dfa_direct_flush_get_device_and_check
	add	x2, x2, :lo12:.L__func__.dfa_direct_flush_get_device_and_check
	bl	_dev_err
	mov	w22, #-5
	b	.LBB46_15
.LBB46_26:
	mov	w3, w0
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.125
	add	x1, x1, :lo12:.L.str.125
	adrp	x2, .L__func__.dfa_direct_flush_get_device_and_check
	add	x2, x2, :lo12:.L__func__.dfa_direct_flush_get_device_and_check
	bl	_dev_err
	mov	w22, #-19
	b	.LBB46_7
.LBB46_27:
	add	x0, sp, #24
	mov	w1, wzr
	mov	w2, #392
	bl	memset
	mov	w8, #53
	add	x0, sp, #24
	mov	w1, #392
	mov	w2, wzr
	mov	w3, #392
	str	xzr, [sp, #8]
	strh	wzr, [sp, #16]
	strb	w8, [sp, #8]
	bl	memset_s
	add	x0, sp, #24
	add	x2, sp, #8
	mov	x1, x19
	mov	x3, x21
	mov	w4, #3
	mov	x5, xzr
	mov	w6, wzr
	mov	w7, wzr
	bl	ufshcd_compose_scsi_cmd
	mov	w8, #1
	mov	w21, #1000
	mov	w22, #20492
	str	w8, [x20, #7228]
	b	.LBB46_29
.LBB46_28:                              //   in Loop: Header=BB46_29 Depth=1
	mov	w0, #42950
	sub	w21, w21, #1
	bl	__const_udelay
	cbz	w21, .LBB46_32
.LBB46_29:                              // =>This Inner Loop Header: Depth=1
	ldr	x8, [x20]
	add	x8, x8, #5, lsl #12             // =20480
	//APP
.Ltmp550:
	ldr	w8, [x8]
.Ltmp551:
	.section	.altinstructions,"a",@progbits
.Ltmp552:
	.word	.Ltmp550-.Ltmp552
.Ltmp554:
	.word	.Ltmp553-.Ltmp554
	.hword	1
	.byte	.Ltmp551-.Ltmp550
	.byte	.Ltmp555-.Ltmp553
	.text

	.text	1
.Ltmp553:
	ldar	w8, [x8]
.Ltmp555:
.Ltmp556:
.org (.Ltmp556-(.Ltmp555-.Ltmp553))+(.Ltmp551-.Ltmp550), 0
.Ltmp557:
.org (.Ltmp557-(.Ltmp551-.Ltmp550))+(.Ltmp555-.Ltmp553), 0
	.text


	//NO_APP
	mov	w9, w8
	//APP
	dmb	oshld
	//NO_APP
	cmn	w8, #1
	//APP
	eor	x9, x9, x9
.Ltmp558:
	cbnz	x9, .Ltmp558
	//NO_APP
	b.ne	.LBB46_28
// %bb.30:                              //   in Loop: Header=BB46_29 Depth=1
	ldr	x8, [x20]
	add	x8, x8, x22
	//APP
.Ltmp559:
	ldr	w8, [x8]
.Ltmp560:
	.section	.altinstructions,"a",@progbits
.Ltmp561:
	.word	.Ltmp559-.Ltmp561
.Ltmp563:
	.word	.Ltmp562-.Ltmp563
	.hword	1
	.byte	.Ltmp560-.Ltmp559
	.byte	.Ltmp564-.Ltmp562
	.text

	.text	1
.Ltmp562:
	ldar	w8, [x8]
.Ltmp564:
.Ltmp565:
.org (.Ltmp565-(.Ltmp564-.Ltmp562))+(.Ltmp560-.Ltmp559), 0
.Ltmp566:
.org (.Ltmp566-(.Ltmp560-.Ltmp559))+(.Ltmp564-.Ltmp562), 0
	.text


	//NO_APP
	mov	w9, w8
	//APP
	dmb	oshld
	//NO_APP
	tst	w8, #0xff
	//APP
	eor	x9, x9, x9
.Ltmp567:
	cbnz	x9, .Ltmp567
	//NO_APP
	b.ne	.LBB46_28
// %bb.31:
	cmp	w21, #0
	b.gt	.LBB46_33
.LBB46_32:
	ldr	x8, [x20]
	mov	w9, #20492
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.129
	add	x1, x1, :lo12:.L.str.129
	add	x8, x8, #5, lsl #12             // =20480
	//APP
.Ltmp568:
	ldr	w2, [x8]
.Ltmp569:
	.section	.altinstructions,"a",@progbits
.Ltmp570:
	.word	.Ltmp568-.Ltmp570
.Ltmp572:
	.word	.Ltmp571-.Ltmp572
	.hword	1
	.byte	.Ltmp569-.Ltmp568
	.byte	.Ltmp573-.Ltmp571
	.text

	.text	1
.Ltmp571:
	ldar	w2, [x8]
.Ltmp573:
.Ltmp574:
.org (.Ltmp574-(.Ltmp573-.Ltmp571))+(.Ltmp569-.Ltmp568), 0
.Ltmp575:
.org (.Ltmp575-(.Ltmp569-.Ltmp568))+(.Ltmp573-.Ltmp571), 0
	.text


	//NO_APP
	mov	w8, w2
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp576:
	cbnz	x8, .Ltmp576
	//NO_APP
	ldr	x8, [x20]
	add	x8, x8, x9
	//APP
.Ltmp577:
	ldr	w3, [x8]
.Ltmp578:
	.section	.altinstructions,"a",@progbits
.Ltmp579:
	.word	.Ltmp577-.Ltmp579
.Ltmp581:
	.word	.Ltmp580-.Ltmp581
	.hword	1
	.byte	.Ltmp578-.Ltmp577
	.byte	.Ltmp582-.Ltmp580
	.text

	.text	1
.Ltmp580:
	ldar	w3, [x8]
.Ltmp582:
.Ltmp583:
.org (.Ltmp583-(.Ltmp582-.Ltmp580))+(.Ltmp578-.Ltmp577), 0
.Ltmp584:
.org (.Ltmp584-(.Ltmp578-.Ltmp577))+(.Ltmp582-.Ltmp580), 0
	.text


	//NO_APP
	mov	w8, w3
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp585:
	cbnz	x8, .Ltmp585
	//NO_APP
	bl	_dev_err
	adrp	x21, .L.str.130
	add	x21, x21, :lo12:.L.str.130
	ldr	x0, [x20, #96]
	mov	x1, x21
	ldr	x2, [x20, #7040]
	bl	_dev_err
	ldr	x0, [x20, #96]
	mov	x1, x21
	ldr	x2, [x20, #7048]
	bl	_dev_err
	ldr	x0, [x20, #96]
	mov	x1, x21
	ldr	x2, [x20, #7056]
	bl	_dev_err
	ldr	x0, [x20, #96]
	mov	x1, x21
	ldr	x2, [x20, #7064]
	bl	_dev_err
.LBB46_33:
	ldr	x8, [x20, #7016]
	mov	w9, #4912
	add	x8, x8, x9
	//APP
.Ltmp586:
	ldr	w8, [x8]
.Ltmp587:
	.section	.altinstructions,"a",@progbits
.Ltmp588:
	.word	.Ltmp586-.Ltmp588
.Ltmp590:
	.word	.Ltmp589-.Ltmp590
	.hword	1
	.byte	.Ltmp587-.Ltmp586
	.byte	.Ltmp591-.Ltmp589
	.text

	.text	1
.Ltmp589:
	ldar	w8, [x8]
.Ltmp591:
.Ltmp592:
.org (.Ltmp592-(.Ltmp591-.Ltmp589))+(.Ltmp587-.Ltmp586), 0
.Ltmp593:
.org (.Ltmp593-(.Ltmp587-.Ltmp586))+(.Ltmp591-.Ltmp589), 0
	.text


	//NO_APP
	mov	w9, w8
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x9, x9, x9
.Ltmp594:
	cbnz	x9, .Ltmp594
	//NO_APP
	cbz	w8, .LBB46_35
// %bb.34:
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.131
	add	x1, x1, :lo12:.L.str.131
	bl	_dev_err
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #4912
	ldr	x8, [x20, #7016]
	add	x8, x8, x9
	mov	w9, #1
	//APP
	str	w9, [x8]
	//NO_APP
.LBB46_35:
	ldr	x22, [x20, #224]
	add	x8, sp, #24
	mov	w9, #96
	mov	w10, #14
	mov	x0, x20
	mov	w1, #14
	str	x8, [x22, #3096]
	add	x21, x22, #3024
	str	w9, [x22, #3112]
	mov	w9, #49408
	ldr	x8, [sp, #304]
	str	wzr, [x22, #3116]
	str	w10, [x22, #3128]
	strb	wzr, [x22, #3201]
	str	x8, [x22, #3104]
	ldr	x8, [sp, #80]
	ldr	x8, [x8, #120]
	str	wzr, [x22, #3124]
	and	x10, x8, #0xff00
	cmp	x10, x9
	mov	w10, #1
	cset	w9, eq
	strb	w10, [x22, #3133]
	bfi	w8, w9, #7, #1
	strb	w8, [x22, #3132]
	bl	dfa_lrbp_configure
	cbnz	w0, .LBB46_44
// %bb.36:
	mov	x0, x20
	mov	x1, x21
	bl	ufshcd_compose_upiu
	cbnz	w0, .LBB46_44
// %bb.37:
	//APP
	dmb	oshst
	//NO_APP
	mov	x0, x20
	ldr	x9, [x20, #7016]
	mov	w10, #4916
	mov	w8, wzr
	add	x9, x9, x10
	//APP
	str	w8, [x9]
	//NO_APP
	bl	ufs_disable_auto_h8_status_sync
	mov	x0, x20
	mov	w1, #14
	bl	dfa_send_command
	//APP
	dsb	st
	//NO_APP
	mov	w10, #80
	ldr	x8, [x20, #7024]
	mov	w21, #31073
	ldrb	w9, [x22, #3217]
	movk	w21, #65534, lsl #16
	mov	w27, #12
	mov	w28, #4192
	str	x22, [sp]                       // 8-byte Folded Spill
	nop
	madd	x26, x9, x10, x8
.LBB46_38:                              // =>This Inner Loop Header: Depth=1
	ldrsw	x8, [x26, #36]
	ldr	x9, [x20, #7016]
	nop
	madd	x8, x8, x27, x9
	add	x8, x8, x28
	//APP
.Ltmp595:
	ldr	w22, [x8]
.Ltmp596:
	.section	.altinstructions,"a",@progbits
.Ltmp597:
	.word	.Ltmp595-.Ltmp597
.Ltmp599:
	.word	.Ltmp598-.Ltmp599
	.hword	1
	.byte	.Ltmp596-.Ltmp595
	.byte	.Ltmp600-.Ltmp598
	.text

	.text	1
.Ltmp598:
	ldar	w22, [x8]
.Ltmp600:
.Ltmp601:
.org (.Ltmp601-(.Ltmp600-.Ltmp598))+(.Ltmp596-.Ltmp595), 0
.Ltmp602:
.org (.Ltmp602-(.Ltmp596-.Ltmp595))+(.Ltmp600-.Ltmp598), 0
	.text


	//NO_APP
	mov	w8, w22
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp603:
	cbnz	x8, .Ltmp603
	//NO_APP
	tbnz	w22, #0, .LBB46_46
// %bb.39:                              //   in Loop: Header=BB46_38 Depth=1
	mov	w0, #42950
	bl	__const_udelay
	adds	w21, w21, #1
	b.lo	.LBB46_38
// %bb.40:
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.132
	add	x1, x1, :lo12:.L.str.132
	bl	_dev_err
	ldp	w6, w5, [x26, #72]
	ldr	w4, [x26, #60]
	cmp	w5, w6
	b.ne	.LBB46_42
// %bb.41:
	cmp	w4, w5
	b.eq	.LBB46_58
.LBB46_42:
	ldr	w2, [x26, #36]
	adrp	x0, .L.str.113
	add	x0, x0, :lo12:.L.str.113
	ldr	w3, [x26, #64]
	adrp	x1, .L__func__.wait_for_command_completion
	add	x1, x1, :lo12:.L__func__.wait_for_command_completion
	bl	printk
	b	.LBB46_58
.LBB46_43:
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.127
	add	x1, x1, :lo12:.L.str.127
	adrp	x2, .L__func__.dfa_direct_flush_get_device_and_check
	add	x2, x2, :lo12:.L__func__.dfa_direct_flush_get_device_and_check
	bl	_dev_err
	mov	w22, #4181
	b	.LBB46_23
.LBB46_44:
	mov	x0, x20
	mov	x1, x21
	bl	dfa_pack_fail_rollback
.LBB46_45:
	mov	x0, x20
	str	wzr, [x20, #7228]
	bl	ufs_cmd_compl_status_sync
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.128
	add	x1, x1, :lo12:.L.str.128
	ldr	x3, [x19, #120]
	adrp	x2, .L__func__.dfa_send_scsi_sync_cache_on_specified_disk
	add	x2, x2, :lo12:.L__func__.dfa_send_scsi_sync_cache_on_specified_disk
	mov	w4, wzr
	bl	_dev_err
	mov	x0, x19
	bl	scsi_device_put
	mov	w22, wzr
	b	.LBB46_15
.LBB46_46:
	add	x21, x26, #68
	mov	x0, x21
	bl	__raw_spin_lock_irqsave
	ldrsw	x9, [x26, #36]
	//APP
	dmb	oshst
	//NO_APP
	ldr	x10, [x20, #7016]
	mov	w11, #12
	mov	w8, wzr
	madd	x9, x9, x11, x10
	mov	w10, #4192
	add	x9, x9, x10
	//APP
	str	w22, [x9]
	//NO_APP
	ldr	x9, [x26, #48]
	ldr	w10, [x26, #36]
	add	x9, x9, x10, lsl #3
	add	x9, x9, #3556
	//APP
.Ltmp604:
	ldr	w9, [x9]
.Ltmp605:
	.section	.altinstructions,"a",@progbits
.Ltmp606:
	.word	.Ltmp604-.Ltmp606
.Ltmp608:
	.word	.Ltmp607-.Ltmp608
	.hword	1
	.byte	.Ltmp605-.Ltmp604
	.byte	.Ltmp609-.Ltmp607
	.text

	.text	1
.Ltmp607:
	ldar	w9, [x9]
.Ltmp609:
.Ltmp610:
.org (.Ltmp610-(.Ltmp609-.Ltmp607))+(.Ltmp605-.Ltmp604), 0
.Ltmp611:
.org (.Ltmp611-(.Ltmp605-.Ltmp604))+(.Ltmp609-.Ltmp607), 0
	.text


	//NO_APP
	mov	w10, w9
	//APP
	dmb	oshld
	//NO_APP
	lsr	w9, w9, #5
	//APP
	eor	x10, x10, x10
.Ltmp612:
	cbnz	x10, .Ltmp612
	//NO_APP
	ldr	w10, [x26, #76]
	ldr	w11, [x26, #32]
	add	w10, w10, #1
	udiv	w12, w10, w11
	msub	w10, w12, w11, w10
	stp	w9, w10, [x26, #72]
	//APP
	dmb	oshst
	//NO_APP
	ldr	x9, [x26, #48]
	ldr	w10, [x26, #36]
	ldr	w11, [x26, #76]
	add	x9, x9, x10, lsl #3
	lsl	w10, w11, #5
	add	x9, x9, #3552
	//APP
	str	w10, [x9]
	//NO_APP
	//APP
	stlrb	w8, [x21]
	//NO_APP
	ldr	w8, [x24]
	ldr	w9, [x25]
	sub	x8, x23, x8
	ldr	x8, [x8]
	ldr	x21, [x8, x9, lsl #3]
	ldrb	w8, [x21, #84]
	cbnz	w8, .LBB46_54
// %bb.47:
	ldr	w8, [x21, #36]
	cbz	w8, .LBB46_62
// %bb.48:
	cmp	x0, #1
	b.eq	.LBB46_54
// %bb.49:
	//APP
	//NO_APP
	ldr	w8, [x24]
	ldr	w9, [x25]
	sub	x8, x23, x8
	ldr	x8, [x8]
	ldr	x26, [x8, x9, lsl #3]
	ldr	w8, [x26, #32]
	sub	w8, w8, #1
	str	w8, [x26, #32]
	ldr	w8, [x26, #32]
	cbnz	w8, .LBB46_52
// %bb.50:
	ldrb	w8, [x26, #84]
	cbnz	w8, .LBB46_52
// %bb.51:
	ldr	x22, [x26, #24]
	adrp	x9, :got:__preempt_disabled
	mov	w11, #1
	ldr	w8, [x22, #8]
	ldr	x9, [x9, :got_lo12:__preempt_disabled]
	lsl	x8, x8, #2
	ldr	w10, [x9, x8]
	sub	w10, w10, #1
	str	w10, [x9, x8]
	str	w11, [x22, #4]
	ldr	w8, [x22, #12]
	cbnz	w8, .LBB46_69
.LBB46_52:
	ldr	w8, [x26, #32]
	tbnz	w8, #31, .LBB46_70
.LBB46_53:
	str	wzr, [x21, #36]
.LBB46_54:
	//APP
	//NO_APP
	ldr	w8, [x24]
	ldr	w9, [x25]
	sub	x8, x23, x8
	ldr	x8, [x8]
	ldr	x21, [x8, x9, lsl #3]
	ldr	w8, [x21, #32]
	sub	w8, w8, #1
	str	w8, [x21, #32]
	ldr	w8, [x21, #32]
	cbnz	w8, .LBB46_57
// %bb.55:
	ldrb	w8, [x21, #84]
	cbnz	w8, .LBB46_57
// %bb.56:
	ldr	x22, [x21, #24]
	adrp	x9, :got:__preempt_disabled
	mov	w11, #1
	ldr	w8, [x22, #8]
	ldr	x9, [x9, :got_lo12:__preempt_disabled]
	lsl	x8, x8, #2
	ldr	w10, [x9, x8]
	sub	w10, w10, #1
	str	w10, [x9, x8]
	str	w11, [x22, #4]
	ldr	w8, [x22, #12]
	cbnz	w8, .LBB46_67
.LBB46_57:
	ldr	w8, [x21, #32]
	tbnz	w8, #31, .LBB46_68
.LBB46_58:
	ldr	x8, [x20, #7040]
	ldr	x21, [sp]                       // 8-byte Folded Reload
	tbnz	w8, #14, .LBB46_61
.LBB46_59:
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.12
	add	x1, x1, :lo12:.L.str.12
	mov	w2, #14
	bl	_dev_err
.LBB46_60:
	mov	w8, #7072
	mov	w9, #1
	add	x8, x20, x8
	//APP
	// atomic_sub
	prfm	pstl1strm, [x8]
.Ltmp613:
	ldxr	w10, [x8]
	sub	w10, w10, w9
	stxr	w11, w10, [x8]
	cbnz	w11, .Ltmp613

	//NO_APP
	str	xzr, [x21, #3096]
	b	.LBB46_45
.LBB46_61:
	mov	w8, #7040
	mov	w9, #16384
	add	x8, x20, x8
	//APP
	// atomic64_fetch_andnot
	prfm	pstl1strm, [x8]
.Ltmp614:
	ldxr	x10, [x8]
	bic	x11, x10, x9
	stlxr	w12, x11, [x8]
	cbnz	w12, .Ltmp614
	dmb	ish
	//NO_APP
	tbz	w10, #14, .LBB46_59
	b	.LBB46_60
.LBB46_62:
	adrp	x0, .L.str.9
	add	x0, x0, :lo12:.L.str.9
	adrp	x1, .L.str.10
	add	x1, x1, :lo12:.L.str.10
	adrp	x3, .L__func__.arch_local_irq_restore
	add	x3, x3, :lo12:.L__func__.arch_local_irq_restore
	mov	w2, #77
	b	.LBB46_72
.LBB46_63:
	bl	kernel_thread_preempt_enable
	str	wzr, [x27, #12]
	ldr	w8, [x26, #32]
	tbz	w8, #31, .LBB46_14
	b	.LBB46_70
.LBB46_64:
	mov	x28, x8
	bl	kernel_thread_preempt_enable
	mov	x8, x28
	str	wzr, [x27, #12]
	ldr	w9, [x8, #32]
	tbz	w9, #31, .LBB46_21
.LBB46_65:
	str	wzr, [x8, #32]
	b	.LBB46_71
.LBB46_66:
	bl	__stack_chk_fail
.LBB46_67:
	bl	kernel_thread_preempt_enable
	str	wzr, [x22, #12]
	ldr	w8, [x21, #32]
	tbz	w8, #31, .LBB46_58
.LBB46_68:
	str	wzr, [x21, #32]
	b	.LBB46_71
.LBB46_69:
	bl	kernel_thread_preempt_enable
	str	wzr, [x22, #12]
	ldr	w8, [x26, #32]
	tbz	w8, #31, .LBB46_53
.LBB46_70:
	str	wzr, [x26, #32]
.LBB46_71:
	adrp	x0, .L.str.9
	add	x0, x0, :lo12:.L.str.9
	adrp	x1, .L.str.10
	add	x1, x1, :lo12:.L.str.10
	adrp	x3, .L__func__.__preempt_count_sub
	add	x3, x3, :lo12:.L__func__.__preempt_count_sub
	mov	w2, #112
.LBB46_72:
	bl	printk
	adrp	x0, .L.str.11
	add	x0, x0, :lo12:.L.str.11
	bl	panic
.Lfunc_end46:
	.size	dfa_direct_flush, .Lfunc_end46-dfa_direct_flush
                                        // -- End function
	.globl	dfa_auto_h8_switch              // -- Begin function dfa_auto_h8_switch
	.p2align	2
	.type	dfa_auto_h8_switch,@function
dfa_auto_h8_switch:                     // @dfa_auto_h8_switch
// %bb.0:
	sub	sp, sp, #32
	adrp	x8, :got:__stack_chk_guard
	stp	x29, x30, [sp, #16]             // 16-byte Folded Spill
	mov	w9, #1
	add	x29, sp, #16
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldr	x8, [x8]
	str	x8, [sp, #8]
	strb	w0, [sp, #1]
	mov	x0, sp
	strb	w9, [sp]
	stur	wzr, [sp, #2]
	bl	dfa_lp_ctrl
	adrp	x8, :got:__stack_chk_guard
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldr	x9, [sp, #8]
	ldr	x8, [x8]
	cmp	x8, x9
	b.ne	.LBB47_2
// %bb.1:
	ldp	x29, x30, [sp, #16]             // 16-byte Folded Reload
	add	sp, sp, #32
	ret
.LBB47_2:
	bl	__stack_chk_fail
.Lfunc_end47:
	.size	dfa_auto_h8_switch, .Lfunc_end47-dfa_auto_h8_switch
                                        // -- End function
	.globl	dfa_lp_ctrl                     // -- Begin function dfa_lp_ctrl
	.p2align	2
	.type	dfa_lp_ctrl,@function
dfa_lp_ctrl:                            // @dfa_lp_ctrl
// %bb.0:
	sub	sp, sp, #80
	adrp	x9, :got:__stack_chk_guard
	stp	x29, x30, [sp, #64]             // 16-byte Folded Spill
	add	x29, sp, #64
	adrp	x10, dfa_sdev
	mov	x8, x0
	ldr	x9, [x9, :got_lo12:__stack_chk_guard]
	ldr	x9, [x9]
	stur	x9, [x29, #-8]
	ldr	x0, [x10, :lo12:dfa_sdev]
	stur	xzr, [x29, #-16]
	str	xzr, [sp, #32]
	cbz	x0, .LBB48_4
// %bb.1:
	ldrh	w9, [x8, #2]
	mov	w10, #315
	ldrh	w11, [x8]
	movk	w10, #2070, lsl #16
	ldrb	w8, [x8, #4]
	mov	x3, xzr
	rev	w9, w9
	mov	w4, wzr
	lsr	w9, w9, #16
	mov	x5, xzr
	sturb	w8, [x29, #-16]
	mov	w8, #32768
	sub	x1, x29, #24
	add	x6, sp, #32
	sturh	w9, [x29, #-18]
	mov	w9, #1
	mov	w2, #3
	mov	w7, #3000
	stur	w10, [x29, #-24]
	sturh	w11, [x29, #-20]
	str	xzr, [sp, #24]
	str	w8, [sp, #16]
	str	x9, [sp, #8]
	str	wzr, [sp]
	bl	__scsi_execute
	cbnz	w0, .LBB48_5
.LBB48_2:
	adrp	x8, :got:__stack_chk_guard
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldur	x9, [x29, #-8]
	ldr	x8, [x8]
	cmp	x8, x9
	b.ne	.LBB48_6
// %bb.3:
	ldp	x29, x30, [sp, #64]             // 16-byte Folded Reload
	add	sp, sp, #80
	ret
.LBB48_4:
	adrp	x8, dfa_hba
	adrp	x1, .L.str.75
	add	x1, x1, :lo12:.L.str.75
	adrp	x2, .L__func__.dfa_lp_ctrl
	add	x2, x2, :lo12:.L__func__.dfa_lp_ctrl
	ldr	x8, [x8, :lo12:dfa_hba]
	ldr	x0, [x8, #96]
	bl	_dev_err
	b	.LBB48_2
.LBB48_5:
	adrp	x8, dfa_hba
	mov	w3, w0
	adrp	x1, .L.str.76
	add	x1, x1, :lo12:.L.str.76
	adrp	x2, .L__func__.dfa_lp_ctrl
	add	x2, x2, :lo12:.L__func__.dfa_lp_ctrl
	ldr	x8, [x8, :lo12:dfa_hba]
	ldr	x0, [x8, #96]
	bl	_dev_err
	b	.LBB48_2
.LBB48_6:
	bl	__stack_chk_fail
.Lfunc_end48:
	.size	dfa_lp_ctrl, .Lfunc_end48-dfa_lp_ctrl
                                        // -- End function
	.p2align	2                               // -- Begin function dfa_acore_irq_handler
	.type	dfa_acore_irq_handler,@function
dfa_acore_irq_handler:                  // @dfa_acore_irq_handler
// %bb.0:
	stp	x29, x30, [sp, #-32]!           // 16-byte Folded Spill
	stp	x20, x19, [sp, #16]             // 16-byte Folded Spill
	mov	w9, #1
	//APP
	dmb	oshst
	//NO_APP
	mov	x19, x1
	ldr	x8, [x1, #7008]
	mov	x29, sp
	add	x8, x8, #12
	//APP
	str	w9, [x8]
	//NO_APP
	ldr	x8, [x1, #7008]
	add	x8, x8, #636
	//APP
.Ltmp615:
	ldr	w20, [x8]
.Ltmp616:
	.section	.altinstructions,"a",@progbits
.Ltmp617:
	.word	.Ltmp615-.Ltmp617
.Ltmp619:
	.word	.Ltmp618-.Ltmp619
	.hword	1
	.byte	.Ltmp616-.Ltmp615
	.byte	.Ltmp620-.Ltmp618
	.text

	.text	1
.Ltmp618:
	ldar	w20, [x8]
.Ltmp620:
.Ltmp621:
.org (.Ltmp621-(.Ltmp620-.Ltmp618))+(.Ltmp616-.Ltmp615), 0
.Ltmp622:
.org (.Ltmp622-(.Ltmp616-.Ltmp615))+(.Ltmp620-.Ltmp618), 0
	.text


	//NO_APP
	mov	w8, w20
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp623:
	cbnz	x8, .Ltmp623
	//NO_APP
	tbnz	w20, #24, .LBB49_7
// %bb.1:
	and	w8, w20, #0xffffff
	cmp	w8, #7
	b.eq	.LBB49_6
// %bb.2:
	cmp	w8, #3
	b.ne	.LBB49_7
// %bb.3:
	ldr	w8, [x19, #7072]
	cbnz	w8, .LBB49_7
// %bb.4:
	ldr	w8, [x19, #7224]
	cbnz	w8, .LBB49_7
// %bb.5:
	mov	x0, x19
	bl	ufs_auto_h8_entry_status_sync
	b	.LBB49_7
.LBB49_6:
	mov	x0, x19
	bl	dfa_dsm_handler
.LBB49_7:
	tbnz	w20, #26, .LBB49_10
// %bb.8:
	tbnz	w20, #25, .LBB49_14
.LBB49_9:
	ldp	x20, x19, [sp, #16]             // 16-byte Folded Reload
	mov	w0, #1
	ldp	x29, x30, [sp], #32             // 16-byte Folded Reload
	ret
.LBB49_10:
	mov	x0, x19
	bl	dfa_dump_uic_error
	mov	w8, #7360
	mov	w9, #2
	add	x8, x19, x8
	//APP
	prfm	pstl1strm, [x8]
.Ltmp624:
	ldxr	w2, [x8]
	eor	w10, w2, #0x1
	cbnz	w10, .Ltmp625
	stlxr	w10, w9, [x8]
	cbnz	w10, .Ltmp624
	dmb	ish
.Ltmp625:
	//NO_APP
	cmp	w2, #2
	b.eq	.LBB49_18
// %bb.11:
	cmp	w2, #1
	b.ne	.LBB49_20
// %bb.12:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.95
	add	x1, x1, :lo12:.L.str.95
	bl	_dev_err
	mov	w8, #7272
	add	x0, x19, x8
	bl	cancel_delayed_work
	mov	w8, #7128
	mov	w9, #7184
	add	x0, x19, x8
	add	x1, x19, x9
	bl	kthread_queue_work
	tbz	w0, #0, .LBB49_22
.LBB49_13:
	str	wzr, [x19, #7360]
	b	.LBB49_9
.LBB49_14:
	mov	x0, x19
	bl	dfa_dump_uic_error
	mov	w9, #7360
	mov	x8, xzr
	add	x9, x19, x9
	mov	w20, #1
	//APP
	prfm	pstl1strm, [x9]
.Ltmp626:
	ldxr	w2, [x9]
	eor	w10, w2, w8
	cbnz	w10, .Ltmp627
	stlxr	w10, w20, [x9]
	cbnz	w10, .Ltmp626
	dmb	ish
.Ltmp627:
	//NO_APP
	ldr	x0, [x19, #96]
	cbnz	w2, .LBB49_17
// %bb.15:
	adrp	x1, .L.str.99
	add	x1, x1, :lo12:.L.str.99
	bl	_dev_err
	str	w20, [x19, #7228]
	mov	w8, #7272
	ldr	x1, [x19, #7264]
	add	x2, x19, x8
	mov	w0, #12
	mov	w3, #200
	bl	queue_delayed_work_on
	tbnz	w0, #0, .LBB49_9
// %bb.16:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.100
	add	x1, x1, :lo12:.L.str.100
	bl	_dev_err
	b	.LBB49_9
.LBB49_17:
	adrp	x1, .L.str.101
	add	x1, x1, :lo12:.L.str.101
	b	.LBB49_19
.LBB49_18:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.97
	add	x1, x1, :lo12:.L.str.97
.LBB49_19:
	bl	_dev_err
	b	.LBB49_9
.LBB49_20:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.98
	add	x1, x1, :lo12:.L.str.98
	bl	_dev_err
	mov	w8, #7128
	mov	w9, #7184
	add	x0, x19, x8
	add	x1, x19, x9
	bl	kthread_queue_work
	tbnz	w0, #0, .LBB49_9
// %bb.21:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.96
	add	x1, x1, :lo12:.L.str.96
	adrp	x2, .L__func__.dfa_proc_immediately_reset
	add	x2, x2, :lo12:.L__func__.dfa_proc_immediately_reset
	bl	_dev_err
	b	.LBB49_9
.LBB49_22:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.96
	add	x1, x1, :lo12:.L.str.96
	adrp	x2, .L__func__.dfa_proc_immediately_reset
	add	x2, x2, :lo12:.L__func__.dfa_proc_immediately_reset
	bl	_dev_err
	b	.LBB49_13
.Lfunc_end49:
	.size	dfa_acore_irq_handler, .Lfunc_end49-dfa_acore_irq_handler
                                        // -- End function
	.p2align	2                               // -- Begin function dfa_mcq_write_msi_msg
	.type	dfa_mcq_write_msi_msg,@function
dfa_mcq_write_msi_msg:                  // @dfa_mcq_write_msi_msg
// %bb.0:
	stp	x29, x30, [sp, #-16]!           // 16-byte Folded Spill
	ldr	x8, [x0, #24]
	mov	x29, sp
	cbz	x8, .LBB50_3
// %bb.1:
	ldr	x8, [x8, #120]
	cbz	x8, .LBB50_4
// %bb.2:
	//APP
	dmb	oshst
	//NO_APP
	ldr	x9, [x8, #7016]
	ldr	w10, [x1]
	add	x9, x9, #16
	//APP
	str	w10, [x9]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x8, [x8, #7016]
	ldr	w9, [x1, #4]
	add	x8, x8, #20
	//APP
	str	w9, [x8]
	//NO_APP
	ldp	x29, x30, [sp], #16             // 16-byte Folded Reload
	ret
.LBB50_3:
	adrp	x0, .L.str.102
	add	x0, x0, :lo12:.L.str.102
	b	.LBB50_5
.LBB50_4:
	adrp	x0, .L.str.103
	add	x0, x0, :lo12:.L.str.103
.LBB50_5:
	adrp	x1, .L__func__.dfa_mcq_write_msi_msg
	add	x1, x1, :lo12:.L__func__.dfa_mcq_write_msi_msg
	bl	printk
	ldp	x29, x30, [sp], #16             // 16-byte Folded Reload
	ret
.Lfunc_end50:
	.size	dfa_mcq_write_msi_msg, .Lfunc_end50-dfa_mcq_write_msi_msg
                                        // -- End function
	.p2align	2                               // -- Begin function dfa_mcq_esi_handler
	.type	dfa_mcq_esi_handler,@function
dfa_mcq_esi_handler:                    // @dfa_mcq_esi_handler
// %bb.0:
	sub	sp, sp, #128
	ldr	x8, [x1, #24]
	stp	x20, x19, [sp, #112]            // 16-byte Folded Spill
	ldrh	w4, [x1, #80]
	stp	x22, x21, [sp, #96]             // 16-byte Folded Spill
	stp	x29, x30, [sp, #32]             // 16-byte Folded Spill
	add	x29, sp, #32
	ldr	x19, [x8, #120]
	stp	x28, x27, [sp, #48]             // 16-byte Folded Spill
	sub	x21, x4, #160
	stp	x26, x25, [sp, #64]             // 16-byte Folded Spill
	stp	x24, x23, [sp, #80]             // 16-byte Folded Spill
	ldr	w8, [x19, #7032]
	cmp	w21, w8
	b.hs	.LBB51_27
// %bb.1:
	adrp	x25, :got:liblinux_thread_offset
	adrp	x26, :got:liblinux_thread_key
	//APP
	mrs	x24, TPIDR_EL0

	//NO_APP
	ldr	x25, [x25, :got_lo12:liblinux_thread_offset]
	ldr	w8, [x25]
	ldr	x26, [x26, :got_lo12:liblinux_thread_key]
	ldr	x10, [x19, #7024]
	sub	x8, x24, x8
	ldr	w9, [x26]
	ldr	x8, [x8]
	ldr	x22, [x8, x9, lsl #3]
	mov	w8, #80
	umaddl	x27, w21, w8, x10
	ldr	w8, [x22, #32]
	add	x20, x27, #68
	cbnz	w8, .LBB51_3
// %bb.2:
	ldrb	w8, [x22, #84]
	cbz	w8, .LBB51_24
.LBB51_3:
	ldr	w9, [x22, #32]
	mov	x8, xzr
	mov	w10, #1
	add	w9, w9, #1
	str	w9, [x22, #32]
	//APP
	//NO_APP
	//APP
	prfm	pstl1strm, [x20]
.Ltmp628:
	ldaxr	w1, [x20]
	eor	w9, w1, w8
	cbnz	w9, .Ltmp629
	stxr	w9, w10, [x20]
	cbnz	w9, .Ltmp628

.Ltmp629:
	//NO_APP
	cbnz	w1, .LBB51_28
.LBB51_4:
	ldr	x8, [x19, #7016]
	add	x9, x21, x21, lsl #1
	lsl	x9, x9, #2
	mov	w10, #4192
	add	x8, x8, x9
	add	x8, x8, x10
	//APP
.Ltmp630:
	ldr	w8, [x8]
.Ltmp631:
	.section	.altinstructions,"a",@progbits
.Ltmp632:
	.word	.Ltmp630-.Ltmp632
.Ltmp634:
	.word	.Ltmp633-.Ltmp634
	.hword	1
	.byte	.Ltmp631-.Ltmp630
	.byte	.Ltmp635-.Ltmp633
	.text

	.text	1
.Ltmp633:
	ldar	w8, [x8]
.Ltmp635:
.Ltmp636:
.org (.Ltmp636-(.Ltmp635-.Ltmp633))+(.Ltmp631-.Ltmp630), 0
.Ltmp637:
.org (.Ltmp637-(.Ltmp631-.Ltmp630))+(.Ltmp635-.Ltmp633), 0
	.text


	//NO_APP
	mov	w10, w8
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x10, x10, x10
.Ltmp638:
	cbnz	x10, .Ltmp638
	//NO_APP
	cbnz	w8, .LBB51_10
.LBB51_5:
	mov	w8, wzr
	//APP
	stlrb	w8, [x20]
	//NO_APP
	//APP
	//NO_APP
	ldr	w8, [x25]
	ldr	w9, [x26]
	sub	x8, x24, x8
	ldr	x8, [x8]
	ldr	x19, [x8, x9, lsl #3]
	ldr	w8, [x19, #32]
	sub	w8, w8, #1
	str	w8, [x19, #32]
	ldr	w8, [x19, #32]
	cbnz	w8, .LBB51_8
// %bb.6:
	ldrb	w8, [x19, #84]
	cbnz	w8, .LBB51_8
// %bb.7:
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
	cbnz	w8, .LBB51_30
.LBB51_8:
	ldr	w8, [x19, #32]
	tbnz	w8, #31, .LBB51_31
.LBB51_9:
	ldp	x20, x19, [sp, #112]            // 16-byte Folded Reload
	mov	w0, #1
	ldp	x22, x21, [sp, #96]             // 16-byte Folded Reload
	ldp	x24, x23, [sp, #80]             // 16-byte Folded Reload
	ldp	x26, x25, [sp, #64]             // 16-byte Folded Reload
	ldp	x28, x27, [sp, #48]             // 16-byte Folded Reload
	ldp	x29, x30, [sp, #32]             // 16-byte Folded Reload
	add	sp, sp, #128
	ret
.LBB51_10:
	//APP
	dmb	oshst
	//NO_APP
	ldr	x10, [x19, #7016]
	add	x9, x10, x9
	mov	w10, #4192
	add	x9, x9, x10
	//APP
	str	w8, [x9]
	//NO_APP
	tbz	w8, #0, .LBB51_5
// %bb.11:
	ldr	x8, [x27, #48]
	ldr	w9, [x27, #36]
	add	x8, x8, x9, lsl #3
	add	x8, x8, #3556
	//APP
.Ltmp639:
	ldr	w9, [x8]
.Ltmp640:
	.section	.altinstructions,"a",@progbits
.Ltmp641:
	.word	.Ltmp639-.Ltmp641
.Ltmp643:
	.word	.Ltmp642-.Ltmp643
	.hword	1
	.byte	.Ltmp640-.Ltmp639
	.byte	.Ltmp644-.Ltmp642
	.text

	.text	1
.Ltmp642:
	ldar	w9, [x8]
.Ltmp644:
.Ltmp645:
.org (.Ltmp645-(.Ltmp644-.Ltmp642))+(.Ltmp640-.Ltmp639), 0
.Ltmp646:
.org (.Ltmp646-(.Ltmp640-.Ltmp639))+(.Ltmp644-.Ltmp642), 0
	.text


	//NO_APP
	mov	w8, w9
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp647:
	cbnz	x8, .Ltmp647
	//NO_APP
	lsr	w9, w9, #5
	ldr	w8, [x27, #76]
	str	w9, [x27, #72]
	cmp	w8, w9
	b.eq	.LBB51_5
// %bb.12:
	mov	w9, #7072
	mov	w21, #216
	mov	w22, #1
	add	x28, x19, x9
.LBB51_13:                              // =>This Inner Loop Header: Depth=1
	ldr	x9, [x27, #16]
	mov	w8, w8
	ldr	x11, [x19, #64]
	add	x8, x9, x8, lsl #5
	ldp	w9, w10, [x8]
	and	x9, x9, #0xffffff80
	bfi	x9, x10, #32, #32
	subs	x9, x9, x11
	b.lo	.LBB51_22
// %bb.14:                              //   in Loop: Header=BB51_13 Depth=1
	ldr	x10, [x19, #304]
	mov	w11, #5120
	add	x10, x11, x10, lsl #8
	udiv	x23, x9, x10
	ldr	w9, [x19, #264]
	cmp	w9, w23
	b.ls	.LBB51_23
// %bb.15:                              //   in Loop: Header=BB51_13 Depth=1
	ldr	x9, [x19, #224]
	cmp	w23, #15
	ldrb	w8, [x8, #16]
	nop
	smaddl	x9, w23, w21, x9
	and	w8, w8, #0xf
	strb	w8, [x9, #195]
	b.eq	.LBB51_19
// %bb.16:                              //   in Loop: Header=BB51_13 Depth=1
	add	w8, w23, #63
	cmp	w23, #0
	csel	w8, w8, w23, lt
	mov	w11, #7040
	asr	w9, w8, #6
	and	w8, w8, #0xffffffc0
	sub	w8, w23, w8
	add	x10, x19, w9, sxtw #3
	lsr	w9, w8, #6
	add	x10, x10, x11
	lsl	x8, x22, x8
	ldr	x11, [x10, w9, uxtw #3]
	tst	x11, x8
	b.eq	.LBB51_21
// %bb.17:                              //   in Loop: Header=BB51_13 Depth=1
	add	x9, x10, x9, lsl #3
	//APP
	// atomic64_fetch_andnot
	prfm	pstl1strm, [x9]
.Ltmp648:
	ldxr	x10, [x9]
	bic	x11, x10, x8
	stlxr	w12, x11, [x9]
	cbnz	w12, .Ltmp648
	dmb	ish
	//NO_APP
	tst	x10, x8
	b.eq	.LBB51_21
.LBB51_18:                              //   in Loop: Header=BB51_13 Depth=1
	//APP
	// atomic_sub
	prfm	pstl1strm, [x28]
.Ltmp649:
	ldxr	w8, [x28]
	sub	w8, w8, w22
	stxr	w9, w8, [x28]
	cbnz	w9, .Ltmp649

	//NO_APP
.LBB51_19:                              //   in Loop: Header=BB51_13 Depth=1
	mov	x0, x19
	mov	w1, w23
	bl	dfa_compl_one_task
	ldr	w8, [x27, #76]
	ldr	w9, [x27, #32]
	add	w8, w8, #1
	udiv	w10, w8, w9
	msub	w8, w10, w9, w8
	str	w8, [x27, #76]
	//APP
	dmb	oshst
	//NO_APP
	ldr	x8, [x27, #48]
	ldr	w9, [x27, #36]
	ldr	w10, [x27, #76]
	add	x8, x8, x9, lsl #3
	lsl	w9, w10, #5
	add	x8, x8, #3552
	//APP
	str	w9, [x8]
	//NO_APP
.LBB51_20:                              //   in Loop: Header=BB51_13 Depth=1
	ldp	w9, w8, [x27, #72]
	cmp	w8, w9
	b.ne	.LBB51_13
	b	.LBB51_5
.LBB51_21:                              //   in Loop: Header=BB51_13 Depth=1
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.12
	add	x1, x1, :lo12:.L.str.12
	mov	w2, w23
	bl	_dev_err
	b	.LBB51_18
.LBB51_22:                              //   in Loop: Header=BB51_13 Depth=1
	ldr	w23, [x19, #264]
.LBB51_23:                              //   in Loop: Header=BB51_13 Depth=1
	ldr	w4, [x27, #36]
	mov	w12, #12
	ldr	x9, [x27, #48]
	adrp	x1, .L.str.105
	add	x1, x1, :lo12:.L.str.105
	ldr	x0, [x19, #96]
	ldr	w3, [x8, #20]
	mov	w2, w23
	add	x9, x9, x4, lsl #3
                                        // kill: def $w4 killed $w4 killed $x4
	add	x9, x9, #2272
	//APP
.Ltmp650:
	ldr	w5, [x9]
.Ltmp651:
	.section	.altinstructions,"a",@progbits
.Ltmp652:
	.word	.Ltmp650-.Ltmp652
.Ltmp654:
	.word	.Ltmp653-.Ltmp654
	.hword	1
	.byte	.Ltmp651-.Ltmp650
	.byte	.Ltmp655-.Ltmp653
	.text

	.text	1
.Ltmp653:
	ldar	w5, [x9]
.Ltmp655:
.Ltmp656:
.org (.Ltmp656-(.Ltmp655-.Ltmp653))+(.Ltmp651-.Ltmp650), 0
.Ltmp657:
.org (.Ltmp657-(.Ltmp651-.Ltmp650))+(.Ltmp655-.Ltmp653), 0
	.text


	//NO_APP
	mov	w8, w5
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp658:
	cbnz	x8, .Ltmp658
	//NO_APP
	ldr	x8, [x27, #48]
	ldr	w9, [x27, #36]
	add	x8, x8, x9, lsl #3
	add	x8, x8, #2276
	//APP
.Ltmp659:
	ldr	w6, [x8]
.Ltmp660:
	.section	.altinstructions,"a",@progbits
.Ltmp661:
	.word	.Ltmp659-.Ltmp661
.Ltmp663:
	.word	.Ltmp662-.Ltmp663
	.hword	1
	.byte	.Ltmp660-.Ltmp659
	.byte	.Ltmp664-.Ltmp662
	.text

	.text	1
.Ltmp662:
	ldar	w6, [x8]
.Ltmp664:
.Ltmp665:
.org (.Ltmp665-(.Ltmp664-.Ltmp662))+(.Ltmp660-.Ltmp659), 0
.Ltmp666:
.org (.Ltmp666-(.Ltmp660-.Ltmp659))+(.Ltmp664-.Ltmp662), 0
	.text


	//NO_APP
	mov	w8, w6
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp667:
	cbnz	x8, .Ltmp667
	//NO_APP
	ldr	x8, [x27, #48]
	ldr	w9, [x27, #36]
	add	x8, x8, x9, lsl #3
	add	x8, x8, #3552
	//APP
.Ltmp668:
	ldr	w7, [x8]
.Ltmp669:
	.section	.altinstructions,"a",@progbits
.Ltmp670:
	.word	.Ltmp668-.Ltmp670
.Ltmp672:
	.word	.Ltmp671-.Ltmp672
	.hword	1
	.byte	.Ltmp669-.Ltmp668
	.byte	.Ltmp673-.Ltmp671
	.text

	.text	1
.Ltmp671:
	ldar	w7, [x8]
.Ltmp673:
.Ltmp674:
.org (.Ltmp674-(.Ltmp673-.Ltmp671))+(.Ltmp669-.Ltmp668), 0
.Ltmp675:
.org (.Ltmp675-(.Ltmp669-.Ltmp668))+(.Ltmp673-.Ltmp671), 0
	.text


	//NO_APP
	mov	w8, w7
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp676:
	cbnz	x8, .Ltmp676
	//NO_APP
	ldr	x8, [x27, #48]
	ldr	w9, [x27, #36]
	add	x8, x8, x9, lsl #3
	add	x8, x8, #3556
	//APP
.Ltmp677:
	ldr	w8, [x8]
.Ltmp678:
	.section	.altinstructions,"a",@progbits
.Ltmp679:
	.word	.Ltmp677-.Ltmp679
.Ltmp681:
	.word	.Ltmp680-.Ltmp681
	.hword	1
	.byte	.Ltmp678-.Ltmp677
	.byte	.Ltmp682-.Ltmp680
	.text

	.text	1
.Ltmp680:
	ldar	w8, [x8]
.Ltmp682:
.Ltmp683:
.org (.Ltmp683-(.Ltmp682-.Ltmp680))+(.Ltmp678-.Ltmp677), 0
.Ltmp684:
.org (.Ltmp684-(.Ltmp678-.Ltmp677))+(.Ltmp682-.Ltmp680), 0
	.text


	//NO_APP
	mov	w9, w8
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x9, x9, x9
.Ltmp685:
	cbnz	x9, .Ltmp685
	//NO_APP
	ldr	x9, [x27, #48]
	ldr	w10, [x27, #36]
	nop
	madd	x9, x10, x12, x9
	mov	w10, #4196
	add	x9, x9, x10
	//APP
.Ltmp686:
	ldr	w9, [x9]
.Ltmp687:
	.section	.altinstructions,"a",@progbits
.Ltmp688:
	.word	.Ltmp686-.Ltmp688
.Ltmp690:
	.word	.Ltmp689-.Ltmp690
	.hword	1
	.byte	.Ltmp687-.Ltmp686
	.byte	.Ltmp691-.Ltmp689
	.text

	.text	1
.Ltmp689:
	ldar	w9, [x9]
.Ltmp691:
.Ltmp692:
.org (.Ltmp692-(.Ltmp691-.Ltmp689))+(.Ltmp687-.Ltmp686), 0
.Ltmp693:
.org (.Ltmp693-(.Ltmp687-.Ltmp686))+(.Ltmp691-.Ltmp689), 0
	.text


	//NO_APP
	mov	w10, w9
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x10, x10, x10
.Ltmp694:
	cbnz	x10, .Ltmp694
	//NO_APP
	ldr	x10, [x27, #48]
	ldr	w11, [x27, #36]
	nop
	madd	x10, x11, x12, x10
	mov	w11, #4192
	add	x10, x10, x11
	//APP
.Ltmp695:
	ldr	w10, [x10]
.Ltmp696:
	.section	.altinstructions,"a",@progbits
.Ltmp697:
	.word	.Ltmp695-.Ltmp697
.Ltmp699:
	.word	.Ltmp698-.Ltmp699
	.hword	1
	.byte	.Ltmp696-.Ltmp695
	.byte	.Ltmp700-.Ltmp698
	.text

	.text	1
.Ltmp698:
	ldar	w10, [x10]
.Ltmp700:
.Ltmp701:
.org (.Ltmp701-(.Ltmp700-.Ltmp698))+(.Ltmp696-.Ltmp695), 0
.Ltmp702:
.org (.Ltmp702-(.Ltmp696-.Ltmp695))+(.Ltmp700-.Ltmp698), 0
	.text


	//NO_APP
	mov	w11, w10
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x11, x11, x11
.Ltmp703:
	cbnz	x11, .Ltmp703
	//NO_APP
	str	w10, [sp, #16]
	str	w9, [sp, #8]
	str	w8, [sp]
	bl	_dev_err
	ldr	w8, [x27, #76]
	ldr	w9, [x27, #32]
	add	w8, w8, #1
	udiv	w10, w8, w9
	msub	w8, w10, w9, w8
	str	w8, [x27, #76]
	//APP
	dmb	oshst
	//NO_APP
	ldr	x8, [x27, #48]
	ldr	w9, [x27, #36]
	ldr	w10, [x27, #76]
	add	x8, x8, x9, lsl #3
	lsl	w9, w10, #5
	add	x8, x8, #3552
	//APP
	str	w9, [x8]
	//NO_APP
	b	.LBB51_20
.LBB51_24:
	ldr	x28, [x22, #24]
	mov	w8, #2
	adrp	x23, :got:__preempt_disabled
	str	w8, [x28, #4]
	//APP
	//NO_APP
	ldr	w8, [x28, #8]
	ldr	x23, [x23, :got_lo12:__preempt_disabled]
	ldr	w9, [x23, x8, lsl #2]
	cmp	w9, #0
	b.gt	.LBB51_29
// %bb.25:
	ldr	w10, [x28, #4]
	cmp	w10, #3
	b.eq	.LBB51_29
.LBB51_26:
	add	w9, w9, #1
	str	w9, [x23, x8, lsl #2]
	b	.LBB51_3
.LBB51_27:
	mov	w2, w0
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.104
	add	x1, x1, :lo12:.L.str.104
	mov	w3, w21
                                        // kill: def $w4 killed $w4 killed $x4
	bl	_dev_err
	b	.LBB51_9
.LBB51_28:
	mov	x0, x20
	bl	queued_spin_lock_slowpath
	b	.LBB51_4
.LBB51_29:
	bl	kernel_thread_preempt_disable
	ldr	w8, [x28, #8]
	ldr	w9, [x23, x8, lsl #2]
	b	.LBB51_26
.LBB51_30:
	bl	kernel_thread_preempt_enable
	str	wzr, [x20, #12]
	ldr	w8, [x19, #32]
	tbz	w8, #31, .LBB51_9
.LBB51_31:
	adrp	x0, .L.str.9
	add	x0, x0, :lo12:.L.str.9
	adrp	x1, .L.str.10
	add	x1, x1, :lo12:.L.str.10
	adrp	x3, .L__func__.__preempt_count_sub
	add	x3, x3, :lo12:.L__func__.__preempt_count_sub
	mov	w2, #112
	str	wzr, [x19, #32]
	bl	printk
	adrp	x0, .L.str.11
	add	x0, x0, :lo12:.L.str.11
	bl	panic
.Lfunc_end51:
	.size	dfa_mcq_esi_handler, .Lfunc_end51-dfa_mcq_esi_handler
                                        // -- End function
	.p2align	2                               // -- Begin function dfa_dump_uic_error
	.type	dfa_dump_uic_error,@function
dfa_dump_uic_error:                     // @dfa_dump_uic_error
// %bb.0:
	sub	sp, sp, #48
	ldr	x9, [x0, #7008]
	stp	x29, x30, [sp, #16]             // 16-byte Folded Spill
	str	x19, [sp, #32]                  // 8-byte Folded Spill
	mov	x8, x0
	adrp	x1, .L.str.94
	add	x1, x1, :lo12:.L.str.94
	add	x9, x9, #572
	//APP
.Ltmp704:
	ldr	w19, [x9]
.Ltmp705:
	.section	.altinstructions,"a",@progbits
.Ltmp706:
	.word	.Ltmp704-.Ltmp706
.Ltmp708:
	.word	.Ltmp707-.Ltmp708
	.hword	1
	.byte	.Ltmp705-.Ltmp704
	.byte	.Ltmp709-.Ltmp707
	.text

	.text	1
.Ltmp707:
	ldar	w19, [x9]
.Ltmp709:
.Ltmp710:
.org (.Ltmp710-(.Ltmp709-.Ltmp707))+(.Ltmp705-.Ltmp704), 0
.Ltmp711:
.org (.Ltmp711-(.Ltmp705-.Ltmp704))+(.Ltmp709-.Ltmp707), 0
	.text


	//NO_APP
	mov	w9, w19
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x9, x9, x9
.Ltmp712:
	cbnz	x9, .Ltmp712
	//NO_APP
	add	x29, sp, #16
	ldr	x9, [x0, #7008]
	ldr	x0, [x0, #96]
	add	x9, x9, #544
	//APP
.Ltmp713:
	ldr	w2, [x9]
.Ltmp714:
	.section	.altinstructions,"a",@progbits
.Ltmp715:
	.word	.Ltmp713-.Ltmp715
.Ltmp717:
	.word	.Ltmp716-.Ltmp717
	.hword	1
	.byte	.Ltmp714-.Ltmp713
	.byte	.Ltmp718-.Ltmp716
	.text

	.text	1
.Ltmp716:
	ldar	w2, [x9]
.Ltmp718:
.Ltmp719:
.org (.Ltmp719-(.Ltmp718-.Ltmp716))+(.Ltmp714-.Ltmp713), 0
.Ltmp720:
.org (.Ltmp720-(.Ltmp714-.Ltmp713))+(.Ltmp718-.Ltmp716), 0
	.text


	//NO_APP
	mov	w9, w2
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x9, x9, x9
.Ltmp721:
	cbnz	x9, .Ltmp721
	//NO_APP
	ldr	x9, [x8, #7008]
	add	x9, x9, #548
	//APP
.Ltmp722:
	ldr	w3, [x9]
.Ltmp723:
	.section	.altinstructions,"a",@progbits
.Ltmp724:
	.word	.Ltmp722-.Ltmp724
.Ltmp726:
	.word	.Ltmp725-.Ltmp726
	.hword	1
	.byte	.Ltmp723-.Ltmp722
	.byte	.Ltmp727-.Ltmp725
	.text

	.text	1
.Ltmp725:
	ldar	w3, [x9]
.Ltmp727:
.Ltmp728:
.org (.Ltmp728-(.Ltmp727-.Ltmp725))+(.Ltmp723-.Ltmp722), 0
.Ltmp729:
.org (.Ltmp729-(.Ltmp723-.Ltmp722))+(.Ltmp727-.Ltmp725), 0
	.text


	//NO_APP
	mov	w9, w3
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x9, x9, x9
.Ltmp730:
	cbnz	x9, .Ltmp730
	//NO_APP
	ldr	x9, [x8, #7008]
	add	x9, x9, #552
	//APP
.Ltmp731:
	ldr	w4, [x9]
.Ltmp732:
	.section	.altinstructions,"a",@progbits
.Ltmp733:
	.word	.Ltmp731-.Ltmp733
.Ltmp735:
	.word	.Ltmp734-.Ltmp735
	.hword	1
	.byte	.Ltmp732-.Ltmp731
	.byte	.Ltmp736-.Ltmp734
	.text

	.text	1
.Ltmp734:
	ldar	w4, [x9]
.Ltmp736:
.Ltmp737:
.org (.Ltmp737-(.Ltmp736-.Ltmp734))+(.Ltmp732-.Ltmp731), 0
.Ltmp738:
.org (.Ltmp738-(.Ltmp732-.Ltmp731))+(.Ltmp736-.Ltmp734), 0
	.text


	//NO_APP
	mov	w9, w4
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x9, x9, x9
.Ltmp739:
	cbnz	x9, .Ltmp739
	//NO_APP
	ldr	x9, [x8, #7008]
	add	x9, x9, #556
	//APP
.Ltmp740:
	ldr	w5, [x9]
.Ltmp741:
	.section	.altinstructions,"a",@progbits
.Ltmp742:
	.word	.Ltmp740-.Ltmp742
.Ltmp744:
	.word	.Ltmp743-.Ltmp744
	.hword	1
	.byte	.Ltmp741-.Ltmp740
	.byte	.Ltmp745-.Ltmp743
	.text

	.text	1
.Ltmp743:
	ldar	w5, [x9]
.Ltmp745:
.Ltmp746:
.org (.Ltmp746-(.Ltmp745-.Ltmp743))+(.Ltmp741-.Ltmp740), 0
.Ltmp747:
.org (.Ltmp747-(.Ltmp741-.Ltmp740))+(.Ltmp745-.Ltmp743), 0
	.text


	//NO_APP
	mov	w9, w5
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x9, x9, x9
.Ltmp748:
	cbnz	x9, .Ltmp748
	//NO_APP
	ldr	x9, [x8, #7008]
	add	x9, x9, #560
	//APP
.Ltmp749:
	ldr	w6, [x9]
.Ltmp750:
	.section	.altinstructions,"a",@progbits
.Ltmp751:
	.word	.Ltmp749-.Ltmp751
.Ltmp753:
	.word	.Ltmp752-.Ltmp753
	.hword	1
	.byte	.Ltmp750-.Ltmp749
	.byte	.Ltmp754-.Ltmp752
	.text

	.text	1
.Ltmp752:
	ldar	w6, [x9]
.Ltmp754:
.Ltmp755:
.org (.Ltmp755-(.Ltmp754-.Ltmp752))+(.Ltmp750-.Ltmp749), 0
.Ltmp756:
.org (.Ltmp756-(.Ltmp750-.Ltmp749))+(.Ltmp754-.Ltmp752), 0
	.text


	//NO_APP
	mov	w9, w6
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x9, x9, x9
.Ltmp757:
	cbnz	x9, .Ltmp757
	//NO_APP
	ldr	x9, [x8, #7008]
	add	x9, x9, #564
	//APP
.Ltmp758:
	ldr	w7, [x9]
.Ltmp759:
	.section	.altinstructions,"a",@progbits
.Ltmp760:
	.word	.Ltmp758-.Ltmp760
.Ltmp762:
	.word	.Ltmp761-.Ltmp762
	.hword	1
	.byte	.Ltmp759-.Ltmp758
	.byte	.Ltmp763-.Ltmp761
	.text

	.text	1
.Ltmp761:
	ldar	w7, [x9]
.Ltmp763:
.Ltmp764:
.org (.Ltmp764-(.Ltmp763-.Ltmp761))+(.Ltmp759-.Ltmp758), 0
.Ltmp765:
.org (.Ltmp765-(.Ltmp759-.Ltmp758))+(.Ltmp763-.Ltmp761), 0
	.text


	//NO_APP
	mov	w9, w7
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x9, x9, x9
.Ltmp766:
	cbnz	x9, .Ltmp766
	//NO_APP
	ldr	x8, [x8, #7008]
	add	x8, x8, #568
	//APP
.Ltmp767:
	ldr	w8, [x8]
.Ltmp768:
	.section	.altinstructions,"a",@progbits
.Ltmp769:
	.word	.Ltmp767-.Ltmp769
.Ltmp771:
	.word	.Ltmp770-.Ltmp771
	.hword	1
	.byte	.Ltmp768-.Ltmp767
	.byte	.Ltmp772-.Ltmp770
	.text

	.text	1
.Ltmp770:
	ldar	w8, [x8]
.Ltmp772:
.Ltmp773:
.org (.Ltmp773-(.Ltmp772-.Ltmp770))+(.Ltmp768-.Ltmp767), 0
.Ltmp774:
.org (.Ltmp774-(.Ltmp768-.Ltmp767))+(.Ltmp772-.Ltmp770), 0
	.text


	//NO_APP
	mov	w9, w8
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x9, x9, x9
.Ltmp775:
	cbnz	x9, .Ltmp775
	//NO_APP
	str	w19, [sp, #8]
	str	w8, [sp]
	bl	_dev_err
	mov	w8, #1
	movk	w8, #57005, lsl #16
	cmp	w19, w8
	b.eq	.LBB52_2
.LBB52_1:
	ldp	x29, x30, [sp, #16]             // 16-byte Folded Reload
	ldr	x19, [sp, #32]                  // 8-byte Folded Reload
	add	sp, sp, #48
	ret
.LBB52_2:
	mov	w0, #35
	mov	x1, xzr
	movk	w0, #32768, lsl #16
	mov	x2, xzr
	bl	rdr_syserr_process_for_ap
	b	.LBB52_1
.Lfunc_end52:
	.size	dfa_dump_uic_error, .Lfunc_end52-dfa_dump_uic_error
                                        // -- End function
	.type	dfa_sdev,@object                // @dfa_sdev
	.local	dfa_sdev
	.comm	dfa_sdev,8,8
	.type	dfa_hba,@object                 // @dfa_hba
	.local	dfa_hba
	.comm	dfa_hba,8,8
	.type	.L.str,@object                  // @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"dfa_sys_ctrl iomap error!\n"
	.size	.L.str, 27

	.type	.L.str.1,@object                // @.str.1
.L.str.1:
	.asciz	"que_proc iomap error!\n"
	.size	.L.str.1, 23

	.type	.L.str.6,@object                // @.str.6
.L.str.6:
	.asciz	"SLOWAUTO_MODE"
	.size	.L.str.6, 14

	.type	.L.str.7,@object                // @.str.7
.L.str.7:
	.asciz	"%s:[RX, TX]: gear=[%d, %d], lane[%d, %d], pwr[%s, %s], rate = %d\n"
	.size	.L.str.7, 66

	.type	.L__func__.dfa_link_startup,@object // @__func__.dfa_link_startup
.L__func__.dfa_link_startup:
	.asciz	"dfa_link_startup"
	.size	.L__func__.dfa_link_startup, 17

	.type	.L.str.8,@object                // @.str.8
.L.str.8:
	.asciz	"Controller uie init failed\n"
	.size	.L.str.8, 28

	.type	.L.str.9,@object                // @.str.9
.L.str.9:
	.asciz	"BUG: failure at %s:%d/%s()!\n"
	.size	.L.str.9, 29

	.type	.L.str.10,@object               // @.str.10
.L.str.10:
	.asciz	"ldk"
	.size	.L.str.10, 4

	.type	.L.str.11,@object               // @.str.11
.L.str.11:
	.asciz	"BUG!"
	.size	.L.str.11, 5

	.type	.L.str.12,@object               // @.str.12
.L.str.12:
	.asciz	"[DFA] clear tag:%d error\n"
	.size	.L.str.12, 26

	.type	.L.str.13,@object               // @.str.13
.L.str.13:
	.asciz	"\0013%s timeout, stop:%u id:%u\n"
	.size	.L.str.13, 29

	.type	.L__func__.dfa_mcq_stop_run_sq,@object // @__func__.dfa_mcq_stop_run_sq
.L__func__.dfa_mcq_stop_run_sq:
	.asciz	"dfa_mcq_stop_run_sq"
	.size	.L__func__.dfa_mcq_stop_run_sq, 20

	.type	.L.str.14,@object               // @.str.14
.L.str.14:
	.asciz	"[DFA] inflt_reqs:0x%llx need requeue\n"
	.size	.L.str.14, 38

	.type	.L.str.15,@object               // @.str.15
.L.str.15:
	.asciz	"[DFA] tag:%d op:0x%x reused error\n"
	.size	.L.str.15, 35

	.type	.L.str.16,@object               // @.str.16
.L.str.16:
	.asciz	"UFS_PRDT_CONFIG: 0x%x\n"
	.size	.L.str.16, 23

	.type	.L.str.17,@object               // @.str.17
.L.str.17:
	.asciz	"%s entry.\n"
	.size	.L.str.17, 11

	.type	.L__func__.dfa_is_need_report_bit_error,@object // @__func__.dfa_is_need_report_bit_error
.L__func__.dfa_is_need_report_bit_error:
	.asciz	"dfa_is_need_report_bit_error"
	.size	.L__func__.dfa_is_need_report_bit_error, 29

	.type	.L.str.18,@object               // @.str.18
.L.str.18:
	.asciz	"%s hba->model = %s.\n"
	.size	.L.str.18, 21

	.type	.L.str.19,@object               // @.str.19
.L.str.19:
	.asciz	"THR950GFCV10"
	.size	.L.str.19, 13

	.type	.L.str.20,@object               // @.str.20
.L.str.20:
	.asciz	"[%s]READ FSR FAILED\n"
	.size	.L.str.20, 21

	.type	.L.str.21,@object               // @.str.21
.L.str.21:
	.asciz	"%s ufs_type is %s.\n"
	.size	.L.str.21, 20

	.type	.L.str.22,@object               // @.str.22
.L.str.22:
	.asciz	"not 950 ES"
	.size	.L.str.22, 11

	.type	.L.str.23,@object               // @.str.23
.L.str.23:
	.asciz	"950 ES"
	.size	.L.str.23, 7

	.type	.L.str.24,@object               // @.str.24
.L.str.24:
	.asciz	"%s: error_code = 0x%x, entry.\n"
	.size	.L.str.24, 31

	.type	.L__func__.dfa_dmd_handle,@object // @__func__.dfa_dmd_handle
.L__func__.dfa_dmd_handle:
	.asciz	"dfa_dmd_handle"
	.size	.L__func__.dfa_dmd_handle, 15

	.type	.L.str.25,@object               // @.str.25
.L.str.25:
	.asciz	"%s: error_code = 0x%x, end.\n"
	.size	.L.str.25, 29

	.type	.L.str.26,@object               // @.str.26
.L.str.26:
	.asciz	"%s error_code = 0x%x.\n"
	.size	.L.str.26, 23

	.type	.L__func__.dfa_dsm_handler,@object // @__func__.dfa_dsm_handler
.L__func__.dfa_dsm_handler:
	.asciz	"dfa_dsm_handler"
	.size	.L__func__.dfa_dsm_handler, 16

	.type	.L.str.27,@object               // @.str.27
.L.str.27:
	.asciz	"MPHY_SYS_CLK_STAT: 0x%x\n"
	.size	.L.str.27, 25

	.type	.L.str.28,@object               // @.str.28
.L.str.28:
	.asciz	"%s ov_flag = %d counter = %d.\n"
	.size	.L.str.28, 31

	.type	.L.str.29,@object               // @.str.29
.L.str.29:
	.asciz	"[DFA] dfa_acore irq resource not available\n"
	.size	.L.str.29, 44

	.type	.L.str.30,@object               // @.str.30
.L.str.30:
	.asciz	"MCQ init failed %d\n"
	.size	.L.str.30, 20

	.type	.L.str.31,@object               // @.str.31
.L.str.31:
	.asciz	"MCQ ESI not available %d\n"
	.size	.L.str.31, 26

	.type	.L.str.32,@object               // @.str.32
.L.str.32:
	.asciz	"dfa DEV_ID 0x%x, ESILBA 0x%x, ESIUBA 0x%x, QUE_CR 0x%x CQIS 0x%x, CQIE 0x%x\n"
	.size	.L.str.32, 77

	.type	.L.str.33,@object               // @.str.33
.L.str.33:
	.asciz	"lrbp addr is NULL\n"
	.size	.L.str.33, 19

	.type	.L.str.34,@object               // @.str.34
.L.str.34:
	.asciz	"Command Descriptor Memory allocation failed\n"
	.size	.L.str.34, 45

	.type	.L.str.35,@object               // @.str.35
.L.str.35:
	.asciz	"LRB Memory allocation failed\n"
	.size	.L.str.35, 30

	.type	.L.str.36,@object               // @.str.36
.L.str.36:
	.asciz	"[DFA] dhq submit entries is full, tag = %u\n"
	.size	.L.str.36, 44

	.type	.L.str.37,@object               // @.str.37
.L.str.37:
	.asciz	"[DFA] op:%u tag:%d error\n"
	.size	.L.str.37, 26

	.type	.L.str.38,@object               // @.str.38
.L.str.38:
	.asciz	"[DFA] in pm progress\n"
	.size	.L.str.38, 22

	.type	.L.str.39,@object               // @.str.39
.L.str.39:
	.asciz	"%s, queue dhba->eh_worker fail\n"
	.size	.L.str.39, 32

	.type	.L__func__.dfa_exec_dev_cmd,@object // @__func__.dfa_exec_dev_cmd
.L__func__.dfa_exec_dev_cmd:
	.asciz	"dfa_exec_dev_cmd"
	.size	.L__func__.dfa_exec_dev_cmd, 17

	.type	.L.str.40,@object               // @.str.40
.L.str.40:
	.asciz	"[DFA] SCSI command result err = 0x%x\n"
	.size	.L.str.40, 38

	.type	.L.str.41,@object               // @.str.41
.L.str.41:
	.asciz	"timedout, op:0x%x, lun:%u, sq:%u, cpu:%u\n"
	.size	.L.str.41, 42

	.type	.L.str.42,@object               // @.str.42
.L.str.42:
	.asciz	"[DFA] QUE_PROC %u SQH:0x%08x SQT:0x%08x CQH:0x%08x CQT:0x%08x CQIE:0x%08x CQIS:0x%08x\n"
	.size	.L.str.42, 87

	.type	.L__func__.dfa_exec_spec_cmd,@object // @__func__.dfa_exec_spec_cmd
.L__func__.dfa_exec_spec_cmd:
	.asciz	"dfa_exec_spec_cmd"
	.size	.L__func__.dfa_exec_spec_cmd, 18

	.type	.L.str.43,@object               // @.str.43
.L.str.43:
	.asciz	"[DFA] need to flush eh_work\n"
	.size	.L.str.43, 29

	.type	.L.str.44,@object               // @.str.44
.L.str.44:
	.asciz	"[DFA] queuecommand_directly err:%d\n"
	.size	.L.str.44, 36

	.type	.L.str.45,@object               // @.str.45
.L.str.45:
	.asciz	"[DFA] queue dhba->eh_worker fail\n"
	.size	.L.str.45, 34

	.type	.L.str.46,@object               // @.str.46
.L.str.46:
	.asciz	"clk_ufs_dfa_sys set clk failed\n"
	.size	.L.str.46, 32

	.type	.L.str.47,@object               // @.str.47
.L.str.47:
	.asciz	"clk_ufs_dfa_sys clk_prepare_enable failed\n"
	.size	.L.str.47, 43

	.type	.L.str.48,@object               // @.str.48
.L.str.48:
	.asciz	"%s, sys clk set failed:%d\n"
	.size	.L.str.48, 27

	.type	.L__func__.dfa_clk_init,@object // @__func__.dfa_clk_init
.L__func__.dfa_clk_init:
	.asciz	"dfa_clk_init"
	.size	.L__func__.dfa_clk_init, 13

	.type	.L.str.49,@object               // @.str.49
.L.str.49:
	.asciz	"%s, sys clk prepare failed:%d\n"
	.size	.L.str.49, 31

	.type	.L.str.50,@object               // @.str.50
.L.str.50:
	.asciz	"UFS 3.1 clk set\n"
	.size	.L.str.50, 17

	.type	.L.str.51,@object               // @.str.51
.L.str.51:
	.asciz	"%s, cpu clk set failed:%d\n"
	.size	.L.str.51, 27

	.type	.L.str.52,@object               // @.str.52
.L.str.52:
	.asciz	"%s, cpu clk prepare failed:%d\n"
	.size	.L.str.52, 31

	.type	.L.str.53,@object               // @.str.53
.L.str.53:
	.asciz	"[DFA] in reset, skip suspend\n"
	.size	.L.str.53, 30

	.type	.L.str.54,@object               // @.str.54
.L.str.54:
	.asciz	"\0013%s, [DFA] set dev pwr mode err:%d\n"
	.size	.L.str.54, 37

	.type	.L__func__.dfa_suspend,@object  // @__func__.dfa_suspend
.L__func__.dfa_suspend:
	.asciz	"dfa_suspend"
	.size	.L__func__.dfa_suspend, 12

	.type	.L.str.55,@object               // @.str.55
.L.str.55:
	.asciz	"\0013%s: AO_MEM_CTRL2 Write fail, resume UFS&DFA fail, fail ret = %d\n"
	.size	.L.str.55, 67

	.type	.L.str.56,@object               // @.str.56
.L.str.56:
	.asciz	"\0013%s, [DFA] set dev active pwr mode err:%d\n"
	.size	.L.str.56, 44

	.type	.L.str.57,@object               // @.str.57
.L.str.57:
	.asciz	"[DFA] in reset, skip resume\n"
	.size	.L.str.57, 29

	.type	.L.str.58,@object               // @.str.58
.L.str.58:
	.asciz	"%s ++\n"
	.size	.L.str.58, 7

	.type	.L__func__.dfa_resume,@object   // @__func__.dfa_resume
.L__func__.dfa_resume:
	.asciz	"dfa_resume"
	.size	.L__func__.dfa_resume, 11

	.type	.L.str.59,@object               // @.str.59
.L.str.59:
	.asciz	"ssu fail, need to flush error handle work\n"
	.size	.L.str.59, 43

	.type	.L.str.60,@object               // @.str.60
.L.str.60:
	.asciz	"%s --\n"
	.size	.L.str.60, 7

	.type	.L.str.61,@object               // @.str.61
.L.str.61:
	.asciz	"resume inline crypt key failed, ret:%d\n"
	.size	.L.str.61, 40

	.type	.L.str.62,@object               // @.str.62
.L.str.62:
	.asciz	"[DFA] tag:%d - type:%d - op:0x%x - cpu:%u - sq:%u - issue:%lld - compl:%lld\n"
	.size	.L.str.62, 77

	.type	.L__func__.dfa_dump_lrbp_status,@object // @__func__.dfa_dump_lrbp_status
.L__func__.dfa_dump_lrbp_status:
	.asciz	"dfa_dump_lrbp_status"
	.size	.L__func__.dfa_dump_lrbp_status, 21

	.type	dfa_mcq_params_init.registered,@object // @dfa_mcq_params_init.registered
	.local	dfa_mcq_params_init.registered
	.comm	dfa_mcq_params_init.registered,1,1
	.type	dfa_mcq_params_init.__key,@object // @dfa_mcq_params_init.__key
	.local	dfa_mcq_params_init.__key
	.comm	dfa_mcq_params_init.__key,1,1
	.type	.L.str.63,@object               // @.str.63
.L.str.63:
	.asciz	"(&dhba->eh_worker)->lock"
	.size	.L.str.63, 25

	.type	.L.str.64,@object               // @.str.64
.L.str.64:
	.asciz	"dfa_eh_worker"
	.size	.L.str.64, 14

	.type	dfa_mcq_params_init.__key.65,@object // @dfa_mcq_params_init.__key.65
	.local	dfa_mcq_params_init.__key.65
	.comm	dfa_mcq_params_init.__key.65,1,1
	.type	.L.str.66,@object               // @.str.66
.L.str.66:
	.asciz	"&dhba->dfa_lock"
	.size	.L.str.66, 16

	.type	.L.str.67,@object               // @.str.67
.L.str.67:
	.asciz	"dfa_reset_workqueue"
	.size	.L.str.67, 20

	.type	.L.str.68,@object               // @.str.68
.L.str.68:
	.asciz	"[DFA] alloc dfa reset workqueue failed!\n"
	.size	.L.str.68, 41

	.type	.L.str.69,@object               // @.str.69
.L.str.69:
	.asciz	"[DFA] log area map error\n"
	.size	.L.str.69, 26

	.type	.L.str.70,@object               // @.str.70
.L.str.70:
	.asciz	"[DFA] log area map virtual error\n"
	.size	.L.str.70, 34

	.type	.L.str.71,@object               // @.str.71
.L.str.71:
	.asciz	"%s try enable fail\n"
	.size	.L.str.71, 20

	.type	.L__func__.dfa_cpu_clk_vote,@object // @__func__.dfa_cpu_clk_vote
.L__func__.dfa_cpu_clk_vote:
	.asciz	"dfa_cpu_clk_vote"
	.size	.L__func__.dfa_cpu_clk_vote, 17

	.type	.L.str.72,@object               // @.str.72
.L.str.72:
	.asciz	"%s enable fail\n"
	.size	.L.str.72, 16

	.type	.L.str.73,@object               // @.str.73
.L.str.73:
	.asciz	"%s: dfa_cpu autodiv timeout\n"
	.size	.L.str.73, 29

	.type	.L__func__.dfa_sram_deep_sleep_vote,@object // @__func__.dfa_sram_deep_sleep_vote
.L__func__.dfa_sram_deep_sleep_vote:
	.asciz	"dfa_sram_deep_sleep_vote"
	.size	.L__func__.dfa_sram_deep_sleep_vote, 25

	.type	.L.str.74,@object               // @.str.74
.L.str.74:
	.asciz	"%s: failed to set timestamp %d\n"
	.size	.L.str.74, 32

	.type	.L__func__.dfa_set_timestamp_attr,@object // @__func__.dfa_set_timestamp_attr
.L__func__.dfa_set_timestamp_attr:
	.asciz	"dfa_set_timestamp_attr"
	.size	.L__func__.dfa_set_timestamp_attr, 23

	.type	.L.str.75,@object               // @.str.75
.L.str.75:
	.asciz	"%s: scsi_device is null\n"
	.size	.L.str.75, 25

	.type	.L__func__.dfa_lp_ctrl,@object  // @__func__.dfa_lp_ctrl
.L__func__.dfa_lp_ctrl:
	.asciz	"dfa_lp_ctrl"
	.size	.L__func__.dfa_lp_ctrl, 12

	.type	.L.str.76,@object               // @.str.76
.L.str.76:
	.asciz	"%s: scsi execute err %d\n"
	.size	.L.str.76, 25

	.type	.L.str.77,@object               // @.str.77
.L.str.77:
	.asciz	"\0013%s mcq stop run err status: %d SQRTS: %x\n"
	.size	.L.str.77, 44

	.type	.L__func__.dfa_mcq_check_stop_run_status,@object // @__func__.dfa_mcq_check_stop_run_status
.L__func__.dfa_mcq_check_stop_run_status:
	.asciz	"dfa_mcq_check_stop_run_status"
	.size	.L__func__.dfa_mcq_check_stop_run_status, 30

	.type	.L.str.78,@object               // @.str.78
.L.str.78:
	.asciz	"[DFA] tag:%d is not sent but completed\n"
	.size	.L.str.78, 40

	.type	.L__func__.__preempt_count_sub,@object // @__func__.__preempt_count_sub
.L__func__.__preempt_count_sub:
	.asciz	"__preempt_count_sub"
	.size	.L__func__.__preempt_count_sub, 20

	.type	.L.str.79,@object               // @.str.79
.L.str.79:
	.asciz	"[DFA] pericrg_perstat0 0x%x\n"
	.size	.L.str.79, 29

	.type	.L.str.80,@object               // @.str.80
.L.str.80:
	.asciz	"[DFA] pericrg_perrststat0 0x%x\n"
	.size	.L.str.80, 32

	.type	.L.str.81,@object               // @.str.81
.L.str.81:
	.asciz	"[DFA] pericrg_adiv_iu_s1 0x%x\n"
	.size	.L.str.81, 31

	.type	.L.str.82,@object               // @.str.82
.L.str.82:
	.asciz	"[DFA] pericrg_clkdiv26 0x%x\n"
	.size	.L.str.82, 29

	.type	.L.str.83,@object               // @.str.83
.L.str.83:
	.asciz	"[DFA] acore_dfa_commu_info2 %u = 0x%x\n"
	.size	.L.str.83, 39

	.type	.L.str.84,@object               // @.str.84
.L.str.84:
	.asciz	"[DFA] dfa_acore_commu_info2 %u = 0x%x\n"
	.size	.L.str.84, 39

	.type	.L.str.85,@object               // @.str.85
.L.str.85:
	.asciz	"dfa hw queue memory allocation failed\n"
	.size	.L.str.85, 39

	.type	.L.str.86,@object               // @.str.86
.L.str.86:
	.asciz	"SQE allocation failed\n"
	.size	.L.str.86, 23

	.type	.L.str.87,@object               // @.str.87
.L.str.87:
	.asciz	"CQE allocation failed\n"
	.size	.L.str.87, 23

	.type	.L.str.88,@object               // @.str.88
.L.str.88:
	.asciz	"[DFA] dfa acore irq err = %d\n"
	.size	.L.str.88, 30

	.type	.L.str.89,@object               // @.str.89
.L.str.89:
	.asciz	"dfa_acore_irq"
	.size	.L.str.89, 14

	.type	.L.str.90,@object               // @.str.90
.L.str.90:
	.asciz	"[DFA] request dfa_acore irq err = %d\n"
	.size	.L.str.90, 38

	.type	.L.str.91,@object               // @.str.91
.L.str.91:
	.asciz	"Failed to request Platform MSI %d\n"
	.size	.L.str.91, 35

	.type	.L.str.92,@object               // @.str.92
.L.str.92:
	.asciz	"dfa-esi"
	.size	.L.str.92, 8

	.type	.L.str.93,@object               // @.str.93
.L.str.93:
	.asciz	"%s: Fail to request IRQ for %d, err = %d\n"
	.size	.L.str.93, 42

	.type	.L__func__.dfa_mcq_config_esi,@object // @__func__.dfa_mcq_config_esi
.L__func__.dfa_mcq_config_esi:
	.asciz	"dfa_mcq_config_esi"
	.size	.L__func__.dfa_mcq_config_esi, 19

	.type	.L.str.94,@object               // @.str.94
.L.str.94:
	.asciz	"[DFA] 0x%x - 0x%x - 0x%x - 0x%x - 0x%x - 0x%x - 0x%x - 0x%x\n"
	.size	.L.str.94, 61

	.type	.L.str.95,@object               // @.str.95
.L.str.95:
	.asciz	"dfa reset immediately, exist delay request %d.\n"
	.size	.L.str.95, 48

	.type	.L.str.96,@object               // @.str.96
.L.str.96:
	.asciz	"%s queue dhba->eh_worker fail\n"
	.size	.L.str.96, 31

	.type	.L__func__.dfa_proc_immediately_reset,@object // @__func__.dfa_proc_immediately_reset
.L__func__.dfa_proc_immediately_reset:
	.asciz	"dfa_proc_immediately_reset"
	.size	.L__func__.dfa_proc_immediately_reset, 27

	.type	.L.str.97,@object               // @.str.97
.L.str.97:
	.asciz	"dfa reset immediately, work doing %d.\n"
	.size	.L.str.97, 39

	.type	.L.str.98,@object               // @.str.98
.L.str.98:
	.asciz	"dfa reset immediately, directly %d.\n"
	.size	.L.str.98, 37

	.type	.L.str.99,@object               // @.str.99
.L.str.99:
	.asciz	"dfa reset delay\n"
	.size	.L.str.99, 17

	.type	.L.str.100,@object              // @.str.100
.L.str.100:
	.asciz	"dfa reset delay, work have be pending.\n"
	.size	.L.str.100, 40

	.type	.L.str.101,@object              // @.str.101
.L.str.101:
	.asciz	"dfa reset delay, wrong status %d.\n"
	.size	.L.str.101, 35

	.type	.L.str.102,@object              // @.str.102
.L.str.102:
	.asciz	"\0013%s, dev error\n"
	.size	.L.str.102, 17

	.type	.L__func__.dfa_mcq_write_msi_msg,@object // @__func__.dfa_mcq_write_msi_msg
.L__func__.dfa_mcq_write_msi_msg:
	.asciz	"dfa_mcq_write_msi_msg"
	.size	.L__func__.dfa_mcq_write_msi_msg, 22

	.type	.L.str.103,@object              // @.str.103
.L.str.103:
	.asciz	"\0013%s, hba error\n"
	.size	.L.str.103, 17

	.type	.L.str.104,@object              // @.str.104
.L.str.104:
	.asciz	"dfa msi irq error irq:%d id:%u, idx:%u\n"
	.size	.L.str.104, 40

	.type	.L.str.105,@object              // @.str.105
.L.str.105:
	.asciz	"[DFA] invalid tag:%u - %x QUE_PROC %u SQH:0x%08x SQT:0x%08x CQH:0x%08x CQT:0x%08x CQIE:0x%08x CQIS:0x%08x\n"
	.size	.L.str.105, 107

	.type	.L.str.106,@object              // @.str.106
.L.str.106:
	.asciz	"[DFA] set tag:%d error\n"
	.size	.L.str.106, 24

	.type	.L__func__.arch_local_irq_restore,@object // @__func__.arch_local_irq_restore
.L__func__.arch_local_irq_restore:
	.asciz	"arch_local_irq_restore"
	.size	.L__func__.arch_local_irq_restore, 23

	.type	__init_completion.__key,@object // @__init_completion.__key
	.local	__init_completion.__key
	.comm	__init_completion.__key,1,1
	.type	.L.str.107,@object              // @.str.107
.L.str.107:
	.asciz	"&x->wait"
	.size	.L.str.107, 9

	.type	.L.str.108,@object              // @.str.108
.L.str.108:
	.asciz	"%s timedout, op:%u, lun:%u\n"
	.size	.L.str.108, 28

	.type	.L__func__.dfa_wait_for_dev_cmd,@object // @__func__.dfa_wait_for_dev_cmd
.L__func__.dfa_wait_for_dev_cmd:
	.asciz	"dfa_wait_for_dev_cmd"
	.size	.L__func__.dfa_wait_for_dev_cmd, 21

	.type	.L.str.109,@object              // @.str.109
.L.str.109:
	.asciz	"[DFA] OCS:0x%x op:0x%x tag:%d need retry\n"
	.size	.L.str.109, 42

	.type	.L.str.110,@object              // @.str.110
.L.str.110:
	.asciz	"[DFA] OCS:%d error handled with DFA for tag %d\n"
	.size	.L.str.110, 48

	.type	.L.str.111,@object              // @.str.111
.L.str.111:
	.asciz	"[DFA] Reject UPIU not fully implemented\n"
	.size	.L.str.111, 41

	.type	.L.str.112,@object              // @.str.112
.L.str.112:
	.asciz	"[DFA] Unexpected request response code = %x\n"
	.size	.L.str.112, 45

	.type	.L.str.113,@object              // @.str.113
.L.str.113:
	.asciz	"\0013%s, [DFA] ID:%u - SQH:%u - SQT:%u - CQH:%u - CQT:%u\n"
	.size	.L.str.113, 55

	.type	.L__func__.__dfa_queuecommand_directly,@object // @__func__.__dfa_queuecommand_directly
.L__func__.__dfa_queuecommand_directly:
	.asciz	"__dfa_queuecommand_directly"
	.size	.L__func__.__dfa_queuecommand_directly, 28

	.type	.L.str.114,@object              // @.str.114
.L.str.114:
	.asciz	"\0013%s, [DFA] wait acore int status timeout, val:0x%x\n"
	.size	.L.str.114, 53

	.type	.L__func__.dfa_power_off,@object // @__func__.dfa_power_off
.L__func__.dfa_power_off:
	.asciz	"dfa_power_off"
	.size	.L__func__.dfa_power_off, 14

	.type	.L.str.115,@object              // @.str.115
.L.str.115:
	.asciz	"\0013%s: AO_MEM_CTRL2 Write fail, set DFA&UFS DS fail, fail ret = %d\n"
	.size	.L.str.115, 67

	.type	.L.str.116,@object              // @.str.116
.L.str.116:
	.asciz	"\0013%s: Wait HSDT_MEM_STATUS fail, fail ret = %d\n"
	.size	.L.str.116, 48

	.type	.L.str.117,@object              // @.str.117
.L.str.117:
	.asciz	"\0013%s, wait QUE_CFG_RAM_STATUS timeout\n"
	.size	.L.str.117, 39

	.type	.L__func__.dfa_suspend_prepare,@object // @__func__.dfa_suspend_prepare
.L__func__.dfa_suspend_prepare:
	.asciz	"dfa_suspend_prepare"
	.size	.L__func__.dfa_suspend_prepare, 20

	.type	.L.str.118,@object              // @.str.118
.L.str.118:
	.asciz	"\0013%s, [DFA] que_proc reload timeout, val:0x%x\n"
	.size	.L.str.118, 47

	.type	.L__func__.dfa_que_proc_reload,@object // @__func__.dfa_que_proc_reload
.L__func__.dfa_que_proc_reload:
	.asciz	"dfa_que_proc_reload"
	.size	.L__func__.dfa_que_proc_reload, 20

	.type	.L__func__.dfa_power_on,@object // @__func__.dfa_power_on
.L__func__.dfa_power_on:
	.asciz	"dfa_power_on"
	.size	.L__func__.dfa_power_on, 13

	.type	.L.str.119,@object              // @.str.119
.L.str.119:
	.asciz	"\0013%s, [DFA] wait HSDT0_QIC_BUS exit H8 timeout\n"
	.size	.L.str.119, 48

	.type	.L.str.120,@object              // @.str.120
.L.str.120:
	.asciz	"\0013%s, [DFA] QIC_PD timeout\n"
	.size	.L.str.120, 28

	.type	.L.str.121,@object              // @.str.121
.L.str.121:
	.asciz	"dfa reset timer woker, work doing %d.\n"
	.size	.L.str.121, 39

	.type	.L.str.122,@object              // @.str.122
.L.str.122:
	.asciz	"dfa reset timer woker, wrong status %d.\n"
	.size	.L.str.122, 41

	.type	.L.str.123,@object              // @.str.123
.L.str.123:
	.asciz	"[DFA] blocked\n"
	.size	.L.str.123, 15

	.type	.L.str.124,@object              // @.str.124
.L.str.124:
	.asciz	"%s, can't get the hostlock!\n"
	.size	.L.str.124, 29

	.type	.L__func__.dfa_direct_flush_get_device_and_check,@object // @__func__.dfa_direct_flush_get_device_and_check
.L__func__.dfa_direct_flush_get_device_and_check:
	.asciz	"dfa_direct_flush_get_device_and_check"
	.size	.L__func__.dfa_direct_flush_get_device_and_check, 38

	.type	.L.str.125,@object              // @.str.125
.L.str.125:
	.asciz	"%s, scsi_device_get error, %d\n"
	.size	.L.str.125, 31

	.type	.L.str.126,@object              // @.str.126
.L.str.126:
	.asciz	"%s, scsi_device not online, %d\n"
	.size	.L.str.126, 32

	.type	.L.str.127,@object              // @.str.127
.L.str.127:
	.asciz	"%s, ufshcd_state isn't operational, ufshcd_state = %d\n"
	.size	.L.str.127, 55

	.type	.L.str.128,@object              // @.str.128
.L.str.128:
	.asciz	"DFA:<%s> Emergency sync cache lun=%llu ret = %d \r\n"
	.size	.L.str.128, 51

	.type	.L__func__.dfa_send_scsi_sync_cache_on_specified_disk,@object // @__func__.dfa_send_scsi_sync_cache_on_specified_disk
.L__func__.dfa_send_scsi_sync_cache_on_specified_disk:
	.asciz	"dfa_send_scsi_sync_cache_on_specified_disk"
	.size	.L__func__.dfa_send_scsi_sync_cache_on_specified_disk, 43

	.type	.L.str.129,@object              // @.str.129
.L.str.129:
	.asciz	"[DFA] wait clear timeout dfx1:0x%x dfx4:%x\n"
	.size	.L.str.129, 44

	.type	.L.str.130,@object              // @.str.130
.L.str.130:
	.asciz	"outstandings:0x%llx\n"
	.size	.L.str.130, 21

	.type	.L.str.131,@object              // @.str.131
.L.str.131:
	.asciz	"[DFA] mcq hw queue flush err intr\n"
	.size	.L.str.131, 35

	.type	.L.str.132,@object              // @.str.132
.L.str.132:
	.asciz	"[DFA] mcq hw synccache queue err intr\n"
	.size	.L.str.132, 39

	.type	.L__func__.wait_for_command_completion,@object // @__func__.wait_for_command_completion
.L__func__.wait_for_command_completion:
	.asciz	"wait_for_command_completion"
	.size	.L__func__.wait_for_command_completion, 28

	.ident	"OHOS (BiSheng Mobile CPU STD 202.1.0.B030) clang version 15.0.4 (llvm-project 7123208073d2)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym kthread_worker_fn
	.addrsig_sym dfa_error_handler
	.addrsig_sym dfa_proc_reset_req_work_fn
	.addrsig_sym delayed_work_timer_fn
	.addrsig_sym dfa_acore_irq_handler
	.addrsig_sym dfa_mcq_write_msi_msg
	.addrsig_sym dfa_mcq_esi_handler
	.addrsig_sym dfa_mcq_params_init.__key
	.addrsig_sym dfa_mcq_params_init.__key.65
	.addrsig_sym __init_completion.__key
