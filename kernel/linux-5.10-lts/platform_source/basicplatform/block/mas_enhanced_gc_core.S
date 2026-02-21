	.text
	.file	"mas_enhanced_gc_core.c"
	.globl	mas_enhanced_gc_enable          // -- Begin function mas_enhanced_gc_enable
	.p2align	2
	.type	mas_enhanced_gc_enable,@function
mas_enhanced_gc_enable:                 // @mas_enhanced_gc_enable
// %bb.0:
	stp	x29, x30, [sp, #-48]!           // 16-byte Folded Spill
	str	x21, [sp, #16]                  // 8-byte Folded Spill
	adrp	x21, g_enhanced_bkops
	stp	x20, x19, [sp, #32]             // 16-byte Folded Spill
	mov	x29, sp
	ldr	x8, [x21, :lo12:g_enhanced_bkops]
	cbnz	x8, .LBB0_5
// %bb.1:
	mov	x19, x0
	adrp	x0, .L.str.1
	add	x0, x0, :lo12:.L.str.1
	mov	x1, xzr
	bl	proc_mkdir
	cbz	x0, .LBB0_6
// %bb.2:
	mov	x20, x0
	adrp	x0, .L.str.3
	add	x0, x0, :lo12:.L.str.3
	adrp	x3, mas_enhanced_gc_fops
	add	x3, x3, :lo12:mas_enhanced_gc_fops
	mov	w1, #432
	mov	x2, x20
	bl	proc_create
	adrp	x0, .L.str.4
	add	x0, x0, :lo12:.L.str.4
	adrp	x3, mas_enhanced_gc_status_fops
	add	x3, x3, :lo12:mas_enhanced_gc_status_fops
	mov	w1, #288
	mov	x2, x20
	mov	x4, x19
	bl	proc_create_data
.LBB0_3:
	str	x19, [x21, :lo12:g_enhanced_bkops]
.LBB0_4:
	ldp	x20, x19, [sp, #32]             // 16-byte Folded Reload
	mov	w0, wzr
	ldr	x21, [sp, #16]                  // 8-byte Folded Reload
	ldp	x29, x30, [sp], #48             // 16-byte Folded Reload
	ret
.LBB0_5:
	adrp	x0, .L.str
	add	x0, x0, :lo12:.L.str
	bl	printk
	b	.LBB0_4
.LBB0_6:
	adrp	x0, .L.str.2
	add	x0, x0, :lo12:.L.str.2
	adrp	x1, .L__func__.mas_enhanced_gc_proc_init
	add	x1, x1, :lo12:.L__func__.mas_enhanced_gc_proc_init
	mov	w2, #161
	bl	printk
	b	.LBB0_3
.Lfunc_end0:
	.size	mas_enhanced_gc_enable, .Lfunc_end0-mas_enhanced_gc_enable
                                        // -- End function
	.p2align	2                               // -- Begin function mas_enhanced_gc_proc_open
	.type	mas_enhanced_gc_proc_open,@function
mas_enhanced_gc_proc_open:              // @mas_enhanced_gc_proc_open
// %bb.0:
	stp	x29, x30, [sp, #-16]!           // 16-byte Folded Spill
	mov	x0, x1
	adrp	x1, mas_enhanced_gc_proc_show
	add	x1, x1, :lo12:mas_enhanced_gc_proc_show
	mov	x2, xzr
	mov	x29, sp
	bl	single_open
	ldp	x29, x30, [sp], #16             // 16-byte Folded Reload
	ret
.Lfunc_end1:
	.size	mas_enhanced_gc_proc_open, .Lfunc_end1-mas_enhanced_gc_proc_open
                                        // -- End function
	.p2align	2                               // -- Begin function mas_enhanced_gc_proc_write
	.type	mas_enhanced_gc_proc_write,@function
mas_enhanced_gc_proc_write:             // @mas_enhanced_gc_proc_write
// %bb.0:
	sub	sp, sp, #80
	adrp	x8, :got:__stack_chk_guard
	stp	x29, x30, [sp, #32]             // 16-byte Folded Spill
	str	x21, [sp, #48]                  // 8-byte Folded Spill
	add	x29, sp, #32
	stp	x20, x19, [sp, #64]             // 16-byte Folded Spill
	adrp	x20, g_enhanced_bkops
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldr	x8, [x8]
	stur	x8, [x29, #-8]
	ldr	x8, [x20, :lo12:g_enhanced_bkops]
	str	wzr, [sp, #16]
	str	xzr, [sp, #8]
	cbz	x8, .LBB2_12
// %bb.1:
	cbz	x1, .LBB2_13
// %bb.2:
	sub	x8, x2, #3
	cmp	x8, #9
	b.hs	.LBB2_13
// %bb.3:
	add	x0, sp, #8
	mov	x19, x2
	add	x21, sp, #8
	bl	__arch_copy_from_user
	cbnz	x0, .LBB2_18
// %bb.4:
	ldrb	w8, [sp, #8]
	and	w9, w8, #0xfe
	cmp	w9, #48
	b.ne	.LBB2_14
// %bb.5:
	cmp	w8, #48
	ldrb	w8, [sp, #9]
	cset	w9, ne
	adrp	x21, user_enhanced_gc
	cmp	w8, #32
	strb	w9, [x21, :lo12:user_enhanced_gc]
	b.ne	.LBB2_15
// %bb.6:
	add	x8, sp, #8
	mov	x1, xzr
	orr	x0, x8, #0x2
	mov	w2, #16
	strb	wzr, [x8, x19]
	bl	simple_strtoul
	adrp	x8, user_enhance_gc_targetsize
	ldrb	w21, [x21, :lo12:user_enhanced_gc]
	str	w0, [x8, :lo12:user_enhance_gc_targetsize]
	adrp	x0, enhanced_gc_lock
	add	x0, x0, :lo12:enhanced_gc_lock
	bl	mutex_lock
	ldr	x9, [x20, :lo12:g_enhanced_bkops]
	adrp	x2, user_enhance_gc_targetsize
	add	x2, x2, :lo12:user_enhance_gc_targetsize
	ldp	x0, x8, [x9]
	cbnz	w21, .LBB2_8
// %bb.7:
	mov	w1, wzr
	b	.LBB2_9
.LBB2_8:
	mov	w1, #1
.LBB2_9:
	blr	x8
	mov	w20, w0
	adrp	x0, enhanced_gc_lock
	add	x0, x0, :lo12:enhanced_gc_lock
	bl	mutex_unlock
	cbnz	w20, .LBB2_17
.LBB2_10:
	adrp	x8, :got:__stack_chk_guard
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldur	x9, [x29, #-8]
	ldr	x8, [x8]
	cmp	x8, x9
	b.ne	.LBB2_19
// %bb.11:
	mov	x0, x19
	ldr	x21, [sp, #48]                  // 8-byte Folded Reload
	ldp	x20, x19, [sp, #64]             // 16-byte Folded Reload
	ldp	x29, x30, [sp, #32]             // 16-byte Folded Reload
	add	sp, sp, #80
	ret
.LBB2_12:
	adrp	x0, .L.str.7
	add	x0, x0, :lo12:.L.str.7
	adrp	x1, .L__func__.mas_enhanced_gc_proc_write
	add	x1, x1, :lo12:.L__func__.mas_enhanced_gc_proc_write
	mov	w2, #69
	b	.LBB2_16
.LBB2_13:
	adrp	x0, .L.str.8
	add	x0, x0, :lo12:.L.str.8
	adrp	x1, .L__func__.mas_enhanced_gc_proc_write
	add	x1, x1, :lo12:.L__func__.mas_enhanced_gc_proc_write
	mov	w2, #73
	b	.LBB2_16
.LBB2_14:
	adrp	x0, .L.str.8
	add	x0, x0, :lo12:.L.str.8
	adrp	x1, .L__func__.mas_enhanced_gc_proc_write
	add	x1, x1, :lo12:.L__func__.mas_enhanced_gc_proc_write
	mov	w2, #81
	b	.LBB2_16
.LBB2_15:
	adrp	x0, .L.str.8
	add	x0, x0, :lo12:.L.str.8
	adrp	x1, .L__func__.mas_enhanced_gc_proc_write
	add	x1, x1, :lo12:.L__func__.mas_enhanced_gc_proc_write
	mov	w2, #86
.LBB2_16:
	bl	printk
	mov	x19, #-22
	b	.LBB2_10
.LBB2_17:
	adrp	x0, .L.str.9
	add	x0, x0, :lo12:.L.str.9
	adrp	x1, .L__func__.mas_enhanced_gc_proc_write
	add	x1, x1, :lo12:.L__func__.mas_enhanced_gc_proc_write
	mov	w2, w20
	bl	printk
	b	.LBB2_10
.LBB2_18:
	sub	x8, x19, x0
	mov	x2, x0
	add	x0, x21, x8
	mov	w1, wzr
	bl	memset
	adrp	x0, .L.str.8
	add	x0, x0, :lo12:.L.str.8
	adrp	x1, .L__func__.mas_enhanced_gc_proc_write
	add	x1, x1, :lo12:.L__func__.mas_enhanced_gc_proc_write
	mov	w2, #77
	bl	printk
	mov	x19, #-14
	b	.LBB2_10
.LBB2_19:
	bl	__stack_chk_fail
.Lfunc_end2:
	.size	mas_enhanced_gc_proc_write, .Lfunc_end2-mas_enhanced_gc_proc_write
                                        // -- End function
	.p2align	2                               // -- Begin function mas_enhanced_gc_proc_show
	.type	mas_enhanced_gc_proc_show,@function
mas_enhanced_gc_proc_show:              // @mas_enhanced_gc_proc_show
// %bb.0:
	stp	x29, x30, [sp, #-32]!           // 16-byte Folded Spill
	adrp	x8, user_enhanced_gc
	adrp	x1, .L.str.5
	add	x1, x1, :lo12:.L.str.5
	str	x19, [sp, #16]                  // 8-byte Folded Spill
	mov	x29, sp
	mov	x19, x0
	ldrb	w2, [x8, :lo12:user_enhanced_gc]
	bl	seq_printf
	adrp	x8, user_enhance_gc_targetsize
	adrp	x1, .L.str.6
	add	x1, x1, :lo12:.L.str.6
	mov	x0, x19
	ldr	w2, [x8, :lo12:user_enhance_gc_targetsize]
	bl	seq_printf
	ldr	x19, [sp, #16]                  // 8-byte Folded Reload
	mov	w0, wzr
	ldp	x29, x30, [sp], #32             // 16-byte Folded Reload
	ret
.Lfunc_end3:
	.size	mas_enhanced_gc_proc_show, .Lfunc_end3-mas_enhanced_gc_proc_show
                                        // -- End function
	.p2align	2                               // -- Begin function mas_enhanced_gc_status_proc_open
	.type	mas_enhanced_gc_status_proc_open,@function
mas_enhanced_gc_status_proc_open:       // @mas_enhanced_gc_status_proc_open
// %bb.0:
	stp	x29, x30, [sp, #-32]!           // 16-byte Folded Spill
	str	x19, [sp, #16]                  // 8-byte Folded Spill
	mov	x29, sp
	mov	x19, x1
	bl	PDE_DATA
	mov	x2, x0
	adrp	x1, mas_enhanced_gc_status_proc_show
	add	x1, x1, :lo12:mas_enhanced_gc_status_proc_show
	mov	x0, x19
	bl	single_open
	ldr	x19, [sp, #16]                  // 8-byte Folded Reload
	ldp	x29, x30, [sp], #32             // 16-byte Folded Reload
	ret
.Lfunc_end4:
	.size	mas_enhanced_gc_status_proc_open, .Lfunc_end4-mas_enhanced_gc_status_proc_open
                                        // -- End function
	.p2align	2                               // -- Begin function mas_enhanced_gc_status_proc_show
	.type	mas_enhanced_gc_status_proc_show,@function
mas_enhanced_gc_status_proc_show:       // @mas_enhanced_gc_status_proc_show
// %bb.0:
	sub	sp, sp, #80
	adrp	x8, :got:__stack_chk_guard
	stp	x29, x30, [sp, #32]             // 16-byte Folded Spill
	str	x21, [sp, #48]                  // 8-byte Folded Spill
	add	x29, sp, #32
	stp	x20, x19, [sp, #64]             // 16-byte Folded Spill
	adrp	x21, enhanced_gc_lock
	add	x21, x21, :lo12:enhanced_gc_lock
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	mov	x19, x0
	ldr	x8, [x8]
	stur	x8, [x29, #-8]
	ldr	x20, [x0, #112]
	mov	x0, x21
	stur	wzr, [x29, #-12]
	stp	wzr, wzr, [sp, #12]
	bl	mutex_lock
	ldr	x8, [x20, #16]
	sub	x1, x29, #12
	ldr	x0, [x20]
	add	x2, sp, #16
	add	x3, sp, #12
	blr	x8
	mov	w20, w0
	mov	x0, x21
	bl	mutex_unlock
	cbnz	w20, .LBB5_4
// %bb.1:
	ldur	w2, [x29, #-12]
	adrp	x1, .L.str.13
	add	x1, x1, :lo12:.L.str.13
	mov	x0, x19
	bl	seq_printf
	ldr	w2, [sp, #16]
	adrp	x1, .L.str.14
	add	x1, x1, :lo12:.L.str.14
	mov	x0, x19
	bl	seq_printf
	ldr	w2, [sp, #12]
	adrp	x1, .L.str.15
	add	x1, x1, :lo12:.L.str.15
	mov	x0, x19
	bl	seq_printf
.LBB5_2:
	adrp	x8, :got:__stack_chk_guard
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldur	x9, [x29, #-8]
	ldr	x8, [x8]
	cmp	x8, x9
	b.ne	.LBB5_5
// %bb.3:
	mov	w0, w20
	ldr	x21, [sp, #48]                  // 8-byte Folded Reload
	ldp	x20, x19, [sp, #64]             // 16-byte Folded Reload
	ldp	x29, x30, [sp, #32]             // 16-byte Folded Reload
	add	sp, sp, #80
	ret
.LBB5_4:
	adrp	x0, .L.str.12
	add	x0, x0, :lo12:.L.str.12
	adrp	x1, .L__func__.mas_enhanced_gc_status_proc_show
	add	x1, x1, :lo12:.L__func__.mas_enhanced_gc_status_proc_show
	mov	w2, w20
	bl	printk
	b	.LBB5_2
.LBB5_5:
	bl	__stack_chk_fail
.Lfunc_end5:
	.size	mas_enhanced_gc_status_proc_show, .Lfunc_end5-mas_enhanced_gc_status_proc_show
                                        // -- End function
	.type	g_enhanced_bkops,@object        // @g_enhanced_bkops
	.local	g_enhanced_bkops
	.comm	g_enhanced_bkops,8,8
	.type	.L.str,@object                  // @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"\0013[BKOPS]mas_enhanced_gc already init!\n"
	.size	.L.str, 40

	.type	.L.str.1,@object                // @.str.1
.L.str.1:
	.asciz	"block"
	.size	.L.str.1, 6

	.type	.L.str.2,@object                // @.str.2
.L.str.2:
	.asciz	"\0013[BKOPS]%s %d: proc mk error.\n"
	.size	.L.str.2, 32

	.type	.L__func__.mas_enhanced_gc_proc_init,@object // @__func__.mas_enhanced_gc_proc_init
.L__func__.mas_enhanced_gc_proc_init:
	.asciz	"mas_enhanced_gc_proc_init"
	.size	.L__func__.mas_enhanced_gc_proc_init, 26

	.type	.L.str.3,@object                // @.str.3
.L.str.3:
	.asciz	"enhanced_gc"
	.size	.L.str.3, 12

	.type	mas_enhanced_gc_fops,@object    // @mas_enhanced_gc_fops
	.section	.data.rel.ro,"aw",@progbits
	.p2align	3
mas_enhanced_gc_fops:
	.word	0                               // 0x0
	.zero	4
	.xword	mas_enhanced_gc_proc_open
	.xword	seq_read
	.xword	0
	.xword	mas_enhanced_gc_proc_write
	.xword	seq_lseek
	.xword	single_release
	.xword	0
	.xword	0
	.xword	0
	.xword	0
	.xword	0
	.size	mas_enhanced_gc_fops, 96

	.type	.L.str.4,@object                // @.str.4
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str.4:
	.asciz	"enhanced_status"
	.size	.L.str.4, 16

	.type	mas_enhanced_gc_status_fops,@object // @mas_enhanced_gc_status_fops
	.section	.data.rel.ro,"aw",@progbits
	.p2align	3
mas_enhanced_gc_status_fops:
	.word	0                               // 0x0
	.zero	4
	.xword	mas_enhanced_gc_status_proc_open
	.xword	seq_read
	.xword	0
	.xword	0
	.xword	seq_lseek
	.xword	single_release
	.xword	0
	.xword	0
	.xword	0
	.xword	0
	.xword	0
	.size	mas_enhanced_gc_status_fops, 96

	.type	.L.str.5,@object                // @.str.5
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str.5:
	.asciz	"user_enhanced_gc:%d\n"
	.size	.L.str.5, 21

	.type	user_enhanced_gc,@object        // @user_enhanced_gc
	.local	user_enhanced_gc
	.comm	user_enhanced_gc,1,4
	.type	.L.str.6,@object                // @.str.6
.L.str.6:
	.asciz	"user_enhance_gc_targetsize:0x%xMB\n"
	.size	.L.str.6, 35

	.type	user_enhance_gc_targetsize,@object // @user_enhance_gc_targetsize
	.local	user_enhance_gc_targetsize
	.comm	user_enhance_gc_targetsize,4,4
	.type	.L.str.7,@object                // @.str.7
.L.str.7:
	.asciz	"\0013[BKOPS]%s %d: g_enhanced_bkops is null.\n"
	.size	.L.str.7, 43

	.type	.L__func__.mas_enhanced_gc_proc_write,@object // @__func__.mas_enhanced_gc_proc_write
.L__func__.mas_enhanced_gc_proc_write:
	.asciz	"mas_enhanced_gc_proc_write"
	.size	.L__func__.mas_enhanced_gc_proc_write, 27

	.type	.L.str.8,@object                // @.str.8
.L.str.8:
	.asciz	"\0013[BKOPS]%s %d: input error.\n"
	.size	.L.str.8, 30

	.type	enhanced_gc_lock,@object        // @enhanced_gc_lock
	.data
	.p2align	3
enhanced_gc_lock:
	.zero	8
	.zero	4
	.zero	4
	.xword	enhanced_gc_lock+16
	.xword	enhanced_gc_lock+16
	.size	enhanced_gc_lock, 32

	.type	.L.str.9,@object                // @.str.9
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str.9:
	.asciz	"\0013[BKOPS]%s: bkops_enhanced_gc_start_stop failed, ret = %d!\n"
	.size	.L.str.9, 61

	.type	.L.str.12,@object               // @.str.12
.L.str.12:
	.asciz	"\0013[BKOPS]%s: mas_enhanced_gc_query failed, ret = %d!\n"
	.size	.L.str.12, 54

	.type	.L__func__.mas_enhanced_gc_status_proc_show,@object // @__func__.mas_enhanced_gc_status_proc_show
.L__func__.mas_enhanced_gc_status_proc_show:
	.asciz	"mas_enhanced_gc_status_proc_show"
	.size	.L__func__.mas_enhanced_gc_status_proc_show, 33

	.type	.L.str.13,@object               // @.str.13
.L.str.13:
	.asciz	"status:%u\n"
	.size	.L.str.13, 11

	.type	.L.str.14,@object               // @.str.14
.L.str.14:
	.asciz	"device_target:0x%xMB\n"
	.size	.L.str.14, 22

	.type	.L.str.15,@object               // @.str.15
.L.str.15:
	.asciz	"now_free_cnt:0x%xMB\n"
	.size	.L.str.15, 21

	.ident	"OHOS (BiSheng Mobile CPU STD 202.1.0.B030) clang version 15.0.4 (llvm-project 7123208073d2)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym mas_enhanced_gc_proc_open
	.addrsig_sym seq_read
	.addrsig_sym mas_enhanced_gc_proc_write
	.addrsig_sym seq_lseek
	.addrsig_sym single_release
	.addrsig_sym mas_enhanced_gc_proc_show
	.addrsig_sym mas_enhanced_gc_status_proc_open
	.addrsig_sym mas_enhanced_gc_status_proc_show
	.addrsig_sym mas_enhanced_gc_fops
	.addrsig_sym mas_enhanced_gc_status_fops
	.addrsig_sym user_enhance_gc_targetsize
	.addrsig_sym enhanced_gc_lock
