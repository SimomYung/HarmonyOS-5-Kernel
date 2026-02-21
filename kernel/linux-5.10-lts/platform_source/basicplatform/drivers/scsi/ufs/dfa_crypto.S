	.text
	.file	"dfa_crypto.c"
	.globl	dfa_resume_inline_crypt_key     // -- Begin function dfa_resume_inline_crypt_key
	.p2align	2
	.type	dfa_resume_inline_crypt_key,@function
dfa_resume_inline_crypt_key:            // @dfa_resume_inline_crypt_key
// %bb.0:
	sub	sp, sp, #48
	adrp	x8, :got:__stack_chk_guard
	stp	x29, x30, [sp, #16]             // 16-byte Folded Spill
	stp	x20, x19, [sp, #32]             // 16-byte Folded Spill
	mov	x19, x0
	adrp	x1, .L.str
	add	x1, x1, :lo12:.L.str
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	adrp	x2, .L__func__.dfa_resume_inline_crypt_key
	add	x2, x2, :lo12:.L__func__.dfa_resume_inline_crypt_key
	add	x29, sp, #16
	ldr	x8, [x8]
	str	x8, [sp, #8]
	ldr	x0, [x0, #96]
	str	wzr, [sp, #4]
	bl	_dev_err
	mov	x0, x19
	bl	ufshcd_support_inline_encrypt
	mov	w20, wzr
	tbz	w0, #0, .LBB0_7
// %bb.1:
	add	x0, sp, #4
	bl	fbe_ca_msp_available
	cbnz	w0, .LBB0_5
// %bb.2:
	ldr	w8, [sp, #4]
	mov	w9, #42405
	movk	w9, #23130, lsl #16
	cmp	w8, w9
	b.ne	.LBB0_9
// %bb.3:
	mov	w0, #2
	bl	fbex_gen_sk
	mov	w20, w0
	cbz	w0, .LBB0_7
// %bb.4:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.2
	add	x1, x1, :lo12:.L.str.2
	b	.LBB0_6
.LBB0_5:
	mov	w20, w0
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.1
	add	x1, x1, :lo12:.L.str.1
.LBB0_6:
	adrp	x2, .L__func__.dfa_resume_inline_crypt_key
	add	x2, x2, :lo12:.L__func__.dfa_resume_inline_crypt_key
	mov	w3, w20
	bl	_dev_err
.LBB0_7:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.3
	add	x1, x1, :lo12:.L.str.3
	adrp	x2, .L__func__.dfa_resume_inline_crypt_key
	add	x2, x2, :lo12:.L__func__.dfa_resume_inline_crypt_key
	bl	_dev_err
	adrp	x8, :got:__stack_chk_guard
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldr	x9, [sp, #8]
	ldr	x8, [x8]
	cmp	x8, x9
	b.ne	.LBB0_10
// %bb.8:
	mov	w0, w20
	ldp	x20, x19, [sp, #32]             // 16-byte Folded Reload
	ldp	x29, x30, [sp, #16]             // 16-byte Folded Reload
	add	sp, sp, #48
	ret
.LBB0_9:
	mov	w20, wzr
	b	.LBB0_7
.LBB0_10:
	bl	__stack_chk_fail
.Lfunc_end0:
	.size	dfa_resume_inline_crypt_key, .Lfunc_end0-dfa_resume_inline_crypt_key
                                        // -- End function
	.globl	dfa_uie_init                    // -- Begin function dfa_uie_init
	.p2align	2
	.type	dfa_uie_init,@function
dfa_uie_init:                           // @dfa_uie_init
// %bb.0:
	sub	sp, sp, #64
	adrp	x8, :got:__stack_chk_guard
	stp	x29, x30, [sp, #16]             // 16-byte Folded Spill
	stp	x22, x21, [sp, #32]             // 16-byte Folded Spill
	adrp	x9, .L.str.6
	add	x9, x9, :lo12:.L.str.6
	stp	x20, x19, [sp, #48]             // 16-byte Folded Spill
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	adrp	x10, .L.str.5
	add	x10, x10, :lo12:.L.str.5
	tst	w1, #0x1
	csel	x19, x10, x9, ne
	mov	w22, w1
	ldr	x8, [x8]
	mov	x20, x0
	adrp	x1, .L.str.4
	add	x1, x1, :lo12:.L.str.4
	adrp	x2, .L__func__.dfa_uie_init
	add	x2, x2, :lo12:.L__func__.dfa_uie_init
	str	x8, [sp, #8]
	mov	x3, x19
	ldr	x0, [x0, #96]
	add	x29, sp, #16
	bl	_dev_err
	mov	x0, x20
	bl	ufshcd_support_inline_encrypt
	tbz	w0, #0, .LBB1_21
// %bb.1:
	mov	w21, #16959
	movk	w21, #15, lsl #16
.LBB1_2:                                // =>This Inner Loop Header: Depth=1
	ldr	x8, [x20]
	add	x8, x8, #52
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
	//APP
	dmb	oshld
	//NO_APP
	mov	w9, w8
	//APP
	eor	x10, x9, x9
.Ltmp8:
	cbnz	x10, .Ltmp8
	//NO_APP
	tbnz	w9, #0, .LBB1_5
// %bb.3:                               //   in Loop: Header=BB1_2 Depth=1
	mov	w0, #42950
	bl	__const_udelay
	sub	w21, w21, #1
	cmn	w21, #1
	b.ne	.LBB1_2
.LBB1_4:
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.9
	add	x1, x1, :lo12:.L.str.9
	bl	_dev_err
	mov	w21, #-1
	b	.LBB1_22
.LBB1_5:
	cmp	w21, #0
	b.le	.LBB1_4
// %bb.6:
	//APP
	dmb	oshst
	//NO_APP
	orr	w8, w8, #0x2
	ldr	x9, [x20]
	add	x9, x9, #52
	//APP
	str	w8, [x9]
	//NO_APP
	bl	fbex_enable_kdf
	cbnz	w0, .LBB1_13
// %bb.7:
	add	x0, sp, #4
	str	wzr, [sp, #4]
	bl	fbe_ca_msp_available
	cbnz	w0, .LBB1_15
// %bb.8:
	ldr	w8, [sp, #4]
	mov	w9, #42405
	movk	w9, #23130, lsl #16
	cmp	w8, w9
	tbz	w22, #0, .LBB1_17
// %bb.9:
	b.ne	.LBB1_11
// %bb.10:
	mov	w0, #2
	bl	fbex_gen_sk
	cbnz	w0, .LBB1_25
.LBB1_11:
	bl	fbex_restore_key
	cbnz	w0, .LBB1_24
// %bb.12:
	adrp	x8, :got:gufs_sk_state
	mov	w9, #1
	ldr	x8, [x8, :got_lo12:gufs_sk_state]
	str	w9, [x8]
	b	.LBB1_19
.LBB1_13:
	mov	w21, w0
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.7
	add	x1, x1, :lo12:.L.str.7
	adrp	x2, .L__func__.dfa_uie_init
	add	x2, x2, :lo12:.L__func__.dfa_uie_init
.LBB1_14:
	mov	w3, w21
	bl	_dev_err
	b	.LBB1_22
.LBB1_15:
	mov	w21, w0
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.10
	add	x1, x1, :lo12:.L.str.10
.LBB1_16:
	adrp	x2, .L__func__.dfa_gen_and_restore_key
	add	x2, x2, :lo12:.L__func__.dfa_gen_and_restore_key
	b	.LBB1_14
.LBB1_17:
	b.ne	.LBB1_19
// %bb.18:
	adrp	x21, :got:fbx_gensk_worker
	adrp	x10, :got:ufshcd_fbex_gen_sk
	mov	x8, #68719476704
	adrp	x1, :got:delayed_work_timer_fn
	mov	w2, #2097152
	mov	x3, xzr
	ldr	x21, [x21, :got_lo12:fbx_gensk_worker]
	mov	x4, xzr
	ldr	x10, [x10, :got_lo12:ufshcd_fbex_gen_sk]
	add	x9, x21, #8
	str	x8, [x21]
	add	x0, x21, #32
	str	x9, [x21, #8]
	stp	x9, x10, [x21, #16]
	ldr	x1, [x1, :got_lo12:delayed_work_timer_fn]
	bl	init_timer_key
	adrp	x8, :got:system_wq
	mov	w0, #12
	mov	x2, x21
	mov	x3, xzr
	ldr	x8, [x8, :got_lo12:system_wq]
	ldr	x1, [x8]
	bl	queue_delayed_work_on
.LBB1_19:
	ldr	x8, [x20, #88]
	mov	w9, #1
	mov	x0, x20
	str	w9, [x8, #2312]
	bl	ufshcd_support_inline_encrypt
	tbz	w0, #0, .LBB1_21
// %bb.20:
	mov	x0, x20
	mov	w1, #262144
	bl	ufshcd_enable_intr
.LBB1_21:
	mov	w21, wzr
.LBB1_22:
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.8
	add	x1, x1, :lo12:.L.str.8
	adrp	x2, .L__func__.dfa_uie_init
	add	x2, x2, :lo12:.L__func__.dfa_uie_init
	mov	x3, x19
	bl	_dev_err
	adrp	x8, :got:__stack_chk_guard
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldr	x9, [sp, #8]
	ldr	x8, [x8]
	cmp	x8, x9
	b.ne	.LBB1_26
// %bb.23:
	mov	w0, w21
	ldp	x20, x19, [sp, #48]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #32]             // 16-byte Folded Reload
	ldp	x29, x30, [sp, #16]             // 16-byte Folded Reload
	add	sp, sp, #64
	ret
.LBB1_24:
	mov	w21, w0
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.12
	add	x1, x1, :lo12:.L.str.12
	b	.LBB1_16
.LBB1_25:
	mov	w21, w0
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.11
	add	x1, x1, :lo12:.L.str.11
	b	.LBB1_16
.LBB1_26:
	bl	__stack_chk_fail
.Lfunc_end1:
	.size	dfa_uie_init, .Lfunc_end1-dfa_uie_init
                                        // -- End function
	.type	.L.str,@object                  // @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"%s ++\n"
	.size	.L.str, 7

	.type	.L__func__.dfa_resume_inline_crypt_key,@object // @__func__.dfa_resume_inline_crypt_key
.L__func__.dfa_resume_inline_crypt_key:
	.asciz	"dfa_resume_inline_crypt_key"
	.size	.L__func__.dfa_resume_inline_crypt_key, 28

	.type	.L.str.1,@object                // @.str.1
.L.str.1:
	.asciz	"%s get msp avalilable state failed %d\n"
	.size	.L.str.1, 39

	.type	.L.str.2,@object                // @.str.2
.L.str.2:
	.asciz	"%s gen sk failed %d\n"
	.size	.L.str.2, 21

	.type	.L.str.3,@object                // @.str.3
.L.str.3:
	.asciz	"%s --\n"
	.size	.L.str.3, 7

	.type	.L.str.4,@object                // @.str.4
.L.str.4:
	.asciz	"%s ++, %s in error handler!\n"
	.size	.L.str.4, 29

	.type	.L__func__.dfa_uie_init,@object // @__func__.dfa_uie_init
.L__func__.dfa_uie_init:
	.asciz	"dfa_uie_init"
	.size	.L__func__.dfa_uie_init, 13

	.type	.L.str.5,@object                // @.str.5
.L.str.5:
	.zero	1
	.size	.L.str.5, 1

	.type	.L.str.6,@object                // @.str.6
.L.str.6:
	.asciz	"not "
	.size	.L.str.6, 5

	.type	.L.str.7,@object                // @.str.7
.L.str.7:
	.asciz	"%s: enable kdf failed! %d\n"
	.size	.L.str.7, 27

	.type	.L.str.8,@object                // @.str.8
.L.str.8:
	.asciz	"%s --, %sin error handler!\n"
	.size	.L.str.8, 28

	.type	.L.str.9,@object                // @.str.9
.L.str.9:
	.asciz	"[DFA] HCE config error\n"
	.size	.L.str.9, 24

	.type	.L.str.10,@object               // @.str.10
.L.str.10:
	.asciz	"%s: mspc_flag get failed, err = %d.\n"
	.size	.L.str.10, 37

	.type	.L__func__.dfa_gen_and_restore_key,@object // @__func__.dfa_gen_and_restore_key
.L__func__.dfa_gen_and_restore_key:
	.asciz	"dfa_gen_and_restore_key"
	.size	.L__func__.dfa_gen_and_restore_key, 24

	.type	.L.str.11,@object               // @.str.11
.L.str.11:
	.asciz	"%s: km sk restore return err = %d\n"
	.size	.L.str.11, 35

	.type	.L.str.12,@object               // @.str.12
.L.str.12:
	.asciz	"%s: restore key err = %d\n"
	.size	.L.str.12, 26

	.ident	"OHOS (BiSheng Mobile CPU STD 202.1.0.B030) clang version 15.0.4 (llvm-project 7123208073d2)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym ufshcd_fbex_gen_sk
	.addrsig_sym delayed_work_timer_fn
	.addrsig_sym fbx_gensk_worker
