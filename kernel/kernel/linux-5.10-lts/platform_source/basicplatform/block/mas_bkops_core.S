	.text
	.file	"mas_bkops_core.c"
	.globl	bkops_idle_work_func            // -- Begin function bkops_idle_work_func
	.p2align	2
	.type	bkops_idle_work_func,@function
bkops_idle_work_func:                   // @bkops_idle_work_func
// %bb.0:
	sub	sp, sp, #64
	adrp	x8, :got:__stack_chk_guard
	stp	x29, x30, [sp, #16]             // 16-byte Folded Spill
	str	x21, [sp, #32]                  // 8-byte Folded Spill
	add	x29, sp, #16
	stp	x20, x19, [sp, #48]             // 16-byte Folded Spill
	mov	x19, x0
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldr	x8, [x8]
	str	x8, [sp, #8]
	ldur	x20, [x0, #-24]
	adrp	x0, bkops_lock
	add	x0, x0, :lo12:bkops_lock
	str	wzr, [sp, #4]
	bl	mutex_lock
	adrp	x21, bkops_enabled
	ldrb	w8, [x21, :lo12:bkops_enabled]
	cmp	w8, #1
	b.ne	.LBB0_4
// %bb.1:
	adrp	x0, bkops_lock
	add	x0, x0, :lo12:bkops_lock
	bl	mutex_unlock
	ldr	x8, [x19, #88]
	str	wzr, [x19, #120]
	and	x8, x8, #0xfffffffffffffffd
	str	x8, [x19, #88]
.LBB0_2:
	adrp	x8, :got:__stack_chk_guard
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldr	x9, [sp, #8]
	ldr	x8, [x8]
	cmp	x8, x9
	b.ne	.LBB0_17
// %bb.3:
	ldp	x20, x19, [sp, #48]             // 16-byte Folded Reload
	ldp	x29, x30, [sp, #16]             // 16-byte Folded Reload
	ldr	x21, [sp, #32]                  // 8-byte Folded Reload
	add	sp, sp, #64
	ret
.LBB0_4:
	ldr	x8, [x20, #8]
	mov	w9, #58020
	ldur	x0, [x19, #-16]
	add	x1, sp, #4
	blraa	x8, x9
	adrp	x8, user_manual_gc
	mov	w20, w0
	ldrb	w9, [x8, :lo12:user_manual_gc]
	cbz	w9, .LBB0_7
// %bb.5:
	ldr	w9, [sp, #4]
	cbnz	w9, .LBB0_7
// %bb.6:
	strb	wzr, [x8, :lo12:user_manual_gc]
.LBB0_7:
	adrp	x0, bkops_lock
	add	x0, x0, :lo12:bkops_lock
	bl	mutex_unlock
	cbnz	w20, .LBB0_14
// %bb.8:
	ldr	x8, [x19, #88]
	ldr	w9, [sp, #4]
	and	x8, x8, #0xfffffffffffffffd
	str	w9, [x19, #120]
	str	x8, [x19, #88]
	cbz	w9, .LBB0_2
// %bb.9:
	ldr	x8, [x19, #88]
	tbnz	w8, #0, .LBB0_2
// %bb.10:
	adrp	x0, bkops_lock
	add	x0, x0, :lo12:bkops_lock
	ldur	x20, [x19, #-24]
	bl	mutex_lock
	ldrb	w8, [x21, :lo12:bkops_enabled]
	cmp	w8, #1
	b.ne	.LBB0_12
// %bb.11:
	adrp	x0, bkops_lock
	add	x0, x0, :lo12:bkops_lock
	bl	mutex_unlock
	b	.LBB0_13
.LBB0_12:
	ldr	x8, [x20]
	mov	w9, #28009
	ldur	x0, [x19, #-16]
	mov	w1, #1
	blraa	x8, x9
	mov	w20, w0
	adrp	x0, bkops_lock
	add	x0, x0, :lo12:bkops_lock
	bl	mutex_unlock
	cbnz	w20, .LBB0_16
.LBB0_13:
	add	x8, x19, #88
	//APP
	// atomic64_or
	prfm	pstl1strm, [x8]
.Ltmp0:
	ldxr	x9, [x8]
	orr	x9, x9, #0x1
	stxr	w10, x9, [x8]
	cbnz	w10, .Ltmp0
	//NO_APP
	b	.LBB0_2
.LBB0_14:
	adrp	x0, .L.str
	add	x0, x0, :lo12:.L.str
.LBB0_15:
	bl	printk
	b	.LBB0_2
.LBB0_16:
	adrp	x0, .L.str.1
	add	x0, x0, :lo12:.L.str.1
	b	.LBB0_15
.LBB0_17:
	bl	__stack_chk_fail
.Lfunc_end0:
	.size	bkops_idle_work_func, .Lfunc_end0-bkops_idle_work_func
                                        // -- End function
	.globl	bkops_io_busyidle_notify_handler // -- Begin function bkops_io_busyidle_notify_handler
	.p2align	2
	.type	bkops_io_busyidle_notify_handler,@function
bkops_io_busyidle_notify_handler:       // @bkops_io_busyidle_notify_handler
// %bb.0:
	stp	x29, x30, [sp, #-48]!           // 16-byte Folded Spill
	str	x21, [sp, #16]                  // 8-byte Folded Spill
	mov	x29, sp
	stp	x20, x19, [sp, #32]             // 16-byte Folded Spill
	cbz	w1, .LBB1_9
// %bb.1:
	cmp	w1, #1
	b.ne	.LBB1_11
// %bb.2:
	ldr	x8, [x0, #120]
	tbnz	w8, #1, .LBB1_14
// %bb.3:
	mov	x19, x0
	ldr	w9, [x0, #156]
	cbnz	w9, .LBB1_13
.LBB1_4:
	tbnz	w8, #4, .LBB1_23
// %bb.5:
	tbnz	w8, #3, .LBB1_26
.LBB1_6:
	tbnz	w8, #2, .LBB1_21
.LBB1_7:
	adrp	x9, user_manual_gc
	ldrb	w9, [x9, :lo12:user_manual_gc]
	cbnz	w9, .LBB1_15
// %bb.8:
	and	x8, x8, #0xfffffffffffffffd
	mov	w0, wzr
	str	x8, [x19, #120]
	b	.LBB1_16
.LBB1_9:
	ldr	x8, [x0, #120]
	tbnz	w8, #5, .LBB1_17
.LBB1_10:
	mov	w0, wzr
	b	.LBB1_16
.LBB1_11:
	mov	w20, w1
	adrp	x0, bkops_io_busyidle_notify_handler._rs
	add	x0, x0, :lo12:bkops_io_busyidle_notify_handler._rs
	adrp	x1, .L__func__.bkops_io_busyidle_notify_handler
	add	x1, x1, :lo12:.L__func__.bkops_io_busyidle_notify_handler
	bl	___ratelimit
	cbz	w0, .LBB1_16
// %bb.12:
	adrp	x0, .L.str.2
	add	x0, x0, :lo12:.L.str.2
	adrp	x1, .L__func__.bkops_io_busyidle_notify_handler
	add	x1, x1, :lo12:.L__func__.bkops_io_busyidle_notify_handler
	mov	w2, w20
	bl	printk
	b	.LBB1_10
.LBB1_13:
	ldr	w9, [x19, #152]
	cbz	w9, .LBB1_4
	b	.LBB1_15
.LBB1_14:
	mov	x19, x0
.LBB1_15:
	orr	x8, x8, #0x2
	adrp	x9, bkops_workqueue_g
	str	x8, [x19, #120]!
	//APP
	// atomic64_or
	prfm	pstl1strm, [x19]
.Ltmp1:
	ldxr	x8, [x19]
	orr	x8, x8, #0x20
	stxr	w10, x8, [x19]
	cbnz	w10, .Ltmp1
	//NO_APP
	sub	x21, x19, #88
	ldr	w0, [x19, #8]
	ldr	x20, [x9, :lo12:bkops_workqueue_g]
	bl	__msecs_to_jiffies
	mov	x3, x0
	mov	w0, #12
	mov	x1, x20
	mov	x2, x21
	bl	queue_delayed_work_on
	mov	w0, #1
.LBB1_16:
	ldp	x20, x19, [sp, #32]             // 16-byte Folded Reload
	ldr	x21, [sp, #16]                  // 8-byte Folded Reload
	ldp	x29, x30, [sp], #48             // 16-byte Folded Reload
	ret
.LBB1_17:
	add	x8, x0, #120
	mov	w9, #32
	//APP
	// atomic64_fetch_andnot
	prfm	pstl1strm, [x8]
.Ltmp2:
	ldxr	x10, [x8]
	bic	x11, x10, x9
	stlxr	w12, x11, [x8]
	cbnz	w12, .Ltmp2
	dmb	ish
	//NO_APP
	tbz	w10, #5, .LBB1_10
// %bb.18:
	mov	x19, x0
	add	x0, x0, #32
	bl	cancel_delayed_work_sync
	ldr	x8, [x19, #120]
	tbz	w8, #0, .LBB1_10
// %bb.19:
	adrp	x0, bkops_lock
	add	x0, x0, :lo12:bkops_lock
	bl	mutex_lock
	adrp	x8, bkops_enabled
	ldrb	w8, [x8, :lo12:bkops_enabled]
	tbz	w8, #0, .LBB1_28
.LBB1_20:
	adrp	x0, bkops_lock
	add	x0, x0, :lo12:bkops_lock
	bl	mutex_unlock
	add	x8, x19, #120
	mov	w9, #1
	//APP
	// atomic64_andnot
	prfm	pstl1strm, [x8]
.Ltmp3:
	ldxr	x10, [x8]
	bic	x10, x10, x9
	stxr	w11, x10, [x8]
	cbnz	w11, .Ltmp3
	//NO_APP
	b	.LBB1_10
.LBB1_21:
	ldp	x11, x10, [x19, #192]
	ldr	x9, [x19, #136]
	sub	x10, x9, x10
	cmp	x10, x11
	b.lo	.LBB1_7
// %bb.22:
	str	x9, [x19, #200]
	b	.LBB1_15
.LBB1_23:
	mov	w0, #1
	bl	ktime_get_with_offset
	bl	ns_to_timespec64
	ldr	x8, [x19, #168]
	cbz	x8, .LBB1_29
// %bb.24:
	ldr	x9, [x19, #160]
	sub	x8, x0, x8
	cmp	x8, x9
	b.ge	.LBB1_29
// %bb.25:
	ldr	x8, [x19, #120]
	tbz	w8, #3, .LBB1_6
.LBB1_26:
	ldp	x11, x10, [x19, #176]
	ldr	x9, [x19, #144]
	sub	x10, x9, x10
	cmp	x10, x11
	b.lo	.LBB1_6
// %bb.27:
	str	x9, [x19, #184]
	b	.LBB1_15
.LBB1_28:
	ldp	x8, x0, [x19, #8]
	mov	w9, #28009
	mov	w1, wzr
	ldr	x8, [x8]
	blraa	x8, x9
	b	.LBB1_20
.LBB1_29:
	ldr	x8, [x19, #120]
	str	x0, [x19, #168]
	b	.LBB1_15
.Lfunc_end1:
	.size	bkops_io_busyidle_notify_handler, .Lfunc_end1-bkops_io_busyidle_notify_handler
                                        // -- End function
	.globl	bkops_pm_callback               // -- Begin function bkops_pm_callback
	.p2align	2
	.type	bkops_pm_callback,@function
bkops_pm_callback:                      // @bkops_pm_callback
// %bb.0:
	sub	x8, x1, #1
	cmp	x8, #3
	b.ls	.LBB2_2
// %bb.1:
	mov	w0, wzr
	ret
.LBB2_2:
	stp	x29, x30, [sp, #-32]!           // 16-byte Folded Spill
	stp	x20, x19, [sp, #16]             // 16-byte Folded Spill
	adrp	x19, bkops_lock
	add	x19, x19, :lo12:bkops_lock
	tst	x8, #0x1
	mov	x0, x19
	mov	x29, sp
	cset	w20, eq
	bl	mutex_lock
	adrp	x8, bkops_enabled
	mov	x0, x19
	strb	w20, [x8, :lo12:bkops_enabled]
	bl	mutex_unlock
	ldp	x20, x19, [sp, #16]             // 16-byte Folded Reload
	mov	w0, #1
	ldp	x29, x30, [sp], #32             // 16-byte Folded Reload
	ret
.Lfunc_end2:
	.size	bkops_pm_callback, .Lfunc_end2-bkops_pm_callback
                                        // -- End function
	.globl	bkops_notify_reboot             // -- Begin function bkops_notify_reboot
	.p2align	2
	.type	bkops_notify_reboot,@function
bkops_notify_reboot:                    // @bkops_notify_reboot
// %bb.0:
	stp	x29, x30, [sp, #-32]!           // 16-byte Folded Spill
	str	x19, [sp, #16]                  // 8-byte Folded Spill
	adrp	x19, bkops_lock
	add	x19, x19, :lo12:bkops_lock
	mov	x29, sp
	mov	x0, x19
	bl	mutex_lock
	adrp	x8, bkops_enabled
	mov	w9, #1
	mov	x0, x19
	strb	w9, [x8, :lo12:bkops_enabled]
	bl	mutex_unlock
	ldr	x19, [sp, #16]                  // 8-byte Folded Reload
	mov	w0, wzr
	ldp	x29, x30, [sp], #32             // 16-byte Folded Reload
	ret
.Lfunc_end3:
	.size	bkops_notify_reboot, .Lfunc_end3-bkops_notify_reboot
                                        // -- End function
	.globl	mas_bkops_alloc                 // -- Begin function mas_bkops_alloc
	.p2align	2
	.type	mas_bkops_alloc,@function
mas_bkops_alloc:                        // @mas_bkops_alloc
// %bb.0:
	stp	x29, x30, [sp, #-32]!           // 16-byte Folded Spill
	adrp	x8, :got:kmalloc_caches
	str	x19, [sp, #16]                  // 8-byte Folded Spill
	mov	w1, #3520
	mov	w2, #208
	mov	x29, sp
	ldr	x8, [x8, :got_lo12:kmalloc_caches]
	ldr	x0, [x8, #64]
	bl	kmem_cache_alloc_trace
	mov	x19, x0
	cbz	x0, .LBB4_3
// %bb.1:
	mov	x8, #68719476704
	adrp	x9, :got:__ufs_bkops_idle_work_func
	adrp	x1, :got:delayed_work_timer_fn
	add	x10, x0, #40
	mov	w11, #23997
	mov	w2, #2097152
	ldr	x9, [x9, :got_lo12:__ufs_bkops_idle_work_func]
	mov	x3, xzr
	ldr	x1, [x1, :got_lo12:delayed_work_timer_fn]
	str	x8, [x0, #32]
	mov	w8, #48793
	str	x10, [x0, #40]
	add	x0, x19, #64
	mov	x4, xzr
	pacia	x9, x11
	str	x10, [x19, #48]
	pacia	x1, x8
	str	x9, [x19, #56]
	bl	init_timer_key
	mov	w8, #1000
	mov	w9, #3600
	mov	w10, #536870912
	str	x8, [x19, #128]
	str	x9, [x19, #160]
	str	x10, [x19, #176]
	str	x10, [x19, #192]
.LBB4_2:
	mov	x0, x19
	ldr	x19, [sp, #16]                  // 8-byte Folded Reload
	ldp	x29, x30, [sp], #32             // 16-byte Folded Reload
	ret
.LBB4_3:
	adrp	x0, .L.str.3
	add	x0, x0, :lo12:.L.str.3
	bl	printk
	b	.LBB4_2
.Lfunc_end4:
	.size	mas_bkops_alloc, .Lfunc_end4-mas_bkops_alloc
                                        // -- End function
	.globl	mas_bkops_manual_gc_proc_show   // -- Begin function mas_bkops_manual_gc_proc_show
	.p2align	2
	.type	mas_bkops_manual_gc_proc_show,@function
mas_bkops_manual_gc_proc_show:          // @mas_bkops_manual_gc_proc_show
// %bb.0:
	stp	x29, x30, [sp, #-16]!           // 16-byte Folded Spill
	adrp	x8, user_manual_gc
	adrp	x1, .L.str.4
	add	x1, x1, :lo12:.L.str.4
	mov	x29, sp
	ldrb	w2, [x8, :lo12:user_manual_gc]
	bl	seq_printf
	mov	w0, wzr
	ldp	x29, x30, [sp], #16             // 16-byte Folded Reload
	ret
.Lfunc_end5:
	.size	mas_bkops_manual_gc_proc_show, .Lfunc_end5-mas_bkops_manual_gc_proc_show
                                        // -- End function
	.globl	mas_bkops_manual_gc_proc_open   // -- Begin function mas_bkops_manual_gc_proc_open
	.p2align	2
	.type	mas_bkops_manual_gc_proc_open,@function
mas_bkops_manual_gc_proc_open:          // @mas_bkops_manual_gc_proc_open
// %bb.0:
	stp	x29, x30, [sp, #-16]!           // 16-byte Folded Spill
	adrp	x8, :got:__cfi_mas_bkops_manual_gc_proc_show
	mov	w9, #41692
	mov	x0, x1
	mov	x2, xzr
	mov	x29, sp
	ldr	x8, [x8, :got_lo12:__cfi_mas_bkops_manual_gc_proc_show]
	pacia	x8, x9
	mov	x1, x8
	bl	single_open
	ldp	x29, x30, [sp], #16             // 16-byte Folded Reload
	ret
.Lfunc_end6:
	.size	mas_bkops_manual_gc_proc_open, .Lfunc_end6-mas_bkops_manual_gc_proc_open
                                        // -- End function
	.globl	mas_bkops_manual_gc_proc_write  // -- Begin function mas_bkops_manual_gc_proc_write
	.p2align	2
	.type	mas_bkops_manual_gc_proc_write,@function
mas_bkops_manual_gc_proc_write:         // @mas_bkops_manual_gc_proc_write
// %bb.0:
	sub	sp, sp, #48
	adrp	x8, :got:__stack_chk_guard
	stp	x29, x30, [sp, #16]             // 16-byte Folded Spill
	stp	x20, x19, [sp, #32]             // 16-byte Folded Spill
	mov	x0, #-22
	add	x29, sp, #16
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldr	x8, [x8]
	str	x8, [sp, #8]
	strb	wzr, [sp, #4]
	cbz	x1, .LBB7_5
// %bb.1:
	cbz	x2, .LBB7_5
// %bb.2:
	mov	x19, x2
	add	x0, sp, #4
	mov	w2, #1
	add	x20, sp, #4
	bl	__arch_copy_from_user
	cbnz	x0, .LBB7_8
// %bb.3:
	ldrb	w8, [sp, #4]
	sub	w9, w8, #50
	and	w9, w9, #0xff
	cmp	w9, #253
	b.ls	.LBB7_7
// %bb.4:
	sub	w8, w8, #48
	adrp	x9, user_manual_gc
	mov	x0, x19
	strb	w8, [x9, :lo12:user_manual_gc]
.LBB7_5:
	adrp	x8, :got:__stack_chk_guard
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldr	x9, [sp, #8]
	ldr	x8, [x8]
	cmp	x8, x9
	b.ne	.LBB7_9
// %bb.6:
	ldp	x20, x19, [sp, #32]             // 16-byte Folded Reload
	ldp	x29, x30, [sp, #16]             // 16-byte Folded Reload
	add	sp, sp, #48
	ret
.LBB7_7:
	adrp	x0, .L.str.5
	add	x0, x0, :lo12:.L.str.5
	adrp	x1, .L__func__.mas_bkops_manual_gc_proc_write
	add	x1, x1, :lo12:.L__func__.mas_bkops_manual_gc_proc_write
	mov	w2, #837
	bl	printk
	mov	x0, #-22
	b	.LBB7_5
.LBB7_8:
	sub	x8, x20, x0
	mov	x2, x0
	add	x0, x8, #1
	mov	w1, wzr
	bl	memset
	mov	x0, #-14
	b	.LBB7_5
.LBB7_9:
	bl	__stack_chk_fail
.Lfunc_end7:
	.size	mas_bkops_manual_gc_proc_write, .Lfunc_end7-mas_bkops_manual_gc_proc_write
                                        // -- End function
	.globl	mas_bkops_status_proc_show      // -- Begin function mas_bkops_status_proc_show
	.p2align	2
	.type	mas_bkops_status_proc_show,@function
mas_bkops_status_proc_show:             // @mas_bkops_status_proc_show
// %bb.0:
	sub	sp, sp, #64
	adrp	x8, :got:__stack_chk_guard
	stp	x29, x30, [sp, #16]             // 16-byte Folded Spill
	str	x21, [sp, #32]                  // 8-byte Folded Spill
	add	x29, sp, #16
	stp	x20, x19, [sp, #48]             // 16-byte Folded Spill
	mov	x19, x0
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldr	x8, [x8]
	str	x8, [sp, #8]
	ldr	x20, [x0, #112]
	adrp	x0, bkops_lock
	add	x0, x0, :lo12:bkops_lock
	str	wzr, [sp, #4]
	ldr	x21, [x20, #8]
	bl	mutex_lock
	adrp	x8, bkops_enabled
	ldrb	w8, [x8, :lo12:bkops_enabled]
	cmp	w8, #1
	b.ne	.LBB8_2
// %bb.1:
	adrp	x0, bkops_lock
	add	x0, x0, :lo12:bkops_lock
	bl	mutex_unlock
	mov	w2, wzr
	b	.LBB8_7
.LBB8_2:
	ldr	x8, [x21, #8]
	mov	w9, #58020
	ldr	x0, [x20, #16]
	add	x1, sp, #4
	blraa	x8, x9
	adrp	x8, user_manual_gc
	mov	w20, w0
	ldrb	w9, [x8, :lo12:user_manual_gc]
	cbz	w9, .LBB8_5
// %bb.3:
	ldr	w9, [sp, #4]
	cbnz	w9, .LBB8_5
// %bb.4:
	strb	wzr, [x8, :lo12:user_manual_gc]
.LBB8_5:
	adrp	x0, bkops_lock
	add	x0, x0, :lo12:bkops_lock
	bl	mutex_unlock
	cbnz	w20, .LBB8_10
// %bb.6:
	ldr	w2, [sp, #4]
.LBB8_7:
	adrp	x1, .L.str.7
	add	x1, x1, :lo12:.L.str.7
	mov	x0, x19
	bl	seq_printf
	mov	w20, wzr
.LBB8_8:
	adrp	x8, :got:__stack_chk_guard
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldr	x9, [sp, #8]
	ldr	x8, [x8]
	cmp	x8, x9
	b.ne	.LBB8_11
// %bb.9:
	mov	w0, w20
	ldr	x21, [sp, #32]                  // 8-byte Folded Reload
	ldp	x20, x19, [sp, #48]             // 16-byte Folded Reload
	ldp	x29, x30, [sp, #16]             // 16-byte Folded Reload
	add	sp, sp, #64
	ret
.LBB8_10:
	adrp	x0, .L.str.6
	add	x0, x0, :lo12:.L.str.6
	adrp	x1, .L__func__.mas_bkops_status_proc_show
	add	x1, x1, :lo12:.L__func__.mas_bkops_status_proc_show
	mov	w2, w20
	bl	printk
	b	.LBB8_8
.LBB8_11:
	bl	__stack_chk_fail
.Lfunc_end8:
	.size	mas_bkops_status_proc_show, .Lfunc_end8-mas_bkops_status_proc_show
                                        // -- End function
	.globl	mas_bkops_status_proc_open      // -- Begin function mas_bkops_status_proc_open
	.p2align	2
	.type	mas_bkops_status_proc_open,@function
mas_bkops_status_proc_open:             // @mas_bkops_status_proc_open
// %bb.0:
	stp	x29, x30, [sp, #-32]!           // 16-byte Folded Spill
	str	x19, [sp, #16]                  // 8-byte Folded Spill
	mov	x29, sp
	mov	x19, x1
	bl	PDE_DATA
	adrp	x1, :got:__cfi_mas_bkops_status_proc_show
	mov	w8, #41692
	mov	x2, x0
	mov	x0, x19
	ldr	x1, [x1, :got_lo12:__cfi_mas_bkops_status_proc_show]
	pacia	x1, x8
	bl	single_open
	ldr	x19, [sp, #16]                  // 8-byte Folded Reload
	ldp	x29, x30, [sp], #32             // 16-byte Folded Reload
	ret
.Lfunc_end9:
	.size	mas_bkops_status_proc_open, .Lfunc_end9-mas_bkops_status_proc_open
                                        // -- End function
	.globl	mas_bkops_enable                // -- Begin function mas_bkops_enable
	.p2align	2
	.type	mas_bkops_enable,@function
mas_bkops_enable:                       // @mas_bkops_enable
// %bb.0:
	stp	x29, x30, [sp, #-48]!           // 16-byte Folded Spill
	stp	x22, x21, [sp, #16]             // 16-byte Folded Spill
	adrp	x21, g_bkops
	stp	x20, x19, [sp, #32]             // 16-byte Folded Spill
	mov	x29, sp
	ldr	x8, [x21, :lo12:g_bkops]
	cbnz	x8, .LBB10_13
// %bb.1:
	mov	x19, x0
	mov	w0, #-22
	cbz	x19, .LBB10_12
// %bb.2:
	cbz	x1, .LBB10_12
// %bb.3:
	ldr	w8, [x19]
	sub	w8, w8, #4
	cmn	w8, #4
	b.ls	.LBB10_14
// %bb.4:
	ldr	x8, [x19, #8]
	cbz	x8, .LBB10_15
// %bb.5:
	adrp	x22, mas_bkops_inited
	mov	x20, x1
	ldrb	w8, [x22, :lo12:mas_bkops_inited]
	tbnz	w8, #0, .LBB10_8
// %bb.6:
	adrp	x0, .L.str.18
	add	x0, x0, :lo12:.L.str.18
	mov	w1, #24
	mov	w2, wzr
	bl	alloc_workqueue
	adrp	x8, bkops_workqueue_g
	str	x0, [x8, :lo12:bkops_workqueue_g]
	cbz	x0, .LBB10_19
// %bb.7:
	adrp	x0, bkops_pm_notifier
	add	x0, x0, :lo12:bkops_pm_notifier
	bl	register_pm_notifier
	adrp	x0, bkops_reboot_notifier
	add	x0, x0, :lo12:bkops_reboot_notifier
	bl	register_reboot_notifier
	mov	w8, #1
	strb	w8, [x22, :lo12:mas_bkops_inited]
.LBB10_8:
	mov	x0, x19
	mov	x1, x20
	bl	mas_bkops_add_debugfs
	cbnz	w0, .LBB10_17
.LBB10_9:
	adrp	x0, .L.str.19
	add	x0, x0, :lo12:.L.str.19
	mov	x1, xzr
	bl	proc_mkdir
	cbz	x0, .LBB10_18
// %bb.10:
	mov	x20, x0
	adrp	x0, .L.str.21
	add	x0, x0, :lo12:.L.str.21
	adrp	x3, mas_bkops_manual_gc_fops
	add	x3, x3, :lo12:mas_bkops_manual_gc_fops
	mov	w1, #432
	mov	x2, x20
	bl	proc_create
	adrp	x0, .L.str.22
	add	x0, x0, :lo12:.L.str.22
	adrp	x3, mas_bkops_status_fops
	add	x3, x3, :lo12:mas_bkops_status_fops
	mov	w1, #288
	mov	x2, x20
	mov	x4, x19
	bl	proc_create_data
.LBB10_11:
	mov	w0, wzr
	str	x19, [x21, :lo12:g_bkops]
.LBB10_12:
	ldp	x20, x19, [sp, #32]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #48             // 16-byte Folded Reload
	ret
.LBB10_13:
	adrp	x0, .L.str.8
	add	x0, x0, :lo12:.L.str.8
	bl	printk
	mov	w0, wzr
	b	.LBB10_12
.LBB10_14:
	adrp	x0, .L.str.9
	add	x0, x0, :lo12:.L.str.9
	b	.LBB10_16
.LBB10_15:
	adrp	x0, .L.str.10
	add	x0, x0, :lo12:.L.str.10
.LBB10_16:
	bl	printk
	mov	w0, #-22
	b	.LBB10_12
.LBB10_17:
	adrp	x0, .L.str.12
	add	x0, x0, :lo12:.L.str.12
	bl	printk
	b	.LBB10_9
.LBB10_18:
	adrp	x0, .L.str.20
	add	x0, x0, :lo12:.L.str.20
	adrp	x1, .L__func__.mas_bkops_manual_gc_proc_init
	add	x1, x1, :lo12:.L__func__.mas_bkops_manual_gc_proc_init
	mov	w2, #890
	bl	printk
	b	.LBB10_11
.LBB10_19:
	adrp	x0, .L.str.11
	add	x0, x0, :lo12:.L.str.11
	adrp	x1, .L__func__.mas_bkops_enable
	add	x1, x1, :lo12:.L__func__.mas_bkops_enable
	mov	w2, #978
	bl	printk
	mov	w0, #-22
	b	.LBB10_12
.Lfunc_end10:
	.size	mas_bkops_enable, .Lfunc_end10-mas_bkops_enable
                                        // -- End function
	.globl	mas_bkops_hm_nofity             // -- Begin function mas_bkops_hm_nofity
	.p2align	2
	.type	mas_bkops_hm_nofity,@function
mas_bkops_hm_nofity:                    // @mas_bkops_hm_nofity
// %bb.0:
	stp	x29, x30, [sp, #-16]!           // 16-byte Folded Spill
	mov	x29, sp
	cbz	x0, .LBB11_5
// %bb.1:
	cmp	x1, #24
	b.ne	.LBB11_5
// %bb.2:
	adrp	x8, g_bkops
	ldr	x8, [x8, :lo12:g_bkops]
	cbz	x8, .LBB11_4
// %bb.3:
	ldr	x9, [x0, #8]
	str	x9, [x8, #136]
	ldr	x9, [x0, #16]
	str	x9, [x8, #144]
	ldr	w1, [x0]
	mov	x0, x8
	bl	bkops_io_busyidle_notify_handler
	ldp	x29, x30, [sp], #16             // 16-byte Folded Reload
	ret
.LBB11_4:
	mov	w0, wzr
	ldp	x29, x30, [sp], #16             // 16-byte Folded Reload
	ret
.LBB11_5:
	adrp	x0, .L.str.13
	add	x0, x0, :lo12:.L.str.13
	adrp	x1, .L__func__.mas_bkops_hm_nofity
	add	x1, x1, :lo12:.L__func__.mas_bkops_hm_nofity
	bl	printk
	mov	w0, #-22
	ldp	x29, x30, [sp], #16             // 16-byte Folded Reload
	ret
.Lfunc_end11:
	.size	mas_bkops_hm_nofity, .Lfunc_end11-mas_bkops_hm_nofity
                                        // -- End function
	.section	.pac_init_section,"ax",@progbits
	.p2align	2                               // -- Begin function __pauth_pac_globals
	.type	.L__pauth_pac_globals,@function
.L__pauth_pac_globals:                  // @__pauth_pac_globals
	.cfi_startproc
// %bb.0:
	adrp	x8, bkops_pm_notifier
	adrp	x10, bkops_reboot_notifier
	adrp	x9, mas_bkops_manual_gc_fops+8
	add	x9, x9, :lo12:mas_bkops_manual_gc_fops+8
	mov	w12, #27662
	mov	w15, #39754
	ldr	x11, [x8, :lo12:bkops_pm_notifier]
	mov	w17, #4975
	ldr	x16, [x10, :lo12:bkops_reboot_notifier]
	ldp	x13, x14, [x9]
	pacia	x11, x12
	pacia	x16, x12
	str	x11, [x8, :lo12:bkops_pm_notifier]
	str	x16, [x10, :lo12:bkops_reboot_notifier]
	mov	w11, #60587
	ldp	x8, x10, [x9, #24]
	pacia	x13, x15
	pacia	x14, x17
	ldr	x12, [x9, #40]
	stp	x13, x14, [x9]
	adrp	x13, mas_bkops_status_fops+8
	add	x13, x13, :lo12:mas_bkops_status_fops+8
	pacia	x8, x17
	pacia	x10, x11
	pacia	x12, x15
	stp	x8, x10, [x9, #24]
	ldp	x8, x10, [x13]
	str	x12, [x9, #40]
	ldp	x9, x12, [x13, #32]
	pacia	x8, x15
	pacia	x10, x17
	pacia	x9, x11
	stp	x8, x10, [x13]
	pacia	x12, x15
	stp	x9, x12, [x13, #32]
	ret
.Lfunc_end12:
	.size	.L__pauth_pac_globals, .Lfunc_end12-.L__pauth_pac_globals
	.cfi_endproc
                                        // -- End function
	.type	.L.str,@object                  // @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"\0013[BKOPS]mas_bkops_query failed!\n"
	.size	.L.str, 34

	.type	.L.str.1,@object                // @.str.1
.L.str.1:
	.asciz	"\0013[BKOPS]mas_bkops_start failed!\n"
	.size	.L.str.1, 34

	.type	bkops_workqueue_g,@object       // @bkops_workqueue_g
	.local	bkops_workqueue_g
	.comm	bkops_workqueue_g,8,8
	.type	bkops_io_busyidle_notify_handler._rs,@object // @bkops_io_busyidle_notify_handler._rs
	.data
	.p2align	3
bkops_io_busyidle_notify_handler._rs:
	.zero	4
	.word	500                             // 0x1f4
	.word	10                              // 0xa
	.word	0                               // 0x0
	.word	0                               // 0x0
	.zero	4
	.xword	0                               // 0x0
	.xword	0                               // 0x0
	.size	bkops_io_busyidle_notify_handler._rs, 40

	.type	.L__func__.bkops_io_busyidle_notify_handler,@object // @__func__.bkops_io_busyidle_notify_handler
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__func__.bkops_io_busyidle_notify_handler:
	.asciz	"bkops_io_busyidle_notify_handler"
	.size	.L__func__.bkops_io_busyidle_notify_handler, 33

	.type	.L.str.2,@object                // @.str.2
.L.str.2:
	.asciz	"\0013[BKOPS]%s unknown state: %d\n"
	.size	.L.str.2, 31

	.type	.L.str.3,@object                // @.str.3
.L.str.3:
	.asciz	"\0013[BKOPS]Failed to alloc bkops_func\n"
	.size	.L.str.3, 37

	.type	.L.str.4,@object                // @.str.4
.L.str.4:
	.asciz	"%d\n"
	.size	.L.str.4, 4

	.type	user_manual_gc,@object          // @user_manual_gc
	.local	user_manual_gc
	.comm	user_manual_gc,1,4
	.type	.L.str.5,@object                // @.str.5
.L.str.5:
	.asciz	"\0013[BKOPS]%s %d: input error.\n"
	.size	.L.str.5, 30

	.type	.L__func__.mas_bkops_manual_gc_proc_write,@object // @__func__.mas_bkops_manual_gc_proc_write
.L__func__.mas_bkops_manual_gc_proc_write:
	.asciz	"mas_bkops_manual_gc_proc_write"
	.size	.L__func__.mas_bkops_manual_gc_proc_write, 31

	.type	.L.str.6,@object                // @.str.6
.L.str.6:
	.asciz	"\0013[BKOPS]%s: mas_bkops_query failed, ret = %d!\n"
	.size	.L.str.6, 48

	.type	.L__func__.mas_bkops_status_proc_show,@object // @__func__.mas_bkops_status_proc_show
.L__func__.mas_bkops_status_proc_show:
	.asciz	"mas_bkops_status_proc_show"
	.size	.L__func__.mas_bkops_status_proc_show, 27

	.type	.L.str.7,@object                // @.str.7
.L.str.7:
	.asciz	"%u\n"
	.size	.L.str.7, 4

	.type	g_bkops,@object                 // @g_bkops
	.local	g_bkops
	.comm	g_bkops,8,8
	.type	.L.str.8,@object                // @.str.8
.L.str.8:
	.asciz	"\0013[BKOPS]bkops already init!\n"
	.size	.L.str.8, 30

	.type	.L.str.9,@object                // @.str.9
.L.str.9:
	.asciz	"\0013[BKOPS]bkops dev type not specified!\n"
	.size	.L.str.9, 40

	.type	.L.str.10,@object               // @.str.10
.L.str.10:
	.asciz	"\0013[BKOPS]bkops_ops is NULL!\n"
	.size	.L.str.10, 29

	.type	.L.str.11,@object               // @.str.11
.L.str.11:
	.asciz	"\0013[BKOPS]%s %d mas_bkops_init failed!\n"
	.size	.L.str.11, 39

	.type	.L__func__.mas_bkops_enable,@object // @__func__.mas_bkops_enable
.L__func__.mas_bkops_enable:
	.asciz	"mas_bkops_enable"
	.size	.L__func__.mas_bkops_enable, 17

	.type	.L.str.12,@object               // @.str.12
.L.str.12:
	.asciz	"\0013[BKOPS]Failed to create mmc bkops debugfs!\n"
	.size	.L.str.12, 46

	.type	.L.str.13,@object               // @.str.13
.L.str.13:
	.asciz	"\0013[BKOPS]%s input err\n"
	.size	.L.str.13, 23

	.type	.L__func__.mas_bkops_hm_nofity,@object // @__func__.mas_bkops_hm_nofity
.L__func__.mas_bkops_hm_nofity:
	.asciz	"mas_bkops_hm_nofity"
	.size	.L__func__.mas_bkops_hm_nofity, 20

	.type	bkops_lock,@object              // @bkops_lock
	.data
	.p2align	3
bkops_lock:
	.zero	8
	.zero	4
	.zero	4
	.xword	bkops_lock+16
	.xword	bkops_lock+16
	.size	bkops_lock, 32

	.type	bkops_enabled,@object           // @bkops_enabled
	.local	bkops_enabled
	.comm	bkops_enabled,1,1
	.type	mas_bkops_inited,@object        // @mas_bkops_inited
	.local	mas_bkops_inited
	.comm	mas_bkops_inited,1,4
	.type	.L.str.18,@object               // @.str.18
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str.18:
	.asciz	"bkops_workqueue"
	.size	.L.str.18, 16

	.type	bkops_pm_notifier,@object       // @bkops_pm_notifier
	.data
	.p2align	3
bkops_pm_notifier:
	.xword	__cfi_bkops_pm_callback
	.xword	0
	.word	0                               // 0x0
	.zero	4
	.size	bkops_pm_notifier, 24

	.type	bkops_reboot_notifier,@object   // @bkops_reboot_notifier
	.p2align	3
bkops_reboot_notifier:
	.xword	__cfi_bkops_notify_reboot
	.xword	0
	.word	2147483647                      // 0x7fffffff
	.zero	4
	.size	bkops_reboot_notifier, 24

	.type	.L.str.19,@object               // @.str.19
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str.19:
	.asciz	"block"
	.size	.L.str.19, 6

	.type	.L.str.20,@object               // @.str.20
.L.str.20:
	.asciz	"\0013[BKOPS]%s %d: proc mk error.\n"
	.size	.L.str.20, 32

	.type	.L__func__.mas_bkops_manual_gc_proc_init,@object // @__func__.mas_bkops_manual_gc_proc_init
.L__func__.mas_bkops_manual_gc_proc_init:
	.asciz	"mas_bkops_manual_gc_proc_init"
	.size	.L__func__.mas_bkops_manual_gc_proc_init, 30

	.type	.L.str.21,@object               // @.str.21
.L.str.21:
	.asciz	"manual_gc"
	.size	.L.str.21, 10

	.type	mas_bkops_manual_gc_fops,@object // @mas_bkops_manual_gc_fops
	.section	.data.rel.ro,"aw",@progbits
	.p2align	3
mas_bkops_manual_gc_fops:
	.word	0                               // 0x0
	.zero	4
	.xword	__cfi_mas_bkops_manual_gc_proc_open
	.xword	seq_read
	.xword	0
	.xword	__cfi_mas_bkops_manual_gc_proc_write
	.xword	seq_lseek
	.xword	single_release
	.xword	0
	.xword	0
	.xword	0
	.xword	0
	.xword	0
	.size	mas_bkops_manual_gc_fops, 96

	.type	.L.str.22,@object               // @.str.22
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str.22:
	.asciz	"status"
	.size	.L.str.22, 7

	.type	mas_bkops_status_fops,@object   // @mas_bkops_status_fops
	.section	.data.rel.ro,"aw",@progbits
	.p2align	3
mas_bkops_status_fops:
	.word	0                               // 0x0
	.zero	4
	.xword	__cfi_mas_bkops_status_proc_open
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
	.size	mas_bkops_status_fops, 96

	.type	.Lg__usr1_hmos_for_chipset_src_increment_sourcecode_out_generic_generic_arm_64only_general_8425P_phone_standard_hongmeng_build_tmp_work_shared_sharedrepo_hm_ldk_nashvilleoh_git_platform_source_basicplatform_block_mas_bkops_core__pauth_pac_globals,@object // @g__usr1_hmos_for_chipset_src_increment_sourcecode_out_generic_generic_arm_64only_general_8425P_phone_standard_hongmeng_build_tmp_work_shared_sharedrepo_hm_ldk_nashvilleoh_git_platform_source_basicplatform_block_mas_bkops_core__pauth_pac_globals
	.section	.preinit_array,"awR",@preinit_array,unique,1
	.p2align	3
.Lg__usr1_hmos_for_chipset_src_increment_sourcecode_out_generic_generic_arm_64only_general_8425P_phone_standard_hongmeng_build_tmp_work_shared_sharedrepo_hm_ldk_nashvilleoh_git_platform_source_basicplatform_block_mas_bkops_core__pauth_pac_globals:
	.xword	.L__pauth_pac_globals
	.size	.Lg__usr1_hmos_for_chipset_src_increment_sourcecode_out_generic_generic_arm_64only_general_8425P_phone_standard_hongmeng_build_tmp_work_shared_sharedrepo_hm_ldk_nashvilleoh_git_platform_source_basicplatform_block_mas_bkops_core__pauth_pac_globals, 8

	.ident	"OHOS (BiSheng Mobile STD 203.0.0.2.B001) clang version 15.0.4 (llvm-project db54e0a809bd)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym __ufs_bkops_idle_work_func
	.addrsig_sym delayed_work_timer_fn
	.addrsig_sym __cfi_mas_bkops_manual_gc_proc_show
	.addrsig_sym __cfi_mas_bkops_status_proc_show
	.addrsig_sym __cfi_bkops_pm_callback
	.addrsig_sym __cfi_bkops_notify_reboot
	.addrsig_sym __cfi_mas_bkops_manual_gc_proc_open
	.addrsig_sym seq_read
	.addrsig_sym __cfi_mas_bkops_manual_gc_proc_write
	.addrsig_sym seq_lseek
	.addrsig_sym single_release
	.addrsig_sym __cfi_mas_bkops_status_proc_open
	.addrsig_sym .L__pauth_pac_globals
	.addrsig_sym bkops_io_busyidle_notify_handler._rs
	.addrsig_sym bkops_lock
	.addrsig_sym bkops_pm_notifier
	.addrsig_sym bkops_reboot_notifier
	.addrsig_sym mas_bkops_manual_gc_fops
	.addrsig_sym mas_bkops_status_fops
