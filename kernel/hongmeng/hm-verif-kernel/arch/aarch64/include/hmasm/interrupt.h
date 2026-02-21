/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 23 15:32:06 2018
 */
#ifndef AARCH64_ASM_INTERRUPT_H
#define AARCH64_ASM_INTERRUPT_H

#include <hmasm/registers.h>
#include <hmkernel/kernel.h>
#include <hmkernel/interrupt/interrupt.h>

/*
 * arm-gic driver can be used in arm32 and 64. Use similar name
 * so arm-gic code can be shared.
 */
#define IRQ_HDLR_FLAG_ARM_IRQ	0x1UL
#define IRQ_HDLR_FLAG_ARM_FIQ	0x2UL
#ifdef CONFIG_PERF_KERNEL
#define IRQ_PRIO_MASK_UNSET	0xffUL
/*
 * all irq priorities are configured 0x60 in the dts except pmu which is 0x40,
 * if we would like to mask all interrupts except pmu, we can just set gic pmr with 0x50
 *
 * this value(0x50) denpends on the setting of ICC_BPR1_EL1, which should be set to 0x5
 */
#define IRQ_PRIO_MASK_PMU	0x50UL
#endif

void __noreturn
arch_handle_el0_irq(const struct arch_uctx *uctx);

void __noreturn
arch_handle_el0_fiq(const struct arch_uctx *uctx);

void __noreturn
arch_handle_el1_fiq(const struct arch_regs *regs);

/* Implemented in entry.S */
extern void __noreturn
arch_process_irq_reset_stack(struct tcb_s *prev_tcb, unsigned long flags,
			     const struct arch_cpu_local_s *cpu_local);

extern void __noreturn
arch_handle_kern_irq(struct tcb_s *prev_tcb);

unsigned long arch_irq_stack_fp(void);
#endif
