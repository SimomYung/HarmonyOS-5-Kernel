/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jul 7 17:22:27 2018
 */
#ifndef AARCH64_ASM_EXCEPTION_H
#define AARCH64_ASM_EXCEPTION_H

#include <uapi/hmasm/exception.h>
#include <mapi/hmasm/exception.h>
#include <hmasm/sysreg.h>
#include <uapi/hmkernel/compiler.h>

#define INV_EXP_SYNC 0
#define INV_EXP_IRQ  1
#define INV_EXP_FIQ  2
#define INV_EXP_ERR  3
#define INV_EXP_EL1_UNHANDLED_SYNC  4
#define INV_EXP_EL1_ERR	5

#ifndef __ASSEMBLY__
#include <hmasm/ucontext.h>

#include <hmkernel/knotifier.h>

/* defined in .S */
extern int exception_vector[];

extern void exception_init(void);

static inline void arch_kexception_init_stack(unsigned int cpu) {}

extern __noreturn void
restore_regs(struct arch_regs *regs);

extern __noreturn void
restore_regs_el1(const struct arch_regs *regs);

extern __noreturn void
restore_regs_to_el1t(const struct arch_regs *regs);

extern u64
arch_kexception_dump_regs_conf_of(void);

extern int
arch_kexception_dump_regs_set_conf(u64 user_regs_region);

extern void
arch_dump_ex_info(const struct __arch_exception_info *ex_info, bool is_panic);

extern void
arch_dump_registers(const struct arch_regs *regs, bool is_panic);

extern void
arch_dump_regs_around(const struct arch_regs *regs, bool is_panic, bool is_kvspace, bool unimpl_exp);

extern void
arch_dump_pc_around(const struct arch_regs *regs, bool is_panic);

extern void
arch_dump_stack_content(const struct arch_regs *regs, bool is_panic, bool is_kvspace);

extern void
arch_dump_registers_nolock(const struct arch_regs *regs, bool is_panic);

extern int
arch_unimpl_ex_register_notif(struct knotifier_obj *knobj);

extern void
arch_unimpl_ex_unregister_notif(struct knotifier_obj *knobj);

extern void
arch_dump_kernel_exception(const struct arch_regs *regs, bool is_kvspace, bool unimpl_exp);

extern void
arch_dump_current_ex_regs(void);

extern void arch_dump_kernel_data(void *k_ptr, unsigned long size, bool is_panic);

extern void __noreturn
unimpl_exception(const struct arch_regs *regs, bool is_kvspace);

extern void __noreturn
unimpl_exception_with_reason(const struct arch_regs *regs, bool is_kvspace, unsigned int reboot_reason);

extern void __noreturn
unimpl_exception_dump_user(const struct arch_regs *regs, bool is_kvspace, bool dump_user, unsigned int reboot_reason);

extern void __noreturn
kernel_stack_overflow(struct arch_regs *regs);

struct arch_cpu_local_s;
extern void __noreturn
arch_handle_data_fault(struct arch_uctx *uctx, struct arch_cpu_local_s *cpu_local, bool is_from_real_el0);

extern void __noreturn
arch_handle_instruction_fault(struct arch_uctx *uctx, struct arch_cpu_local_s *cpu_local, bool is_from_real_el0);

extern void __noreturn
arch_handle_debug_fault(struct arch_uctx *uctx, bool is_from_real_el0);

extern void __noreturn
arch_handle_single_step_fault(struct arch_uctx *uctx, bool is_from_real_el0);

extern void __noreturn
arch_handle_wp_fault(struct arch_uctx *uctx, bool is_from_real_el0);

extern void __noreturn
arch_handle_bp_fault(struct arch_uctx *uctx, bool is_from_real_el0);

extern void __noreturn
arch_handle_und_ins_fault(struct arch_uctx *uctx);

extern void __noreturn
arch_handle_pac_fault(struct arch_uctx *uctx, bool is_from_real_el0);

#endif

#endif
