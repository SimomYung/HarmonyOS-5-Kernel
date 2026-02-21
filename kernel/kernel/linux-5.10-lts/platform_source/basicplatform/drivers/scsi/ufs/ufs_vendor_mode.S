	.text
	.file	"ufs_vendor_mode.c"
	.globl	ufs_ioctl_vendor_package        // -- Begin function ufs_ioctl_vendor_package
	.p2align	2
	.type	ufs_ioctl_vendor_package,@function
ufs_ioctl_vendor_package:               // @ufs_ioctl_vendor_package
// %bb.0:
	sub	sp, sp, #64
	adrp	x8, :got:__stack_chk_guard
	stp	x29, x30, [sp, #16]             // 16-byte Folded Spill
	stp	x22, x21, [sp, #32]             // 16-byte Folded Spill
	add	x29, sp, #16
	stp	x20, x19, [sp, #48]             // 16-byte Folded Spill
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldr	x8, [x8]
	str	x8, [sp, #8]
	cbnz	x1, .LBB0_5
// %bb.1:
	mov	x20, x0
	ldr	x0, [x0, #96]
	adrp	x1, .L.str
	add	x1, x1, :lo12:.L.str
	bl	_dev_err
.LBB0_2:
	mov	w20, #-22
.LBB0_3:
	adrp	x8, :got:__stack_chk_guard
	ldr	x8, [x8, :got_lo12:__stack_chk_guard]
	ldr	x9, [sp, #8]
	ldr	x8, [x8]
	cmp	x8, x9
	b.ne	.LBB0_26
// %bb.4:
	mov	w0, w20
	ldp	x20, x19, [sp, #48]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #32]             // 16-byte Folded Reload
	ldp	x29, x30, [sp, #16]             // 16-byte Folded Reload
	add	sp, sp, #64
	ret
.LBB0_5:
	cbnz	x2, .LBB0_7
// %bb.6:
	mov	x20, x0
	ldr	x0, [x0, #96]
	adrp	x1, .L.str.1
	add	x1, x1, :lo12:.L.str.1
	adrp	x2, .L__func__.ufs_ioctl_vendor_package
	add	x2, x2, :lo12:.L__func__.ufs_ioctl_vendor_package
	bl	_dev_err
	b	.LBB0_2
.LBB0_7:
	mov	x20, x0
	adrp	x0, :got:s_health_info_lock
	mov	x21, x2
	mov	x19, x1
	ldr	x0, [x0, :got_lo12:s_health_info_lock]
	bl	mutex_lock
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.2
	add	x1, x1, :lo12:.L.str.2
	adrp	x2, .L__func__.ufs_ioctl_vendor_package
	add	x2, x2, :lo12:.L__func__.ufs_ioctl_vendor_package
	bl	_dev_info
	ldr	w3, [x19, #8]
	cmp	w3, #10
	b.ge	.LBB0_18
// %bb.8:
	ldr	x0, [x19]
	cbz	x0, .LBB0_14
.LBB0_9:
	sxtw	x8, w3
	mov	x1, x21
	mov	w2, #16
	strb	wzr, [sp, #4]
	add	x22, x0, x8, lsl #5
	mov	x0, x22
	bl	__arch_copy_from_user
	cbnz	x0, .LBB0_20
.LBB0_10:
	add	x1, x21, #16
	add	x0, sp, #4
	mov	w2, #1
	add	x22, sp, #4
	bl	__arch_copy_from_user
	cbnz	x0, .LBB0_22
.LBB0_11:
	ldrb	w8, [sp, #4]
	cbnz	w8, .LBB0_16
.LBB0_12:
	ldr	w8, [x19, #8]
	mov	w20, wzr
	add	w8, w8, #1
	str	w8, [x19, #8]
.LBB0_13:
	adrp	x0, :got:s_health_info_lock
	ldr	x0, [x0, :got_lo12:s_health_info_lock]
	bl	mutex_unlock
	b	.LBB0_3
.LBB0_14:
	adrp	x8, :got:kmalloc_caches
	mov	w1, #3520
	mov	w2, #320
	ldr	x8, [x8, :got_lo12:kmalloc_caches]
	ldr	x0, [x8, #72]
	bl	kmem_cache_alloc_trace
	str	x0, [x19]
	cbz	x0, .LBB0_19
// %bb.15:
	mov	w3, wzr
	str	wzr, [x19, #8]
	b	.LBB0_9
.LBB0_16:
	adrp	x8, :got:kmalloc_caches
	mov	w1, #3520
	mov	w2, #4096
	ldr	x8, [x8, :got_lo12:kmalloc_caches]
	ldr	x0, [x8, #96]
	bl	kmem_cache_alloc_trace
	ldr	x8, [x19]
	ldrsw	x9, [x19, #8]
	add	x8, x8, x9, lsl #5
	str	x0, [x8, #24]
	ldr	x8, [x19]
	ldrsw	x9, [x19, #8]
	add	x8, x8, x9, lsl #5
	ldr	x9, [x8, #24]
	cbz	x9, .LBB0_25
// %bb.17:
	mov	w9, #1
	str	w9, [x8, #16]
	b	.LBB0_12
.LBB0_18:
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.3
	add	x1, x1, :lo12:.L.str.3
	adrp	x2, .L__func__.ufs_ioctl_vendor_package
	add	x2, x2, :lo12:.L__func__.ufs_ioctl_vendor_package
                                        // kill: def $w3 killed $w3 killed $x3
	bl	_dev_err
	mov	w20, #-22
	b	.LBB0_13
.LBB0_19:
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.4
	add	x1, x1, :lo12:.L.str.4
	adrp	x2, .L__func__.ufs_ioctl_vendor_package
	add	x2, x2, :lo12:.L__func__.ufs_ioctl_vendor_package
	bl	_dev_err
	mov	w20, #-12
	b	.LBB0_13
.LBB0_20:
	sub	x8, x22, x0
	mov	x22, x0
	add	x8, x8, #16
	mov	w1, wzr
	mov	x0, x8
	mov	x2, x22
	bl	memset
	cbz	w22, .LBB0_10
// %bb.21:
	adrp	x1, .L.str.13
	add	x1, x1, :lo12:.L.str.13
	b	.LBB0_24
.LBB0_22:
	mov	x21, x0
	sub	x8, x22, x0
	add	x0, x8, #1
	mov	w1, wzr
	mov	x2, x21
	bl	memset
	cbz	w21, .LBB0_11
// %bb.23:
	adrp	x1, .L.str.14
	add	x1, x1, :lo12:.L.str.14
.LBB0_24:
	ldr	x0, [x20, #96]
	adrp	x2, .L__func__.ufs_get_user_cmd
	add	x2, x2, :lo12:.L__func__.ufs_get_user_cmd
	bl	_dev_err
.LBB0_25:
	mov	w20, #-12
	b	.LBB0_13
.LBB0_26:
	bl	__stack_chk_fail
.Lfunc_end0:
	.size	ufs_ioctl_vendor_package, .Lfunc_end0-ufs_ioctl_vendor_package
                                        // -- End function
	.globl	ufs_ioctl_vendor_package_tick   // -- Begin function ufs_ioctl_vendor_package_tick
	.p2align	2
	.type	ufs_ioctl_vendor_package_tick,@function
ufs_ioctl_vendor_package_tick:          // @ufs_ioctl_vendor_package_tick
// %bb.0:
	stp	x29, x30, [sp, #-96]!           // 16-byte Folded Spill
	stp	x28, x27, [sp, #16]             // 16-byte Folded Spill
	mov	x29, sp
	stp	x26, x25, [sp, #32]             // 16-byte Folded Spill
	stp	x24, x23, [sp, #48]             // 16-byte Folded Spill
	stp	x22, x21, [sp, #64]             // 16-byte Folded Spill
	stp	x20, x19, [sp, #80]             // 16-byte Folded Spill
	cbz	x0, .LBB1_4
// %bb.1:
	cbz	x1, .LBB1_4
// %bb.2:
	cbnz	x2, .LBB1_7
// %bb.3:
	mov	x20, x0
	ldr	x0, [x0, #96]
	adrp	x1, .L.str
	add	x1, x1, :lo12:.L.str
	bl	_dev_err
	b	.LBB1_5
.LBB1_4:
	adrp	x0, .L.str.5
	add	x0, x0, :lo12:.L.str.5
	bl	printk
.LBB1_5:
	mov	w21, #-22
.LBB1_6:
	mov	w0, w21
	ldp	x20, x19, [sp, #80]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #64]             // 16-byte Folded Reload
	ldp	x24, x23, [sp, #48]             // 16-byte Folded Reload
	ldp	x26, x25, [sp, #32]             // 16-byte Folded Reload
	ldp	x28, x27, [sp, #16]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #96             // 16-byte Folded Reload
	ret
.LBB1_7:
	cbnz	x3, .LBB1_9
// %bb.8:
	mov	x20, x0
	ldr	x0, [x0, #96]
	adrp	x1, .L.str.1
	add	x1, x1, :lo12:.L.str.1
	adrp	x2, .L__func__.ufs_ioctl_vendor_package_tick
	add	x2, x2, :lo12:.L__func__.ufs_ioctl_vendor_package_tick
	bl	_dev_err
	b	.LBB1_5
.LBB1_9:
	mov	x20, x0
	adrp	x0, :got:s_health_info_lock
	mov	x22, x3
	mov	x19, x2
	mov	x23, x1
	ldr	x0, [x0, :got_lo12:s_health_info_lock]
	bl	mutex_lock
	adrp	x9, :got:jiffies
	adrp	x8, ufs_ioctl_vendor_package_tick.s_timeout
	ldr	x9, [x9, :got_lo12:jiffies]
	ldr	x10, [x9]
	ldr	x11, [x8, :lo12:ufs_ioctl_vendor_package_tick.s_timeout]
	cmp	x10, x11
	b.pl	.LBB1_11
// %bb.10:
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.6
	add	x1, x1, :lo12:.L.str.6
	adrp	x2, .L__func__.ufs_ioctl_vendor_package_tick
	add	x2, x2, :lo12:.L__func__.ufs_ioctl_vendor_package_tick
	bl	_dev_err
	mov	w21, #-16
	ldr	x0, [x19]
	cbnz	x0, .LBB1_27
	b	.LBB1_36
.LBB1_11:
	ldr	x9, [x9]
	adrp	x1, .L.str.7
	add	x1, x1, :lo12:.L.str.7
	ldr	x0, [x20, #96]
	adrp	x2, .L__func__.ufs_ioctl_vendor_package_tick
	add	x2, x2, :lo12:.L__func__.ufs_ioctl_vendor_package_tick
	add	x9, x9, #43, lsl #12            // =176128
	add	x9, x9, #3872
	str	x9, [x8, :lo12:ufs_ioctl_vendor_package_tick.s_timeout]
	bl	_dev_info
	ldr	x8, [x19]
	cbz	x8, .LBB1_38
// %bb.12:
	ldr	x0, [x20, #88]
	bl	scsi_block_requests
	mov	x0, x20
	mov	w1, #5000
	bl	wait_for_ufs_all_complete
	cbnz	w0, .LBB1_39
// %bb.13:
	ldr	w8, [x19, #8]
	cmp	w8, #1
	b.lt	.LBB1_25
// %bb.14:
	mov	x25, xzr
	mov	x26, xzr
	mov	x27, xzr
	mov	x24, xzr
	b	.LBB1_16
.LBB1_15:                               //   in Loop: Header=BB1_16 Depth=1
	ldrsw	x8, [x19, #8]
	add	x27, x27, #1
	add	x26, x26, #256
	add	x25, x25, #32
	cmp	x27, x8
	b.ge	.LBB1_25
.LBB1_16:                               // =>This Inner Loop Header: Depth=1
	ldr	x28, [x19]
	mov	w9, #1
	add	x2, x28, x25
	ldrb	w3, [x2]
	sub	w8, w3, #4
	cmp	w8, #62
	lsl	x8, x9, x8
	mov	x9, #81
	movk	x9, #80, lsl #32
	movk	x9, #16384, lsl #48
	and	x8, x8, x9
	ccmp	x8, #0, #4, ls
	b.ne	.LBB1_37
// %bb.17:                              //   in Loop: Header=BB1_16 Depth=1
	cmp	w3, #138
	b.eq	.LBB1_37
// %bb.18:                              //   in Loop: Header=BB1_16 Depth=1
	cmp	w3, #136
	b.eq	.LBB1_37
// %bb.19:                              //   in Loop: Header=BB1_16 Depth=1
	add	x8, x28, x25
	ldr	w9, [x8, #16]
	cbz	w9, .LBB1_21
// %bb.20:                              //   in Loop: Header=BB1_16 Depth=1
	ldr	x24, [x8, #24]
	cbz	x24, .LBB1_42
.LBB1_21:                               //   in Loop: Header=BB1_16 Depth=1
	mov	x0, x20
	mov	x1, x23
	mov	x3, x24
	bl	ufshcd_send_vendor_scsi_cmd
	cbnz	w0, .LBB1_41
// %bb.22:                              //   in Loop: Header=BB1_16 Depth=1
	cbz	x24, .LBB1_15
// %bb.23:                              //   in Loop: Header=BB1_16 Depth=1
	and	x8, x26, #0xffffff00
	mov	x1, x24
	add	x0, x22, x8
	mov	w2, #256
	bl	__arch_copy_to_user
	mov	x21, x0
	cbz	w21, .LBB1_15
// %bb.24:
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.19
	add	x1, x1, :lo12:.L.str.19
	adrp	x2, .L__func__.vendor_data_copy
	add	x2, x2, :lo12:.L__func__.vendor_data_copy
	b	.LBB1_40
.LBB1_25:
	mov	w21, wzr
.LBB1_26:
	ldr	x0, [x20, #88]
	bl	scsi_unblock_requests
	ldr	x0, [x19]
	cbz	x0, .LBB1_36
.LBB1_27:
	ldr	w8, [x19, #8]
	cmp	w8, #1
	b.lt	.LBB1_35
// %bb.28:
	mov	x20, xzr
	mov	x22, xzr
	b	.LBB1_30
.LBB1_29:                               //   in Loop: Header=BB1_30 Depth=1
	ldrsw	x8, [x19, #8]
	add	x22, x22, #1
	add	x20, x20, #32
	cmp	x22, x8
	b.ge	.LBB1_34
.LBB1_30:                               // =>This Inner Loop Header: Depth=1
	ldr	x8, [x19]
	add	x9, x8, x20
	ldr	w10, [x9, #16]
	cbz	w10, .LBB1_32
// %bb.31:                              //   in Loop: Header=BB1_30 Depth=1
	str	wzr, [x9, #16]
	ldr	x8, [x19]
.LBB1_32:                               //   in Loop: Header=BB1_30 Depth=1
	add	x8, x8, x20
	ldr	x0, [x8, #24]
	cbz	x0, .LBB1_29
// %bb.33:                              //   in Loop: Header=BB1_30 Depth=1
	bl	kfree
	ldr	x8, [x19]
	add	x8, x8, x20
	str	xzr, [x8, #24]
	b	.LBB1_29
.LBB1_34:
	ldr	x0, [x19]
.LBB1_35:
	bl	kfree
	str	xzr, [x19]
.LBB1_36:
	adrp	x0, :got:s_health_info_lock
	str	wzr, [x19, #8]
	ldr	x0, [x0, :got_lo12:s_health_info_lock]
	bl	mutex_unlock
	b	.LBB1_6
.LBB1_37:
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.16
	add	x1, x1, :lo12:.L.str.16
	adrp	x2, .L__func__.vendor_data_copy
	add	x2, x2, :lo12:.L__func__.vendor_data_copy
	bl	_dev_err
	mov	w21, #-22
	b	.LBB1_26
.LBB1_38:
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.8
	add	x1, x1, :lo12:.L.str.8
	adrp	x2, .L__func__.ufs_ioctl_vendor_package_tick
	add	x2, x2, :lo12:.L__func__.ufs_ioctl_vendor_package_tick
	bl	_dev_err
	mov	w21, #-22
	ldr	x0, [x19]
	cbnz	x0, .LBB1_27
	b	.LBB1_36
.LBB1_39:
	mov	w21, w0
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.9
	add	x1, x1, :lo12:.L.str.9
	adrp	x2, .L__func__.ufs_ioctl_vendor_package_tick
	add	x2, x2, :lo12:.L__func__.ufs_ioctl_vendor_package_tick
.LBB1_40:
	mov	w3, w21
	bl	_dev_err
	b	.LBB1_26
.LBB1_41:
	mov	w21, w0
	ldrb	w3, [x28, x25]
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.18
	add	x1, x1, :lo12:.L.str.18
	adrp	x2, .L__func__.vendor_data_copy
	add	x2, x2, :lo12:.L__func__.vendor_data_copy
	mov	w4, w21
	bl	_dev_err
	b	.LBB1_26
.LBB1_42:
	ldr	x0, [x20, #96]
	adrp	x1, .L.str.17
	add	x1, x1, :lo12:.L.str.17
	bl	_dev_err
	mov	w21, #-22
	b	.LBB1_26
.Lfunc_end1:
	.size	ufs_ioctl_vendor_package_tick, .Lfunc_end1-ufs_ioctl_vendor_package_tick
                                        // -- End function
	.type	s_health_info_lock,@object      // @s_health_info_lock
	.data
	.globl	s_health_info_lock
	.p2align	3
s_health_info_lock:
	.zero	8
	.zero	4
	.zero	4
	.xword	s_health_info_lock+16
	.xword	s_health_info_lock+16
	.size	s_health_info_lock, 32

	.type	.L.str,@object                  // @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"ufs_vendor_ioctl_state_p addr is NULL\n"
	.size	.L.str, 39

	.type	.L.str.1,@object                // @.str.1
.L.str.1:
	.asciz	"%s: User buffer is NULL!\n"
	.size	.L.str.1, 26

	.type	.L__func__.ufs_ioctl_vendor_package,@object // @__func__.ufs_ioctl_vendor_package
.L__func__.ufs_ioctl_vendor_package:
	.asciz	"ufs_ioctl_vendor_package"
	.size	.L__func__.ufs_ioctl_vendor_package, 25

	.type	.L.str.2,@object                // @.str.2
.L.str.2:
	.asciz	"%s: ioctl vendor cmd buffering\n"
	.size	.L.str.2, 32

	.type	.L.str.3,@object                // @.str.3
.L.str.3:
	.asciz	"%s: ioctl vendor cmd over count, %d\n"
	.size	.L.str.3, 37

	.type	.L.str.4,@object                // @.str.4
.L.str.4:
	.asciz	"%s: vendor cmd mem alloc error\n"
	.size	.L.str.4, 32

	.type	ufs_ioctl_vendor_package_tick.s_timeout,@object // @ufs_ioctl_vendor_package_tick.s_timeout
	.local	ufs_ioctl_vendor_package_tick.s_timeout
	.comm	ufs_ioctl_vendor_package_tick.s_timeout,8,8
	.type	.L.str.5,@object                // @.str.5
.L.str.5:
	.asciz	"\0013ufshcd :hba addr is NULL\n"
	.size	.L.str.5, 28

	.type	.L__func__.ufs_ioctl_vendor_package_tick,@object // @__func__.ufs_ioctl_vendor_package_tick
.L__func__.ufs_ioctl_vendor_package_tick:
	.asciz	"ufs_ioctl_vendor_package_tick"
	.size	.L__func__.ufs_ioctl_vendor_package_tick, 30

	.type	.L.str.6,@object                // @.str.6
.L.str.6:
	.asciz	"%s: wait for half an hour and try again\n"
	.size	.L.str.6, 41

	.type	.L.str.7,@object                // @.str.7
.L.str.7:
	.asciz	"%s: send all vendor cmd\n"
	.size	.L.str.7, 25

	.type	.L.str.8,@object                // @.str.8
.L.str.8:
	.asciz	"%s: vendor package cmd not init\n"
	.size	.L.str.8, 33

	.type	.L.str.9,@object                // @.str.9
.L.str.9:
	.asciz	"%s: wait for req cleard time out, ret == %d\n"
	.size	.L.str.9, 45

	.type	.L.str.13,@object               // @.str.13
.L.str.13:
	.asciz	"%s: copy cdb error\n"
	.size	.L.str.13, 20

	.type	.L__func__.ufs_get_user_cmd,@object // @__func__.ufs_get_user_cmd
.L__func__.ufs_get_user_cmd:
	.asciz	"ufs_get_user_cmd"
	.size	.L__func__.ufs_get_user_cmd, 17

	.type	.L.str.14,@object               // @.str.14
.L.str.14:
	.asciz	"%s: copy data flag\n"
	.size	.L.str.14, 20

	.type	.L.str.16,@object               // @.str.16
.L.str.16:
	.asciz	"%s: not allow ioctl this cmd:0x%x\n"
	.size	.L.str.16, 35

	.type	.L__func__.vendor_data_copy,@object // @__func__.vendor_data_copy
.L__func__.vendor_data_copy:
	.asciz	"vendor_data_copy"
	.size	.L__func__.vendor_data_copy, 17

	.type	.L.str.17,@object               // @.str.17
.L.str.17:
	.asciz	"data bufffer is null\n"
	.size	.L.str.17, 22

	.type	.L.str.18,@object               // @.str.18
.L.str.18:
	.asciz	"%s: send cmd[0x%x] error %d\n"
	.size	.L.str.18, 29

	.type	.L.str.19,@object               // @.str.19
.L.str.19:
	.asciz	"%s: copy to user error %d\n"
	.size	.L.str.19, 27

	.ident	"OHOS (BiSheng Mobile STD 203.0.0.2.B001) clang version 15.0.4 (llvm-project db54e0a809bd)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym s_health_info_lock
	.addrsig_sym jiffies
