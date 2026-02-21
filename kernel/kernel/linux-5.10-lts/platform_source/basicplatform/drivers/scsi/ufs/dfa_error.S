	.text
	.file	"dfa_error.c"
	.globl	dfa_eh_timed_out                // -- Begin function dfa_eh_timed_out
	.p2align	2
	.type	dfa_eh_timed_out,@function
dfa_eh_timed_out:                       // @dfa_eh_timed_out
// %bb.0:
	stp	x29, x30, [sp, #-80]!           // 16-byte Folded Spill
	ldr	x9, [x0, #56]
	stp	x24, x23, [sp, #32]             // 16-byte Folded Spill
	mov	w8, #9360
	stp	x22, x21, [sp, #48]             // 16-byte Folded Spill
	stp	x20, x19, [sp, #64]             // 16-byte Folded Spill
	mov	x20, x0
	ldr	x23, [x9]
	mov	x2, xzr
	str	x25, [sp, #16]                  // 8-byte Folded Spill
	mov	x29, sp
	add	x21, x23, x8
	ldrsw	x1, [x23, #2584]
	mov	x0, x21
	add	x19, x23, #2320
	bl	find_next_bit
	ldr	w8, [x23, #2584]
	mov	x22, x0
	cmp	w8, w22
	b.le	.LBB0_4
// %bb.1:
	mov	w24, #216
	mov	x25, #4294967296
.LBB0_2:                                // =>This Inner Loop Header: Depth=1
	ldr	x9, [x23, #2544]
	nop
	smaddl	x9, w22, w24, x9
	ldr	x9, [x9, #72]
	cmp	x9, x20
	b.eq	.LBB0_6
// %bb.3:                               //   in Loop: Header=BB0_2 Depth=1
	lsl	x9, x22, #32
	sxtw	x1, w8
	add	x9, x9, x25
	mov	x0, x21
	asr	x2, x9, #32
	bl	find_next_bit
	ldr	w8, [x23, #2584]
	mov	x22, x0
	cmp	w8, w22
	b.gt	.LBB0_2
.LBB0_4:
	mov	x0, x19
	bl	dfa_dump_lrbp_status
	ldr	x8, [x20, #224]
	adrp	x1, .L.str.1
	add	x1, x1, :lo12:.L.str.1
	ldr	x9, [x20, #272]
	ldr	x0, [x23, #2416]
	ldrb	w2, [x8]
	ldr	w3, [x9, #32]
	bl	_dev_err
	mov	w0, #1
.LBB0_5:
	ldp	x20, x19, [sp, #64]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #48]             // 16-byte Folded Reload
	ldp	x24, x23, [sp, #32]             // 16-byte Folded Reload
	ldr	x25, [sp, #16]                  // 8-byte Folded Reload
	ldp	x29, x30, [sp], #80             // 16-byte Folded Reload
	ret
.LBB0_6:
	ldr	x8, [x20, #272]
	ldr	x20, [x23, #2416]
	ldr	w21, [x8, #32]
	bl	ktime_get
	mov	x4, x0
	adrp	x1, .L.str
	add	x1, x1, :lo12:.L.str
	mov	x0, x20
	mov	w2, w22
	mov	w3, w21
	bl	_dev_err
	mov	x0, x19
	bl	dfa_dump_lrbp_status
	mov	w2, #18271
	mov	x0, x19
	mov	w1, w22
	movk	w2, #14160, lsl #16
	bl	dsm_ufs_update_upiu_info
	mov	x0, x19
	bl	schedule_ufs_dsm_work
	mov	w0, wzr
	b	.LBB0_5
.Lfunc_end0:
	.size	dfa_eh_timed_out, .Lfunc_end0-dfa_eh_timed_out
                                        // -- End function
	.globl	dfa_try_to_abort_task           // -- Begin function dfa_try_to_abort_task
	.p2align	2
	.type	dfa_try_to_abort_task,@function
dfa_try_to_abort_task:                  // @dfa_try_to_abort_task
// %bb.0:
	sub	sp, sp, #112
	adrp	x8, :got:__stack_chk_guard
	stp	x29, x30, [sp, #16]             // 16-byte Folded Spill
	str	x27, [sp, #32]                  // 8-byte Folded Spill
	add	w9, w1, #63
	stp	x26, x25, [sp, #48]             // 16-byte Folded Spill
	cmp	w1, #0
	stp	x24, x23, [sp, #64]             // 16-byte Folded Spill
	mov	w10, #216
	stp	x22, x21, [sp, #80]             // 16-byte Folded Spill
	csel	w9, w9, w1, lt
	stp	x20, x19, [sp, #96]             // 16-byte Folded Spill
	mov	w19, w1
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	asr	w9, w9, #6
	mov	w12, #7040
	mov	x20, x0
	add	x9, x0, w9, sxtw #3
	mov	w11, #15
	ldr	x8, [x8]
	add	x24, x9, x12
	mov	w26, #-10
	adrp	x21, .L.str.3
	add	x21, x21, :lo12:.L.str.3
	add	x29, sp, #16
	str	x8, [sp, #8]
	ldr	x8, [x0, #224]
	strb	w11, [sp, #4]
	nop
	smaddl	x8, w1, w10, x8
	mov	w10, #1
	lsl	x25, x10, x19
	add	x23, x8, #108
.LBB1_1:                                // =>This Inner Loop Header: Depth=1
	ldrb	w1, [x23]
	add	x4, sp, #4
	mov	x0, x20
	mov	w2, w19
	mov	w3, #128
	bl	dfa_exec_tm_cmd
	mov	w22, w0
	ldrb	w27, [sp, #4]
	cbnz	w0, .LBB1_3
// %bb.2:                               //   in Loop: Header=BB1_1 Depth=1
	cmp	w27, #8
	b.eq	.LBB1_9
.LBB1_3:                                //   in Loop: Header=BB1_1 Depth=1
	orr	w8, w22, w27
	ldr	x0, [x20, #96]
	cbnz	w8, .LBB1_12
// %bb.4:                               //   in Loop: Header=BB1_1 Depth=1
	mov	x1, x21
	mov	w2, w19
	bl	_dev_err
	ldr	x8, [x24]
	tst	x8, x25
	b.eq	.LBB1_13
// %bb.5:                               //   in Loop: Header=BB1_1 Depth=1
	mov	w0, #100
	mov	w1, #200
	bl	usleep_range
	adds	w26, w26, #1
	b.lo	.LBB1_1
// %bb.6:
	mov	w0, #-16
.LBB1_7:
	adrp	x8, :got:__stack_chk_guard
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldr	x9, [sp, #8]
	ldr	x8, [x8]
	cmp	x8, x9
	b.ne	.LBB1_14
// %bb.8:
	ldp	x20, x19, [sp, #96]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #80]             // 16-byte Folded Reload
	ldp	x24, x23, [sp, #64]             // 16-byte Folded Reload
	ldp	x26, x25, [sp, #48]             // 16-byte Folded Reload
	ldp	x29, x30, [sp, #16]             // 16-byte Folded Reload
	ldr	x27, [sp, #32]                  // 8-byte Folded Reload
	add	sp, sp, #112
	ret
.LBB1_9:
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.2
	add	x1, x1, :lo12:.L.str.2
	mov	w2, w19
	bl	_dev_err
	ldrb	w1, [x23]
	add	x4, sp, #4
	mov	x0, x20
	mov	w2, w19
	mov	w3, #1
	bl	dfa_exec_tm_cmd
	cbnz	w0, .LBB1_7
// %bb.10:
	ldrb	w21, [sp, #4]
	cbz	w21, .LBB1_7
// %bb.11:
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.6
	add	x1, x1, :lo12:.L.str.6
	mov	w2, w19
	mov	w3, w21
	bl	_dev_err
	mov	w0, w21
	b	.LBB1_7
.LBB1_12:
	adrp	x1, .L.str.5
	add	x1, x1, :lo12:.L.str.5
	mov	w2, w19
	mov	w3, w22
	bl	_dev_err
	cmp	w22, #0
	csel	w0, w27, w22, eq
	b	.LBB1_7
.LBB1_13:
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.4
	add	x1, x1, :lo12:.L.str.4
	mov	w2, w19
	mov	w3, wzr
	bl	_dev_err
	mov	w0, wzr
	b	.LBB1_7
.LBB1_14:
	bl	__stack_chk_fail
.Lfunc_end1:
	.size	dfa_try_to_abort_task, .Lfunc_end1-dfa_try_to_abort_task
                                        // -- End function
	.p2align	2                               // -- Begin function dfa_exec_tm_cmd
	.type	dfa_exec_tm_cmd,@function
dfa_exec_tm_cmd:                        // @dfa_exec_tm_cmd
// %bb.0:
	sub	sp, sp, #192
	adrp	x8, :got:__stack_chk_guard
	stp	x29, x30, [sp, #96]             // 16-byte Folded Spill
	str	x27, [sp, #112]                 // 8-byte Folded Spill
	add	x29, sp, #96
	stp	x26, x25, [sp, #128]            // 16-byte Folded Spill
	stp	x24, x23, [sp, #144]            // 16-byte Folded Spill
	mov	w23, w1
	stp	x22, x21, [sp, #160]            // 16-byte Folded Spill
	mov	w22, w2
	stp	x20, x19, [sp, #176]            // 16-byte Folded Spill
	and	w24, w3, #0xff
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	mov	x21, x4
	mov	x19, x0
	adrp	x1, .L.str.38
	add	x1, x1, :lo12:.L.str.38
	mov	w2, w23
	ldr	x8, [x8]
	mov	w3, w22
	mov	w4, w24
	stur	x8, [x29, #-8]
	ldr	x0, [x0, #96]
	stur	wzr, [x29, #-12]
	stp	xzr, xzr, [sp, #64]
	stp	xzr, xzr, [sp, #48]
	bl	_dev_err
	add	x20, x19, #3888
	mov	x0, x20
	bl	mutex_lock
	sub	x1, x29, #12
	mov	x0, x19
	bl	dfa_get_dev_cmd_tag
	tbz	w0, #0, .LBB2_10
.LBB2_1:
	add	x8, sp, #48
	adrp	x1, .L.str.41
	add	x1, x1, :lo12:.L.str.41
	add	x0, x8, #8
	adrp	x2, __init_completion.__key
	add	x2, x2, :lo12:__init_completion.__key
	str	wzr, [sp, #48]
	bl	__init_swait_queue_head
	ldr	x8, [x19, #224]
	mov	w10, #216
	ldursw	x9, [x29, #-12]
	nop
	madd	x25, x9, x10, x8
	mov	x27, x25
	ldr	x8, [x27, #72]!
	cbnz	x8, .LBB2_16
.LBB2_2:
	mov	w8, #7368
	add	x0, x19, x8
	bl	__raw_spin_lock_irqsave
	ldur	w1, [x29, #-12]
	adrp	x2, .L__func__.dfa_exec_tm_cmd
	add	x2, x2, :lo12:.L__func__.dfa_exec_tm_cmd
	mov	x0, x19
	bl	dfa_lrbp_configure
	cbnz	w0, .LBB2_6
// %bb.3:
	ldur	w8, [x29, #-12]
	mov	w9, #256
	mov	x0, x19
	mov	x1, x25
	strb	wzr, [sp, #8]
	stp	xzr, xzr, [x27]
	str	xzr, [x27, #16]
	str	w8, [x25, #104]
	strb	wzr, [x25, #177]
	strh	w9, [x25, #108]
	bl	ufshcd_update_dev_cmd_lun
	mov	w8, #1
	add	x1, sp, #8
	mov	x0, x25
	mov	w2, #3
	str	w8, [x25, #100]
	bl	ufshcd_prepare_req_desc_hdr
	ldr	x26, [x25, #16]
	mov	w1, #32
	mov	w2, wzr
	mov	w3, #32
	mov	x0, x26
	bl	memset_s
	lsl	w8, w23, #8
	lsl	w9, w24, #8
	orr	w8, w8, #0x4000000
	rev	w10, w23
	rev	w8, w8
	rev	w11, w22
	mov	w1, #52
	mov	w2, wzr
	mov	w3, #52
	stp	w8, w9, [x26]
	stp	w10, w11, [x26, #12]
	ldr	x0, [x25, #24]
	bl	memset_s
	add	x8, sp, #48
	mov	x0, x19
	str	x8, [x19, #3920]
	//APP
	dsb	st
	//NO_APP
	ldur	w1, [x29, #-12]
	bl	dfa_send_command
	mov	x0, x19
	bl	dfa_irq_sche_unlock
	ldr	x0, [x19, #3920]
	mov	w1, #10
	bl	wait_for_completion_timeout
	mov	x22, x0
	ldur	w1, [x29, #-12]
	mov	x0, x19
	mov	x2, x25
	bl	dfa_reset_dev_cmd_complete
	cbz	x22, .LBB2_17
// %bb.4:
	ldrb	w26, [x25, #195]
	cbnz	w26, .LBB2_15
// %bb.5:
	ldr	x8, [x25, #24]
	ldrb	w8, [x8, #15]
	strb	w8, [x21]
	b	.LBB2_7
.LBB2_6:
	mov	w26, w0
	mov	x0, x19
	bl	dfa_irq_sche_unlock
.LBB2_7:
	add	x0, x19, #3928
	mov	w1, #3
	mov	w2, #1
	mov	x3, xzr
	bl	__wake_up
	mov	x0, x20
	bl	mutex_unlock
	mov	x0, x19
	bl	dfa_put_dev_cmd_tag
	cmn	w26, #110
	b.eq	.LBB2_13
.LBB2_8:
	adrp	x8, :got:__stack_chk_guard
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldur	x9, [x29, #-8]
	ldr	x8, [x8]
	cmp	x8, x9
	b.ne	.LBB2_18
// %bb.9:
	mov	w0, w26
	ldr	x27, [sp, #112]                 // 8-byte Folded Reload
	ldp	x20, x19, [sp, #176]            // 16-byte Folded Reload
	ldp	x22, x21, [sp, #160]            // 16-byte Folded Reload
	ldp	x24, x23, [sp, #144]            // 16-byte Folded Reload
	ldp	x26, x25, [sp, #128]            // 16-byte Folded Reload
	ldp	x29, x30, [sp, #96]             // 16-byte Folded Reload
	add	sp, sp, #192
	ret
.LBB2_10:
	add	x0, sp, #8
	mov	w1, wzr
	stp	xzr, xzr, [sp, #32]
	stp	xzr, xzr, [sp, #16]
	str	xzr, [sp, #8]
	bl	init_wait_entry
	add	x25, x19, #3928
.LBB2_11:                               // =>This Inner Loop Header: Depth=1
	add	x1, sp, #8
	mov	x0, x25
	mov	w2, #2
	bl	prepare_to_wait_event
	sub	x1, x29, #12
	mov	x0, x19
	bl	dfa_get_dev_cmd_tag
	tbnz	w0, #0, .LBB2_14
// %bb.12:                              //   in Loop: Header=BB2_11 Depth=1
	bl	schedule
	b	.LBB2_11
.LBB2_13:
	mov	x0, x19
	bl	dfa_reset_directly
	b	.LBB2_8
.LBB2_14:
	add	x1, sp, #8
	mov	x0, x25
	bl	finish_wait
	b	.LBB2_1
.LBB2_15:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.39
	add	x1, x1, :lo12:.L.str.39
	adrp	x2, .L__func__.dfa_exec_tm_cmd
	add	x2, x2, :lo12:.L__func__.dfa_exec_tm_cmd
	mov	w3, w26
	bl	_dev_err
	b	.LBB2_7
.LBB2_16:
	adrp	x0, .L.str.33
	add	x0, x0, :lo12:.L.str.33
	mov	w1, #133
	mov	w2, #9
	mov	x3, xzr
	bl	warn_slowpath_fmt
	b	.LBB2_2
.LBB2_17:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.40
	add	x1, x1, :lo12:.L.str.40
	adrp	x2, .L__func__.dfa_exec_tm_cmd
	add	x2, x2, :lo12:.L__func__.dfa_exec_tm_cmd
	bl	_dev_err
	mov	x0, x19
	bl	dfa_dump_lrbp_status
	ldr	x8, [x19, #376]
	mov	w26, #-110
	add	x8, x8, #1
	str	x8, [x19, #376]
	b	.LBB2_7
.LBB2_18:
	bl	__stack_chk_fail
.Lfunc_end2:
	.size	dfa_exec_tm_cmd, .Lfunc_end2-dfa_exec_tm_cmd
                                        // -- End function
	.globl	dfa_print_host_state            // -- Begin function dfa_print_host_state
	.p2align	2
	.type	dfa_print_host_state,@function
dfa_print_host_state:                   // @dfa_print_host_state
// %bb.0:
	stp	x29, x30, [sp, #-48]!           // 16-byte Folded Spill
	stp	x20, x19, [sp, #32]             // 16-byte Folded Spill
	mov	x19, x0
	mov	w8, #4120
	stp	x22, x21, [sp, #16]             // 16-byte Folded Spill
	add	x21, x0, x8
	ldr	x20, [x0, #112]
	ldr	x0, [x0, #96]
	adrp	x1, .L.str.7
	add	x1, x1, :lo12:.L.str.7
	ldr	w2, [x19, #1240]
	mov	x29, sp
	bl	_dev_err
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.8
	add	x1, x1, :lo12:.L.str.8
	ldr	w2, [x19, #1584]
	ldr	w3, [x19, #1588]
	bl	_dev_err
	ldp	w2, w3, [x19, #128]
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.9
	add	x1, x1, :lo12:.L.str.9
	bl	_dev_err
	mov	w8, #5040
	ldr	x0, [x19, #96]
	ldr	w2, [x19, #208]
	adrp	x1, .L.str.10
	add	x1, x1, :lo12:.L.str.10
	ldrb	w3, [x19, x8]
	bl	_dev_err
	ldp	x8, x0, [x19, #88]
	adrp	x1, .L.str.11
	add	x1, x1, :lo12:.L.str.11
	ldrb	w2, [x19, #4072]
	ldrh	w8, [x8, #592]
	and	w3, w8, #0x1
	bl	_dev_err
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.12
	add	x1, x1, :lo12:.L.str.12
	ldr	w2, [x19, #4328]
	bl	_dev_err
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.13
	add	x1, x1, :lo12:.L.str.13
	ldr	w2, [x19, #1248]
	ldr	w3, [x19, #4140]
	bl	_dev_err
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.14
	add	x1, x1, :lo12:.L.str.14
	ldr	w2, [x19, #272]
	ldr	w3, [x19, #256]
	ldr	w4, [x19, #4548]
	bl	_dev_err
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.15
	add	x1, x1, :lo12:.L.str.15
	ldr	w2, [x19, #708]
	ldr	w3, [x19, #712]
	bl	_dev_err
	cbz	x20, .LBB3_2
// %bb.1:
	ldp	x2, x3, [x20, #200]
	adrp	x1, .L.str.16
	add	x1, x1, :lo12:.L.str.16
	ldr	x0, [x19, #96]
	ldr	x4, [x20, #216]
	bl	_dev_err
.LBB3_2:
	adrp	x20, .L.str.17
	add	x20, x20, :lo12:.L.str.17
	ldr	x0, [x19, #96]
	mov	x1, x20
	ldr	x3, [x19, #7040]
	mov	w2, wzr
	bl	_dev_err
	ldr	x0, [x19, #96]
	mov	x1, x20
	ldr	x3, [x19, #7048]
	mov	w2, #1
	bl	_dev_err
	ldr	x0, [x19, #96]
	mov	x1, x20
	ldr	x3, [x19, #7056]
	mov	w2, #2
	bl	_dev_err
	ldr	x0, [x19, #96]
	mov	x1, x20
	ldr	x3, [x19, #7064]
	mov	w2, #3
	bl	_dev_err
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.18
	add	x1, x1, :lo12:.L.str.18
	ldr	w2, [x19, #7072]
	bl	_dev_err
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.19
	add	x1, x1, :lo12:.L.str.19
	ldr	w2, [x19, #7224]
	bl	_dev_err
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.20
	add	x1, x1, :lo12:.L.str.20
	ldr	w2, [x19, #7228]
	bl	_dev_err
	ldr	x22, [x19, #4120]
	cmp	x22, x21
	b.eq	.LBB3_10
// %bb.3:
	adrp	x20, .L.str.21
	add	x20, x20, :lo12:.L.str.21
	b	.LBB3_5
.LBB3_4:                                //   in Loop: Header=BB3_5 Depth=1
	ldr	x22, [x22]
	cmp	x22, x21
	b.eq	.LBB3_10
.LBB3_5:                                // =>This Inner Loop Header: Depth=1
	ldr	x8, [x22, #16]
	cbz	x8, .LBB3_4
// %bb.6:                               //   in Loop: Header=BB3_5 Depth=1
	cmn	x8, #1, lsl #12                 // =4096
	b.hi	.LBB3_4
// %bb.7:                               //   in Loop: Header=BB3_5 Depth=1
	ldr	w8, [x22, #36]
	cbz	w8, .LBB3_4
// %bb.8:                               //   in Loop: Header=BB3_5 Depth=1
	ldr	w8, [x22, #32]
	cbz	w8, .LBB3_4
// %bb.9:                               //   in Loop: Header=BB3_5 Depth=1
	ldr	x0, [x19, #96]
	mov	x1, x20
	ldr	x2, [x22, #24]
	ldr	w3, [x22, #40]
	bl	_dev_err
	b	.LBB3_4
.LBB3_10:
	ldp	x20, x19, [sp, #32]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #48             // 16-byte Folded Reload
	ret
.Lfunc_end3:
	.size	dfa_print_host_state, .Lfunc_end3-dfa_print_host_state
                                        // -- End function
	.globl	dfa_abort                       // -- Begin function dfa_abort
	.p2align	2
	.type	dfa_abort,@function
dfa_abort:                              // @dfa_abort
// %bb.0:
	sub	sp, sp, #112
	ldr	x8, [x0, #56]
	stp	x20, x19, [sp, #96]             // 16-byte Folded Spill
	stp	x22, x21, [sp, #80]             // 16-byte Folded Spill
	mov	x21, x0
	mov	x2, x21
	stp	x29, x30, [sp, #16]             // 16-byte Folded Spill
	ldr	x19, [x8]
	stp	x28, x27, [sp, #32]             // 16-byte Folded Spill
	stp	x26, x25, [sp, #48]             // 16-byte Folded Spill
	add	x29, sp, #16
	stp	x24, x23, [sp, #64]             // 16-byte Folded Spill
	add	x20, x19, #2320
	mov	x0, x19
	mov	x1, x20
	bl	ufshcd_get_tag_from_cmd
                                        // kill: def $w0 killed $w0 def $x0
	add	w8, w0, #63
	cmp	w0, #0
	csel	w8, w8, w0, lt
	mov	w22, w0
	asr	w8, w8, #6
	add	x9, x20, w8, sxtw #3
	ldr	x8, [x19, #2544]
	ldr	x9, [x9, #7040]
	lsr	x9, x9, x0
	tbnz	w9, #0, .LBB4_2
// %bb.1:
	ubfx	x8, x22, #6, #26
	ldr	x0, [x19, #2416]
	adrp	x1, .L.str.22
	add	x1, x1, :lo12:.L.str.22
	add	x8, x20, w8, uxtw #3
	mov	w2, w22
	ldr	x3, [x8, #7040]
	bl	_dev_err
	b	.LBB4_6
.LBB4_2:
	mov	w9, #216
	umaddl	x23, w0, w9, x8
	ldr	x0, [x19, #2416]
	ldrb	w8, [x23, #108]
	cmp	w8, #208
	b.ne	.LBB4_4
// %bb.3:
	adrp	x1, .L.str.23
	add	x1, x1, :lo12:.L.str.23
	bl	_dev_err
	b	.LBB4_5
.LBB4_4:
	adrp	x1, .L.str.24
	add	x1, x1, :lo12:.L.str.24
	mov	w2, w22
	bl	_dev_err
	mov	w8, #9400
	ldrb	w8, [x19, x8]
	sub	w8, w8, #1
	cmp	w8, #1
	b.hi	.LBB4_8
.LBB4_5:
	mov	x0, x20
	bl	dfa_reset_directly
.LBB4_6:
	mov	w0, #8194
.LBB4_7:
	ldp	x20, x19, [sp, #96]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #80]             // 16-byte Folded Reload
	ldp	x24, x23, [sp, #64]             // 16-byte Folded Reload
	ldp	x26, x25, [sp, #48]             // 16-byte Folded Reload
	ldp	x28, x27, [sp, #32]             // 16-byte Folded Reload
	ldp	x29, x30, [sp, #16]             // 16-byte Folded Reload
	add	sp, sp, #112
	ret
.LBB4_8:
	ldr	x8, [x19, #2544]
	mov	w27, #216
	madd	x8, x22, x27, x8
	ldr	x0, [x8, #72]
	bl	scsi_print_command
	ldr	w8, [x19, #6460]
	cbnz	w8, .LBB4_10
// %bb.9:
	mov	x0, x20
	bl	dfa_print_host_state
	mov	x0, x20
	bl	dfa_dump_lrbp_status
	ldr	w8, [x20, #4140]
.LBB4_10:
	add	w8, w8, #1
	mov	x2, x21
	str	w8, [x20, #4140]
	ldr	x8, [x21, #56]
	ldr	x24, [x8]
	add	x26, x24, #2320
	mov	x0, x24
	mov	x1, x26
	bl	ufshcd_get_tag_from_cmd
	ldr	x25, [x24, #2544]
                                        // kill: def $w0 killed $w0 def $x0
	sxtw	x9, w0
	smaddl	x28, w0, w27, x25
	ldrb	w8, [x28, #176]!
	cbnz	w8, .LBB4_17
// %bb.11:
	ldr	x27, [x24, #9344]
	adrp	x1, .L.str.45
	add	x1, x1, :lo12:.L.str.45
	ldr	x0, [x24, #2416]
	mov	w3, w9
	str	x9, [sp, #8]                    // 8-byte Folded Spill
	ldr	w2, [x27, #36]
	bl	_dev_err
	mov	x0, x27
	mov	w1, #1
	bl	dfa_mcq_stop_run_sq
	mov	x0, x26
	bl	dfa_dump_lrbp_status
	ldp	w9, w14, [x27, #60]
	ldr	w8, [x27, #32]
	udiv	w10, w14, w8
	msub	w10, w10, w8, w14
	cmp	w10, w9
	b.eq	.LBB4_22
// %bb.12:
	ldr	x12, [sp, #8]                   // 8-byte Folded Reload
	mov	w11, #216
	mov	w10, wzr
	madd	x11, x12, x11, x25
	ldr	x12, [x27]
	add	x12, x12, x14, lsl #6
	add	w14, w14, #1
	ldr	x11, [x11, #184]
	ldr	w13, [x11, #16]
	b	.LBB4_14
.LBB4_13:                               //   in Loop: Header=BB4_14 Depth=1
	add	w15, w14, w10
	add	w10, w10, #1
	udiv	w16, w15, w8
	msub	w15, w16, w8, w15
	cmp	w15, w9
	b.eq	.LBB4_22
.LBB4_14:                               // =>This Inner Loop Header: Depth=1
	mov	w15, w10
	add	x15, x12, x15, lsl #6
	ldr	w16, [x15, #16]
	cmp	w16, w13
	b.ne	.LBB4_13
// %bb.15:                              //   in Loop: Header=BB4_14 Depth=1
	ldr	w16, [x15, #20]
	ldr	w17, [x11, #20]
	cmp	w16, w17
	b.ne	.LBB4_13
// %bb.16:
	ldr	w8, [x15]
	mov	x0, x27
	mov	w1, wzr
	orr	w8, w8, #0xf0000000
	str	w8, [x15]
	bl	dfa_mcq_stop_run_sq
	ldr	x0, [x24, #2416]
	adrp	x1, .L.str.43
	add	x1, x1, :lo12:.L.str.43
	ldr	w2, [x27, #36]
	ldr	x3, [sp, #8]                    // 8-byte Folded Reload
                                        // kill: def $w3 killed $w3 killed $x3
	bl	_dev_err
	b	.LBB4_18
.LBB4_17:
	ldr	x0, [x24, #2416]
	adrp	x1, .L.str.42
	add	x1, x1, :lo12:.L.str.42
	mov	w2, w9
	bl	_dev_err
.LBB4_18:
	mov	w25, #-14
.LBB4_19:
	mov	x0, x20
	bl	dfa_put_dev_cmd_tag
	ldr	x0, [x19, #2416]
	adrp	x1, .L.str.25
	add	x1, x1, :lo12:.L.str.25
	mov	w2, w25
	bl	_dev_err
	mov	w8, #9360
	ldrsw	x1, [x19, #2584]
	add	x20, x19, x8
	mov	x2, xzr
	mov	x0, x20
	bl	find_next_bit
	ldr	w9, [x19, #2584]
	mov	x8, x0
	cmp	w9, w8
	b.le	.LBB4_24
// %bb.20:
	mov	w21, #216
	mov	w22, #1
.LBB4_21:                               // =>This Inner Loop Header: Depth=1
	ldr	x9, [x19, #2544]
	mov	x0, x20
	smaddl	x9, w8, w21, x9
	add	w8, w8, #1
	sxtw	x2, w8
	strb	w22, [x9, #176]
	ldrsw	x1, [x19, #2584]
	bl	find_next_bit
	ldr	w9, [x19, #2584]
	mov	x8, x0
	mov	w0, #8195
	cmp	w9, w8
	b.gt	.LBB4_21
	b	.LBB4_7
.LBB4_22:
	mov	x0, x27
	mov	w1, wzr
	bl	dfa_mcq_stop_run_sq
	mov	x0, x26
	ldr	x1, [sp, #8]                    // 8-byte Folded Reload
                                        // kill: def $w1 killed $w1 killed $x1
	bl	dfa_try_to_abort_task
	cbnz	w0, .LBB4_25
// %bb.23:
	mov	x0, x21
	bl	scsi_dma_unmap
	ldr	x8, [x19, #2544]
	mov	w9, #216
	madd	x8, x22, x9, x8
	str	xzr, [x8, #72]
	bl	ktime_get
	mov	x2, x0
	mov	x0, x20
	mov	x1, x23
	bl	ufstt_unprep_handler
	mov	w8, #6248
	mov	w1, #3
	add	x0, x19, x8
	mov	w2, #1
	mov	x3, xzr
	bl	__wake_up
	mov	x0, x20
	bl	dfa_put_dev_cmd_tag
	b	.LBB4_6
.LBB4_24:
	mov	w0, #8195
	b	.LBB4_7
.LBB4_25:
	mov	w25, w0
	ldr	x0, [x24, #2416]
	adrp	x1, .L.str.44
	add	x1, x1, :lo12:.L.str.44
	mov	w2, w25
	bl	_dev_err
	mov	w8, #1
	strb	w8, [x28]
	b	.LBB4_19
.Lfunc_end4:
	.size	dfa_abort, .Lfunc_end4-dfa_abort
                                        // -- End function
	.globl	dfa_reset_directly              // -- Begin function dfa_reset_directly
	.p2align	2
	.type	dfa_reset_directly,@function
dfa_reset_directly:                     // @dfa_reset_directly
// %bb.0:
	stp	x29, x30, [sp, #-80]!           // 16-byte Folded Spill
	str	x25, [sp, #16]                  // 8-byte Folded Spill
	mov	x29, sp
	stp	x24, x23, [sp, #32]             // 16-byte Folded Spill
	stp	x22, x21, [sp, #48]             // 16-byte Folded Spill
	stp	x20, x19, [sp, #64]             // 16-byte Folded Spill
	mov	x19, x0
	bl	dfa_prepare_reset
	cbz	w0, .LBB5_2
.LBB5_1:
	ldp	x20, x19, [sp, #64]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #48]             // 16-byte Folded Reload
	ldp	x24, x23, [sp, #32]             // 16-byte Folded Reload
	ldr	x25, [sp, #16]                  // 8-byte Folded Reload
	ldp	x29, x30, [sp], #80             // 16-byte Folded Reload
	ret
.LBB5_2:
	mov	w9, #7232
	mov	w8, #7364
	add	x20, x19, x9
	add	x21, x19, x8
	mov	x0, x20
	bl	mutex_lock
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.55
	add	x1, x1, :lo12:.L.str.55
	bl	_dev_err
	mov	w1, #18254
	mov	x0, x19
	movk	w1, #14160, lsl #16
	bl	dsm_ufs_update_host_reset_info
	cbnz	w0, .LBB5_4
// %bb.3:
	mov	x0, x19
	bl	schedule_ufs_dsm_work
.LBB5_4:
	mov	x0, x19
	bl	dfa_early_reset
	cbnz	w0, .LBB5_7
// %bb.5:
	mov	x0, x19
	bl	dfa_que_proc_init
	cbnz	w0, .LBB5_7
// %bb.6:
	mov	x0, x19
	bl	dfa_dhq_slot_clear
.LBB5_7:
	mov	x0, x19
	bl	dfa_dump_lrbp_status
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.56
	add	x1, x1, :lo12:.L.str.56
	bl	_dev_err
	//APP
	// atomic_add
	prfm	pstl1strm, [x21]
.Ltmp0:
	ldxr	w8, [x21]
	add	w8, w8, #1
	stxr	w9, w8, [x21]
	cbnz	w9, .Ltmp0

	//NO_APP
	mov	x0, x19
	mov	w1, #1
	str	wzr, [x19, #7224]
	bl	dfa_uie_init
	cbnz	w0, .LBB5_16
// %bb.8:
	ldr	x21, [x19, #88]
	mov	x1, xzr
	mov	w8, #6193
	add	x25, x19, x8
	mov	x0, x21
	bl	__scsi_iterate_devices
	cbz	x0, .LBB5_13
// %bb.9:
	mov	x22, x0
	adrp	x23, .L.str.57
	add	x23, x23, :lo12:.L.str.57
	adrp	x24, .L__func__.dfa_send_request_sense
	add	x24, x24, :lo12:.L__func__.dfa_send_request_sense
	b	.LBB5_11
.LBB5_10:                               //   in Loop: Header=BB5_11 Depth=1
	mov	x0, x21
	mov	x1, x22
	bl	__scsi_iterate_devices
	mov	x22, x0
	cbz	x0, .LBB5_13
.LBB5_11:                               // =>This Inner Loop Header: Depth=1
	mov	x0, x19
	mov	x1, x22
	mov	w2, #1500
	mov	w3, wzr
	bl	ufshcd_send_scsi_request_sense
	cbz	w0, .LBB5_10
// %bb.12:                              //   in Loop: Header=BB5_11 Depth=1
	mov	w3, w0
	ldr	x0, [x19, #96]
	mov	x1, x23
	mov	x2, x24
	bl	_dev_err
	b	.LBB5_10
.LBB5_13:
	mov	x0, x19
	str	wzr, [x19, #7228]
	strb	wzr, [x25]
	bl	hufs_wb_config
	ldr	x8, [x19, #88]
	adrp	x10, .L.str.29
	add	x10, x10, :lo12:.L.str.29
	ldr	x9, [x8, #152]
	ldr	x9, [x9, #8]
	cmp	x9, x10
	b.eq	.LBB5_15
.LBB5_14:
	mov	x0, x19
	strb	wzr, [x8, #496]
	bl	dfa_set_timestamp_attr
	mov	x0, x20
	bl	mutex_unlock
	b	.LBB5_1
.LBB5_15:
	mov	w0, wzr
	bl	sd_proxy_block_cmd
	ldr	x8, [x19, #88]
	b	.LBB5_14
.LBB5_16:
	adrp	x0, .L.str.32
	add	x0, x0, :lo12:.L.str.32
	adrp	x1, .L.str.33
	add	x1, x1, :lo12:.L.str.33
	adrp	x3, .L__func__.dfa_reset_directly
	add	x3, x3, :lo12:.L__func__.dfa_reset_directly
	mov	w2, #688
	bl	printk
	adrp	x0, .L.str.34
	add	x0, x0, :lo12:.L.str.34
	bl	panic
.Lfunc_end5:
	.size	dfa_reset_directly, .Lfunc_end5-dfa_reset_directly
                                        // -- End function
	.globl	dfa_dump_fw_log                 // -- Begin function dfa_dump_fw_log
	.p2align	2
	.type	dfa_dump_fw_log,@function
dfa_dump_fw_log:                        // @dfa_dump_fw_log
// %bb.0:
	stp	x29, x30, [sp, #-32]!           // 16-byte Folded Spill
	ldr	x8, [x0, #104]
	str	x19, [sp, #16]                  // 8-byte Folded Spill
	mov	x29, sp
	cbz	x8, .LBB6_2
// %bb.1:
	ldr	x8, [x0, #112]
	cbnz	x8, .LBB6_3
.LBB6_2:
	ldr	x19, [sp, #16]                  // 8-byte Folded Reload
	ldp	x29, x30, [sp], #32             // 16-byte Folded Reload
	ret
.LBB6_3:
	add	x8, x0, #120
	//APP
	// atomic64_add_return
	prfm	pstl1strm, [x8]
.Ltmp1:
	ldxr	x9, [x8]
	add	x9, x9, #1
	stlxr	w10, x9, [x8]
	cbnz	w10, .Ltmp1
	dmb	ish
	//NO_APP
	ldp	x2, x8, [x0, #104]
	and	x9, x9, #0x3
	mov	w10, #17408
	mov	x19, x0
	mov	w1, #17408
	mov	w3, #17408
	madd	x0, x9, x10, x8
	bl	memcpy_s
	cbnz	w0, .LBB6_5
// %bb.4:
	ldr	x8, [x19]
	adrp	x1, .L.str.46
	add	x1, x1, :lo12:.L.str.46
	ldr	x0, [x8, #96]
	bl	_dev_err
	ldr	x5, [x19, #104]
	adrp	x0, .L.str.47
	add	x0, x0, :lo12:.L.str.47
	adrp	x1, .L.str.48
	add	x1, x1, :lo12:.L.str.48
	mov	w2, #2
	mov	w3, #32
	mov	w4, #4
	mov	w6, #17408
	mov	w7, #1
	bl	print_hex_dump
	ldr	x8, [x19]
	adrp	x1, .L.str.49
	add	x1, x1, :lo12:.L.str.49
	ldr	x0, [x8, #96]
	bl	_dev_err
	b	.LBB6_2
.LBB6_5:
	adrp	x0, .L.str.26
	add	x0, x0, :lo12:.L.str.26
	bl	printk
	b	.LBB6_2
.Lfunc_end6:
	.size	dfa_dump_fw_log, .Lfunc_end6-dfa_dump_fw_log
                                        // -- End function
	.globl	dfa_prepare_reset               // -- Begin function dfa_prepare_reset
	.p2align	2
	.type	dfa_prepare_reset,@function
dfa_prepare_reset:                      // @dfa_prepare_reset
// %bb.0:
	stp	x29, x30, [sp, #-80]!           // 16-byte Folded Spill
	str	x28, [sp, #16]                  // 8-byte Folded Spill
	mov	x29, sp
	stp	x24, x23, [sp, #32]             // 16-byte Folded Spill
	stp	x22, x21, [sp, #48]             // 16-byte Folded Spill
	stp	x20, x19, [sp, #64]             // 16-byte Folded Spill
	sub	sp, sp, #448
	adrp	x8, :got:__stack_chk_guard
	mov	w9, #6764
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldr	x8, [x8]
	stur	x8, [x29, #-8]
	ldrb	w8, [x0, x9]
	cbnz	w8, .LBB7_5
// %bb.1:
	mov	x19, x0
	ldr	x0, [x0, #96]
	adrp	x1, .L.str.27
	add	x1, x1, :lo12:.L.str.27
.LBB7_2:
	bl	_dev_err
	mov	w0, #1
.LBB7_3:
	adrp	x8, :got:__stack_chk_guard
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldur	x9, [x29, #-8]
	ldr	x8, [x8]
	cmp	x8, x9
	b.ne	.LBB7_51
// %bb.4:
	add	sp, sp, #448
	ldp	x20, x19, [sp, #64]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #48]             // 16-byte Folded Reload
	ldp	x24, x23, [sp, #32]             // 16-byte Folded Reload
	ldr	x28, [sp, #16]                  // 8-byte Folded Reload
	ldp	x29, x30, [sp], #80             // 16-byte Folded Reload
	ret
.LBB7_5:
	mov	w8, #7232
	mov	x19, x0
	add	x20, x0, x8
	mov	x0, x20
	bl	mutex_lock
	ldr	w8, [x19, #7224]
	cbnz	w8, .LBB7_29
// %bb.6:
	mov	w8, #1
	adrp	x9, .L.str.29
	add	x9, x9, :lo12:.L.str.29
	str	w8, [x19, #7224]
	str	w8, [x19, #7228]
	ldr	x8, [x19, #88]
	ldr	x8, [x8, #152]
	ldr	x8, [x8, #8]
	cmp	x8, x9
	b.ne	.LBB7_8
// %bb.7:
	mov	w0, #1
	bl	sd_proxy_block_cmd
.LBB7_8:
	mov	x0, x20
	bl	mutex_unlock
	mov	x0, x19
	mov	w1, wzr
	bl	dfa_lowpower_vote_status_sync
	adrp	x9, :got:gufs_sk_state
	ldr	w8, [x19, #7032]
	ldr	x9, [x9, :got_lo12:gufs_sk_state]
	cmp	w8, #61
	str	wzr, [x9]
	b.lo	.LBB7_11
// %bb.9:
	mov	w20, #60
	mov	w22, #4912
	mov	w23, #4036
	adrp	x21, .L.str.50
	add	x21, x21, :lo12:.L.str.50
.LBB7_10:                               // =>This Inner Loop Header: Depth=1
	ldr	x8, [x19, #7016]
	mov	x1, x21
	ldr	x9, [x19, #6992]
	mov	w2, w20
	add	x8, x8, x23
	sub	x8, x8, #1284
	ldr	x0, [x9, #96]
	//APP
.Ltmp2:
	ldr	w3, [x8]
.Ltmp3:
	.section	.altinstructions,"a",@progbits
.Ltmp4:
	.word	.Ltmp2-.Ltmp4
.Ltmp6:
	.word	.Ltmp5-.Ltmp6
	.hword	1
	.byte	.Ltmp3-.Ltmp2
	.byte	.Ltmp7-.Ltmp5
	.text

	.text	1
.Ltmp5:
	ldar	w3, [x8]
.Ltmp7:
.Ltmp8:
.org (.Ltmp8-(.Ltmp7-.Ltmp5))+(.Ltmp3-.Ltmp2), 0
.Ltmp9:
.org (.Ltmp9-(.Ltmp3-.Ltmp2))+(.Ltmp7-.Ltmp5), 0
	.text


	//NO_APP
	mov	w8, w3
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp10:
	cbnz	x8, .Ltmp10
	//NO_APP
	ldr	x8, [x19, #7016]
	add	x8, x8, x23
	sub	x8, x8, #1280
	//APP
.Ltmp11:
	ldr	w4, [x8]
.Ltmp12:
	.section	.altinstructions,"a",@progbits
.Ltmp13:
	.word	.Ltmp11-.Ltmp13
.Ltmp15:
	.word	.Ltmp14-.Ltmp15
	.hword	1
	.byte	.Ltmp12-.Ltmp11
	.byte	.Ltmp16-.Ltmp14
	.text

	.text	1
.Ltmp14:
	ldar	w4, [x8]
.Ltmp16:
.Ltmp17:
.org (.Ltmp17-(.Ltmp16-.Ltmp14))+(.Ltmp12-.Ltmp11), 0
.Ltmp18:
.org (.Ltmp18-(.Ltmp12-.Ltmp11))+(.Ltmp16-.Ltmp14), 0
	.text


	//NO_APP
	mov	w8, w4
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp19:
	cbnz	x8, .Ltmp19
	//NO_APP
	ldr	x8, [x19, #7016]
	add	x8, x8, x23
	sub	x8, x8, #4
	//APP
.Ltmp20:
	ldr	w5, [x8]
.Ltmp21:
	.section	.altinstructions,"a",@progbits
.Ltmp22:
	.word	.Ltmp20-.Ltmp22
.Ltmp24:
	.word	.Ltmp23-.Ltmp24
	.hword	1
	.byte	.Ltmp21-.Ltmp20
	.byte	.Ltmp25-.Ltmp23
	.text

	.text	1
.Ltmp23:
	ldar	w5, [x8]
.Ltmp25:
.Ltmp26:
.org (.Ltmp26-(.Ltmp25-.Ltmp23))+(.Ltmp21-.Ltmp20), 0
.Ltmp27:
.org (.Ltmp27-(.Ltmp21-.Ltmp20))+(.Ltmp25-.Ltmp23), 0
	.text


	//NO_APP
	mov	w8, w5
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp28:
	cbnz	x8, .Ltmp28
	//NO_APP
	ldr	x8, [x19, #7016]
	add	x8, x8, x23
	//APP
.Ltmp29:
	ldr	w6, [x8]
.Ltmp30:
	.section	.altinstructions,"a",@progbits
.Ltmp31:
	.word	.Ltmp29-.Ltmp31
.Ltmp33:
	.word	.Ltmp32-.Ltmp33
	.hword	1
	.byte	.Ltmp30-.Ltmp29
	.byte	.Ltmp34-.Ltmp32
	.text

	.text	1
.Ltmp32:
	ldar	w6, [x8]
.Ltmp34:
.Ltmp35:
.org (.Ltmp35-(.Ltmp34-.Ltmp32))+(.Ltmp30-.Ltmp29), 0
.Ltmp36:
.org (.Ltmp36-(.Ltmp30-.Ltmp29))+(.Ltmp34-.Ltmp32), 0
	.text


	//NO_APP
	mov	w8, w6
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp37:
	cbnz	x8, .Ltmp37
	//NO_APP
	ldr	x8, [x19, #7016]
	add	x8, x8, x22
	add	x8, x8, #4
	//APP
.Ltmp38:
	ldr	w7, [x8]
.Ltmp39:
	.section	.altinstructions,"a",@progbits
.Ltmp40:
	.word	.Ltmp38-.Ltmp40
.Ltmp42:
	.word	.Ltmp41-.Ltmp42
	.hword	1
	.byte	.Ltmp39-.Ltmp38
	.byte	.Ltmp43-.Ltmp41
	.text

	.text	1
.Ltmp41:
	ldar	w7, [x8]
.Ltmp43:
.Ltmp44:
.org (.Ltmp44-(.Ltmp43-.Ltmp41))+(.Ltmp39-.Ltmp38), 0
.Ltmp45:
.org (.Ltmp45-(.Ltmp39-.Ltmp38))+(.Ltmp43-.Ltmp41), 0
	.text


	//NO_APP
	mov	w8, w7
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x8, x8, x8
.Ltmp46:
	cbnz	x8, .Ltmp46
	//NO_APP
	ldr	x8, [x19, #7016]
	add	x8, x8, x22
	//APP
.Ltmp47:
	ldr	w8, [x8]
.Ltmp48:
	.section	.altinstructions,"a",@progbits
.Ltmp49:
	.word	.Ltmp47-.Ltmp49
.Ltmp51:
	.word	.Ltmp50-.Ltmp51
	.hword	1
	.byte	.Ltmp48-.Ltmp47
	.byte	.Ltmp52-.Ltmp50
	.text

	.text	1
.Ltmp50:
	ldar	w8, [x8]
.Ltmp52:
.Ltmp53:
.org (.Ltmp53-(.Ltmp52-.Ltmp50))+(.Ltmp48-.Ltmp47), 0
.Ltmp54:
.org (.Ltmp54-(.Ltmp48-.Ltmp47))+(.Ltmp52-.Ltmp50), 0
	.text


	//NO_APP
	mov	w9, w8
	//APP
	dmb	oshld
	//NO_APP
	//APP
	eor	x9, x9, x9
.Ltmp55:
	cbnz	x9, .Ltmp55
	//NO_APP
	str	w8, [sp]
	bl	_dev_err
	ldr	w8, [x19, #7032]
	add	x20, x20, #1
	add	x22, x22, #12
	add	x23, x23, #8
	cmp	x20, x8
	b.lo	.LBB7_10
.LBB7_11:
	mov	w8, #27
	add	x0, sp, #48
	mov	w1, wzr
	mov	w2, #392
	strh	wzr, [sp, #44]
	str	w8, [sp, #40]
	str	xzr, [sp, #32]
	bl	memset
	ldr	x8, [x19, #296]
	mov	w10, #2000
	ldr	x9, [x19, #88]
	ldr	x8, [x8, #240]
	ldr	x0, [x9, #56]
	tst	x8, #0x40
	mov	w8, #12000
	csel	w21, w10, w8, eq
	bl	__raw_spin_lock_irqsave
	ldr	x20, [x19, #112]
	cbnz	x20, .LBB7_30
// %bb.12:
	mov	x22, x0
.LBB7_13:
	mov	w0, #-19
.LBB7_14:
	adrp	x8, :got:liblinux_thread_offset
	adrp	x12, :got:liblinux_thread_key
	//APP
	mrs	x11, TPIDR_EL0

	//NO_APP
	mov	w9, wzr
	ldr	x8, [x8, :got_lo12:liblinux_thread_offset]
	ldr	x10, [x19, #88]
	ldr	w8, [x8]
	ldr	x10, [x10, #56]
	ldr	x12, [x12, :got_lo12:liblinux_thread_key]
	//APP
	stlrb	w9, [x10]
	//NO_APP
	sub	x8, x11, x8
	ldr	w23, [x12]
	ldr	x9, [x8]
	ldr	x9, [x9, x23, lsl #3]
	ldrb	w10, [x9, #92]
	cbnz	w10, .LBB7_22
// %bb.15:
	ldr	w10, [x9, #44]
	cbz	w10, .LBB7_46
// %bb.16:
	cmp	x22, #1
	b.eq	.LBB7_22
// %bb.17:
	str	xzr, [x9, #24]
	//APP
	//NO_APP
	ldr	x10, [x8]
	ldr	x22, [x10, x23, lsl #3]
	ldr	w10, [x22, #40]
	sub	w10, w10, #1
	str	w10, [x22, #40]
	ldr	w10, [x22, #40]
	cbnz	w10, .LBB7_20
// %bb.18:
	ldrb	w10, [x22, #92]
	cbnz	w10, .LBB7_20
// %bb.19:
	ldr	x10, [x22, #32]
	//APP
	mrs	x11, TPIDRRO_EL0
	lsr	x11, x11, #56

	//NO_APP
	adrp	x12, :got:__preempt_disabled
	and	x11, x11, #0xffffffff
	lsl	x11, x11, #2
	mov	w14, #1
	ldr	x12, [x12, :got_lo12:__preempt_disabled]
	ldr	w13, [x12, x11]
	sub	w13, w13, #1
	str	w13, [x12, x11]
	str	w14, [x10, #4]
	ldr	w11, [x10, #8]
	cbnz	w11, .LBB7_48
.LBB7_20:
	ldr	w10, [x22, #40]
	tbnz	w10, #31, .LBB7_49
.LBB7_21:
	str	wzr, [x9, #44]
.LBB7_22:
	//APP
	//NO_APP
	ldr	x8, [x8]
	ldr	x22, [x8, x23, lsl #3]
	ldr	w8, [x22, #40]
	sub	w8, w8, #1
	str	w8, [x22, #40]
	ldr	w8, [x22, #40]
	cbnz	w8, .LBB7_25
// %bb.23:
	ldrb	w8, [x22, #92]
	cbnz	w8, .LBB7_25
// %bb.24:
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
	cbnz	w8, .LBB7_47
.LBB7_25:
	ldr	w8, [x22, #40]
	tbnz	w8, #31, .LBB7_49
.LBB7_26:
	mov	w8, #6992
	add	x22, x19, x8
	cbnz	w0, .LBB7_34
// %bb.27:
	adrp	x8, :got:kmalloc_caches
	mov	w9, #48
	mov	w1, #3520
	mov	w2, #96
	ldr	x8, [x8, :got_lo12:kmalloc_caches]
	strb	w9, [sp, #44]
	ldr	x0, [x8, #56]
	bl	kmem_cache_alloc_trace
	cbnz	x0, .LBB7_42
// %bb.28:
	adrp	x1, .L.str.53
	add	x1, x1, :lo12:.L.str.53
	b	.LBB7_35
.LBB7_29:
	mov	x0, x20
	bl	mutex_unlock
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.28
	add	x1, x1, :lo12:.L.str.28
	b	.LBB7_2
.LBB7_30:
	mov	x22, x0
	mov	x0, x20
	bl	scsi_device_get
	cbnz	w0, .LBB7_14
// %bb.31:
	ldr	w8, [x20, #2136]
	cmp	w8, #7
	b.hi	.LBB7_14
// %bb.32:
	mov	w9, #1
	lsl	w8, w9, w8
	mov	w9, #208
	tst	w8, w9
	b.eq	.LBB7_14
// %bb.33:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.51
	add	x1, x1, :lo12:.L.str.51
	bl	_dev_err
	mov	x0, x20
	bl	scsi_device_put
	b	.LBB7_13
.LBB7_34:
	adrp	x1, .L.str.52
	add	x1, x1, :lo12:.L.str.52
.LBB7_35:
	ldr	x0, [x19, #96]
	bl	_dev_err
.LBB7_36:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.30
	add	x1, x1, :lo12:.L.str.30
	bl	_dev_err
.LBB7_37:
	mov	x0, x22
	bl	dfa_dump_fw_log
	ldr	x8, [x19, #7016]
	mov	x0, x19
	add	x8, x8, #4
	//APP
.Ltmp56:
	ldr	w8, [x8]
.Ltmp57:
	.section	.altinstructions,"a",@progbits
.Ltmp58:
	.word	.Ltmp56-.Ltmp58
.Ltmp60:
	.word	.Ltmp59-.Ltmp60
	.hword	1
	.byte	.Ltmp57-.Ltmp56
	.byte	.Ltmp61-.Ltmp59
	.text

	.text	1
.Ltmp59:
	ldar	w8, [x8]
.Ltmp61:
.Ltmp62:
.org (.Ltmp62-(.Ltmp61-.Ltmp59))+(.Ltmp57-.Ltmp56), 0
.Ltmp63:
.org (.Ltmp63-(.Ltmp57-.Ltmp56))+(.Ltmp61-.Ltmp59), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w9, w8
	and	w8, w8, #0xfffffffe
	//APP
	eor	x9, x9, x9
.Ltmp64:
	cbnz	x9, .Ltmp64
	//NO_APP
	//APP
	dmb	oshst
	//NO_APP
	ldr	x9, [x19, #7016]
	add	x9, x9, #4
	//APP
	str	w8, [x9]
	//NO_APP
	bl	dfa_mcq_transfer_req_compl_all
	mov	w21, #4999
.LBB7_38:                               // =>This Inner Loop Header: Depth=1
	ldr	x8, [x19, #7008]
	add	x8, x8, #68
	//APP
.Ltmp65:
	ldr	w20, [x8]
.Ltmp66:
	.section	.altinstructions,"a",@progbits
.Ltmp67:
	.word	.Ltmp65-.Ltmp67
.Ltmp69:
	.word	.Ltmp68-.Ltmp69
	.hword	1
	.byte	.Ltmp66-.Ltmp65
	.byte	.Ltmp70-.Ltmp68
	.text

	.text	1
.Ltmp68:
	ldar	w20, [x8]
.Ltmp70:
.Ltmp71:
.org (.Ltmp71-(.Ltmp70-.Ltmp68))+(.Ltmp66-.Ltmp65), 0
.Ltmp72:
.org (.Ltmp72-(.Ltmp66-.Ltmp65))+(.Ltmp70-.Ltmp68), 0
	.text


	//NO_APP
	//APP
	dmb	oshld
	//NO_APP
	mov	w8, w20
	//APP
	eor	x9, x8, x8
.Ltmp73:
	cbnz	x9, .Ltmp73
	//NO_APP
	tbnz	w8, #4, .LBB7_40
// %bb.39:                              //   in Loop: Header=BB7_38 Depth=1
	mov	w0, #4295
	bl	__const_udelay
	sub	w21, w21, #1
	cmn	w21, #1
	b.ne	.LBB7_38
	b	.LBB7_44
.LBB7_40:
	//APP
	dmb	oshst
	//NO_APP
	cmp	w21, #0
	ldr	x8, [x19, #7008]
	mov	w9, #48
	add	x8, x8, #68
	//APP
	str	w9, [x8]
	//NO_APP
	b.le	.LBB7_44
.LBB7_41:
	mov	w0, wzr
	b	.LBB7_3
.LBB7_42:
	mov	x23, x0
	add	x0, sp, #48
	mov	w1, #392
	mov	w2, wzr
	mov	w3, #392
	bl	memset_s
	add	x0, sp, #48
	add	x2, sp, #40
	mov	x1, x20
	mov	x3, x23
	mov	w4, #3
	mov	x5, xzr
	mov	w6, wzr
	mov	w7, wzr
	bl	ufshcd_compose_scsi_cmd
	add	x1, sp, #48
	mov	x0, x19
	mov	w2, w21
	bl	dfa_exec_spec_cmd
	mov	w21, w0
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.54
	add	x1, x1, :lo12:.L.str.54
	mov	w2, w21
	bl	_dev_err
	lsr	w8, w21, #24
	cmp	w8, #8
	b.eq	.LBB7_45
.LBB7_43:
	mov	x0, x23
	bl	kfree
	mov	x0, x20
	bl	scsi_device_put
	cbz	w21, .LBB7_37
	b	.LBB7_36
.LBB7_44:
	ldr	x0, [x19, #96]
	adrp	x1, .L.str.31
	add	x1, x1, :lo12:.L.str.31
	mov	w2, w20
	bl	_dev_err
	b	.LBB7_41
.LBB7_45:
	add	x2, sp, #32
	mov	x0, x20
	mov	x1, xzr
	bl	scsi_print_sense_hdr
	b	.LBB7_43
.LBB7_46:
	adrp	x0, .L.str.32
	add	x0, x0, :lo12:.L.str.32
	adrp	x1, .L.str.33
	add	x1, x1, :lo12:.L.str.33
	adrp	x3, .L__func__.arch_local_irq_restore
	add	x3, x3, :lo12:.L__func__.arch_local_irq_restore
	mov	w2, #80
	b	.LBB7_50
.LBB7_47:
	str	w0, [sp, #28]                   // 4-byte Folded Spill
	bl	kernel_thread_preempt_enable
	ldr	w0, [sp, #28]                   // 4-byte Folded Reload
	str	wzr, [x23, #8]
	ldr	w8, [x22, #40]
	tbz	w8, #31, .LBB7_26
	b	.LBB7_49
.LBB7_48:
	str	w0, [sp, #28]                   // 4-byte Folded Spill
	mov	x24, x10
	stp	x8, x9, [sp, #8]                // 16-byte Folded Spill
	bl	kernel_thread_preempt_enable
	ldp	x8, x9, [sp, #8]                // 16-byte Folded Reload
	ldr	w0, [sp, #28]                   // 4-byte Folded Reload
	str	wzr, [x24, #8]
	ldr	w10, [x22, #40]
	tbz	w10, #31, .LBB7_21
.LBB7_49:
	str	wzr, [x22, #40]
	adrp	x0, .L.str.32
	add	x0, x0, :lo12:.L.str.32
	adrp	x1, .L.str.33
	add	x1, x1, :lo12:.L.str.33
	adrp	x3, .L__func__.__preempt_count_sub
	add	x3, x3, :lo12:.L__func__.__preempt_count_sub
	mov	w2, #150
.LBB7_50:
	bl	printk
	adrp	x0, .L.str.34
	add	x0, x0, :lo12:.L.str.34
	bl	panic
.LBB7_51:
	bl	__stack_chk_fail
.Lfunc_end7:
	.size	dfa_prepare_reset, .Lfunc_end7-dfa_prepare_reset
                                        // -- End function
	.globl	dfa_error_handler               // -- Begin function dfa_error_handler
	.p2align	2
	.type	dfa_error_handler,@function
dfa_error_handler:                      // @dfa_error_handler
// %bb.0:
	stp	x29, x30, [sp, #-16]!           // 16-byte Folded Spill
	ldur	x0, [x0, #-192]
	mov	x29, sp
	bl	dfa_reset_directly
	ldp	x29, x30, [sp], #16             // 16-byte Folded Reload
	ret
.Lfunc_end8:
	.size	dfa_error_handler, .Lfunc_end8-dfa_error_handler
                                        // -- End function
	.globl	dfa_schedule_eh                 // -- Begin function dfa_schedule_eh
	.p2align	2
	.type	dfa_schedule_eh,@function
dfa_schedule_eh:                        // @dfa_schedule_eh
// %bb.0:
	stp	x29, x30, [sp, #-32]!           // 16-byte Folded Spill
	cmn	x0, #2320
	str	x19, [sp, #16]                  // 8-byte Folded Spill
	mov	x29, sp
	b.eq	.LBB9_4
// %bb.1:
	ldr	w8, [x0, #9544]
	cbnz	w8, .LBB9_5
// %bb.2:
	mov	x19, x0
	mov	w8, #9448
	mov	w9, #9504
	add	x0, x0, x8
	add	x1, x19, x9
	bl	kthread_queue_work
	tbz	w0, #0, .LBB9_6
.LBB9_3:
	ldr	x19, [sp, #16]                  // 8-byte Folded Reload
	ldp	x29, x30, [sp], #32             // 16-byte Folded Reload
	ret
.LBB9_4:
	adrp	x0, .L.str.35
	add	x0, x0, :lo12:.L.str.35
	adrp	x1, .L__func__.dfa_schedule_eh
	add	x1, x1, :lo12:.L__func__.dfa_schedule_eh
	bl	printk
	b	.LBB9_3
.LBB9_5:
	mov	x19, x0
	ldr	x0, [x0, #2416]
	adrp	x1, .L.str.36
	add	x1, x1, :lo12:.L.str.36
	b	.LBB9_7
.LBB9_6:
	ldr	x0, [x19, #2416]
	adrp	x1, .L.str.37
	add	x1, x1, :lo12:.L.str.37
.LBB9_7:
	adrp	x2, .L__func__.dfa_schedule_eh
	add	x2, x2, :lo12:.L__func__.dfa_schedule_eh
	bl	_dev_err
	b	.LBB9_3
.Lfunc_end9:
	.size	dfa_schedule_eh, .Lfunc_end9-dfa_schedule_eh
                                        // -- End function
	.p2align	2                               // -- Begin function dfa_irq_sche_unlock
	.type	dfa_irq_sche_unlock,@function
dfa_irq_sche_unlock:                    // @dfa_irq_sche_unlock
// %bb.0:
	sub	sp, sp, #64
	adrp	x8, :got:liblinux_thread_offset
	stp	x29, x30, [sp, #32]             // 16-byte Folded Spill
	str	x19, [sp, #48]                  // 8-byte Folded Spill
	adrp	x12, :got:liblinux_thread_key
	//APP
	mrs	x11, TPIDR_EL0

	//NO_APP
	mov	w9, wzr
	ldr	x8, [x8, :got_lo12:liblinux_thread_offset]
	mov	w10, #7368
	ldr	x12, [x12, :got_lo12:liblinux_thread_key]
	add	x10, x0, x10
	//APP
	stlrb	w9, [x10]
	//NO_APP
	add	x29, sp, #32
	ldr	w8, [x8]
	ldr	w9, [x12]
	sub	x8, x11, x8
	ldr	x10, [x8]
	ldr	x10, [x10, x9, lsl #3]
	ldrb	w11, [x10, #92]
	cbnz	w11, .LBB10_6
// %bb.1:
	ldr	w11, [x10, #44]
	cbz	w11, .LBB10_15
// %bb.2:
	str	xzr, [x10, #24]
	//APP
	//NO_APP
	ldr	x11, [x8]
	ldr	x11, [x11, x9, lsl #3]
	ldr	w12, [x11, #40]
	sub	w12, w12, #1
	str	w12, [x11, #40]
	ldr	w12, [x11, #40]
	cbnz	w12, .LBB10_4
// %bb.3:
	ldrb	w12, [x11, #92]
	cbz	w12, .LBB10_12
.LBB10_4:
	ldr	w12, [x11, #40]
	tbnz	w12, #31, .LBB10_16
// %bb.5:
	str	wzr, [x10, #44]
.LBB10_6:
	//APP
	//NO_APP
	ldr	x8, [x8]
	ldr	x8, [x8, x9, lsl #3]
	ldr	w9, [x8, #40]
	sub	w9, w9, #1
	str	w9, [x8, #40]
	ldr	w9, [x8, #40]
	cbnz	w9, .LBB10_8
// %bb.7:
	ldrb	w9, [x8, #92]
	cbz	w9, .LBB10_10
.LBB10_8:
	ldr	w9, [x8, #40]
	tbnz	w9, #31, .LBB10_14
// %bb.9:
	ldp	x29, x30, [sp, #32]             // 16-byte Folded Reload
	ldr	x19, [sp, #48]                  // 8-byte Folded Reload
	add	sp, sp, #64
	ret
.LBB10_10:
	ldr	x9, [x8, #32]
	//APP
	mrs	x10, TPIDRRO_EL0
	lsr	x10, x10, #56

	//NO_APP
	adrp	x11, :got:__preempt_disabled
	and	x10, x10, #0xffffffff
	lsl	x10, x10, #2
	mov	w13, #1
	ldr	x11, [x11, :got_lo12:__preempt_disabled]
	ldr	w12, [x11, x10]
	sub	w12, w12, #1
	str	w12, [x11, x10]
	str	w13, [x9, #4]
	ldr	w10, [x9, #8]
	cbz	w10, .LBB10_8
// %bb.11:
	str	x8, [x29, #24]                  // 8-byte Folded Spill
	mov	x19, x9
	bl	kernel_thread_preempt_enable
	ldr	x8, [x29, #24]                  // 8-byte Folded Reload
	str	wzr, [x19, #8]
	b	.LBB10_8
.LBB10_12:
	ldr	x12, [x11, #32]
	//APP
	mrs	x13, TPIDRRO_EL0
	lsr	x13, x13, #56

	//NO_APP
	adrp	x14, :got:__preempt_disabled
	and	x13, x13, #0xffffffff
	lsl	x13, x13, #2
	mov	w16, #1
	ldr	x14, [x14, :got_lo12:__preempt_disabled]
	ldr	w15, [x14, x13]
	sub	w15, w15, #1
	str	w15, [x14, x13]
	str	w16, [x12, #4]
	ldr	w13, [x12, #8]
	cbz	w13, .LBB10_4
// %bb.13:
	str	x9, [x29, #24]                  // 8-byte Folded Spill
	mov	x19, x12
	str	x8, [sp, #16]                   // 8-byte Folded Spill
	stur	x10, [x29, #-8]                 // 8-byte Folded Spill
	str	x11, [sp, #8]                   // 8-byte Folded Spill
	bl	kernel_thread_preempt_enable
	ldp	x11, x8, [sp, #8]               // 16-byte Folded Reload
	ldur	x10, [x29, #-8]                 // 8-byte Folded Reload
	ldr	x9, [x29, #24]                  // 8-byte Folded Reload
	str	wzr, [x19, #8]
	b	.LBB10_4
.LBB10_14:
	str	wzr, [x8, #40]
	b	.LBB10_17
.LBB10_15:
	adrp	x0, .L.str.32
	add	x0, x0, :lo12:.L.str.32
	adrp	x1, .L.str.33
	add	x1, x1, :lo12:.L.str.33
	adrp	x3, .L__func__.arch_local_irq_restore
	add	x3, x3, :lo12:.L__func__.arch_local_irq_restore
	mov	w2, #80
	b	.LBB10_18
.LBB10_16:
	str	wzr, [x11, #40]
.LBB10_17:
	adrp	x0, .L.str.32
	add	x0, x0, :lo12:.L.str.32
	adrp	x1, .L.str.33
	add	x1, x1, :lo12:.L.str.33
	adrp	x3, .L__func__.__preempt_count_sub
	add	x3, x3, :lo12:.L__func__.__preempt_count_sub
	mov	w2, #150
.LBB10_18:
	bl	printk
	adrp	x0, .L.str.34
	add	x0, x0, :lo12:.L.str.34
	bl	panic
.Lfunc_end10:
	.size	dfa_irq_sche_unlock, .Lfunc_end10-dfa_irq_sche_unlock
                                        // -- End function
	.p2align	2                               // -- Begin function __raw_spin_lock_irqsave
	.type	__raw_spin_lock_irqsave,@function
__raw_spin_lock_irqsave:                // @__raw_spin_lock_irqsave
// %bb.0:
	sub	sp, sp, #64
	adrp	x9, :got:liblinux_thread_offset
	stp	x29, x30, [sp, #16]             // 16-byte Folded Spill
	stp	x22, x21, [sp, #32]             // 16-byte Folded Spill
	adrp	x10, :got:liblinux_thread_key
	stp	x20, x19, [sp, #48]             // 16-byte Folded Spill
	//APP
	mrs	x8, TPIDR_EL0

	//NO_APP
	ldr	x9, [x9, :got_lo12:liblinux_thread_offset]
	add	x29, sp, #16
	ldr	x10, [x10, :got_lo12:liblinux_thread_key]
	ldr	w9, [x9]
	sub	x22, x8, x9
	ldr	w8, [x10]
	ldr	x10, [x22]
	ldr	x21, [x10, x8, lsl #3]
	ldrb	w10, [x21, #92]
	cbnz	w10, .LBB11_5
// %bb.1:
	ldr	w20, [x21, #44]
	cbnz	w20, .LBB11_6
// %bb.2:
	xpaci	x30
	mov	x19, x0
	str	x30, [x21, #24]
	ldr	x10, [x22]
	ldr	x10, [x10, x8, lsl #3]
	ldr	w11, [x10, #40]
	cbnz	w11, .LBB11_4
// %bb.3:
	ldrb	w11, [x10, #92]
	cbz	w11, .LBB11_15
.LBB11_4:
	ldr	w11, [x10, #40]
	mov	w12, #1
	add	w11, w11, #1
	str	w11, [x10, #40]
	//APP
	//NO_APP
	str	w12, [x21, #44]
	ldr	x9, [x22]
	ldr	x21, [x9, x8, lsl #3]
	b	.LBB11_7
.LBB11_5:
	mov	x19, x0
	mov	x20, xzr
	ldr	w8, [x21, #40]
	cbz	w8, .LBB11_8
	b	.LBB11_9
.LBB11_6:
	mov	x19, x0
.LBB11_7:
	cmp	w20, #0
	cset	w20, ne
	ldr	w8, [x21, #40]
	cbnz	w8, .LBB11_9
.LBB11_8:
	ldrb	w8, [x21, #92]
	cbz	w8, .LBB11_11
.LBB11_9:
	ldr	w9, [x21, #40]
	mov	x8, xzr
	mov	w10, #1
	add	w9, w9, #1
	str	w9, [x21, #40]
	//APP
	//NO_APP
	//APP
	prfm	pstl1strm, [x19]
.Ltmp74:
	ldaxr	w1, [x19]
	eor	w9, w1, w8
	cbnz	w9, .Ltmp75
	stxr	w9, w10, [x19]
	cbnz	w9, .Ltmp74

.Ltmp75:
	//NO_APP
	cbnz	w1, .LBB11_19
.LBB11_10:
	mov	x0, x20
	ldp	x20, x19, [sp, #48]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #32]             // 16-byte Folded Reload
	ldp	x29, x30, [sp, #16]             // 16-byte Folded Reload
	add	sp, sp, #64
	ret
.LBB11_11:
	ldr	x9, [x21, #32]
	mov	w8, #2
	adrp	x22, :got:__preempt_disabled
	str	w8, [x9, #4]
	//APP
	//NO_APP
	//APP
	mrs	x10, TPIDRRO_EL0
	lsr	x10, x10, #56

	//NO_APP
	ldr	x22, [x22, :got_lo12:__preempt_disabled]
	ldr	w8, [x22, w10, uxtw #2]
	cmp	w8, #0
	b.gt	.LBB11_20
// %bb.12:
	ldr	w9, [x9, #4]
	cmp	w9, #3
	b.eq	.LBB11_20
// %bb.13:
	and	x9, x10, #0xffffffff
.LBB11_14:
	add	w8, w8, #1
	str	w8, [x22, x9, lsl #2]
	b	.LBB11_9
.LBB11_15:
	ldr	x13, [x10, #32]
	mov	w11, #2
	str	w11, [x13, #4]
	adrp	x11, :got:__preempt_disabled
	//APP
	//NO_APP
	//APP
	mrs	x14, TPIDRRO_EL0
	lsr	x14, x14, #56

	//NO_APP
	ldr	x11, [x11, :got_lo12:__preempt_disabled]
	ldr	w12, [x11, w14, uxtw #2]
	cmp	w12, #0
	b.gt	.LBB11_21
// %bb.16:
	ldr	w13, [x13, #4]
	cmp	w13, #3
	b.eq	.LBB11_21
// %bb.17:
	and	x13, x14, #0xffffffff
.LBB11_18:
	add	w12, w12, #1
	str	w12, [x11, x13, lsl #2]
	b	.LBB11_4
.LBB11_19:
	mov	x0, x19
	bl	queued_spin_lock_slowpath
	b	.LBB11_10
.LBB11_20:
	bl	kernel_thread_preempt_disable
	//APP
	mrs	x9, TPIDRRO_EL0
	lsr	x9, x9, #56

	//NO_APP
	ldr	w8, [x22, w9, uxtw #2]
	and	x9, x9, #0xffffffff
	b	.LBB11_14
.LBB11_21:
	stp	x8, x10, [sp]                   // 16-byte Folded Spill
	bl	kernel_thread_preempt_disable
	adrp	x11, :got:__preempt_disabled
	ldr	x11, [x11, :got_lo12:__preempt_disabled]
	ldp	x8, x10, [sp]                   // 16-byte Folded Reload
	//APP
	mrs	x13, TPIDRRO_EL0
	lsr	x13, x13, #56

	//NO_APP
	ldr	w12, [x11, w13, uxtw #2]
	and	x13, x13, #0xffffffff
	b	.LBB11_18
.Lfunc_end11:
	.size	__raw_spin_lock_irqsave, .Lfunc_end11-__raw_spin_lock_irqsave
                                        // -- End function
	.type	.L.str,@object                  // @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"[DFA] index:%d, tag:%d, current:%lld\n"
	.size	.L.str, 38

	.type	.L.str.1,@object                // @.str.1
.L.str.1:
	.asciz	"[DFA] cmd[%x] tag[%d] is timeout which can't be found."
	.size	.L.str.1, 55

	.type	.L.str.2,@object                // @.str.2
.L.str.2:
	.asciz	"[DFA] cmd pending in the device. tag = %d\n"
	.size	.L.str.2, 43

	.type	.L.str.3,@object                // @.str.3
.L.str.3:
	.asciz	"[DFA] cmd at tag %d not pending in the device\n"
	.size	.L.str.3, 47

	.type	.L.str.4,@object                // @.str.4
.L.str.4:
	.asciz	"[DFA] cmd at tag=%d is cleared, err:%d\n"
	.size	.L.str.4, 40

	.type	.L.str.5,@object                // @.str.5
.L.str.5:
	.asciz	"[DFA] no response from device. tag:%d, err:%d\n"
	.size	.L.str.5, 47

	.type	.L.str.6,@object                // @.str.6
.L.str.6:
	.asciz	"[DFA] issued tag = %d, err %d\n"
	.size	.L.str.6, 31

	.type	.L.str.7,@object                // @.str.7
.L.str.7:
	.asciz	"UFS Host state=%d\n"
	.size	.L.str.7, 19

	.type	.L.str.8,@object                // @.str.8
.L.str.8:
	.asciz	"saved_err=0x%x, saved_uic_err=0x%x\n"
	.size	.L.str.8, 36

	.type	.L.str.9,@object                // @.str.9
.L.str.9:
	.asciz	"Device power mode=%d, UIC link state=%d\n"
	.size	.L.str.9, 41

	.type	.L.str.10,@object               // @.str.10
.L.str.10:
	.asciz	"PM in progress=%d, sys. suspended=%d\n"
	.size	.L.str.10, 38

	.type	.L.str.11,@object               // @.str.11
.L.str.11:
	.asciz	"Auto BKOPS=%d, Host self-block=%d\n"
	.size	.L.str.11, 35

	.type	.L.str.12,@object               // @.str.12
.L.str.12:
	.asciz	"Clk gate=%d\n"
	.size	.L.str.12, 13

	.type	.L.str.13,@object               // @.str.13
.L.str.13:
	.asciz	"error handling flags=0x%x, req. abort count=%d\n"
	.size	.L.str.13, 48

	.type	.L.str.14,@object               // @.str.14
.L.str.14:
	.asciz	"hba->ufs_version=0x%x, Host capabilities=0x%x, caps=0x%x\n"
	.size	.L.str.14, 58

	.type	.L.str.15,@object               // @.str.15
.L.str.15:
	.asciz	"quirks=0x%x, dev. quirks=0x%x\n"
	.size	.L.str.15, 31

	.type	.L.str.16,@object               // @.str.16
.L.str.16:
	.asciz	"UFS dev info: %.8s %.16s rev %.4s\n"
	.size	.L.str.16, 35

	.type	.L.str.17,@object               // @.str.17
.L.str.17:
	.asciz	"outstanding:%u - 0x%llx\n"
	.size	.L.str.17, 25

	.type	.L.str.18,@object               // @.str.18
.L.str.18:
	.asciz	"dfa_inflts:%d\n"
	.size	.L.str.18, 15

	.type	.L.str.19,@object               // @.str.19
.L.str.19:
	.asciz	"dfa reset:%d\n"
	.size	.L.str.19, 14

	.type	.L.str.20,@object               // @.str.20
.L.str.20:
	.asciz	"dfa blocked:%d\n"
	.size	.L.str.20, 16

	.type	.L.str.21,@object               // @.str.21
.L.str.21:
	.asciz	"clk: %s, rate: %u\n"
	.size	.L.str.21, 19

	.type	.L.str.22,@object               // @.str.22
.L.str.22:
	.asciz	"[DFA] cmd at tag:%u already completed, outstanding:0x%lx\n"
	.size	.L.str.22, 58

	.type	.L.str.23,@object               // @.str.23
.L.str.23:
	.asciz	"[DFA] try to abort W-LUN\n"
	.size	.L.str.23, 26

	.type	.L.str.24,@object               // @.str.24
.L.str.24:
	.asciz	"[DFA] Device abort task at tag %u\n"
	.size	.L.str.24, 35

	.type	.L.str.25,@object               // @.str.25
.L.str.25:
	.asciz	"[DFA] failed with err %d\n"
	.size	.L.str.25, 26

	.type	.L.str.26,@object               // @.str.26
.L.str.26:
	.asciz	"\0013dfa_dump_fw_log memcpy_s fail\n"
	.size	.L.str.26, 33

	.type	.L.str.27,@object               // @.str.27
.L.str.27:
	.asciz	"[DFA] skip reset before ufs init completed\n"
	.size	.L.str.27, 44

	.type	.L.str.28,@object               // @.str.28
.L.str.28:
	.asciz	"[DFA] in reset, skip this one\n"
	.size	.L.str.28, 31

	.type	.L.str.29,@object               // @.str.29
.L.str.29:
	.asciz	"ufshcd"
	.size	.L.str.29, 7

	.type	.L.str.30,@object               // @.str.30
.L.str.30:
	.asciz	"[DFA] send pwrdown mode failed\n"
	.size	.L.str.30, 32

	.type	.L.str.31,@object               // @.str.31
.L.str.31:
	.asciz	"[DFA] wait DFA_CPU_PD timeout, val:0x%x\n"
	.size	.L.str.31, 41

	.type	.L.str.32,@object               // @.str.32
.L.str.32:
	.asciz	"BUG: failure at %s:%d/%s()!\n"
	.size	.L.str.32, 29

	.type	.L.str.33,@object               // @.str.33
.L.str.33:
	.asciz	"ldk"
	.size	.L.str.33, 4

	.type	.L__func__.dfa_reset_directly,@object // @__func__.dfa_reset_directly
.L__func__.dfa_reset_directly:
	.asciz	"dfa_reset_directly"
	.size	.L__func__.dfa_reset_directly, 19

	.type	.L.str.34,@object               // @.str.34
.L.str.34:
	.asciz	"BUG!"
	.size	.L.str.34, 5

	.type	.L.str.35,@object               // @.str.35
.L.str.35:
	.asciz	"\0013%s, hba NULL\n"
	.size	.L.str.35, 16

	.type	.L__func__.dfa_schedule_eh,@object // @__func__.dfa_schedule_eh
.L__func__.dfa_schedule_eh:
	.asciz	"dfa_schedule_eh"
	.size	.L__func__.dfa_schedule_eh, 16

	.type	.L.str.36,@object               // @.str.36
.L.str.36:
	.asciz	"%s, in reset\n"
	.size	.L.str.36, 14

	.type	.L.str.37,@object               // @.str.37
.L.str.37:
	.asciz	"%s, queue dhba->eh_worker fail\n"
	.size	.L.str.37, 32

	.type	.L.str.38,@object               // @.str.38
.L.str.38:
	.asciz	"[DFA], abort lun:%d, tag:%d, func:%u\n"
	.size	.L.str.38, 38

	.type	.L__func__.dfa_exec_tm_cmd,@object // @__func__.dfa_exec_tm_cmd
.L__func__.dfa_exec_tm_cmd:
	.asciz	"dfa_exec_tm_cmd"
	.size	.L__func__.dfa_exec_tm_cmd, 16

	.type	.L.str.39,@object               // @.str.39
.L.str.39:
	.asciz	"%s: failed, ocs = 0x%x\n"
	.size	.L.str.39, 24

	.type	.L.str.40,@object               // @.str.40
.L.str.40:
	.asciz	"%s timedout\n"
	.size	.L.str.40, 13

	.type	__init_completion.__key,@object // @__init_completion.__key
	.local	__init_completion.__key
	.comm	__init_completion.__key,1,1
	.type	.L.str.41,@object               // @.str.41
.L.str.41:
	.asciz	"&x->wait"
	.size	.L.str.41, 9

	.type	.L__func__.arch_local_irq_restore,@object // @__func__.arch_local_irq_restore
.L__func__.arch_local_irq_restore:
	.asciz	"arch_local_irq_restore"
	.size	.L__func__.arch_local_irq_restore, 23

	.type	.L__func__.__preempt_count_sub,@object // @__func__.__preempt_count_sub
.L__func__.__preempt_count_sub:
	.asciz	"__preempt_count_sub"
	.size	.L__func__.__preempt_count_sub, 20

	.type	.L.str.42,@object               // @.str.42
.L.str.42:
	.asciz	"[DFA] skip abort. tag %d failed earlier\n"
	.size	.L.str.42, 41

	.type	.L.str.43,@object               // @.str.43
.L.str.43:
	.asciz	"[DFA] cmd found in sq. dhq=%d, tag=%d\n"
	.size	.L.str.43, 39

	.type	.L.str.44,@object               // @.str.44
.L.str.44:
	.asciz	"[DFA] device abort failed %d\n"
	.size	.L.str.44, 30

	.type	.L.str.45,@object               // @.str.45
.L.str.45:
	.asciz	"[DFA] abort entry id:%u tag:%d\n"
	.size	.L.str.45, 32

	.type	.L.str.46,@object               // @.str.46
.L.str.46:
	.asciz	"DFA FW log print start\n"
	.size	.L.str.46, 24

	.type	.L.str.47,@object               // @.str.47
.L.str.47:
	.asciz	"\0013"
	.size	.L.str.47, 3

	.type	.L.str.48,@object               // @.str.48
.L.str.48:
	.asciz	"DFA FW log:"
	.size	.L.str.48, 12

	.type	.L.str.49,@object               // @.str.49
.L.str.49:
	.asciz	"DFA FW log print end\n"
	.size	.L.str.49, 22

	.type	.L.str.50,@object               // @.str.50
.L.str.50:
	.asciz	"[DFA] QUE_PROC %u SQH:0x%08x SQT:0x%08x CQH:0x%08x CQT:0x%08x CQIE:0x%08x CQIS:0x%08x\n"
	.size	.L.str.50, 87

	.type	.L__const.dfa_send_scsi_ssu.cmd,@object // @__const.dfa_send_scsi_ssu.cmd
	.section	.rodata,"a",@progbits
.L__const.dfa_send_scsi_ssu.cmd:
	.asciz	"\033\000\000\000\000"
	.size	.L__const.dfa_send_scsi_ssu.cmd, 6

	.type	.L.str.51,@object               // @.str.51
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str.51:
	.asciz	"[DFA] sdp get failed\n"
	.size	.L.str.51, 22

	.type	.L.str.52,@object               // @.str.52
.L.str.52:
	.asciz	"[DFA] sdp NULL\n"
	.size	.L.str.52, 16

	.type	.L.str.53,@object               // @.str.53
.L.str.53:
	.asciz	"[DFA] sense buffer alloc failed\n"
	.size	.L.str.53, 33

	.type	.L.str.54,@object               // @.str.54
.L.str.54:
	.asciz	"[DFA] emergency ssu ret:%d\n"
	.size	.L.str.54, 28

	.type	.L.str.55,@object               // @.str.55
.L.str.55:
	.asciz	"dfa_ufs_reset enter\n"
	.size	.L.str.55, 21

	.type	.L.str.56,@object               // @.str.56
.L.str.56:
	.asciz	"dfa_ufs_reset exit\n"
	.size	.L.str.56, 20

	.type	.L.str.57,@object               // @.str.57
.L.str.57:
	.asciz	"%s: failed with err %d\n"
	.size	.L.str.57, 24

	.type	.L__func__.dfa_send_request_sense,@object // @__func__.dfa_send_request_sense
.L__func__.dfa_send_request_sense:
	.asciz	"dfa_send_request_sense"
	.size	.L__func__.dfa_send_request_sense, 23

	.ident	"OHOS (BiSheng Mobile STD 203.0.0.2.B001) clang version 15.0.4 (llvm-project db54e0a809bd)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym __init_completion.__key
