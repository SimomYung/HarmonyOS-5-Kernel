/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: SError
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 30 17:21:53 2022
 */

#ifndef AARCH64_HMASM_SERROR_H
#define AARCH64_HMASM_SERROR_H

#include <hmkernel/compiler.h>

struct arch_uctx;
struct arch_regs;
struct knotifier_obj;

extern void __noreturn arch_handle_el0_serror(struct arch_uctx *uctx, u64 esr);
extern void __noreturn arch_handle_el1_serror(const struct arch_regs *regs, u64 esr);
extern int arch_serror_register_notif(struct knotifier_obj *knobj);
extern void arch_serror_unregister_notif(struct knotifier_obj *knobj);

#endif /* !AARCH64_HMASM_SERROR_H */
