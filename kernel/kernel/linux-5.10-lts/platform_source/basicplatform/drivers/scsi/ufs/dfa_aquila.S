	.text
	.file	"dfa_aquila.c"
	.globl	dfa_que_proc_init               // -- Begin function dfa_que_proc_init
	.p2align	2
	.type	dfa_que_proc_init,@function
dfa_que_proc_init:                      // @dfa_que_proc_init
// %bb.0:
	stp	x29, x30, [sp, #-32]!           // 16-byte Folded Spill
	mov	w8, #13572
	stp	x20, x19, [sp, #16]             // 16-byte Folded Spill
	movk	w8, #49920, lsl #16
	mov	x19, x0
	orr	x0, x8, #0x1
	mov	x1, xzr
	mov	x2, xzr
	mov	x3, xzr
	mov	x29, sp
	bl	atfd_ufs_smc
	cbnz	w0, .LBB0_6
// %bb.1:
	mov	w0, #21475
	bl	__const_udelay
	mov	w0, #13572
	mov	w1, #48
	movk	w0, #49920, lsl #16
	mov	x2, xzr
	mov	w3, #5000
	bl	atfd_ufs_smc
	cbnz	w0, .LBB0_7
// %bb.2:
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #131072
	ldr	x8, [x19, #7016]
	//APP
	str	w9, [x8]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #64
	ldr	x8, [x19, #7016]
	movk	w9, #65157, lsl #16
	mov	x0, x19
	add	x8, x8, #16
	//APP
	str	w9, [x8]
	//NO_APP
	ldr	x8, [x19, #7016]
	add	x8, x8, #4
	//APP
.Ltmp0:
	ldr	w8, [x8]
.Ltmp1:
	.section	.altinstructions,"a",@progbits
.Ltmp2:
	.word	.Ltmp0-.Ltmp2
.Ltmp4:
	.word	.Ltmp3-.Ltmp4
	.hword	1
	.byte	.Ltmp1-.Ltmp0
	.byte	.Ltmp5-.Ltmp3
	.text

	.text	1
.Ltmp3:
	ldar	w8, [x8]
.Ltmp5:
.Ltmp6:
.org (.Ltmp6-(.Ltmp5-.Ltmp3))+(.Ltmp1-.Ltmp0), 0
.Ltmp7:
.org (.Ltmp7-(.Ltmp1-.Ltmp0))+(.Ltmp5-.Ltmp3), 0
	.text


	//NO_APP
	mov	w9, w8
	//APP
	dmb	oshld
	//NO_APP
	orr	w8, w8, #0x1
	//APP
	eor	x9, x9, x9
.Ltmp8:
	cbnz	x9, .Ltmp8
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
	ldrb	w8, [x19, #259]
	tbnz	w8, #1, .LBB0_5
.LBB0_3:
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #5168
	ldr	x8, [x19, #7016]
	mov	w20, wzr
	add	x8, x8, x9
	mov	w9, #1
	//APP
	str	w9, [x8]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x8, [x19, #7016]
	mov	w9, #5188
	add	x8, x8, x9
	mov	w9, #513
	//APP
	str	w9, [x8]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x8, [x19, #7016]
	mov	w9, #5208
	add	x8, x8, x9
	mov	w9, #5121
	//APP
	str	w9, [x8]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x8, [x19, #7016]
	mov	w9, #5228
	add	x8, x8, x9
	mov	w9, #9729
	//APP
	str	w9, [x8]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x8, [x19, #7016]
	mov	w9, #5248
	add	x8, x8, x9
	mov	w9, #10241
	//APP
	str	w9, [x8]
	//NO_APP
.LBB0_4:
	mov	w0, w20
	ldp	x20, x19, [sp, #16]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #32             // 16-byte Folded Reload
	ret
.LBB0_5:
	mov	x0, x19
	mov	w1, #1
	bl	dfa_enable_ooo_mode
	b	.LBB0_3
.LBB0_6:
	mov	w20, w0
	adrp	x0, .L.str
	add	x0, x0, :lo12:.L.str
	b	.LBB0_8
.LBB0_7:
	mov	w20, w0
	adrp	x0, .L.str.1
	add	x0, x0, :lo12:.L.str.1
.LBB0_8:
	adrp	x1, .L__func__.dfa_que_proc_init
	add	x1, x1, :lo12:.L__func__.dfa_que_proc_init
	mov	w2, w20
	bl	printk
	b	.LBB0_4
.Lfunc_end0:
	.size	dfa_que_proc_init, .Lfunc_end0-dfa_que_proc_init
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
.Ltmp9:
	ldr	w8, [x8]
.Ltmp10:
	.section	.altinstructions,"a",@progbits
.Ltmp11:
	.word	.Ltmp9-.Ltmp11
.Ltmp13:
	.word	.Ltmp12-.Ltmp13
	.hword	1
	.byte	.Ltmp10-.Ltmp9
	.byte	.Ltmp14-.Ltmp12
	.text

	.text	1
.Ltmp12:
	ldar	w8, [x8]
.Ltmp14:
.Ltmp15:
.org (.Ltmp15-(.Ltmp14-.Ltmp12))+(.Ltmp10-.Ltmp9), 0
.Ltmp16:
.org (.Ltmp16-(.Ltmp10-.Ltmp9))+(.Ltmp14-.Ltmp12), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w8, w8
	//APP
	eor	x9, x8, x8
.Ltmp17:
	cbnz	x9, .Ltmp17
	//NO_APP
	tbnz	w8, #5, .LBB1_2
// %bb.1:
	ldp	x29, x30, [sp], #16             // 16-byte Folded Reload
	ret
.LBB1_2:
	mov	w0, #300
	bl	msleep
	ldp	x29, x30, [sp], #16             // 16-byte Folded Reload
	ret
.Lfunc_end1:
	.size	dfa_em_check, .Lfunc_end1-dfa_em_check
                                        // -- End function
	.globl	dfa_early_reset                 // -- Begin function dfa_early_reset
	.p2align	2
	.type	dfa_early_reset,@function
dfa_early_reset:                        // @dfa_early_reset
// %bb.0:
	stp	x29, x30, [sp, #-48]!           // 16-byte Folded Spill
	str	x21, [sp, #16]                  // 8-byte Folded Spill
	ldr	x21, [x0, #296]
	stp	x20, x19, [sp, #32]             // 16-byte Folded Spill
	mov	x19, x0
	//APP
	dmb	oshst
	//NO_APP
	mov	w20, #4999
	ldr	x9, [x21, #32]
	mov	x29, sp
	mov	w8, #268435456
	add	x9, x9, #96
	//APP
	str	w8, [x9]
	//NO_APP
.LBB2_1:                                // =>This Inner Loop Header: Depth=1
	ldr	x8, [x21, #96]
	add	x8, x8, #24
	//APP
.Ltmp18:
	ldr	w8, [x8]
.Ltmp19:
	.section	.altinstructions,"a",@progbits
.Ltmp20:
	.word	.Ltmp18-.Ltmp20
.Ltmp22:
	.word	.Ltmp21-.Ltmp22
	.hword	1
	.byte	.Ltmp19-.Ltmp18
	.byte	.Ltmp23-.Ltmp21
	.text

	.text	1
.Ltmp21:
	ldar	w8, [x8]
.Ltmp23:
.Ltmp24:
.org (.Ltmp24-(.Ltmp23-.Ltmp21))+(.Ltmp19-.Ltmp18), 0
.Ltmp25:
.org (.Ltmp25-(.Ltmp19-.Ltmp18))+(.Ltmp23-.Ltmp21), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w8, w8
	//APP
	eor	x9, x8, x8
.Ltmp26:
	cbnz	x9, .Ltmp26
	//NO_APP
	tbnz	w8, #0, .LBB2_19
// %bb.2:                               //   in Loop: Header=BB2_1 Depth=1
	mov	w0, #4295
	bl	__const_udelay
	sub	w20, w20, #1
	cmn	w20, #1
	b.ne	.LBB2_1
.LBB2_3:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.2
	add	x1, x1, :lo12:.L.str.2
	bl	_dev_err
.LBB2_4:
	mov	w20, #4999
.LBB2_5:                                // =>This Inner Loop Header: Depth=1
	ldr	x8, [x21, #96]
	add	x8, x8, #28
	//APP
.Ltmp27:
	ldr	w8, [x8]
.Ltmp28:
	.section	.altinstructions,"a",@progbits
.Ltmp29:
	.word	.Ltmp27-.Ltmp29
.Ltmp31:
	.word	.Ltmp30-.Ltmp31
	.hword	1
	.byte	.Ltmp28-.Ltmp27
	.byte	.Ltmp32-.Ltmp30
	.text

	.text	1
.Ltmp30:
	ldar	w8, [x8]
.Ltmp32:
.Ltmp33:
.org (.Ltmp33-(.Ltmp32-.Ltmp30))+(.Ltmp28-.Ltmp27), 0
.Ltmp34:
.org (.Ltmp34-(.Ltmp28-.Ltmp27))+(.Ltmp32-.Ltmp30), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w8, w8
	//APP
	eor	x9, x8, x8
.Ltmp35:
	cbnz	x9, .Ltmp35
	//NO_APP
	tbnz	w8, #0, .LBB2_20
// %bb.6:                               //   in Loop: Header=BB2_5 Depth=1
	mov	w0, #4295
	bl	__const_udelay
	sub	w20, w20, #1
	cmn	w20, #1
	b.ne	.LBB2_5
.LBB2_7:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.3
	add	x1, x1, :lo12:.L.str.3
	bl	_dev_err
.LBB2_8:
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #1
	ldr	x8, [x21, #80]
	add	x8, x8, #468
	//APP
	str	w9, [x8]
	//NO_APP
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
.Ltmp36:
	ldr	w8, [x8]
.Ltmp37:
	.section	.altinstructions,"a",@progbits
.Ltmp38:
	.word	.Ltmp36-.Ltmp38
.Ltmp40:
	.word	.Ltmp39-.Ltmp40
	.hword	1
	.byte	.Ltmp37-.Ltmp36
	.byte	.Ltmp41-.Ltmp39
	.text

	.text	1
.Ltmp39:
	ldar	w8, [x8]
.Ltmp41:
.Ltmp42:
.org (.Ltmp42-(.Ltmp41-.Ltmp39))+(.Ltmp37-.Ltmp36), 0
.Ltmp43:
.org (.Ltmp43-(.Ltmp37-.Ltmp36))+(.Ltmp41-.Ltmp39), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w8, w8
	//APP
	eor	x9, x8, x8
.Ltmp44:
	cbnz	x9, .Ltmp44
	//NO_APP
	tbz	w8, #5, .LBB2_10
// %bb.9:
	mov	w0, #300
	bl	msleep
.LBB2_10:
	ldr	x20, [x21, #208]
	mov	x0, x20
	bl	clk_disable
	mov	x0, x20
	bl	clk_unprepare
	ldrb	w8, [x21, #240]
	tbnz	w8, #6, .LBB2_13
// %bb.11:
	ldr	x8, [x19, #88]
	ldrh	w8, [x8, #546]
	cbnz	w8, .LBB2_13
// %bb.12:
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #4128
	ldr	x8, [x21, #56]
	movk	w9, #4128, lsl #16
	//APP
	str	w9, [x8]
	//NO_APP
.LBB2_13:
	//APP
	dmb	oshst
	//NO_APP
	mov	w20, #-5000
	ldr	x8, [x21, #96]
	mov	w9, #262148
	add	x8, x8, #1060
	//APP
	str	w9, [x8]
	//NO_APP
.LBB2_14:                               // =>This Inner Loop Header: Depth=1
	ldr	x8, [x21, #96]
	add	x8, x8, #1064
	//APP
.Ltmp45:
	ldr	w8, [x8]
.Ltmp46:
	.section	.altinstructions,"a",@progbits
.Ltmp47:
	.word	.Ltmp45-.Ltmp47
.Ltmp49:
	.word	.Ltmp48-.Ltmp49
	.hword	1
	.byte	.Ltmp46-.Ltmp45
	.byte	.Ltmp50-.Ltmp48
	.text

	.text	1
.Ltmp48:
	ldar	w8, [x8]
.Ltmp50:
.Ltmp51:
.org (.Ltmp51-(.Ltmp50-.Ltmp48))+(.Ltmp46-.Ltmp45), 0
.Ltmp52:
.org (.Ltmp52-(.Ltmp46-.Ltmp45))+(.Ltmp50-.Ltmp48), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w8, w8
	//APP
	eor	x9, x8, x8
.Ltmp53:
	cbnz	x9, .Ltmp53
	//NO_APP
	tbnz	w8, #2, .LBB2_21
// %bb.15:                              //   in Loop: Header=BB2_14 Depth=1
	mov	w0, #4295
	bl	__const_udelay
	adds	w20, w20, #1
	b.lo	.LBB2_14
// %bb.16:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.4
	add	x1, x1, :lo12:.L.str.4
.LBB2_17:
	bl	_dev_err
	mov	w0, #-1
.LBB2_18:
	ldp	x20, x19, [sp, #32]             // 16-byte Folded Reload
	ldr	x21, [sp, #16]                  // 8-byte Folded Reload
	ldp	x29, x30, [sp], #48             // 16-byte Folded Reload
	ret
.LBB2_19:
	cmp	w20, #0
	b.le	.LBB2_3
	b	.LBB2_4
.LBB2_20:
	cmp	w20, #0
	b.le	.LBB2_7
	b	.LBB2_8
.LBB2_21:
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #4096
	ldr	x8, [x21, #64]
	mov	w20, #-5000
	add	x8, x8, #96
	//APP
	str	w9, [x8]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #262144
	ldr	x8, [x21, #96]
	add	x8, x8, #1060
	//APP
	str	w9, [x8]
	//NO_APP
.LBB2_22:                               // =>This Inner Loop Header: Depth=1
	ldr	x8, [x21, #96]
	add	x8, x8, #1064
	//APP
.Ltmp54:
	ldr	w8, [x8]
.Ltmp55:
	.section	.altinstructions,"a",@progbits
.Ltmp56:
	.word	.Ltmp54-.Ltmp56
.Ltmp58:
	.word	.Ltmp57-.Ltmp58
	.hword	1
	.byte	.Ltmp55-.Ltmp54
	.byte	.Ltmp59-.Ltmp57
	.text

	.text	1
.Ltmp57:
	ldar	w8, [x8]
.Ltmp59:
.Ltmp60:
.org (.Ltmp60-(.Ltmp59-.Ltmp57))+(.Ltmp55-.Ltmp54), 0
.Ltmp61:
.org (.Ltmp61-(.Ltmp55-.Ltmp54))+(.Ltmp59-.Ltmp57), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w8, w8
	//APP
	eor	x9, x8, x8
.Ltmp62:
	cbnz	x9, .Ltmp62
	//NO_APP
	tbz	w8, #2, .LBB2_25
// %bb.23:                              //   in Loop: Header=BB2_22 Depth=1
	mov	w0, #4295
	bl	__const_udelay
	adds	w20, w20, #1
	b.lo	.LBB2_22
// %bb.24:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.5
	add	x1, x1, :lo12:.L.str.5
	b	.LBB2_17
.LBB2_25:
	mov	x0, x19
	bl	ufs_clk_init
	mov	x0, x19
	bl	ufs_soc_init
	ldr	x21, [x19, #296]
	mov	w11, #2
	adrp	x1, .L.str.20
	add	x1, x1, :lo12:.L.str.20
	ldr	x8, [x21, #48]
	ldr	x9, [x21, #240]
	add	x8, x8, #1220
	//APP
.Ltmp63:
	ldr	w8, [x8]
.Ltmp64:
	.section	.altinstructions,"a",@progbits
.Ltmp65:
	.word	.Ltmp63-.Ltmp65
.Ltmp67:
	.word	.Ltmp66-.Ltmp67
	.hword	1
	.byte	.Ltmp64-.Ltmp63
	.byte	.Ltmp68-.Ltmp66
	.text

	.text	1
.Ltmp66:
	ldar	w8, [x8]
.Ltmp68:
.Ltmp69:
.org (.Ltmp69-(.Ltmp68-.Ltmp66))+(.Ltmp64-.Ltmp63), 0
.Ltmp70:
.org (.Ltmp70-(.Ltmp64-.Ltmp63))+(.Ltmp68-.Ltmp66), 0
	.text


	//NO_APP
	mov	w10, w8
	//APP
	dmb	oshld
	//NO_APP
	tst	x9, #0x40
	//APP
	eor	x10, x10, x10
.Ltmp71:
	cbnz	x10, .Ltmp71
	//NO_APP
	mov	w9, #18
	ldr	x10, [x21, #48]
	csel	w9, w11, w9, eq
	and	w8, w8, #0x600
	orr	w20, w9, w8
	add	x10, x10, #1220
	//APP
.Ltmp72:
	ldr	w10, [x10]
.Ltmp73:
	.section	.altinstructions,"a",@progbits
.Ltmp74:
	.word	.Ltmp72-.Ltmp74
.Ltmp76:
	.word	.Ltmp75-.Ltmp76
	.hword	1
	.byte	.Ltmp73-.Ltmp72
	.byte	.Ltmp77-.Ltmp75
	.text

	.text	1
.Ltmp75:
	ldar	w10, [x10]
.Ltmp77:
.Ltmp78:
.org (.Ltmp78-(.Ltmp77-.Ltmp75))+(.Ltmp73-.Ltmp72), 0
.Ltmp79:
.org (.Ltmp79-(.Ltmp73-.Ltmp72))+(.Ltmp77-.Ltmp75), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w12, w10
	//APP
	eor	x8, x12, x12
.Ltmp80:
	cbnz	x8, .Ltmp80
	//NO_APP
	bfi	w20, w10, #11, #1
	ldr	x0, [x19, #96]
	mov	w2, w20
	bl	_dev_err
	//APP
	dmb	oshst
	//NO_APP
	ldr	x8, [x19, #7008]
	add	x8, x8, #260
	//APP
	str	w20, [x8]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	mov	w8, #7080
	ldr	x9, [x19, #7008]
	ldrb	w8, [x19, x8]
	add	x9, x9, #256
	//APP
	str	w8, [x9]
	//NO_APP
	ldrb	w8, [x21, #240]
	tbz	w8, #6, .LBB2_31
.LBB2_26:
	//APP
	dmb	oshst
	//NO_APP
	mov	w10, #8388608
	ldr	x9, [x21, #32]
	//APP
	str	w10, [x9]
	//NO_APP
	ldr	x9, [x19, #7008]
	mov	w20, #4999
	mov	w8, wzr
	add	x9, x9, #36
	//APP
.Ltmp81:
	ldr	w9, [x9]
.Ltmp82:
	.section	.altinstructions,"a",@progbits
.Ltmp83:
	.word	.Ltmp81-.Ltmp83
.Ltmp85:
	.word	.Ltmp84-.Ltmp85
	.hword	1
	.byte	.Ltmp82-.Ltmp81
	.byte	.Ltmp86-.Ltmp84
	.text

	.text	1
.Ltmp84:
	ldar	w9, [x9]
.Ltmp86:
.Ltmp87:
.org (.Ltmp87-(.Ltmp86-.Ltmp84))+(.Ltmp82-.Ltmp81), 0
.Ltmp88:
.org (.Ltmp88-(.Ltmp82-.Ltmp81))+(.Ltmp86-.Ltmp84), 0
	.text


	//NO_APP
	mov	w10, w9
	//APP
	dmb	oshld
	//NO_APP
	and	w9, w9, #0xfffffffe
	//APP
	eor	x10, x10, x10
.Ltmp89:
	cbnz	x10, .Ltmp89
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x10, [x19, #7008]
	add	x10, x10, #36
	//APP
	str	w9, [x10]
	//NO_APP
	ldr	x9, [x21, #32]
	add	x9, x9, #100
	//APP
.Ltmp90:
	ldr	w9, [x9]
.Ltmp91:
	.section	.altinstructions,"a",@progbits
.Ltmp92:
	.word	.Ltmp90-.Ltmp92
.Ltmp94:
	.word	.Ltmp93-.Ltmp94
	.hword	1
	.byte	.Ltmp91-.Ltmp90
	.byte	.Ltmp95-.Ltmp93
	.text

	.text	1
.Ltmp93:
	ldar	w9, [x9]
.Ltmp95:
.Ltmp96:
.org (.Ltmp96-(.Ltmp95-.Ltmp93))+(.Ltmp91-.Ltmp90), 0
.Ltmp97:
.org (.Ltmp97-(.Ltmp91-.Ltmp90))+(.Ltmp95-.Ltmp93), 0
	.text


	//NO_APP
	mov	w10, w9
	//APP
	dmb	oshld
	//NO_APP
	orr	w9, w9, #0x10000000
	//APP
	eor	x10, x10, x10
.Ltmp98:
	cbnz	x10, .Ltmp98
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x10, [x21, #32]
	add	x10, x10, #100
	//APP
	str	w9, [x10]
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x9, [x19, #7008]
	add	x9, x9, #100
	//APP
	str	w8, [x9]
	//NO_APP
.LBB2_27:                               // =>This Inner Loop Header: Depth=1
	ldr	x8, [x19, #7008]
	add	x8, x8, #104
	//APP
.Ltmp99:
	ldr	w8, [x8]
.Ltmp100:
	.section	.altinstructions,"a",@progbits
.Ltmp101:
	.word	.Ltmp99-.Ltmp101
.Ltmp103:
	.word	.Ltmp102-.Ltmp103
	.hword	1
	.byte	.Ltmp100-.Ltmp99
	.byte	.Ltmp104-.Ltmp102
	.text

	.text	1
.Ltmp102:
	ldar	w8, [x8]
.Ltmp104:
.Ltmp105:
.org (.Ltmp105-(.Ltmp104-.Ltmp102))+(.Ltmp100-.Ltmp99), 0
.Ltmp106:
.org (.Ltmp106-(.Ltmp100-.Ltmp99))+(.Ltmp104-.Ltmp102), 0
	.text


	//NO_APP
	mov	w9, w8
	//APP
	dmb	oshld
	//NO_APP
	tst	w8, #0x3
	//APP
	eor	x9, x9, x9
.Ltmp107:
	cbnz	x9, .Ltmp107
	//NO_APP
	b.eq	.LBB2_29
// %bb.28:                              //   in Loop: Header=BB2_27 Depth=1
	mov	w0, #4295
	bl	__const_udelay
	sub	w20, w20, #1
	cmn	w20, #1
	b.ne	.LBB2_27
	b	.LBB2_32
.LBB2_29:
	cmp	w20, #0
	b.le	.LBB2_32
// %bb.30:
	ldr	x8, [x19, #7008]
	mov	w0, wzr
	add	x8, x8, #36
	//APP
.Ltmp108:
	ldr	w8, [x8]
.Ltmp109:
	.section	.altinstructions,"a",@progbits
.Ltmp110:
	.word	.Ltmp108-.Ltmp110
.Ltmp112:
	.word	.Ltmp111-.Ltmp112
	.hword	1
	.byte	.Ltmp109-.Ltmp108
	.byte	.Ltmp113-.Ltmp111
	.text

	.text	1
.Ltmp111:
	ldar	w8, [x8]
.Ltmp113:
.Ltmp114:
.org (.Ltmp114-(.Ltmp113-.Ltmp111))+(.Ltmp109-.Ltmp108), 0
.Ltmp115:
.org (.Ltmp115-(.Ltmp109-.Ltmp108))+(.Ltmp113-.Ltmp111), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w9, w8
	orr	w8, w8, #0x1
	//APP
	eor	x9, x9, x9
.Ltmp116:
	cbnz	x9, .Ltmp116
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x9, [x19, #7008]
	add	x9, x9, #36
	//APP
	str	w8, [x9]
	//NO_APP
	b	.LBB2_18
.LBB2_31:
	ldr	x8, [x21, #48]
	add	x8, x8, #12
	//APP
.Ltmp117:
	ldr	w8, [x8]
.Ltmp118:
	.section	.altinstructions,"a",@progbits
.Ltmp119:
	.word	.Ltmp117-.Ltmp119
.Ltmp121:
	.word	.Ltmp120-.Ltmp121
	.hword	1
	.byte	.Ltmp118-.Ltmp117
	.byte	.Ltmp122-.Ltmp120
	.text

	.text	1
.Ltmp120:
	ldar	w8, [x8]
.Ltmp122:
.Ltmp123:
.org (.Ltmp123-(.Ltmp122-.Ltmp120))+(.Ltmp118-.Ltmp117), 0
.Ltmp124:
.org (.Ltmp124-(.Ltmp118-.Ltmp117))+(.Ltmp122-.Ltmp120), 0
	.text


	//NO_APP
	mov	w9, w8
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x9, x9, x9
.Ltmp125:
	cbnz	x9, .Ltmp125
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x9, [x19, #7008]
	add	x9, x9, #284
	//APP
	str	w8, [x9]
	//NO_APP
	ldr	x8, [x21, #48]
	add	x8, x8, #96
	//APP
.Ltmp126:
	ldr	w8, [x8]
.Ltmp127:
	.section	.altinstructions,"a",@progbits
.Ltmp128:
	.word	.Ltmp126-.Ltmp128
.Ltmp130:
	.word	.Ltmp129-.Ltmp130
	.hword	1
	.byte	.Ltmp127-.Ltmp126
	.byte	.Ltmp131-.Ltmp129
	.text

	.text	1
.Ltmp129:
	ldar	w8, [x8]
.Ltmp131:
.Ltmp132:
.org (.Ltmp132-(.Ltmp131-.Ltmp129))+(.Ltmp127-.Ltmp126), 0
.Ltmp133:
.org (.Ltmp133-(.Ltmp127-.Ltmp126))+(.Ltmp131-.Ltmp129), 0
	.text


	//NO_APP
	mov	w9, w8
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x9, x9, x9
.Ltmp134:
	cbnz	x9, .Ltmp134
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x9, [x19, #7008]
	add	x9, x9, #288
	//APP
	str	w8, [x9]
	//NO_APP
	b	.LBB2_26
.LBB2_32:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.21
	add	x1, x1, :lo12:.L.str.21
	b	.LBB2_17
.Lfunc_end2:
	.size	dfa_early_reset, .Lfunc_end2-dfa_early_reset
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
.Ltmp135:
	ldr	w8, [x8]
.Ltmp136:
	.section	.altinstructions,"a",@progbits
.Ltmp137:
	.word	.Ltmp135-.Ltmp137
.Ltmp139:
	.word	.Ltmp138-.Ltmp139
	.hword	1
	.byte	.Ltmp136-.Ltmp135
	.byte	.Ltmp140-.Ltmp138
	.text

	.text	1
.Ltmp138:
	ldar	w8, [x8]
.Ltmp140:
.Ltmp141:
.org (.Ltmp141-(.Ltmp140-.Ltmp138))+(.Ltmp136-.Ltmp135), 0
.Ltmp142:
.org (.Ltmp142-(.Ltmp136-.Ltmp135))+(.Ltmp140-.Ltmp138), 0
	.text


	//NO_APP
	mov	w9, w8
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x9, x9, x9
.Ltmp143:
	cbnz	x9, .Ltmp143
	//NO_APP
	ldr	x0, [x22, #200]
	tbz	w8, #0, .LBB3_8
// %bb.1:
	mov	w1, #11392
	movk	w1, #6378, lsl #16
	bl	clk_set_rate
	cbnz	w0, .LBB3_17
.LBB3_2:
	ldr	x20, [x22, #200]
	mov	x0, x20
	bl	clk_prepare
	cbnz	w0, .LBB3_21
// %bb.3:
	mov	x0, x20
	bl	clk_enable
	cbnz	w0, .LBB3_20
.LBB3_4:
	mov	w1, #30976
	ldr	x0, [x22, #208]
	movk	w1, #14709, lsl #16
	bl	clk_set_rate
	cbnz	w0, .LBB3_15
.LBB3_5:
	ldr	x21, [x22, #208]
	mov	x0, x21
	bl	clk_prepare
	cbnz	w0, .LBB3_16
// %bb.6:
	mov	x0, x21
	bl	clk_enable
	cbnz	w0, .LBB3_18
.LBB3_7:
	ldp	x20, x19, [sp, #32]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #48             // 16-byte Folded Reload
	ret
.LBB3_8:
	mov	w1, #55744
	movk	w1, #3646, lsl #16
	bl	clk_set_rate
	cbz	w0, .LBB3_10
// %bb.9:
	mov	w3, w0
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.8
	add	x1, x1, :lo12:.L.str.8
	adrp	x2, .L__func__.dfa_clk_init
	add	x2, x2, :lo12:.L__func__.dfa_clk_init
	bl	_dev_err
.LBB3_10:
	ldr	x21, [x22, #200]
	mov	x0, x21
	bl	clk_prepare
	mov	w20, w0
	cbnz	w0, .LBB3_13
// %bb.11:
	mov	x0, x21
	bl	clk_enable
	cbz	w0, .LBB3_14
// %bb.12:
	mov	w20, w0
	mov	x0, x21
	bl	clk_unprepare
.LBB3_13:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.9
	add	x1, x1, :lo12:.L.str.9
	adrp	x2, .L__func__.dfa_clk_init
	add	x2, x2, :lo12:.L__func__.dfa_clk_init
	mov	w3, w20
	bl	_dev_err
.LBB3_14:
	adrp	x1, .L.str.10
	add	x1, x1, :lo12:.L.str.10
	b	.LBB3_22
.LBB3_15:
	mov	w3, w0
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.11
	add	x1, x1, :lo12:.L.str.11
	adrp	x2, .L__func__.dfa_clk_init
	add	x2, x2, :lo12:.L__func__.dfa_clk_init
	bl	_dev_err
	b	.LBB3_5
.LBB3_16:
	mov	w20, w0
	b	.LBB3_19
.LBB3_17:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.6
	add	x1, x1, :lo12:.L.str.6
	bl	_dev_err
	b	.LBB3_2
.LBB3_18:
	mov	w20, w0
	mov	x0, x21
	bl	clk_unprepare
.LBB3_19:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.12
	add	x1, x1, :lo12:.L.str.12
	adrp	x2, .L__func__.dfa_clk_init
	add	x2, x2, :lo12:.L__func__.dfa_clk_init
	mov	w3, w20
	bl	_dev_err
	b	.LBB3_7
.LBB3_20:
	mov	x0, x20
	bl	clk_unprepare
.LBB3_21:
	adrp	x1, .L.str.7
	add	x1, x1, :lo12:.L.str.7
.LBB3_22:
	ldr	x0, [x19, #96]
	bl	_dev_err
	b	.LBB3_4
.Lfunc_end3:
	.size	dfa_clk_init, .Lfunc_end3-dfa_clk_init
                                        // -- End function
	.globl	dfa_power_off                   // -- Begin function dfa_power_off
	.p2align	2
	.type	dfa_power_off,@function
dfa_power_off:                          // @dfa_power_off
// %bb.0:
	stp	x29, x30, [sp, #-48]!           // 16-byte Folded Spill
	stp	x22, x21, [sp, #16]             // 16-byte Folded Spill
	ldr	x21, [x0, #296]
	stp	x20, x19, [sp, #32]             // 16-byte Folded Spill
	mov	x19, x0
	//APP
	dmb	oshst
	//NO_APP
	mov	w20, #4999
	ldr	x9, [x0, #7016]
	mov	x29, sp
	mov	w8, #1
	add	x9, x9, #24
	//APP
	str	w8, [x9]
	//NO_APP
.LBB4_1:                                // =>This Inner Loop Header: Depth=1
	ldr	x8, [x19, #7016]
	add	x8, x8, #28
	//APP
.Ltmp144:
	ldr	w8, [x8]
.Ltmp145:
	.section	.altinstructions,"a",@progbits
.Ltmp146:
	.word	.Ltmp144-.Ltmp146
.Ltmp148:
	.word	.Ltmp147-.Ltmp148
	.hword	1
	.byte	.Ltmp145-.Ltmp144
	.byte	.Ltmp149-.Ltmp147
	.text

	.text	1
.Ltmp147:
	ldar	w8, [x8]
.Ltmp149:
.Ltmp150:
.org (.Ltmp150-(.Ltmp149-.Ltmp147))+(.Ltmp145-.Ltmp144), 0
.Ltmp151:
.org (.Ltmp151-(.Ltmp145-.Ltmp144))+(.Ltmp149-.Ltmp147), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w8, w8
	//APP
	eor	x9, x8, x8
.Ltmp152:
	cbnz	x9, .Ltmp152
	//NO_APP
	tbnz	w8, #0, .LBB4_3
// %bb.2:                               //   in Loop: Header=BB4_1 Depth=1
	mov	w0, #4295
	bl	__const_udelay
	sub	w20, w20, #1
	cmn	w20, #1
	b.ne	.LBB4_1
	b	.LBB4_4
.LBB4_3:
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #1
	ldr	x8, [x19, #7016]
	add	x8, x8, #28
	//APP
	str	w9, [x8]
	//NO_APP
.LBB4_4:
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
	b.le	.LBB4_19
// %bb.5:
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
.LBB4_6:                                // =>This Inner Loop Header: Depth=1
	ldr	x8, [x19, #7008]
	add	x8, x8, #68
	//APP
.Ltmp153:
	ldr	w20, [x8]
.Ltmp154:
	.section	.altinstructions,"a",@progbits
.Ltmp155:
	.word	.Ltmp153-.Ltmp155
.Ltmp157:
	.word	.Ltmp156-.Ltmp157
	.hword	1
	.byte	.Ltmp154-.Ltmp153
	.byte	.Ltmp158-.Ltmp156
	.text

	.text	1
.Ltmp156:
	ldar	w20, [x8]
.Ltmp158:
.Ltmp159:
.org (.Ltmp159-(.Ltmp158-.Ltmp156))+(.Ltmp154-.Ltmp153), 0
.Ltmp160:
.org (.Ltmp160-(.Ltmp154-.Ltmp153))+(.Ltmp158-.Ltmp156), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w8, w20
	//APP
	eor	x9, x8, x8
.Ltmp161:
	cbnz	x9, .Ltmp161
	//NO_APP
	tbnz	w8, #4, .LBB4_8
// %bb.7:                               //   in Loop: Header=BB4_6 Depth=1
	mov	w0, #4295
	bl	__const_udelay
	sub	w22, w22, #1
	cmn	w22, #1
	b.ne	.LBB4_6
	b	.LBB4_18
.LBB4_8:
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
	b.le	.LBB4_18
// %bb.9:
	mov	w8, #13572
	mov	x1, xzr
	movk	w8, #49920, lsl #16
	mov	x2, xzr
	orr	x0, x8, #0x2
	mov	x3, xzr
	bl	atfd_ufs_smc
	cbnz	w0, .LBB4_20
// %bb.10:
	mov	w0, #13572
	mov	w1, #32
	movk	w0, #49920, lsl #16
	mov	w2, #1
	mov	w3, #5000
	bl	atfd_ufs_smc
	cbnz	w0, .LBB4_21
// %bb.11:
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
.Ltmp162:
	ldr	w8, [x8]
.Ltmp163:
	.section	.altinstructions,"a",@progbits
.Ltmp164:
	.word	.Ltmp162-.Ltmp164
.Ltmp166:
	.word	.Ltmp165-.Ltmp166
	.hword	1
	.byte	.Ltmp163-.Ltmp162
	.byte	.Ltmp167-.Ltmp165
	.text

	.text	1
.Ltmp165:
	ldar	w8, [x8]
.Ltmp167:
.Ltmp168:
.org (.Ltmp168-(.Ltmp167-.Ltmp165))+(.Ltmp163-.Ltmp162), 0
.Ltmp169:
.org (.Ltmp169-(.Ltmp163-.Ltmp162))+(.Ltmp167-.Ltmp165), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w8, w8
	//APP
	eor	x9, x8, x8
.Ltmp170:
	cbnz	x9, .Ltmp170
	//NO_APP
	tbnz	w8, #5, .LBB4_17
.LBB4_12:
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
	tbnz	w0, #0, .LBB4_15
// %bb.13:
	ldr	x20, [x21, #208]
	mov	x0, x20
	bl	clk_prepare
	cbnz	w0, .LBB4_24
// %bb.14:
	mov	x0, x20
	bl	clk_enable
	cbnz	w0, .LBB4_23
.LBB4_15:
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #8388608
	ldr	x8, [x21, #32]
	add	x8, x8, #2384
	//APP
	str	w9, [x8]
	//NO_APP
	ldr	x19, [x21, #208]
	mov	x0, x19
	bl	clk_disable
	mov	x0, x19
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
	mov	w20, wzr
.LBB4_16:
	mov	w0, w20
	ldp	x20, x19, [sp, #32]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #48             // 16-byte Folded Reload
	ret
.LBB4_17:
	mov	w0, #300
	bl	msleep
	b	.LBB4_12
.LBB4_18:
	adrp	x0, .L.str.13
	add	x0, x0, :lo12:.L.str.13
	adrp	x1, .L__func__.dfa_power_off
	add	x1, x1, :lo12:.L__func__.dfa_power_off
	mov	w2, w20
	bl	printk
	mov	w20, #-1
	b	.LBB4_16
.LBB4_19:
	adrp	x0, .L.str.22
	add	x0, x0, :lo12:.L.str.22
	adrp	x1, .L__func__.dfa_suspend_prepare
	add	x1, x1, :lo12:.L__func__.dfa_suspend_prepare
	bl	printk
	mov	w20, #-1
	b	.LBB4_16
.LBB4_20:
	mov	w20, w0
	adrp	x0, .L.str.14
	add	x0, x0, :lo12:.L.str.14
	b	.LBB4_22
.LBB4_21:
	mov	w20, w0
	adrp	x0, .L.str.1
	add	x0, x0, :lo12:.L.str.1
.LBB4_22:
	adrp	x1, .L__func__.dfa_power_off
	add	x1, x1, :lo12:.L__func__.dfa_power_off
	mov	w2, w20
	bl	printk
	b	.LBB4_16
.LBB4_23:
	mov	x0, x20
	bl	clk_unprepare
.LBB4_24:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.15
	add	x1, x1, :lo12:.L.str.15
	adrp	x2, .L__func__.dfa_power_off
	add	x2, x2, :lo12:.L__func__.dfa_power_off
	bl	_dev_err
	mov	w20, #-1
	b	.LBB4_16
.Lfunc_end4:
	.size	dfa_power_off, .Lfunc_end4-dfa_power_off
                                        // -- End function
	.globl	dfa_que_proc_reload             // -- Begin function dfa_que_proc_reload
	.p2align	2
	.type	dfa_que_proc_reload,@function
dfa_que_proc_reload:                    // @dfa_que_proc_reload
// %bb.0:
	stp	x29, x30, [sp, #-48]!           // 16-byte Folded Spill
	str	x21, [sp, #16]                  // 8-byte Folded Spill
	mov	w21, #4999
	stp	x20, x19, [sp, #32]             // 16-byte Folded Spill
	mov	x20, x0
	//APP
	dmb	oshst
	//NO_APP
	mov	x29, sp
	ldr	x8, [x0, #7016]
	mov	w9, #2
	add	x8, x8, #24
	//APP
	str	w9, [x8]
	//NO_APP
.LBB5_1:                                // =>This Inner Loop Header: Depth=1
	ldr	x8, [x20, #7008]
	add	x8, x8, #68
	//APP
.Ltmp171:
	ldr	w19, [x8]
.Ltmp172:
	.section	.altinstructions,"a",@progbits
.Ltmp173:
	.word	.Ltmp171-.Ltmp173
.Ltmp175:
	.word	.Ltmp174-.Ltmp175
	.hword	1
	.byte	.Ltmp172-.Ltmp171
	.byte	.Ltmp176-.Ltmp174
	.text

	.text	1
.Ltmp174:
	ldar	w19, [x8]
.Ltmp176:
.Ltmp177:
.org (.Ltmp177-(.Ltmp176-.Ltmp174))+(.Ltmp172-.Ltmp171), 0
.Ltmp178:
.org (.Ltmp178-(.Ltmp172-.Ltmp171))+(.Ltmp176-.Ltmp174), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w8, w19
	//APP
	eor	x9, x8, x8
.Ltmp179:
	cbnz	x9, .Ltmp179
	//NO_APP
	tbnz	w8, #1, .LBB5_3
// %bb.2:                               //   in Loop: Header=BB5_1 Depth=1
	mov	w0, #4295
	bl	__const_udelay
	sub	w21, w21, #1
	cmn	w21, #1
	b.ne	.LBB5_1
	b	.LBB5_4
.LBB5_3:
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #2
	ldr	x8, [x20, #7008]
	add	x8, x8, #68
	//APP
	str	w9, [x8]
	//NO_APP
.LBB5_4:
	//APP
	dmb	oshst
	//NO_APP
	mov	w0, wzr
	ldr	x8, [x20, #7016]
	cmp	w21, #0
	add	x8, x8, #24
	//APP
	str	w0, [x8]
	//NO_APP
	b.le	.LBB5_6
.LBB5_5:
	ldp	x20, x19, [sp, #32]             // 16-byte Folded Reload
	ldr	x21, [sp, #16]                  // 8-byte Folded Reload
	ldp	x29, x30, [sp], #48             // 16-byte Folded Reload
	ret
.LBB5_6:
	adrp	x0, .L.str.16
	add	x0, x0, :lo12:.L.str.16
	adrp	x1, .L__func__.dfa_que_proc_reload
	add	x1, x1, :lo12:.L__func__.dfa_que_proc_reload
	mov	w2, w19
	bl	printk
	mov	w0, #-1
	b	.LBB5_5
.Lfunc_end5:
	.size	dfa_que_proc_reload, .Lfunc_end5-dfa_que_proc_reload
                                        // -- End function
	.globl	dfa_power_on                    // -- Begin function dfa_power_on
	.p2align	2
	.type	dfa_power_on,@function
dfa_power_on:                           // @dfa_power_on
// %bb.0:
	stp	x29, x30, [sp, #-48]!           // 16-byte Folded Spill
	stp	x22, x21, [sp, #16]             // 16-byte Folded Spill
	ldr	x22, [x0, #296]
	stp	x20, x19, [sp, #32]             // 16-byte Folded Spill
	mov	x19, x0
	mov	w0, #108
	mov	w1, #1
	mov	x29, sp
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
	cbnz	w0, .LBB6_27
// %bb.1:
	ldr	x8, [x22, #48]
	add	x8, x8, #1220
	//APP
.Ltmp180:
	ldr	w8, [x8]
.Ltmp181:
	.section	.altinstructions,"a",@progbits
.Ltmp182:
	.word	.Ltmp180-.Ltmp182
.Ltmp184:
	.word	.Ltmp183-.Ltmp184
	.hword	1
	.byte	.Ltmp181-.Ltmp180
	.byte	.Ltmp185-.Ltmp183
	.text

	.text	1
.Ltmp183:
	ldar	w8, [x8]
.Ltmp185:
.Ltmp186:
.org (.Ltmp186-(.Ltmp185-.Ltmp183))+(.Ltmp181-.Ltmp180), 0
.Ltmp187:
.org (.Ltmp187-(.Ltmp181-.Ltmp180))+(.Ltmp185-.Ltmp183), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w9, w8
	tst	w8, #0x1
	//APP
	eor	x8, x9, x9
.Ltmp188:
	cbnz	x8, .Ltmp188
	//NO_APP
	mov	w9, #55744
	mov	w8, #11392
	movk	w9, #3646, lsl #16
	movk	w8, #6378, lsl #16
	ldr	x0, [x22, #200]
	csel	x1, x9, x8, eq
	bl	clk_set_rate
	cbnz	w0, .LBB6_28
// %bb.2:
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
	cbnz	w0, .LBB6_29
// %bb.3:
	mov	x0, x21
	bl	clk_enable
	cbnz	w0, .LBB6_30
// %bb.4:
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
	cbnz	w0, .LBB6_33
// %bb.5:
	ldr	x21, [x22, #208]
	mov	x0, x21
	bl	clk_prepare
	cbnz	w0, .LBB6_34
// %bb.6:
	mov	x0, x21
	bl	clk_enable
	cbnz	w0, .LBB6_35
// %bb.7:
	mov	w20, #4999
.LBB6_8:                                // =>This Inner Loop Header: Depth=1
	ldr	x8, [x22, #96]
	add	x8, x8, #1064
	//APP
.Ltmp189:
	ldr	w8, [x8]
.Ltmp190:
	.section	.altinstructions,"a",@progbits
.Ltmp191:
	.word	.Ltmp189-.Ltmp191
.Ltmp193:
	.word	.Ltmp192-.Ltmp193
	.hword	1
	.byte	.Ltmp190-.Ltmp189
	.byte	.Ltmp194-.Ltmp192
	.text

	.text	1
.Ltmp192:
	ldar	w8, [x8]
.Ltmp194:
.Ltmp195:
.org (.Ltmp195-(.Ltmp194-.Ltmp192))+(.Ltmp190-.Ltmp189), 0
.Ltmp196:
.org (.Ltmp196-(.Ltmp190-.Ltmp189))+(.Ltmp194-.Ltmp192), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w8, w8
	//APP
	eor	x9, x8, x8
.Ltmp197:
	cbnz	x9, .Ltmp197
	//NO_APP
	tbz	w8, #2, .LBB6_10
// %bb.9:                               //   in Loop: Header=BB6_8 Depth=1
	mov	w0, #4295
	bl	__const_udelay
	sub	w20, w20, #1
	cmn	w20, #1
	b.ne	.LBB6_8
	b	.LBB6_22
.LBB6_10:
	cmp	w20, #0
	b.le	.LBB6_22
// %bb.11:
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
.Ltmp198:
	ldr	w9, [x9]
.Ltmp199:
	.section	.altinstructions,"a",@progbits
.Ltmp200:
	.word	.Ltmp198-.Ltmp200
.Ltmp202:
	.word	.Ltmp201-.Ltmp202
	.hword	1
	.byte	.Ltmp199-.Ltmp198
	.byte	.Ltmp203-.Ltmp201
	.text

	.text	1
.Ltmp201:
	ldar	w9, [x9]
.Ltmp203:
.Ltmp204:
.org (.Ltmp204-(.Ltmp203-.Ltmp201))+(.Ltmp199-.Ltmp198), 0
.Ltmp205:
.org (.Ltmp205-(.Ltmp199-.Ltmp198))+(.Ltmp203-.Ltmp201), 0
	.text


	//NO_APP
	mov	w10, w9
	//APP
	dmb	oshld
	//NO_APP
	and	w9, w9, #0xfffffffe
	//APP
	eor	x10, x10, x10
.Ltmp206:
	cbnz	x10, .Ltmp206
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
.LBB6_12:                               // =>This Inner Loop Header: Depth=1
	ldr	x8, [x19, #7008]
	add	x8, x8, #104
	//APP
.Ltmp207:
	ldr	w8, [x8]
.Ltmp208:
	.section	.altinstructions,"a",@progbits
.Ltmp209:
	.word	.Ltmp207-.Ltmp209
.Ltmp211:
	.word	.Ltmp210-.Ltmp211
	.hword	1
	.byte	.Ltmp208-.Ltmp207
	.byte	.Ltmp212-.Ltmp210
	.text

	.text	1
.Ltmp210:
	ldar	w8, [x8]
.Ltmp212:
.Ltmp213:
.org (.Ltmp213-(.Ltmp212-.Ltmp210))+(.Ltmp208-.Ltmp207), 0
.Ltmp214:
.org (.Ltmp214-(.Ltmp208-.Ltmp207))+(.Ltmp212-.Ltmp210), 0
	.text


	//NO_APP
	mov	w9, w8
	//APP
	dmb	oshld
	//NO_APP
	tst	w8, #0x3
	//APP
	eor	x9, x9, x9
.Ltmp215:
	cbnz	x9, .Ltmp215
	//NO_APP
	b.eq	.LBB6_14
// %bb.13:                              //   in Loop: Header=BB6_12 Depth=1
	mov	w0, #4295
	bl	__const_udelay
	sub	w20, w20, #1
	cmn	w20, #1
	b.ne	.LBB6_12
	b	.LBB6_23
.LBB6_14:
	cmp	w20, #0
	b.le	.LBB6_23
// %bb.15:
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
.LBB6_16:                               // =>This Inner Loop Header: Depth=1
	ldr	x8, [x19, #7008]
	add	x8, x8, #68
	//APP
.Ltmp216:
	ldr	w20, [x8]
.Ltmp217:
	.section	.altinstructions,"a",@progbits
.Ltmp218:
	.word	.Ltmp216-.Ltmp218
.Ltmp220:
	.word	.Ltmp219-.Ltmp220
	.hword	1
	.byte	.Ltmp217-.Ltmp216
	.byte	.Ltmp221-.Ltmp219
	.text

	.text	1
.Ltmp219:
	ldar	w20, [x8]
.Ltmp221:
.Ltmp222:
.org (.Ltmp222-(.Ltmp221-.Ltmp219))+(.Ltmp217-.Ltmp216), 0
.Ltmp223:
.org (.Ltmp223-(.Ltmp217-.Ltmp216))+(.Ltmp221-.Ltmp219), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w8, w20
	//APP
	eor	x9, x8, x8
.Ltmp224:
	cbnz	x9, .Ltmp224
	//NO_APP
	tbnz	w8, #1, .LBB6_18
// %bb.17:                              //   in Loop: Header=BB6_16 Depth=1
	mov	w0, #4295
	bl	__const_udelay
	sub	w21, w21, #1
	cmn	w21, #1
	b.ne	.LBB6_16
	b	.LBB6_19
.LBB6_18:
	//APP
	dmb	oshst
	//NO_APP
	mov	w9, #2
	ldr	x8, [x19, #7008]
	add	x8, x8, #68
	//APP
	str	w9, [x8]
	//NO_APP
.LBB6_19:
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
	b.le	.LBB6_37
// %bb.20:
	ldr	x8, [x19, #7008]
	mov	w20, wzr
	add	x8, x8, #36
	//APP
.Ltmp225:
	ldr	w8, [x8]
.Ltmp226:
	.section	.altinstructions,"a",@progbits
.Ltmp227:
	.word	.Ltmp225-.Ltmp227
.Ltmp229:
	.word	.Ltmp228-.Ltmp229
	.hword	1
	.byte	.Ltmp226-.Ltmp225
	.byte	.Ltmp230-.Ltmp228
	.text

	.text	1
.Ltmp228:
	ldar	w8, [x8]
.Ltmp230:
.Ltmp231:
.org (.Ltmp231-(.Ltmp230-.Ltmp228))+(.Ltmp226-.Ltmp225), 0
.Ltmp232:
.org (.Ltmp232-(.Ltmp226-.Ltmp225))+(.Ltmp230-.Ltmp228), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w9, w8
	orr	w8, w8, #0x1
	//APP
	eor	x9, x9, x9
.Ltmp233:
	cbnz	x9, .Ltmp233
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x9, [x19, #7008]
	add	x9, x9, #36
	//APP
	str	w8, [x9]
	//NO_APP
.LBB6_21:
	mov	w0, w20
	ldp	x20, x19, [sp, #32]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #48             // 16-byte Folded Reload
	ret
.LBB6_22:
	adrp	x0, .L.str.18
	add	x0, x0, :lo12:.L.str.18
	b	.LBB6_24
.LBB6_23:
	adrp	x0, .L.str.19
	add	x0, x0, :lo12:.L.str.19
.LBB6_24:
	adrp	x1, .L__func__.dfa_power_on
	add	x1, x1, :lo12:.L__func__.dfa_power_on
	bl	printk
.LBB6_25:
	ldr	x19, [x22, #208]
	mov	x0, x19
	bl	clk_disable
	mov	x0, x19
	bl	clk_unprepare
.LBB6_26:
	ldr	x19, [x22, #200]
	mov	x0, x19
	bl	clk_disable
	mov	x0, x19
	bl	clk_unprepare
	mov	w20, #-1
	b	.LBB6_21
.LBB6_27:
	mov	w20, w0
	adrp	x0, .L.str.17
	add	x0, x0, :lo12:.L.str.17
	adrp	x1, .L__func__.dfa_power_on
	add	x1, x1, :lo12:.L__func__.dfa_power_on
	mov	w2, w20
	bl	printk
	b	.LBB6_21
.LBB6_28:
	mov	w20, w0
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.8
	add	x1, x1, :lo12:.L.str.8
	b	.LBB6_32
.LBB6_29:
	mov	w20, w0
	b	.LBB6_31
.LBB6_30:
	mov	w20, w0
	mov	x0, x21
	bl	clk_unprepare
.LBB6_31:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.9
	add	x1, x1, :lo12:.L.str.9
.LBB6_32:
	adrp	x2, .L__func__.dfa_power_on
	add	x2, x2, :lo12:.L__func__.dfa_power_on
	mov	w3, w20
	bl	_dev_err
	b	.LBB6_21
.LBB6_33:
	mov	w3, w0
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.11
	add	x1, x1, :lo12:.L.str.11
	adrp	x2, .L__func__.dfa_power_on
	add	x2, x2, :lo12:.L__func__.dfa_power_on
	bl	_dev_err
	b	.LBB6_26
.LBB6_34:
	mov	w20, w0
	b	.LBB6_36
.LBB6_35:
	mov	w20, w0
	mov	x0, x21
	bl	clk_unprepare
.LBB6_36:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.12
	add	x1, x1, :lo12:.L.str.12
	adrp	x2, .L__func__.dfa_power_on
	add	x2, x2, :lo12:.L__func__.dfa_power_on
	mov	w3, w20
	bl	_dev_err
	b	.LBB6_26
.LBB6_37:
	adrp	x0, .L.str.16
	add	x0, x0, :lo12:.L.str.16
	adrp	x1, .L__func__.dfa_que_proc_reload
	add	x1, x1, :lo12:.L__func__.dfa_que_proc_reload
	mov	w2, w20
	bl	printk
	b	.LBB6_25
.Lfunc_end6:
	.size	dfa_power_on, .Lfunc_end6-dfa_power_on
                                        // -- End function
	.type	.L.str,@object                  // @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"\0013%s: AO_MEM_CTRL2 Write fail, resume DFA fail, fail ret = %d\n"
	.size	.L.str, 63

	.type	.L__func__.dfa_que_proc_init,@object // @__func__.dfa_que_proc_init
.L__func__.dfa_que_proc_init:
	.asciz	"dfa_que_proc_init"
	.size	.L__func__.dfa_que_proc_init, 18

	.type	.L.str.1,@object                // @.str.1
.L.str.1:
	.asciz	"\0013%s: Wait HSDT_MEM_STATUS fail, fail ret = %d\n"
	.size	.L.str.1, 48

	.type	.L.str.2,@object                // @.str.2
.L.str.2:
	.asciz	"[DFA] HSDT0_QIC_RD_NOPENDING timeout\n"
	.size	.L.str.2, 38

	.type	.L.str.3,@object                // @.str.3
.L.str.3:
	.asciz	"[DFA] HSDT0_QIC_WR_NOPENDING timeout\n"
	.size	.L.str.3, 38

	.type	.L.str.4,@object                // @.str.4
.L.str.4:
	.asciz	"[DFA] HSDT0_QIC_BUS exit timeout\n"
	.size	.L.str.4, 34

	.type	.L.str.5,@object                // @.str.5
.L.str.5:
	.asciz	"[DFA] HSDT0_QIC_SYSPDC_PWRD_ACK timeout\n"
	.size	.L.str.5, 41

	.type	.L.str.6,@object                // @.str.6
.L.str.6:
	.asciz	"clk_ufs_dfa_sys set clk failed\n"
	.size	.L.str.6, 32

	.type	.L.str.7,@object                // @.str.7
.L.str.7:
	.asciz	"clk_ufs_dfa_sys clk_prepare_enable failed\n"
	.size	.L.str.7, 43

	.type	.L.str.8,@object                // @.str.8
.L.str.8:
	.asciz	"%s, sys clk set failed:%d\n"
	.size	.L.str.8, 27

	.type	.L__func__.dfa_clk_init,@object // @__func__.dfa_clk_init
.L__func__.dfa_clk_init:
	.asciz	"dfa_clk_init"
	.size	.L__func__.dfa_clk_init, 13

	.type	.L.str.9,@object                // @.str.9
.L.str.9:
	.asciz	"%s, sys clk prepare failed:%d\n"
	.size	.L.str.9, 31

	.type	.L.str.10,@object               // @.str.10
.L.str.10:
	.asciz	"UFS 3.1 clk set\n"
	.size	.L.str.10, 17

	.type	.L.str.11,@object               // @.str.11
.L.str.11:
	.asciz	"%s, cpu clk set failed:%d\n"
	.size	.L.str.11, 27

	.type	.L.str.12,@object               // @.str.12
.L.str.12:
	.asciz	"%s, cpu clk prepare failed:%d\n"
	.size	.L.str.12, 31

	.type	.L.str.13,@object               // @.str.13
.L.str.13:
	.asciz	"\0013%s, [DFA] wait acore int status timeout, val:0x%x\n"
	.size	.L.str.13, 53

	.type	.L__func__.dfa_power_off,@object // @__func__.dfa_power_off
.L__func__.dfa_power_off:
	.asciz	"dfa_power_off"
	.size	.L__func__.dfa_power_off, 14

	.type	.L.str.14,@object               // @.str.14
.L.str.14:
	.asciz	"\0013%s: AO_MEM_CTRL2 Write fail, set DFA&UFS DS fail, fail ret = %d\n"
	.size	.L.str.14, 67

	.type	.L.str.15,@object               // @.str.15
.L.str.15:
	.asciz	"%s try enable fail\n"
	.size	.L.str.15, 20

	.type	.L.str.16,@object               // @.str.16
.L.str.16:
	.asciz	"\0013%s, [DFA] que_proc reload timeout, val:0x%x\n"
	.size	.L.str.16, 47

	.type	.L__func__.dfa_que_proc_reload,@object // @__func__.dfa_que_proc_reload
.L__func__.dfa_que_proc_reload:
	.asciz	"dfa_que_proc_reload"
	.size	.L__func__.dfa_que_proc_reload, 20

	.type	.L.str.17,@object               // @.str.17
.L.str.17:
	.asciz	"\0013%s: AO_MEM_CTRL2 Write fail, resume UFS&DFA fail, fail ret = %d\n"
	.size	.L.str.17, 67

	.type	.L__func__.dfa_power_on,@object // @__func__.dfa_power_on
.L__func__.dfa_power_on:
	.asciz	"dfa_power_on"
	.size	.L__func__.dfa_power_on, 13

	.type	.L.str.18,@object               // @.str.18
.L.str.18:
	.asciz	"\0013%s, [DFA] wait HSDT0_QIC_BUS exit H8 timeout\n"
	.size	.L.str.18, 48

	.type	.L.str.19,@object               // @.str.19
.L.str.19:
	.asciz	"\0013%s, [DFA] QIC_PD timeout\n"
	.size	.L.str.19, 28

	.type	.L.str.20,@object               // @.str.20
.L.str.20:
	.asciz	"DFA_BOOT_MODE_IND val = 0x%x\n"
	.size	.L.str.20, 30

	.type	.L.str.21,@object               // @.str.21
.L.str.21:
	.asciz	"[DFA] DFA_QIC_PD TimeOut\n"
	.size	.L.str.21, 26

	.type	.L.str.22,@object               // @.str.22
.L.str.22:
	.asciz	"\0013%s, wait QUE_CFG_RAM_STATUS timeout\n"
	.size	.L.str.22, 39

	.type	.L__func__.dfa_suspend_prepare,@object // @__func__.dfa_suspend_prepare
.L__func__.dfa_suspend_prepare:
	.asciz	"dfa_suspend_prepare"
	.size	.L__func__.dfa_suspend_prepare, 20

	.ident	"OHOS (BiSheng Mobile STD 203.0.0.2.B001) clang version 15.0.4 (llvm-project db54e0a809bd)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
