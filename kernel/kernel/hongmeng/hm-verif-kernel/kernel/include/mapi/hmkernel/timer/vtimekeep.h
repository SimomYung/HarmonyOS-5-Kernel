/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Acquire tock/frequency of time keeping
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 08 09:58:11 2020
 */
#ifndef MAPI_HMKERNEL_VTIMEKEEP_H
#define MAPI_HMKERNEL_VTIMEKEEP_H

#include <hmkernel/compiler.h>
#include <hmkernel/types.h>

extern u64 vtimekeep_tock(void);
extern u64 vtimekeep_tock_mono(void);
extern u64 vtimekeep_tock_mono_coarse_nolock(void);
extern u64 vtimekeep_tock_boot(void);
extern s64 vtimekeep_tock_offset(void);
extern u64 vtimekeep_tock_start(void);
extern u64 vtimekeep_tock_mono_offs(void);
extern u64 vtimekeep_tock_real(void);

extern u64 vtimekeep_tock_boot_to_mono(u64 tock);
extern u64 vtimekeep_tock_mono_to_boot(u64 tock);
extern u64 vtimekeep_tock_boot_to_rato(u64 boot_tock);
extern u64 vtimekeep_tock_rato_to_boot(u64 tock);

/* called by sysconf for syscall */
extern u32 vtimekeep_freq(void);

/*
 * is_uctx32: Check with arch_uctx_is_ucontext_32bit()
 * utimeout: User ptr which point to timespec
 * tock: Value of timespec
 */
extern int vtimekeep_read_tock_from_user(bool is_uctx32, const void __user *utimeout, u64 *tock);
extern int vtimekeep_write_tock_to_user(bool is_uctx32, void __user *utimeout, u64 tock);

/* Dump current time */
extern void vtimekeep_dump(const char *category, const char *msg);

/* Dump vtimekeep tock during suspend */
extern void vtimekeep_suspend_tock_dump(const char *category, const char *msg);

#endif
