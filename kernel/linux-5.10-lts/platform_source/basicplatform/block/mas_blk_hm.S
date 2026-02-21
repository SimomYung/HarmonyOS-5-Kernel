	.text
	.file	"mas_blk_hm.c"
	.globl	mas_blk_hm_support_cp_notify    // -- Begin function mas_blk_hm_support_cp_notify
	.p2align	2
	.type	mas_blk_hm_support_cp_notify,@function
mas_blk_hm_support_cp_notify:           // @mas_blk_hm_support_cp_notify
// %bb.0:
	stp	x29, x30, [sp, #-16]!           // 16-byte Folded Spill
	adrp	x0, .L.str.2
	add	x0, x0, :lo12:.L.str.2
	mov	w1, #2
	mov	x29, sp
	bl	printk
	adrp	x8, :got:__tracepoint_ldk_vh_sd_proxy_block_event_cmd
	ldr	x8, [x8, :got_lo12:__tracepoint_ldk_vh_sd_proxy_block_event_cmd]
	ldr	w9, [x8, #8]
	cmp	w9, #1
	b.lt	.LBB0_3
// %bb.1:
	ldr	x8, [x8, #56]
	cbz	x8, .LBB0_3
// %bb.2:
	ldr	x0, [x8, #8]
	mov	w1, #2
	bl	__traceiter_ldk_vh_sd_proxy_block_event_cmd
.LBB0_3:
	adrp	x0, .L.str
	add	x0, x0, :lo12:.L.str
	bl	printk
	ldp	x29, x30, [sp], #16             // 16-byte Folded Reload
	ret
.Lfunc_end0:
	.size	mas_blk_hm_support_cp_notify, .Lfunc_end0-mas_blk_hm_support_cp_notify
                                        // -- End function
	.globl	mas_blk_hm_support_bkops_notify // -- Begin function mas_blk_hm_support_bkops_notify
	.p2align	2
	.type	mas_blk_hm_support_bkops_notify,@function
mas_blk_hm_support_bkops_notify:        // @mas_blk_hm_support_bkops_notify
// %bb.0:
	stp	x29, x30, [sp, #-16]!           // 16-byte Folded Spill
	adrp	x0, .L.str.2
	add	x0, x0, :lo12:.L.str.2
	mov	w1, #3
	mov	x29, sp
	bl	printk
	adrp	x8, :got:__tracepoint_ldk_vh_sd_proxy_block_event_cmd
	ldr	x8, [x8, :got_lo12:__tracepoint_ldk_vh_sd_proxy_block_event_cmd]
	ldr	w9, [x8, #8]
	cmp	w9, #1
	b.lt	.LBB1_3
// %bb.1:
	ldr	x8, [x8, #56]
	cbz	x8, .LBB1_3
// %bb.2:
	ldr	x0, [x8, #8]
	mov	w1, #3
	bl	__traceiter_ldk_vh_sd_proxy_block_event_cmd
.LBB1_3:
	adrp	x0, .L.str.1
	add	x0, x0, :lo12:.L.str.1
	bl	printk
	ldp	x29, x30, [sp], #16             // 16-byte Folded Reload
	ret
.Lfunc_end1:
	.size	mas_blk_hm_support_bkops_notify, .Lfunc_end1-mas_blk_hm_support_bkops_notify
                                        // -- End function
	.type	.L.str,@object                  // @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"\0013[BLK-IO]mas_blk_hm_support_cp_notify\n"
	.size	.L.str, 40

	.type	.L.str.1,@object                // @.str.1
.L.str.1:
	.asciz	"\0013[BLK-IO]mas_blk_hm_support_bkops_notify\n"
	.size	.L.str.1, 43

	.type	.L.str.2,@object                // @.str.2
.L.str.2:
	.asciz	"\0013[BLK-IO]mas_blk_hm_notify %d\n"
	.size	.L.str.2, 32

	.ident	"OHOS (BiSheng Mobile CPU STD 202.1.0.B030) clang version 15.0.4 (llvm-project 7123208073d2)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym __tracepoint_ldk_vh_sd_proxy_block_event_cmd
